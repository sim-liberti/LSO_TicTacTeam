#ifndef TRIS_SIGNALS_H
#define TRIS_SIGNALS_H

#include "types.h"

typedef enum segnali_buffer_enum {
    NUOVA_PARTITA = 1,
    NUOVA_MOSSA = 2,
    GESTISCI_GUEST = 3,
    GESTISCI_PAREGGIO = 4
} segnali_buffer_enum;

typedef struct buffer_generico_sig{
    segnali_buffer_enum segnale;
    union {
        nuova_partita_sig nuova_partita;
        nuova_mossa_sig nuova_mossa;
        gestisci_guest_sig gestisci_guest;
        gestisci_pareggio_sig gestisci_pareggio;
    };
} buffer_generico_sig;

typedef struct nuova_partita_sig {
    int id_owner;
} nuova_partita_sig;

typedef struct nuova_mossa_sig {
    int id_partita;
    int id_giocatore;
    simboli_partita_enum simbolo;
    int coord_x;
    int coord_y;
    int turno;
} nuova_mossa_sig;

typedef struct gestisci_guest_sig {
    int id_partita;
    int id_guest;
    int stato_guest;
} gestisci_guest_sig;

typedef struct gestisci_pareggio_sig {
    int id_partita;
    int risposta_owner;
    int risposta_guest;
} gestisci_pareggio_sig;

#endif