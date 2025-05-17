#include "logic.h"
#include "utils.h"

cJSON* get_match_list(match *match_list){
    cJSON *array = cJSON_CreateArray();
    for (int i = 0; i < MAX_GAMES_NUM; i++) {
        if (match_list[i].owner_id != 0) {
            cJSON *p = cJSON_CreateObject();
            cJSON_AddNumberToObject(p, "match_id", match_list[i].match_id);
            cJSON_AddNumberToObject(p, "owner_id", match_list[i].owner_id);
            cJSON_AddStringToObject(p, "owner_username", match_list[i].owner_username);
            cJSON_AddNumberToObject(p, "match_state", match_list[i].match_state);
            cJSON_AddItemToArray(array, p);
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
            match_list[i].owner_username = buffer->owner_username;
            match_list[i].match_state = MATCH_STATE_CREATING;
            pthread_mutex_unlock(&mem.lock);
            return get_match_list(match_list);
        }
    }    

    cJSON *json = cJSON_CreateObject();
    cJSON_AddStringToObject(json, "error", "no free slot to create a match");
    return json;
}

cJSON* make_move(int socket_fd, make_move_buffer *buffer, match *match_list) {
    int match_id = buffer->match_id;
    int player_id = buffer->player_id;
    int x_coord = buffer->x_coord;
    int y_coord = buffer->y_coord;
    match *current_match = &match_list[match_id];
        
    char *match_outcome = "ongoing";
    if (current_match->match_state == MATCH_STATE_DRAW)
        match_outcome = "draw";
    else if (current_match->match_state == MATCH_STATE_COMPLETED)
        match_outcome = player_id == socket_fd ? "win" : "lose";

    cJSON *json = cJSON_CreateObject();
    cJSON_AddStringToObject(json, "match_outcome", match_outcome);
    cJSON_AddNumberToObject(json, "x_coord", x_coord);
    cJSON_AddNumberToObject(json, "y_coord", y_coord);

    return json;
}

