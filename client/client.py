import socket
import random

import utils

client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

server_address = ('localhost', 8080)
client_socket.connect(server_address)

client_id = client_socket.recv(1024)
data = client_socket.recv(1024)
print("Il tuo id è: ", client_id.decode())
print("Messaggio ricevuto dal server:", data.decode())


while(True):
    nuova_partita = input("Vuoi creare una partita (s/n/q - quit)? ")

    if nuova_partita == "s":
        partita = utils.BufferNuovaPartita()
    elif nuova_partita == "n":
        pass
    elif nuova_partita == "q":
        break

client_socket.close()