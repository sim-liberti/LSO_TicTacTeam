#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#include <stdbool.h>

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

volatile sig_atomic_t server_running = 1;

void handle_sigint(int sig) {
    printf("%s\n", "Chiudo il server...");
    server_running = 0;
}

SharedMemory mem;

void* handle_client(void* arg) {
    int client_socket = *(int*)arg;
    free(arg);

    char buffer_str[2048];

    printf("Client %d connesso.\n", client_socket);

    // Invia lista partite iniziale
    char first_conn[sizeof(mem.lista_partite)+10];
    sprintf(first_conn, "%d||%s", client_socket, visualizza_partite(mem.lista_partite));
    send(client_socket, first_conn, strlen(first_conn), 0);

    int read_bytes;
    while ((read_bytes = recv(client_socket, buffer_str, sizeof(buffer_str) - 1, 0)) > 0) {
        // printf("Ricevuto segnale da client: %d\n", client_socket);
        buffer_str[read_bytes] = '\0';
        // printf("Ecco il buffer: %s\n", buffer_str);

        buffer_generico buffer;
        json_to_buffer(buffer_str, &buffer);
        
        char* dynamic_response = NULL;
        int socket_destinatario = client_socket;

        switch(buffer.segnale){
            case LISTA_PARTITE:
                // printf("Ricevuto segnale LISTA_PARTITE\n");
                dynamic_response = visualizza_partite(mem.lista_partite);
                break;
                case NUOVA_PARTITA:
                // printf("Ricevuto segnale NUOVA_PARTITA\n");
                pthread_mutex_lock(&mem.lock);
                dynamic_response = crea_nuova_partita(&buffer.nuova_partita, mem.lista_partite);
                pthread_mutex_unlock(&mem.lock);
                break; 
            case NUOVA_MOSSA:
                fine_partita_enum stato_fine;
                dynamic_response = inserisci_mossa(&buffer.nuova_mossa, mem.lista_partite, &stato_fine);
                break;
            case GESTISCI_GUEST:
                dynamic_response = gestisci_richiesta_guest(&buffer.gestisci_guest, mem.lista_partite, &socket_destinatario);
                break;
            case GESTISCI_PAREGGIO:
                dynamic_response = gestisci_pareggio(&buffer.gestisci_pareggio, mem.lista_partite);
                break;
            case CANCELLA_PARTITA:
                dynamic_response = cancella_partita(buffer.cancella_partita.id_partita, mem.lista_partite);
                break;
        }

        send(socket_destinatario, dynamic_response, strlen(dynamic_response), 0);
        free(dynamic_response); // solo se è stato allocato dinamicamente
        socket_destinatario = client_socket;
    }

    close(client_socket);
    return NULL;
}

int main() {
    int server_fd, client_socket;
    struct sockaddr_in address;
    socklen_t addrlen = sizeof(address);
    
    setvbuf(stdout, NULL, _IONBF, 0);
    signal(SIGINT, handle_sigint);
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
    
    int socket = bind(server_fd, (struct sockaddr*)&address, sizeof(address));
    if (socket < 0) {
        perror("Socket non allocata");
        return 1;
    }
    listen(server_fd, 5);

    printf("Server in ascolto sulla porta %d...\n", PORT);

    while (server_running) {
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

