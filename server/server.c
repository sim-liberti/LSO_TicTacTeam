#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#ifdef _WIN32
// Implementazione Socket Windows
#include <Winsock2.h> 
#include <ws2tcpip.h>
#else
// Implementazione Socket Linux
#include <arpa/inet.h> 
#endif

#include "server_functions.h"

#define PORT 8080

SharedMemory mem;

void* handle_client(void* arg) {
    int client_socket = *(int*)arg;
    free(arg);

    char buffer_str[2048];

    // Invia lista partite iniziale
    char id_socket[20];
    sprintf(id_socket, "%d", client_socket);
    send(client_socket, id_socket, strlen(id_socket), 0);
    char *initial_list = visualizza_partite(mem.lista_partite);
    send(client_socket, initial_list, strlen(initial_list), 0);

    int read_bytes;
    while ((read_bytes = recv(client_socket, buffer_str, sizeof(buffer_str) - 1, 0)) > 0) {
        buffer_str[read_bytes] = '\0';

        buffer_generico buffer;
        json_to_buffer(buffer_str, &buffer);

        char* dynamic_response = NULL;

        switch(buffer.segnale){
            case NUOVA_PARTITA:
                pthread_mutex_lock(&mem.lock);
                dynamic_response = crea_nuova_partita(&buffer.nuova_partita, mem.lista_partite);
                pthread_mutex_unlock(&mem.lock);
                break; 
            case NUOVA_MOSSA: {
                fine_partita_enum stato_fine;
                dynamic_response = inserisci_mossa(&buffer.nuova_mossa, mem.lista_partite, &stato_fine);
                break;
            }
            case GESTISCI_PAREGGIO:
                dynamic_response = gestisci_pareggio(&buffer.gestisci_pareggio, mem.lista_partite);
                break;
            case CANCELLA_PARTITA:
                dynamic_response = cancella_partita(buffer.cancella_partita.id_partita, mem.lista_partite);
                break;
        }

        if (dynamic_response != NULL) {
            send(client_socket, dynamic_response, strlen(dynamic_response), 0);
            free(dynamic_response); // solo se è stato allocato dinamicamente
        } else {
            send(client_socket, "OK", 2, 0);
        }
    }

    close(client_socket);
    return NULL;
}

int main() {
    int server_fd, client_socket;
    struct sockaddr_in address;
    socklen_t addrlen = sizeof(address);

    pthread_mutex_init(&mem.lock, NULL);
    
    #ifdef _WIN32
    // Inizializzazione Winsock
    WSADATA wsaData;
    int wsa_result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (wsa_result != 0) {
        printf("Errore in WSAStartup: %d\n", wsa_result);
        return 1;
    }
    #endif

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);


    bind(server_fd, (struct sockaddr*)&address, sizeof(address));
    listen(server_fd, 5);

    printf("Server in ascolto sulla porta %d...\n", PORT);

    while (1) {
        client_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen);

        if (client_socket >= 0) {
            int* pclient = malloc(sizeof(int));
            *pclient = client_socket;
    
            pthread_t tid;
            pthread_create(&tid, NULL, handle_client, pclient);
            pthread_detach(tid);
        }
    }

    #ifdef _WIN32
    WSACleanup();  // <-- Molto importante!
    #endif

    close(server_fd);
    pthread_mutex_destroy(&mem.lock);
    return 0;
}

