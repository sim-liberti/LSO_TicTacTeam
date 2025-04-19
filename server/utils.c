#include "utils.h"

void json_to_buffer(char *json, generic_buffer *buffer){
    cJSON *json_obj = cJSON_Parse(json);
    if (json_obj == NULL) {
        //printf("Errore JSON");
        return;
    }
    
    signals_enum sig = cJSON_GetNumberValue(
        cJSON_GetObjectItem(json_obj, "segnale")
    );

    switch(sig){
        case SIG_GET_MATCH_LIST:
            buffer->sig = SIG_GET_MATCH_LIST;
        break;
        case SIG_CREATE_NEW_MATCH:
            buffer->sig = SIG_CREATE_NEW_MATCH;
            cJSON *new_mach_json = cJSON_GetObjectItem(json, "new_match");
            if (new_mach_json == NULL) 
                return;
            buffer->new_match.owner_id = cJSON_GetNumberValue(
                cJSON_GetObjectItem(new_mach_json, "owner_id")
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
            // buffer->make_move.simbolo = cJSON_GetNumberValue(
            //     cJSON_GetObjectItem(make_move_json, "simbolo")
            // );
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
            buffer->guest_response.owner_answ = cJSON_GetNumberValue(
                cJSON_GetObjectItem(guest_response_json, "owner_answ")
            );
        break;
        case SIG_HANDLE_DRAW:
            buffer->sig = SIG_HANDLE_DRAW;
        break;
        case SIG_DELETE_MATCH:
            buffer->sig = SIG_DELETE_MATCH;
            cJSON *delete_match_json = cJSON_GetObjectItem(json_obj, "delete_match");
            buffer->delete_match.match_id = cJSON_GetNumberValue(
                cJSON_GetObjectItem(guest_response_json, "match_id")
            );
        break;
    }
}
