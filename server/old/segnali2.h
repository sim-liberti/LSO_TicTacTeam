#ifndef TRIS_SIGNALS_H
#define TRIS_SIGNALS_H

typedef enum segnali_buffer_enum {
    LISTA_PARTITE = 0,
    NUOVA_PARTITA = 1,
    NUOVA_MOSSA = 2,
    NOTIFICA_GUEST = 3,
    RISPOSTA_GUEST = 4,
    GESTISCI_PAREGGIO = 5,
    CANCELLA_PARTITA = 6
} segnali_buffer_enum;

typedef struct buffer_nuova_partita {
    int id_owner;
} buffer_nuova_partita;

typedef struct buffer_nuova_mossa {
    int id_partita;
    int id_giocatore;
    simboli_partita_enum simbolo;
    int coord_x;
    int coord_y;
    int turno;
} buffer_nuova_mossa;

typedef struct buffer_notifica_guest {
    int id_partita;
    int id_guest;
    int stato_guest;
} buffer_notifica_guest;

typedef struct buffer_risposta_guest {
    int id_partita;
    int id_guest;
    int risposta_owner;
} buffer_risposta_guest;

typedef struct buffer_gestisci_pareggio {
    int id_partita;
    int id_giocatore;
    int risposta;
} buffer_gestisci_pareggio;

typedef struct buffer_cancella_partita {
    int id_partita;
} buffer_cancella_partita;

typedef struct buffer_generico{
    segnali_buffer_enum segnale;
    union {
        buffer_nuova_partita nuova_partita;
        buffer_nuova_mossa nuova_mossa;
        buffer_notifica_guest notifica_guest;
        buffer_risposta_guest risposta_guest;
        buffer_gestisci_pareggio gestisci_pareggio;
        buffer_cancella_partita cancella_partita;
    };
} buffer_generico;

#endif