from time import sleep

import core.globals as globals
import core.buffers as buffers
from core.connection import ClientConnection

from gui.app import App
import customtkinter as ctk
# from core.handler import handle_message

HOST = 'localhost'
PORT = 8080

def main():
    globals.client = ClientConnection(HOST, PORT)
    globals.client.connect()
    if not globals.client.is_connected:
        return
    
    globals.client.first_msg_received.wait()

    ctk.set_appearance_mode("dark")
    ctk.set_default_color_theme("dark-blue")
    globals.app = App()
    globals.app.mainloop()
    

if __name__ == "__main__":
    main()
