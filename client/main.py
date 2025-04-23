from time import sleep

import core.globals as globals
import core.buffers as buffers
import gui.cli as cli
from core.connection import ClientConnection
# from core.handler import handle_message

HOST = 'localhost'
PORT = 8080

def main():
    client = ClientConnection(HOST, PORT)
    client.connect()
    if not client.is_connected:
        return
    
    client.first_msg_received.wait()

    cli.print_message(client, "r", globals.match_list)

    while(client.is_connected):
        cli.print_menu()
        choice = input("> ").strip().lower()

        if choice == "r":
            buffer = buffers.MatchListBuffer()
            client.send_message(buffer.serialize())
            response = client.wait_response(timeout=150)
            if response:
                globals.match_list = response["match_list"]
                cli.print_message(client, choice, response)

        if choice == "n":
            buffer = buffers.CreateNewMatchBuffer(globals.socket_id)
            client.send_message(buffer.serialize())
            response = client.wait_response(timeout=150)
            if response:
                globals.match_list = response["match_list"]
                cli.print_message(client, choice, response)

        if choice == "q":
            client.disconnect()
            return

if __name__ == "__main__":
    main()
