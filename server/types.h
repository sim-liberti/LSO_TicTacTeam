#ifndef TRIS_STRUCTS_H
#define TRIS_STRUCTS_H

#include <pthread.h>
#include "segnali.h"

#define MAX_ARRAY_LEN 100

typedef struct {
    struct partita lista_partite[MAX_ARRAY_LEN];
    pthread_mutex_t lock;
} SharedMemory;

typedef enum stato_partita_enum {
    CREAZIONE,
    IN_ATTESA,
    IN_CORSO,
    TERMINATA
} stato_partita_enum;

typedef enum fine_partita_enum {
    VITTORIA,
    PAREGGIO
} fine_partita_enum;

typedef enum simboli_partita_enum {
    X = 1,
    O = 2
} simboli_partita_enum;

typedef struct partita {
    int id_partita;
    int id_owner;
    int id_guest;
    enum stato_partita_enum stato_partita;
    int griglia[3][3];
    int turno;

    gestione_pareggio pareggio;

    pthread_mutex_t lock;
    pthread_cond_t cond;
    pthread_cond_t cond_turno_owner;
    pthread_cond_t cond_turno_guest;
} partita;

typedef struct giocatore {
    int id_giocatore;
    int simbolo;
    partita *partita_corrente;
    partitite_giocatore *partite;
} giocatore;

typedef struct partitite_giocatore {
    int id_partita;
    partitite_giocatore *next;
} partitite_giocatore;

typedef struct gestione_pareggio {
    int risposta_owner;
    int risposta_guest;
} gestione_pareggio;

typedef struct gestione_turno {
    int turno_owner;
    int turno_guest;
} gestione_turno;

#endif // TRIS_STRUCTS_H
