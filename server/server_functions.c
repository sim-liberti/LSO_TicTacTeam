#include "utils/cJSON.h"

#include "server_functions.h"

char* visualizza_partite(partita lista_partite[100]){
    cJSON *json = cJSON_CreateObject();
    cJSON *array = cJSON_CreateArray();

    for (int i = 0; i < 100; i++) {
        if (lista_partite[i].id_owner != 0) {
            cJSON *p = cJSON_CreateObject();
            cJSON_AddNumberToObject(p, "id_partita", lista_partite[i].id_partita);
            cJSON_AddNumberToObject(p, "id_owner", lista_partite[i].id_owner);
            cJSON_AddNumberToObject(p, "stato_partita", lista_partite[i].stato_partita);

            cJSON_AddItemToArray(array, p); // aggiunge la partita all'array
        }
    }
    cJSON_AddItemToObject(json, "lista_partite", array); 

    char *json_string = cJSON_PrintUnformatted(json);
    cJSON_Delete(json);
    return json_string;
}

char* crea_nuova_partita(buffer_nuova_partita *nuova_partita, partita *lista_partite){
    int i = 0;

    for (i = 0; i < MAX_ARRAY_LEN; i++) {
        if (lista_partite[i].id_owner == 0){
            lista_partite[i].id_partita = i;
            lista_partite[i].id_owner = nuova_partita->id_owner;
            lista_partite[i].stato_partita = CREAZIONE;
            return visualizza_partite(lista_partite);
        }
    }    
    
    return "Errore: Nessuno slot disponibile per creare una nuova partita.";
}

char* inserisci_mossa(buffer_nuova_mossa *nuova_mossa, partita *lista_partite, fine_partita_enum *stato_fine){
    int id_partita = nuova_mossa->id_partita;
    int coord_x = nuova_mossa->coord_x;
    int coord_y = nuova_mossa->coord_y;
    int id_giocatore = nuova_mossa->id_giocatore;
    simboli_partita_enum simbolo = nuova_mossa->simbolo;

    partita *partita_corrente = &lista_partite[id_partita];

    pthread_mutex_lock(&partita_corrente->lock);

    while ((id_giocatore == partita_corrente->id_owner && partita_corrente->turno != 0) ||
           (id_giocatore == partita_corrente->id_guest && partita_corrente->turno != 1)) {
        if (id_giocatore == partita_corrente->id_owner) {
            pthread_cond_wait(&partita_corrente->cond_turno_owner, &partita_corrente->lock);
        } else {
            pthread_cond_wait(&partita_corrente->cond_turno_guest, &partita_corrente->lock);
        }
    }

    partita_corrente->griglia[coord_x][coord_y] = simbolo;
    partita_corrente->turno = (partita_corrente->turno + 1) % 2;

    int vittoria = controlla_vittoria(partita_corrente->griglia, coord_x, coord_y);

    if (id_giocatore == partita_corrente->id_owner)
        pthread_cond_signal(&partita_corrente->cond_turno_guest);
    else if (id_giocatore == partita_corrente->id_guest)
        pthread_cond_signal(&partita_corrente->cond_turno_owner);

    char *response;

    if (vittoria)
        response = "";
        
    // if (vittoria){
    //     int i, j;
    //     partita_corrente->id_owner = nuova_mossa->id_giocatore;
    //     partita_corrente->id_guest = 0;
    //     partita_corrente->stato_partita = IN_ATTESA;
    //     for (i = 0; i < 3; i++)
    //         for (j = 0; j < 3; j++)
    //             partita_corrente->griglia[i][j] = 0;

    //     *stato_fine = VITTORIA;
    //     return "vittoria";
    // }
    // if (!vittoria && (partita_corrente->turno == 9))
    //     partita_corrente->stato_partita = TERMINATA;
    //     *stato_fine = PAREGGIO;
    //     return "pareggio";
    
    response = "Matrice aggiornata";
    
    pthread_mutex_unlock(&partita_corrente->lock);
    return response;
}

