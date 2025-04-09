#include "server_functions.h"

#include "cjson/cJSON.h"

char* visualizza_partite(partite lista_partite[100]){
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

char* crea_nuova_partita(nuova_partita_sig *nuova_partita, partite *lista_partite){
    int i = 0;

    for (i = 0; i < MAX_ARRAY_LEN; i++) {
        if (lista_partite[i].id_owner == 0){
            lista_partite[i].id_owner = nuova_partita->id_owner;
            lista_partite[i].stato_partita = CREAZIONE;
            return pack_buffer_to_json(lista_partite[i]);
        }
    }    
}

char* inserisci_mossa(nuova_mossa_sig *nuova_mossa, partite *lista_partite){
    int id_partita = nuova_mossa->id_partita;
    int coord_x = nuova_mossa->coord_x;
    int coord_y = nuova_mossa->coord_y;
    simboli_partita_enum simbolo = nuova_mossa->simbolo;

    partite partita_corrente = lista_partite[id_partita];

    partita_corrente.partita[coord_x][coord_y] = simbolo;
    partita_corrente.turno++;

    int vittoria = controlla_vittoria(partita_corrente.partita, coord_x, coord_y);

    if (vittoria){
        int i, j;
        partita_corrente.id_owner = nuova_mossa->id_giocatore;
        partita_corrente.id_guest = 0;
        partita_corrente.stato_partita = IN_ATTESA;
        for (i = 0; i < 3; i++)
            for (j = 0; j < 3; j++)
                partita_corrente.partita[i][j] = 0;   
        return "vittoria";
    }
    if (!vittoria && (partita_corrente.turno == 9))
        partita_corrente.stato_partita = TERMINATA;
        return "pareggio";
}

void gestisci_richiesta_guest(char *buffer){
    /*Il buffer contiene l'id della partita e l'id del guest
    invio un messaggio all'owner della partita del tipo "Il giocatore id_guest vuole unirsi alla partita id_partita, accetti?"
    se accetta aggiorno il guest della partita, lo status con "In_Corso" e la starto (aspetto le mosse)
    se rifiuta invio un messaggio al guest del tipo "Partita id_partita rifiutata"
    */
}

void gestisci_pareggio(char *buffer){
    /*Se entrambi accettano, pulisco la griglia della partita e setto lo stato della partita in In_Corso
    chi rifiuta viene buttato fuori
    se uno solo accetta, diventa l'owner, pulisco la griglia della partita e setto lo stato in "InAttesa"
    se entrambi rifiutano libero la partita con la funzione "Pulisci_partita"
    */
}

void unpack_json_to_buffer(char *json_input, buffer_generico_sig *buffer){
    cJSON *json_obj = cJSON_Parse(json_input);
    if (json_obj == NULL) {
        printf("Errore JSON");
        return -1;
    }
    segnali_buffer_enum segnale = cJSON_GetNumberValue(
        cJSON_GetObjectItem(json_obj, "segnale")
    );
    switch (segnale){
        case NUOVA_PARTITA:
            buffer->segnale = NUOVA_PARTITA;
            cJSON *nuova_partita_json = cJSON_GetObjectItem(json_obj, "nuova_partita");
            
            if (nuova_partita_json == NULL) 
                return -1;
            
            buffer->nuova_partita.id_owner = cJSON_GetNumberValue(
                cJSON_GetObjectItem(nuova_partita_json, "id_owner")
            );

            break;
        case NUOVA_MOSSA:
            buffer->segnale = NUOVA_MOSSA;
            cJSON *nuova_mossa_json = cJSON_GetObjectItem(json_obj, "nuova_mossa");

            if (nuova_mossa_json == NULL) 
                return -1;

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
        case GESTISCI_GUEST:
            break;
        case GESTISCI_PAREGGIO:
            break;
    }
 
}

char *pack_buffer_to_json(partite lista_partite) {
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
