import customtkinter as ctk

from core import controller
from ..popups import Popup
import utils

FONT = ("Helvetica", 18)
TEXT_COLOR = "white"

class LoginFrame(ctk.CTkFrame):
    page_title = "Login"

    def __init__(self, master):
        super().__init__(master)
        
        ctk.CTkLabel(master=self, text="Entra Nel Gioco Tic Tac Toe!", text_color=TEXT_COLOR, font=("Helvetica", 30)).pack(padx=50, pady=50)
        ctk.CTkLabel(master=self, text="Scegli un Username").pack()
        self.username_entry = ctk.CTkEntry(master=self, width=300, placeholder_text="Username")
        self.username_entry.focus()
        self.username_entry.pack()
        ctk.CTkLabel(master=self, text="Inserisci hostname o ip del server (http://ip:8080)").pack()
        self.host_entry = ctk.CTkEntry(master=self, width=300, textvariable=ctk.StringVar(self, f"{utils.get_client_ip()}:8080"))
        self.host_entry.pack()
        ctk.CTkButton(master=self, text="Enter", command=self.login).pack(padx=10, pady=10)

    def login(self):
        username = self.username_entry.get()
        hostname = f"{utils.get_client_ip()}:8080" if self.host_entry.get() == "" else self.host_entry.get()

        if username == "":
            popup = Popup(master=self, title="Errore", message="Inserisci il tuo Username")
            popup.show()
            return
        if not utils.is_ip_valid(hostname):
            popup = Popup(master=self, title="Errore", message="Hostname o ip in formato errato")
            popup.show()
            return

        ip, port = hostname.split(":")[0], hostname.split(":")[1]

        logged_in, msg = controller.login(username, ip, port)
        if logged_in:
            utils.switch_frame("Home")
        else:
            popup = Popup(master=self, title="Errore", message=msg)
            popup.show()
