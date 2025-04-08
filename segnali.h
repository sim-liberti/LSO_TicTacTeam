#ifndef TRIS_SIGNALS_H
#define TRIS_SIGNALS_H

#include "types.h"

enum segnali_buffer_enum {
    NUOVA_PARTITA = 1,
    NUOVA_MOSSA = 2,
    GESTISCI_GUEST = 3,
    GESTISCI_PAREGGIO = 4
};

typedef struct nuova_partita_sig {
    int segnale;
    int id_owner;
} nuova_partita_sig;

typedef struct nuova_mossa_sig {
    int segnale;
    int simbolo;
    int coord_x;
    int coord_y;
    int id_partita;
} nuova_mossa_sig;

typedef struct gestisci_guest_sig {
    int segnale;
    int id_partita;
    int id_guest;
    int stato_guest;
} gestisci_guest_sig;

typedef struct gestisci_pareggio_sig {
    int segnale;
    int id_partita;
    int risposta_owner;
    int risposta_guest;
} gestisci_pareggio_sig;

#endif