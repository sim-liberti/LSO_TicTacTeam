from time import sleep
import customtkinter as ctk
from PIL import Image, ImageTk

import core.globals as globals
from core.connection import ClientConnection

from gui.app import App
# from core.handler import handle_message

def main():
    ctk.set_appearance_mode("dark")
    ctk.set_default_color_theme("dark-blue")
    globals.app = App()
    #globals.app.iconphoto(False, ImageTk.PhotoImage(Image.open("./tic-tac-toe.png")))
    globals.app.mainloop()

if __name__ == "__main__":
    main()
