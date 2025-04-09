#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#include "types.h"

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8080

int main() {
    int sock;
    struct sockaddr_in serv_addr;
    char buffer[1024] = {0};

    
    sock = socket(AF_INET, SOCK_STREAM, 0);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SERVER_PORT);
    
    inet_pton(AF_INET, SERVER_IP, &serv_addr.sin_addr);
    connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    
    int valread = read(sock, buffer, sizeof(buffer));
    printf("%s\n", buffer);
    
    while(1) {
        printf("SCEGLI UNO STATO DA INVIARE:\n");
        printf("- 1: NUOVA_PARTITA\n");
        printf("- 2: NUOVA_MOSSA\n");
        printf("- 3: ACCETTA_GUEST\n");
        printf("- 4: RICOMINCIA_PARTITA\n");
        printf(": ");
        int scelta;
        scanf("%d", &scelta);
    
        char *message;
        switch (scelta) {
            case 1:
                message = "1";
                break;
            case 2:
                message = "2";
                break;
            case 3:
                message = "3";
                break;
            case 4:
                message = "4";
                break;
        }

        send(sock, message, strlen(message), 0);
    }
    // int valread = read(sock, buffer, sizeof(buffer));
    // printf("Risposta del server: %s\n", buffer);

    close(sock);
    return 0;
}