cJSON* send_guest_request(int socket_fd, guest_request_buffer *buffer, match *match_list){
    int owner_id = match_list[buffer->match_id].owner_id;

    if (socket_fd == owner_id) {
        // Messaggio per l'owner: hai ricevuto una nuova richiesta
        cJSON* owner_payload = cJSON_CreateObject();
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

cJSON* send_guest_response(guest_response_buffer *buffer, match *match_list){
    match *current_match = &match_list[buffer->match_id];
    cJSON* json = cJSON_CreateObject();
    
    if (buffer->owner_answ == 0) {
        cJSON_AddStringToObject(json, "info", "request to join denied");
        cJSON_AddNumberToObject(json, "match_id", buffer->match_id);
        return json;
    }
    
    cJSON_AddNumberToObject(json, "match_id", current_match->match_id);
    cJSON_AddNumberToObject(json, "owner_id", current_match->owner_id);
    cJSON_AddStringToObject(json, "owner_username", current_match->owner_username);
    cJSON_AddNumberToObject(json, "guest_id", current_match->guest_id);
    cJSON_AddStringToObject(json, "guest_username", current_match->guest_username);
    cJSON_AddNumberToObject(json, "turn", current_match->turn);

    return json;
}

cJSON* handle_draw(handle_draw_buffer *buffer, match *match_list){
    cJSON* json = cJSON_CreateObject();
    int restart = 0;

    if (match_list[buffer->match_id].guest_id == 0) {
        cJSON_AddStringToObject(json, "info", "game ended");
    } else {
        restart = 1;
        cJSON_AddStringToObject(json, "info", "game restarted");
    }
    
    cJSON_AddNumberToObject(json, "restart", restart);
    return json;
}

cJSON* delete_match(int match_id, match *match_list){
    pthread_mutex_lock(&mem.lock);
    
    match *current_match = &match_list[match_id];
    // Cancella dalla lista delle partite del giocatore la match corrente da rimuovere
    current_match->match_id = 0;
    current_match->owner_id = 0;
    current_match->owner_username = "";
    current_match->guest_id = 0;
    current_match->guest_username = "";
    current_match->match_state = MATCH_STATE_CREATING;
    current_match->turn = 0;
    for(int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            current_match->grid[i][j] = 0;

    while (current_match->requests){
        match_request *tmp = current_match->requests;
        current_match->requests = current_match->requests->next;
        free(tmp);
    }

    pthread_mutex_unlock(&mem.lock);
    return get_match_list(match_list);
}

void start_match(guest_response_buffer *buffer, match *match_list) {
    if (buffer->owner_answ == 0) return;

    match *current_match = &match_list[buffer->match_id];
    current_match->guest_id = buffer->guest_id;
    current_match->match_state = MATCH_STATE_ONGOING;
    current_match->guest_username = buffer->guest_username;
    current_match->turn = rand() % 2;

    while (current_match->requests){
        match_request *tmp = current_match->requests;
        current_match->requests = current_match->requests->next;
        free(tmp);
    }
}

void update_match(make_move_buffer *buffer, match *match_list) {
    int match_id = buffer->match_id;
    int x_coord = buffer->x_coord;
    int y_coord = buffer->y_coord;
    int symbol = buffer->symbol;

    match *current_match = &match_list[match_id];

    current_match->grid[x_coord][y_coord] = symbol;
    current_match->turn = (current_match->turn + 1) % 2;
    
    if(check_win(current_match->grid, x_coord, y_coord))
        current_match->match_state = MATCH_STATE_COMPLETED;
    else if(check_draw(current_match->grid))
        current_match->match_state = MATCH_STATE_DRAW;
}

void clean_match(int player_id, match *current_match){
    if (current_match->owner_id != player_id){
        current_match->owner_id = current_match->guest_id;
        current_match->owner_username = current_match->guest_username;
    }
    current_match->guest_id = 0;
    current_match->guest_username = "";
    current_match->match_state = MATCH_STATE_CREATING;
    current_match->turn = 0;
    for(int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            current_match->grid[i][j] = 0;
}

void wait_draw(handle_draw_buffer *buffer, match *match_list){
    match *current_match = &match_list[buffer->match_id];
    pthread_mutex_lock(&current_match->lock);

    if (buffer->player_id == current_match->owner_id)
        current_match->draw.owner_answ = buffer->answ;
    else if (buffer->player_id == current_match->guest_id)
        current_match->draw.guest_answ = buffer->answ;

    while (current_match->draw.owner_answ == 0 || current_match->draw.guest_answ == 0)
        pthread_cond_wait(&current_match->cond, &current_match->lock);

    if (current_match->draw.draw_handled == 0) {
        int owner_answ = current_match->draw.owner_answ;
        int guest_answ = current_match->draw.guest_answ;
        
        if ((owner_answ == guest_answ) && (guest_answ == 1)){
            for(int i = 0; i < 3; i++)
                for (int j = 0; j < 3; j++)
                    current_match->grid[i][j] = 0;
        }
        else if ((owner_answ == guest_answ) && (guest_answ == 2))
            delete_match(buffer->match_id, match_list);
        else if (owner_answ == 2){
            clean_match(current_match->guest_id, current_match);
        } else {
            clean_match(current_match->owner_id, current_match);
        }
        current_match->draw.draw_handled = 1;
        int thread_signaled = pthread_cond_signal(&current_match->cond);
        printf("[DEBUG] Thread signaled %d\n", thread_signaled);
    } else {
        current_match->draw.owner_answ = 0;
        current_match->draw.guest_answ = 0;
        current_match->draw.draw_handled = 0;
    }
    
    pthread_mutex_unlock(&current_match->lock);
}

int remove_client_games(int player_id, match * match_list){
    int retval = 0;

    for (int i = 0; i < MAX_GAMES_NUM; i++) {
        if ((match_list[i].owner_id == player_id) && (match_list[i].guest_id==0))
            delete_match(i, match_list);
        else if ((match_list[i].owner_id == player_id) && (match_list[i].guest_id!=0)){
            //notifico il guest della disconnessione
            match *current_match = &match_list[i];
            pthread_mutex_lock(&current_match->lock);
            clean_match(match_list[i].guest_id, match_list);
            pthread_mutex_unlock(&current_match->lock);
            retval = current_match->owner_id;
        }
        else if ((match_list[i].guest_id == player_id)){
            //notifico l'owner della disconnessione
            match *current_match = &match_list[i];
            pthread_mutex_lock(&current_match->lock);
            clean_match(match_list[i].owner_id, match_list);
            pthread_mutex_unlock(&current_match->lock);
            retval = current_match->owner_id;
        }
    }
    return retval;
}