char* notifica_richiesta_guest(buffer_notifica_guest *buffer, partita *lista_partite, int *socket_destinatario){ 
    *socket_destinatario = lista_partite[buffer->id_partita].id_owner;
    char *response = malloc(300);
    sprintf(response, "{\"notifica\": \"Il giocatore %d vuole unirsi alla partita %d.\", \"id_guest\": %d, \"id_partita\": %d}", buffer->id_guest, buffer->id_partita, buffer->id_guest, buffer->id_partita);
    return response;
}

char* accetta_rifiuta_guest(buffer_risposta_guest *buffer, partita *lista_partite, int socket) {
    int risposta_owner = buffer->risposta_owner;
    char *response_owner = malloc(100);
    char *response_guest = malloc(100);
    
    if (!risposta_owner) {
        // Da mandare al guest
        sprintf(response_guest, "{\"notifica\": \"La richiesta alla partita %d è stata rifiutata.\"}", buffer->id_partita);
        return response_guest;
    }

    int i = 0;
    while ((lista_partite[i].id_guest != buffer->id_guest) && (i < MAX_ARRAY_LEN)){
        if (lista_partite[i].id_guest == buffer->id_guest) {
            // Da mandare all'owner
            sprintf(response_owner, "{\"notifica\": \"L'utente è già in partita\"}", buffer->id_partita);
            return response_owner;
        }
        i++;
    }

    partita *partita_corrente = &lista_partite[buffer->id_partita];
    partita_corrente->id_guest = buffer->id_guest;
    partita_corrente->stato_partita = IN_CORSO;

    sprintf(response_owner, "{\"segnale\": \"La partita sta per iniziare\"}", buffer->id_partita);

    send(socket, response_owner, strlen(response_owner), 0);
    send(buffer->id_guest, response_owner, strlen(response_owner), 0);
    return "";
}

char* gestisci_pareggio(buffer_gestisci_pareggio *buffer, partita *lista_partite){
    partita *partita_corrente = &lista_partite[buffer->id_partita];
    pthread_mutex_lock(&partita_corrente->lock);

    if (buffer->id_giocatore == partita_corrente->id_owner)
        partita_corrente->pareggio.risposta_owner = buffer->id_giocatore;
    else if (buffer->id_giocatore == partita_corrente->id_guest)
        partita_corrente->pareggio.risposta_guest = buffer->id_giocatore;

    while (partita_corrente->pareggio.risposta_owner == 0 || partita_corrente->pareggio.risposta_guest == 0)
        pthread_cond_wait(&partita_corrente->cond, &partita_corrente->lock);

    int risposta_owner = partita_corrente->pareggio.risposta_owner;
    int risposta_guest = partita_corrente->pareggio.risposta_guest;

    char *response;

    if (risposta_owner == risposta_guest == 1)
        response = "";  // Si pulisce la griglia
    else if (risposta_owner == risposta_guest == 2)
        response = cancella_partita(buffer->id_partita, lista_partite);
    else if (risposta_owner == 0)
        response = ""; // Guest è il nuovo owner e si pulisce la griglia
    else
        response = ""; // Owner non cambia e si pulisce la griglia

    pthread_mutex_unlock(&partita_corrente->lock);
    return response; 
}

char* cancella_partita(int id_partita, partita *lista_partite) {
    partita *partita_corrente = &lista_partite[id_partita];
    // Cancella dalla lista delle partite del giocatore la partita corrente da rimuovere
    partita_corrente->id_partita = 0;
    partita_corrente->id_owner = 0;
    partita_corrente->id_guest = 0;
    partita_corrente->turno = 0;
    partita_corrente->stato_partita = CREAZIONE;
    int i,j;
    for(i = 0; i < 3; i++)
        for (j = 0; j < 3; j++)
            partita_corrente->griglia[i][j] = 0;
    
    return visualizza_partite(lista_partite);
}

