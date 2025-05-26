#include "handler.h"
#include "buffer.h"
#include "utils.h"

void handle_sigint(int sig) {
    printf("\n[INFO] SIGINT received. Exiting cleanly...\n");
    server_running = 0;
}

void* handle_client(void* arg) {
    int client_socket = *(int*)arg;
    free(arg);

    char buffer_str[2048];

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
                match *current_match = &mem.match_list[buffer.make_move.match_id];
                int owner_socket = mem.match_list[buffer.guest_request.match_id].owner_id;
                if ((current_match->owner_id == 0) || (current_match->owner_id == buffer.guest_request.guest_id) || (current_match->guest_id !=0) || (current_match->match_state != MATCH_STATE_CREATING)){
                    buffer.guest_request.match_id=-1;
                    send_message(client_socket, MESSAGE_RESPONSE, &buffer);
                }else{
                    send_message(owner_socket, MESSAGE_NOTIFICATION, &buffer);
                    send_message(client_socket, MESSAGE_RESPONSE, &buffer);
                }       
            break;
            case SIG_GUEST_RESPONSE:
                int guest_socket = buffer.guest_response.guest_id;
                
                int guest_state=send(guest_socket,"0",1,0);
                if (guest_state==-1){ 
                    buffer.guest_response.owner_answ=3;
                    send_message(client_socket, MESSAGE_RESPONSE, &buffer);
                } else if (check_in_game(guest_socket, mem.match_list)){
                    buffer.guest_response.owner_answ=4;
                    send_message(client_socket, MESSAGE_RESPONSE, &buffer);
                }else{
                    send_message(guest_socket, MESSAGE_ALERT, &buffer);
                    send_message(client_socket, MESSAGE_RESPONSE, &buffer);
                }   
            break;
            case SIG_MAKE_MOVE:
                int opponent_id;
                current_match = &mem.match_list[buffer.make_move.match_id];
                if (buffer.make_move.player_id == mem.match_list[buffer.make_move.match_id].owner_id)
                    opponent_id = mem.match_list[buffer.make_move.match_id].guest_id;
                else
                    opponent_id = mem.match_list[buffer.make_move.match_id].owner_id;

                pthread_mutex_lock(&current_match->lock);
                update_match(&buffer.make_move, mem.match_list);
                send_message(opponent_id, MESSAGE_ALERT, &buffer);
                send_message(buffer.make_move.player_id, MESSAGE_RESPONSE, &buffer);
                if (current_match->match_state == MATCH_STATE_COMPLETED)
                    clean_match(buffer.make_move.player_id, current_match);
                pthread_mutex_unlock(&current_match->lock);
            break;
            case SIG_HANDLE_DRAW:
                wait_draw(&buffer.handle_draw, mem.match_list);
                send_message(client_socket, MESSAGE_RESPONSE, &buffer);
            break;
            default:
                send_message(client_socket, MESSAGE_RESPONSE, &buffer);
            break;
        }
    }

    printf("[INFO] Client %d disconnected\n",client_socket);
    handle_client_disconnection(client_socket);
    
    close(client_socket);
    return NULL;
}

void handle_client_disconnection(int client_socket){
    int alert_socket = remove_client_games(client_socket, mem.match_list);
    if (alert_socket!=0)
        send_disconnection_message(alert_socket);
}