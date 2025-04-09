#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>

#include "types.h"
#include "server_functions.h"

#define PORT 8080

struct partite lista_partite[MAX_ARRAY_LEN];
pthread_mutex_t partite_mutex;

void* handle_client(void* arg) {
    int client_socket = *(int*)arg;
    free(arg);

    char *lista_partite_json = visualizza_partite(lista_partite);
    send(client_socket, lista_partite_json, strlen(lista_partite_json), 0);

    buffer_generico_sig buffer;
    unpack_json_to_buffer("", &buffer);

    while(1){
        char buffer[1024] = {0};
        int read_bytes = recv(client_socket, buffer, sizeof(buffer), 0);

        if (read_bytes > 0) {
            printf("Client %d ha inviato: %s\n", client_socket, buffer);

            if (strncmp(buffer, "1", 1) == 0) {
                // Crea partita
            } else if (strncmp(buffer, "2", 1) == 0) {
                // Registra mossa
            } else if (strncmp(buffer, "3", 1) == 0) {
                // Accetta sfidante
            } else if (strncmp(buffer, "4", 1) == 0) {
                // Pulisci
            }
        }
    }

    close(client_socket);
    return NULL;
}

int main() {
    int server_fd, client_socket;
    struct sockaddr_in address;
    socklen_t addrlen = sizeof(address);

    lista_partite[0].id_partita = 10;
    lista_partite[0].id_owner = 85;
    lista_partite[0].stato_partita = IN_CORSO;

    pthread_mutex_init(&partite_mutex, NULL);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == 0) {
        perror("socket fallita");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;  // accetta da ogni IP
    address.sin_port = htons(PORT);

    bind(server_fd, (struct sockaddr*)&address, sizeof(address));
    listen(server_fd, 5);

    printf("Server in ascolto sulla porta %d...\n", PORT);

    while (1) {
        client_socket = accept(server_fd, (struct sockaddr*)&address, &addrlen);
        if (client_socket >= 0) {
            int* pclient = malloc(sizeof(int));
            *pclient = client_socket;

            pthread_t tid;
            pthread_create(&tid, NULL, handle_client, pclient);
            pthread_detach(tid);
        }
    }

    pthread_mutex_destroy(&partite_mutex);
    return 0;
}

