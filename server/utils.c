#include "utils.h"
#include "logic.h"

bool check_win(int match[3][3], int x_coord, int y_coord) {
    if ((match[x_coord][0] == match[x_coord][1]) && (match[x_coord][1] == match[x_coord][2]))
        return true;

    if ((match[0][y_coord] == match[1][y_coord]) && (match[1][y_coord] == match[2][y_coord]))
        return true;

    if ((x_coord == y_coord) && ((match[0][0] == match[1][1]) && (match[1][1] == match[2][2]))) 
        return true;
    
    if ((x_coord == 2-y_coord) && ((match[0][2] == match[1][1]) && (match[1][1] == match[2][0])))
        return true;

    return false; 
}

bool check_draw(int match[3][3]) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (match[i][j] == 0) {
                return false; // Ci sono ancora celle vuote, quindi non è un pareggio
            }
        }
    }
    return true;
}

void convert_json_to_buffer(char *json, generic_buffer *buffer){
    cJSON *json_obj = cJSON_Parse(json);
    if (json_obj == NULL) {
        //printf("Errore JSON");
        return;
    }
    cJSON *sig_item = cJSON_GetObjectItem(json_obj, "sig");
    if (sig_item == NULL || !cJSON_IsNumber(sig_item)){
        printf("[ERROR] Signal not found in buffer");
        return;
    }
    signals_enum sig = cJSON_GetNumberValue(sig_item);

    switch(sig){
        case SIG_GET_MATCH_LIST:
            buffer->sig = SIG_GET_MATCH_LIST;
        break;
        case SIG_CREATE_NEW_MATCH:
            buffer->sig = SIG_CREATE_NEW_MATCH;
            cJSON *new_mach_json = cJSON_GetObjectItem(json_obj, "new_match");
            if (new_mach_json == NULL) 
                return;
            buffer->new_match.owner_id = cJSON_GetNumberValue(
                cJSON_GetObjectItem(new_mach_json, "owner_id")
            );
            buffer->new_match.owner_username = cJSON_GetStringValue(
                cJSON_GetObjectItem(new_mach_json, "owner_username")
            );
        break;
        case SIG_MAKE_MOVE:
            buffer->sig = SIG_MAKE_MOVE;
            cJSON *make_move_json = cJSON_GetObjectItem(json_obj, "make_move");
            if (make_move_json == NULL) 
                return;
            buffer->make_move.match_id = cJSON_GetNumberValue(
                cJSON_GetObjectItem(make_move_json, "match_id")
            );
            buffer->make_move.player_id = cJSON_GetNumberValue(
                cJSON_GetObjectItem(make_move_json, "player_id")
            );
            buffer->make_move.symbol = cJSON_GetNumberValue(
                cJSON_GetObjectItem(make_move_json, "symbol")
            );
            buffer->make_move.x_coord = cJSON_GetNumberValue(
                cJSON_GetObjectItem(make_move_json, "x_coord")
            );   
            buffer->make_move.y_coord = cJSON_GetNumberValue(
                cJSON_GetObjectItem(make_move_json, "y_coord")
            );
        break;
        case SIG_GUEST_REQUEST:
            buffer->sig = SIG_GUEST_REQUEST;
            cJSON *guest_request_json = cJSON_GetObjectItem(json_obj, "guest_request");
            if (guest_request_json == NULL) 
                return;
            buffer->guest_request.match_id = cJSON_GetNumberValue(
                cJSON_GetObjectItem(guest_request_json, "match_id")
            );
            buffer->guest_request.guest_id = cJSON_GetNumberValue(
                cJSON_GetObjectItem(guest_request_json, "guest_id")
            );
            buffer->guest_request.guest_username = cJSON_GetStringValue(
                cJSON_GetObjectItem(guest_request_json, "guest_username")
            );
        break;
        case SIG_GUEST_RESPONSE:
            buffer->sig = SIG_GUEST_RESPONSE;
            cJSON *guest_response_json = cJSON_GetObjectItem(json_obj, "guest_response");
            if (guest_response_json == NULL) 
                return;
            buffer->guest_response.match_id = cJSON_GetNumberValue(
                cJSON_GetObjectItem(guest_response_json, "match_id")
            );
            buffer->guest_response.guest_id = cJSON_GetNumberValue(
                cJSON_GetObjectItem(guest_response_json, "guest_id")
            );
            buffer->guest_response.guest_username = cJSON_GetStringValue(
                cJSON_GetObjectItem(guest_response_json, "guest_username")
            );
            buffer->guest_response.owner_answ = cJSON_GetNumberValue(
                cJSON_GetObjectItem(guest_response_json, "owner_answ")
            );
        break;
        case SIG_HANDLE_DRAW:
            buffer->sig = SIG_HANDLE_DRAW;
            cJSON *handle_draw_json = cJSON_GetObjectItem(json_obj, "handle_draw");
            buffer->handle_draw.match_id = cJSON_GetNumberValue(
                cJSON_GetObjectItem(handle_draw_json, "match_id")
            );
            buffer->handle_draw.player_id = cJSON_GetNumberValue(
                cJSON_GetObjectItem(handle_draw_json, "player_id")
            );
            buffer->handle_draw.answ = cJSON_GetNumberValue(
                cJSON_GetObjectItem(handle_draw_json, "answ")
            );
        break;
        case SIG_DELETE_MATCH:
            buffer->sig = SIG_DELETE_MATCH;
            cJSON *delete_match_json = cJSON_GetObjectItem(json_obj, "delete_match");
            buffer->delete_match.match_id = cJSON_GetNumberValue(
                cJSON_GetObjectItem(delete_match_json, "match_id")
            );
        break;
    }
}

