import customtkinter as ctk

from core import controller
from .home_frame import HomeFrame
from ..popups import Popup
import utils

FONT = ("Helvetica", 18)
TEXT_COLOR = "white"

class LoginFrame(ctk.CTkFrame):
    def __init__(self, master):
        super().__init__(master)
        
        ctk.CTkLabel(master=self, text="Entra Nel Gioco Tic Tac Toe!", text_color=TEXT_COLOR, font=("Helvetica", 30)).pack(padx=50, pady=50)
        self.username_entry = ctk.CTkEntry(master=self, width=300, placeholder_text="Scegli Username")
        self.username_entry.focus()  #barra di scrittura sul testo
        self.username_entry.pack()
        ctk.CTkButton(master=self, text="Enter", command=self.login).pack(padx=10, pady=10)

    def login(self):
        username = self.username_entry.get()
        
        if username == "":
            popup = Popup(master=self, title="Errore", message="Inserisci il tuo Username")
            popup.show()
            return

        logged_in, msg = controller.login(username)
        if logged_in:
            utils.switch_frame(HomeFrame)
        else:
            popup = Popup(master=self, title="Errore", message=msg)
            popup.show()
