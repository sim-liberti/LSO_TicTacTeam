#include "handler.h"
#include "buffer.h"
#include "utils.h"

void* handle_client(void* arg) {
    int client_socket = *(int*)arg;
    free(arg);

    char buffer_str[2048];

    printf("Client %d connected.\n", client_socket);

    // Invia lista partite iniziale
    char welcome_msg[sizeof(mem.games_list)+10];
    sprintf(welcome_msg, "%d||%s", client_socket, visualizza_partite(mem.games_list));
    send(client_socket, welcome_msg, strlen(welcome_msg), 0);

    int read_bytes;
    while ((read_bytes = recv(client_socket, buffer_str, sizeof(buffer_str) - 1, 0)) > 0) {
        buffer_str[read_bytes] = '\0';

        generic_buffer buffer;
        json_to_buffer(buffer_str, &buffer);

        switch(buffer.sig){
            case SIG_GET_MATCH_LIST:

            break;
            case SIG_CREATE_NEW_MATCH:

            break;
            case SIG_MAKE_MOVE:

            break;
            case SIG_GUEST_REQUEST:

            break;
            case SIG_GUEST_RESPONSE:

            break;
            case SIG_HANDLE_DRAW:

            break;
            case SIG_DELETE_MATCH:

            break;
        }
    }

    close(client_socket);
    return NULL;
}
