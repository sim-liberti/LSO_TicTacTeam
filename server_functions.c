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

void crea_nuova_partita(char *buffer){
    /*Il buffer in input contiene l'id dell'owner 
    scorre l'array delle partite fino a trovarne owner 0, se la trova l'aggiorna e restituisce il suo indice */
    
}

void inserisci_mossa(char *buffer){

}

void gestisci_richiesta_guest(char *buffer){

}

void gestisci_pareggio(char *buffer){

}