void json_to_buffer(char *json_input, buffer_generico *buffer){
    cJSON *json_obj = cJSON_Parse(json_input);
    if (json_obj == NULL) {
        //printf("Errore JSON");
        return;
    }
    segnali_buffer_enum segnale = cJSON_GetNumberValue(
        cJSON_GetObjectItem(json_obj, "segnale")
    );
    switch (segnale){
        case LISTA_PARTITE:
            buffer->segnale = LISTA_PARTITE;
            break;
        case NUOVA_PARTITA:
            buffer->segnale = NUOVA_PARTITA;
            cJSON *nuova_partita_json = cJSON_GetObjectItem(json_obj, "nuova_partita");
            
            if (nuova_partita_json == NULL) 
                return;
            
            buffer->nuova_partita.id_owner = cJSON_GetNumberValue(
                cJSON_GetObjectItem(nuova_partita_json, "id_owner")
            );

            break;
        case NUOVA_MOSSA:
            buffer->segnale = NUOVA_MOSSA;
            cJSON *nuova_mossa_json = cJSON_GetObjectItem(json_obj, "nuova_mossa");

            if (nuova_mossa_json == NULL) 
                return;

            buffer->nuova_mossa.id_partita = cJSON_GetNumberValue(
                cJSON_GetObjectItem(nuova_mossa_json, "id_partita")
            );
            buffer->nuova_mossa.id_giocatore = cJSON_GetNumberValue(
                cJSON_GetObjectItem(nuova_mossa_json, "id_giocatore")
            );
            buffer->nuova_mossa.simbolo = cJSON_GetNumberValue(
                cJSON_GetObjectItem(nuova_mossa_json, "simbolo")
            );
            buffer->nuova_mossa.coord_x = cJSON_GetNumberValue(
                cJSON_GetObjectItem(nuova_mossa_json, "coord_x")
            );   
            buffer->nuova_mossa.coord_y = cJSON_GetNumberValue(
                cJSON_GetObjectItem(nuova_mossa_json, "coord_y")
            );
            
            break;
        case NOTIFICA_GUEST:
            buffer->segnale = NOTIFICA_GUEST;
            cJSON *notifica_guest_json = cJSON_GetObjectItem(json_obj, "notifica_guest");

            if (notifica_guest_json == NULL) 
                return;

            buffer->notifica_guest.id_partita = cJSON_GetNumberValue(
                cJSON_GetObjectItem(notifica_guest_json, "id_partita")
            );
            buffer->notifica_guest.id_guest = cJSON_GetNumberValue(
                cJSON_GetObjectItem(notifica_guest_json, "id_guest")
            );
            
            break;
        case RISPOSTA_GUEST:
            buffer->segnale = RISPOSTA_GUEST;
            cJSON *risposta_guest_json = cJSON_GetObjectItem(json_obj, "risposta_guest");

            if (risposta_guest_json == NULL) 
                return;

            buffer->risposta_guest.id_partita = cJSON_GetNumberValue(
                cJSON_GetObjectItem(risposta_guest_json, "id_partita")
            );
            buffer->risposta_guest.id_guest = cJSON_GetNumberValue(
                cJSON_GetObjectItem(risposta_guest_json, "id_guest")
            );
            buffer->risposta_guest.risposta_owner = cJSON_GetNumberValue(
                cJSON_GetObjectItem(risposta_guest_json, "risposta_owner")
            );
            
            break;
        case GESTISCI_PAREGGIO:
            break;
    }
 
}

char *buffer_to_json(segnali_buffer_enum segnale, partita lista_partite) {
    return "";
}

int controlla_vittoria(int partita[3][3], int coord_x, int coord_y) {
    if (partita[coord_x][0] == partita[coord_x][1] == partita[coord_x][2])
        return 1;

    if (partita[0][coord_y] == partita[1][coord_y] == partita[2][coord_y])
        return 1;

    if ((coord_x == coord_y) && (partita[0][0] == partita[1][1] == partita[2][2])) 
        return 1;
    
    if ((coord_x == 2-coord_y) && (partita[0][2] == partita[1][1] == partita[2][0]))
        return 1;

    return 0; 
}

