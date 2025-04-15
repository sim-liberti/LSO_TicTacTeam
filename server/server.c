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
    
    char *response;
    // 
    // json_to_buffer("", &buffer);

    while(1){
        char *buffer_str;
        int read_bytes = recv(client_socket, buffer_str, sizeof(buffer_str), 0);
        buffer_generico buffer;
        json_to_buffer(buffer_str, &buffer);

        switch(buffer.segnale){
            case NUOVA_PARTITA:
                pthread_mutex_lock(&mem.lock);
                response = crea_nuova_partita(&buffer.nuova_partita, mem.lista_partite);
                // Restituiamo al client la lista delle partite
                // e la nuova partita creata
                pthread_mutex_unlock(&mem.lock);
            break; 
            case NUOVA_MOSSA:
                fine_partita_enum stato_fine;
                response = inserisci_mossa(&buffer.nuova_mossa, mem.lista_partite, &stato_fine);
            break; 
            case GESTISCI_GUEST:

            break; 
            case GESTISCI_PAREGGIO:
                response = gestisci_pareggio(&buffer.gestisci_pareggio, mem.lista_partite);
            break;
            case CANCELLA_PARTITA:
                response = cancella_partita(buffer.cancella_partita.id_partita, mem.lista_partite);
            break;
        }
    }

    send(client_socket, response, strlen(response), 0);
    close(client_socket);
    return NULL;
}

int main() {
    int server_fd, client_socket;
    struct sockaddr_in address;
    socklen_t addrlen = sizeof(address);

    pthread_mutex_init(&mem.lock, NULL);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    bind(server_fd, (struct sockaddr*)&address, sizeof(address));
    listen(server_fd, 5);

    printf("Server in ascolto sulla porta %d...\n", PORT);

    while (1) {
        client_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen);
        int* pclient = malloc(sizeof(int));
        *pclient = client_socket;

        pthread_t tid;
        pthread_create(&tid, NULL, handle_client, pclient);
        pthread_detach(tid);
    }

    close(server_fd);
    pthread_mutex_destroy(&mem.lock);
    return 0;
}

