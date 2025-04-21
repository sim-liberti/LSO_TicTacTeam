#ifndef TRIS_COMM_H
#define TRIS_COMM_H

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

#define MAX_GAMES_NUM 100
#define PORT 8080

typedef struct match{
    int match_id;
    int owner_id;
    int guest_id;
    //stato_partita_enum stato_partita;
    int grid[3][3];
    int turn;

    //gestione_pareggio pareggio;

    pthread_mutex_t lock;
    pthread_cond_t cond;
    pthread_cond_t cond_turno_owner;
    pthread_cond_t cond_turno_guest;
} match;

typedef struct {
    match match_list[100];
    pthread_mutex_t lock;
} SharedMemory;

extern SharedMemory mem;

#endif // TRIS_COMM_H