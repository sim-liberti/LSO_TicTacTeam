#include "logic.h"
#include "utils.h"

cJSON* get_match_list(match *match_list){
    cJSON *array = cJSON_CreateArray();
    for (int i = 0; i < MAX_GAMES_NUM; i++) {
        if (match_list[i].owner_id != 0) {
            cJSON *p = cJSON_CreateObject();
            cJSON_AddNumberToObject(p, "match_id", match_list[i].match_id);
            cJSON_AddNumberToObject(p, "owner_id", match_list[i].owner_id);
            cJSON_AddNumberToObject(p, "match_state", match_list[i].match_state);
            cJSON_AddItemToArray(array, p); // aggiunge la match all'array
        }
    }
    return array;
}

cJSON* create_new_match(create_new_match_buffer *buffer, match *match_list){
    pthread_mutex_lock(&mem.lock);

    for (int i = 0; i < MAX_GAMES_NUM; i++) {
        if (match_list[i].owner_id == 0){
            match_list[i].match_id = i;
            match_list[i].owner_id = buffer->owner_id;
            //match_list[i].stato_match = CREAZIONE;
            pthread_mutex_unlock(&mem.lock);
            return get_match_list(match_list);
        }
    }    

    cJSON *json = cJSON_CreateObject();
    cJSON_AddStringToObject(json, "error", "no free slot to create a match");
    return json;
}

cJSON* make_move(){
    return NULL;
}

cJSON* send_guest_request(int socket_fd, guest_request_buffer *buffer, match *match_list){
    int owner_id = match_list[buffer->match_id].owner_id;

    if (socket_fd == owner_id) {
        // Messaggio per l'owner: hai ricevuto una nuova richiesta
        cJSON* owner_payload = cJSON_CreateObject();
        cJSON_AddStringToObject(owner_payload, "info", "new match access request");
        cJSON_AddNumberToObject(owner_payload, "match_id", buffer->match_id);
        cJSON_AddNumberToObject(owner_payload, "guest_id", buffer->guest_id);
        cJSON_AddStringToObject(owner_payload, "guest_username", buffer->guest_username);
        return owner_payload;
    } 
    
    // Messaggio per il guest: conferma che la richiesta è stata inviata
    cJSON* guest_payload = cJSON_CreateObject();
    cJSON_AddStringToObject(guest_payload, "info", "request to join sent");
    cJSON_AddNumberToObject(guest_payload, "match_id", buffer->match_id);
    return guest_payload;
}

cJSON* send_guest_response(){
    return NULL;
}

cJSON* handle_draw(){
    return NULL;
}

cJSON* delete_match(delete_match_buffer *buffer, match *match_list){
    pthread_mutex_lock(&mem.lock);
    
    match *current_match = &match_list[buffer->match_id];
    // Cancella dalla lista delle partite del giocatore la match corrente da rimuovere
    current_match->match_id = 0;
    current_match->owner_id = 0;
    current_match->guest_id = 0;
    current_match->match_state = MATCH_STATE_CREATING;
    int i,j;
    for(i = 0; i < 3; i++)
        for (j = 0; j < 3; j++)
            current_match->grid[i][j] = 0;

    while (current_match->requests){
        match_request *tmp = current_match->requests;
        current_match->requests = current_match->requests->next;
        free(tmp);
    }

    pthread_mutex_unlock(&mem.lock);
    return get_match_list(match_list);
}
