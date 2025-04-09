#ifndef TRIS_STRUCTS_H
#define TRIS_STRUCTS_H

#include "segnali.h"

#define MAX_ARRAY_LEN 100

enum stato_partita_enum {
    CREAZIONE,
    IN_ATTESA,
    IN_CORSO,
    TERMINATA
};

typedef enum simboli_partita_enum {
    X = 1,
    O = 2
} simboli_partita_enum;

typedef struct partite {
    int id_partita;
    int id_owner;
    int id_guest;
    enum stato_partita_enum stato_partita;
    int partita[3][3];
    int turno;
} partite;

typedef struct partite_giocatore {
    partite *partite;
    struct partite_giocatore *next;
} partite_giocatore;

typedef struct giocatore {
    int id_giocatore;
    int simbolo;
    partite *partita_corrente;
    partite_giocatore *partite_giocatore;
} giocatore;

#endif // TRIS_STRUCTS_H
