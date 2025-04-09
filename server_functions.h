#ifndef TRIS_FUNCS_H
#define TRIS_FUNCS_H

#include "types.h"
#include "segnali.h"

char* visualizza_partite(partite lista_partite[100]);
char* crea_nuova_partita(nuova_partita_sig *, partite *);
char* inserisci_mossa(nuova_mossa_sig *, partite *);
void gestisci_richiesta_guest(char *);
void gestisci_pareggio(char *);

void unpack_json_to_buffer(char *, buffer_generico_sig *);
char *pack_buffer_to_json(partite);

#endif