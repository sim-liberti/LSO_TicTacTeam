#include "handler.h"
#include "buffer.h"
#include "utils.h"

void* handle_client(void* arg) {
    int client_socket = *(int*)arg;
    free(arg);

    char buffer_str[2048];

    printf("Client %d connected.\n", client_socket);

    // Invia lista partite iniziale
    cJSON *welcome_msg = build_first_connection_message(client_socket, mem.match_list);
    char *json_str = cJSON_PrintUnformatted(welcome_msg);
    send(client_socket, json_str, strlen(json_str), 0);

    int read_bytes;
    while ((read_bytes = recv(client_socket, buffer_str, sizeof(buffer_str) - 1, 0)) > 0) {
        buffer_str[read_bytes] = '\0';

        generic_buffer buffer;
        convert_json_to_buffer(buffer_str, &buffer);

        switch(buffer.sig){
            case SIG_GUEST_REQUEST:
                int owner_socket = mem.match_list[buffer.guest_request.match_id].owner_id;
                send_message(owner_socket, MESSAGE_NOTIFICATION, &buffer);
                send_message(client_socket, MESSAGE_RESPONSE, &buffer);
            break;
            default:
                send_message(client_socket, MESSAGE_RESPONSE, &buffer);
            break;
        }
    }

    close(client_socket);
    return NULL;
}
