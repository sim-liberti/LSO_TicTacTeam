import socket, os, threading, json, time
from queue import Queue

import utils
import funcs

client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server_address = ('localhost', 8080)
client_socket.connect(server_address)

queue_risposte = Queue()
queue_notifiche = Queue()

first_conn_data = client_socket.recv(1024).decode().split("||")

os.system('cls' if os.name == 'nt' else 'clear')

client_id = int(first_conn_data[0])
print(f"Il tuo id è: {client_id}")

lista_partite = json.loads(first_conn_data[1])
print("\nLista partite:")
funcs.stampa_lista_partite(lista_partite)

def ricevi_messaggi(sock):
    while True:
        try:
            msg: str = sock.recv(1024).decode()
            if not msg:
                print("Connessione chiusa.")
                break

            try:
                data = json.loads(msg)
            except json.JSONDecodeError:
                queue_notifiche.put(msg)
                continue

            if isinstance(data, dict) and "notifica" in data:
                queue_notifiche.put(data)
                print(f"Notifica in attesa: {data}")
            else:
                queue_risposte.put(data)
                print(f"Segnale arrivato: {data}")

        except:
            print("Errore nella ricezione.")
            break

def attesa_risposta(timeout=5):
    t0 = time.time()
    while time.time() - t0 < timeout:
        if not queue_risposte.empty():
            return queue_risposte.get()
        time.sleep(0.1)
    return None

threading.Thread(target=ricevi_messaggi, args=(client_socket,), daemon=True).start()

while(True):

    print('''\nScegli cosa fare
    - RICARICA LISTA (r)
    - NUOVA PARTITA (n)
    - ENTRA IN PARTITA (p)
    - ACCETTA RICHIESTA (a)
    - ESCI (q)''')
    scelta = input(": ")

    if scelta == "r":

        buffer = utils.BufferListaPartite()
        client_socket.send(buffer.serialize().encode('utf-8'))
        risposta = attesa_risposta()
        if risposta:
            lista_partite = risposta
            os.system('cls' if os.name == 'nt' else 'clear')
            funcs.stampa_lista_partite(risposta)
            
    elif scelta == "n":

        buffer = utils.BufferNuovaPartita(client_id)
        client_socket.send(buffer.serialize().encode('utf-8'))
        risposta = attesa_risposta()
        if risposta:
            lista_partite = risposta
            os.system('cls' if os.name == 'nt' else 'clear')
            funcs.stampa_lista_partite(risposta)

    elif scelta == "p":
        
        id_partita = int(input("Scegli una partita (id): "))
        buffer = utils.BufferRichiestaPartita(id_partita, client_id)
        client_socket.send(buffer.serialize().encode('utf-8'))
        risposta = attesa_risposta()
        if risposta:
            os.system('cls' if os.name == 'nt' else 'clear')
            print("Richiesta per la partita inviata.")
            funcs.stampa_lista_partite(lista_partite)

    elif scelta == "a":
        
        print("...")

        if queue_notifiche.empty():
            print("nessuna notifica")
            continue

        data = queue_notifiche.get()
        id_partita = data["id_partita"]
        id_guest = data["id_guest"]
        buffer = utils.BufferAccettaRichiesta(id_partita, id_guest, 1)
        client_socket.send(buffer.serialize().encode('utf-8'))
        risposta = attesa_risposta()
        if risposta:
            print(risposta)

    elif scelta == "q":
        break

client_socket.close()