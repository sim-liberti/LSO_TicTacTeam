import socket
import json
from dataclasses import asdict

import utils
import funcs

client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

server_address = ('localhost', 8080)
client_socket.connect(server_address)

client_id = int(client_socket.recv(1024).decode())
print(f"Il tuo id è: {client_id}")

data = client_socket.recv(1024)
print("\nLista partite:")
funcs.stampa_lista_partite(data.decode())

while(True):
    print("\nScegli cosa fare\n - RICARICA LISTA (r)\n - NUOVA PARTITA(n)\n - ESCI(q)")
    scelta = input(": ")

    if scelta == "r":

        segnale = utils.BufferListaPartite()
        client_socket.send(segnale.serialize().encode('utf-8'))
        lista_json = client_socket.recv(1024).decode()
        funcs.stampa_lista_partite(lista_json)
        
    elif scelta == "n":

        partita = utils.BufferNuovaPartita(client_id)
        client_socket.send(partita.serialize().encode('utf-8'))
        data = client_socket.recv(1024)
        funcs.stampa_lista_partite(data.decode())

    elif scelta == "q":
        break

client_socket.close()