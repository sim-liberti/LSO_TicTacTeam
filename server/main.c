#include <fcntl.h>
#include <time.h>

#include "handler.h"

SharedMemory mem;
volatile sig_atomic_t server_running = 1;

int main() {
    int server_fd, client_socket;
    struct sockaddr_in address;
    socklen_t addrlen = sizeof(address);
    int opt = 1;
    
    static int initialized = 0;
    if (!initialized) {
        srand(time(NULL));
        initialized = 1;
    }

    setvbuf(stdout, NULL, _IONBF, 0);
    signal(SIGINT, handle_sigint);
    // pthread_mutex_init(&mem.lock, NULL);
    
    #ifdef _WIN32
    // Inizializzazione Winsock
    WSADATA wsaData;
    int wsa_result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (wsa_result != 0) {
        printf("[ERROR] WSAStartup failed: %d\n", wsa_result);
        return 1;
    }
    #endif

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    // Rende la "accept" non bloccante - Usato per gestire correttamente il SIGINT
    fcntl(server_fd, F_SETFL, O_NONBLOCK);
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    
    int socket = bind(server_fd, (struct sockaddr*)&address, sizeof(address));
    if (socket < 0) {
        perror("[ERROR] Could not create socket. Exiting...");
        return 1;
    }
    listen(server_fd, 5);

    printf("[INFO] Server is running on port %d. Press Ctrl+C to stop.\n", PORT);

    while (server_running) {
        client_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen);

        if (client_socket >= 0) {
            int* pclient = malloc(sizeof(int));
            *pclient = client_socket;
            
            printf("[INFO] Client connected using socket %d.\n", *(int*)pclient);
            
            pthread_t tid;
            pthread_create(&tid, NULL, handle_client, pclient);
            pthread_detach(tid);
        } else {
            sleep(0.1);
        }
    }

    #ifdef _WIN32
    WSACleanup();
    #endif

    close(server_fd);
    printf("[INFO] Server stopped.\n");
    //pthread_mutex_destroy(&mem.lock);
    return 0;
}
