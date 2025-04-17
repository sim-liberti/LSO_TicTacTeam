import socket, os, threading
from queue import Queue

import utils
import funcs

client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

server_address = ('localhost', 8080)
client_socket.connect(server_address)

os.system('cls' if os.name == 'nt' else 'clear')

first_conn_data = client_socket.recv(1024).decode().split("||")

client_id = int(first_conn_data[0])
print(f"Il tuo id è: {client_id}")

print("\nLista partite:")
funcs.stampa_lista_partite(first_conn_data[1])

coda_richieste = Queue()

def ricevi_richieste(sock):
    while True:
        try:
            msg = sock.recv(1024).decode()
            if msg:
                coda_richieste.put(msg)
            else:
                break
        except:
            print("Errore nella ricezione.")
            break
            
threading.Thread(target=ricevi_richieste, args=(client_socket,), daemon=True).start()

while(True):

    print('''\nScegli cosa fare
          - RICARICA LISTA (r)
          - NUOVA PARTITA(n)
          - ENTRA IN PARTITA(p)
          - ESCI(q)''')
    scelta = input(": ")

    while not coda_richieste.empty():
        print(f"Messaggio in attesa: {coda_richieste.get()}")

    if scelta == "r":

        segnale = utils.BufferListaPartite()
        client_socket.send(segnale.serialize().encode('utf-8'))
        lista_json = client_socket.recv(1024).decode()
        os.system('cls' if os.name == 'nt' else 'clear')
        funcs.stampa_lista_partite(lista_json)
        
    elif scelta == "n":

        partita = utils.BufferNuovaPartita(client_id)
        client_socket.send(partita.serialize().encode('utf-8'))
        data = client_socket.recv(1024)
        os.system('cls' if os.name == 'nt' else 'clear')
        funcs.stampa_lista_partite(data.decode())

    elif scelta == "p":
        
        id_partita = int(input("Scegli una partita (id): "))
        richiesta = utils.BufferRichiestaPartita(id_partita, client_id)
        client_socket.send(richiesta.serialize().encode('utf-8'))
        # Invio al server id_partita
        # Il server risponde con la griglia dopo aver controllato
        # gli id dei giocatori

    elif scelta == "q":
        break

client_socket.close()