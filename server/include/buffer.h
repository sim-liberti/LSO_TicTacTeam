#ifndef TRIS_BUFFER_H
#define TRIS_BUFFER_H

typedef enum signals_enum {
    SIG_GET_MATCH_LIST    = 0,  // Richiesta o invio della lista partite
    SIG_CREATE_NEW_MATCH  = 1,  // Creazione nuova partita
    SIG_MAKE_MOVE         = 2,  // Invio di una nuova mossa
    SIG_GUEST_REQUEST     = 3,  // Notifica al proprietario che un guest vuole entrare
    SIG_GUEST_RESPONSE    = 4,  // Risposta del proprietario alla richiesta guest
    SIG_HANDLE_DRAW       = 5,  // Gestione della richiesta di pareggio
    SIG_DELETE_MATCH      = 6,  // Cancellazione della partita
    SIG_DISCONNECTION_MSG = 7   // Invio messaggio di disconnessione
} signals_enum;

typedef struct create_new_match_buffer {
    int owner_id;
    char *owner_username;
} create_new_match_buffer;

typedef struct make_move_buffer {
    int match_id;
    int player_id;
    int symbol;
    int x_coord;
    int y_coord;
    int turn;
} make_move_buffer;

typedef struct guest_request_buffer {
    int match_id;
    int guest_id;
    char *guest_username;
} guest_request_buffer;

typedef struct guest_response_buffer {
    int match_id;
    int guest_id;
    char *guest_username;
    int owner_answ;
} guest_response_buffer;

typedef struct handle_draw_buffer {
    int match_id;
    int player_id;
    int answ;
} handle_draw_buffer;

typedef struct delete_match_buffer {
    int match_id;
} delete_match_buffer;

typedef struct generic_buffer {
    signals_enum sig;
    union {
        create_new_match_buffer new_match;
        make_move_buffer make_move;
        guest_request_buffer guest_request;
        guest_response_buffer guest_response;
        handle_draw_buffer handle_draw;
        delete_match_buffer delete_match;
    };
} generic_buffer;

#endif //TRIS_BUFFER_H