#ifndef TRIS_STRUCTS_H
#define TRIS_STRUCTS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#include <stdbool.h>

#ifdef _WIN32
// Implementazione Socket Windows
#include <Winsock2.h> 
#include <ws2tcpip.h>
#else
// Implementazione Socket Linux
#include <arpa/inet.h> 
#endif

#define MAX_ARRAY_LEN 100

typedef enum {
    CREAZIONE,
    IN_ATTESA,
    IN_CORSO,
    TERMINATA
} stato_partita_enum;

typedef enum {
    VITTORIA,
    PAREGGIO
} fine_partita_enum;

typedef enum {
    X = 1,
    O = 2
} simboli_partita_enum;

typedef struct gestione_pareggio{
    int risposta_owner;
    int risposta_guest;
} gestione_pareggio;

typedef struct partita{
    int id_partita;
    int id_owner;
    int id_guest;
    stato_partita_enum stato_partita;
    int griglia[3][3];
    int turno;

    gestione_pareggio pareggio;

    pthread_mutex_t lock;
    pthread_cond_t cond;
    pthread_cond_t cond_turno_owner;
    pthread_cond_t cond_turno_guest;
} partita;

typedef struct {
    struct partita lista_partite[100];
    pthread_mutex_t lock;
} SharedMemory;

typedef struct partitite_giocatore partitite_giocatore;
typedef struct partitite_giocatore {
    int id_partita;
    partitite_giocatore *next;
} partitite_giocatore;

typedef struct {
    int id_giocatore;
    int simbolo;
    partita *partita_corrente;
    partitite_giocatore *partite;
} giocatore;

typedef struct {
    int turno_owner;
    int turno_guest;
} gestione_turno;

#endif // TRIS_STRUCTS_H
