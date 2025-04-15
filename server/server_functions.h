#ifndef TRIS_FUNCS_H
#define TRIS_FUNCS_H

#include "types.h"
#include "segnali.h"

char* visualizza_partite(partita *);
char* crea_nuova_partita(buffer_nuova_partita *, partita *);
char* inserisci_mossa(buffer_nuova_mossa *, partita *, fine_partita_enum *);
char* gestisci_richiesta_guest(char *);
char* gestisci_pareggio(buffer_gestisci_pareggio *, partita *);
char* cancella_partita(int, partita *);

void json_to_buffer(char *, buffer_generico *);
char* buffer_to_json(partita);
int controlla_vittoria(int (*)[3], int, int);

#endif