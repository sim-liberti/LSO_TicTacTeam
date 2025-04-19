#include "include/handler.h"

SharedMemory mem;
volatile sig_atomic_t server_running = 1;

int main() {
    int server_fd, client_socket;
    struct sockaddr_in address;
    socklen_t addrlen = sizeof(address);
    
    setvbuf(stdout, NULL, _IONBF, 0);
    // signal(SIGINT, handle_sigint);
    // pthread_mutex_init(&mem.lock, NULL);
    
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
    WSACleanup();
    #endif

    close(server_fd);
    //pthread_mutex_destroy(&mem.lock);
    return 0;
}