cJSON* build_first_connection_message(int socket_id, match *match_list) {
    cJSON *message = cJSON_CreateObject();

    cJSON_AddStringToObject(message, "type", "first_connection");

    cJSON *content = cJSON_CreateObject();
    cJSON_AddNumberToObject(content, "socket_id", socket_id);
    cJSON_AddItemToObject(content, "match_list", get_match_list(match_list));  // restituisce array o oggetto

    cJSON_AddItemToObject(message, "content", content);

    return message;
}

cJSON* build_message(int socket_fd, message_type_enum message_type, generic_buffer *buffer){
    cJSON *message = cJSON_CreateObject();

    switch(message_type) {
        case MESSAGE_NOTIFICATION:
            cJSON_AddStringToObject(message, "type", "notification");
        break;
        case MESSAGE_ALERT:
            cJSON_AddStringToObject(message, "type", "alert");
        break;
        case MESSAGE_RESPONSE:
            cJSON_AddStringToObject(message, "type", "response");
        break;
    }
    
    cJSON *content = cJSON_CreateObject();
    cJSON_AddNumberToObject(content, "sig", buffer->sig);
    switch(buffer->sig) {
        case SIG_GET_MATCH_LIST:
            cJSON_AddItemToObject(content, "match_list", get_match_list(mem.match_list));
        break;
        case SIG_CREATE_NEW_MATCH:
            cJSON_AddItemToObject(content, "match_list", create_new_match(&buffer->new_match, mem.match_list));
        break;
        case SIG_MAKE_MOVE:
            cJSON_AddItemToObject(content, "turn", make_move(socket_fd, &buffer->make_move, mem.match_list));
        break;
        case SIG_GUEST_REQUEST:
            content = send_guest_request(socket_fd, &buffer->guest_request, mem.match_list);
        break;
        case SIG_GUEST_RESPONSE:
            cJSON_AddItemToObject(content, "match_data", send_guest_response(&buffer->guest_response, mem.match_list));
        break;
        case SIG_HANDLE_DRAW:
            cJSON_AddItemToObject(content, "draw_result", handle_draw(&buffer->handle_draw, mem.match_list));
        break;
        case SIG_DELETE_MATCH:
            cJSON_AddItemToObject(content, "match_list", delete_match(buffer->delete_match.match_id, mem.match_list));
        break;
    }

    cJSON_AddItemToObject(message, "content", content);
    return message;
}

int send_message(int socket_fd, message_type_enum message_type, generic_buffer *buffer){
    cJSON *message = build_message(socket_fd, message_type, buffer);
    char *message_str = cJSON_PrintUnformatted(message);
    int sent = send(socket_fd, message_str, strlen(message_str), 0);
    
    free(message_str);
    cJSON_Delete(message);
    return sent;
}

int send_disconnection_message(int socket_fd) {
    cJSON *message = cJSON_CreateObject();
    cJSON_AddStringToObject(message, "type", "alert");

    cJSON *content = cJSON_CreateObject();
    cJSON_AddNumberToObject(content, "sig", SIG_DISCONNECTION_MSG);
    cJSON_AddStringToObject(content, "info", "client disconnected");
    
    cJSON_AddItemToObject(message, "content", content);

    char *message_str = cJSON_PrintUnformatted(message);
    int sent = send(socket_fd, message_str, strlen(message_str), 0);
    
    free(message_str);
    cJSON_Delete(message);
    return sent;
}
