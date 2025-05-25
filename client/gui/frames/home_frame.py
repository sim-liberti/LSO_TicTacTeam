import customtkinter as ctk

from core import controller
from ..popups import Popup
import utils

FONT = ("Helvetica", 18)
TEXT_COLOR = "white"

class HomeFrame(ctk.CTkFrame):
    page_title = "Home"
    def __init__(self, master):
        super().__init__(master)
        # Layout a griglia 3x1
        self.grid_rowconfigure(1, weight=1)
        self.grid_columnconfigure(0, weight=1)

        # Header
        self.header = FrameHeader(master=self)
        self.header.grid(row=0, column=0, sticky="nsew")
        
        # Middle container
        self.middle = ctk.CTkFrame(master=self)
        self.middle.grid(row=1, column=0, sticky="nsew")
        self.middle.grid_rowconfigure(0, weight=1)
        self.middle.grid_columnconfigure(0, weight=7)
        self.middle.grid_columnconfigure(1, weight=3)

        # Left frame
        self.left_frame = FrameMatches(master=self.middle)
        self.left_frame.grid(row=0, column=0, sticky="nsew", padx=5, pady=5)
        
        # Right frame
        self.right_frame = FrameYourMatches(master=self.middle)
        self.right_frame.grid(row=0, column=1, sticky="nsew", padx=5, pady=5)
        
        # Footer
        self.footer = FrameNotifications(master=self)
        self.footer.grid(row=2, column=0, sticky="nsew")

class FrameHeader(ctk.CTkFrame):
    def __init__(self, master, *args, **kwargs):
        super().__init__(
            master=master, 
            height=50, 
        )

        self.header_label=ctk.CTkLabel(master=self, text=f"Benvenuto {utils.get_client_username()}", text_color=TEXT_COLOR, font=("Helvetica",30))
        self.header_label.pack()

class FrameMatches(ctk.CTkScrollableFrame):
    def __init__(self, master, *args, **kwargs):
        super().__init__(
            master=master,
        )
        ctk.CTkLabel(master=self, text="Lista Partite Globali", text_color=TEXT_COLOR, font=FONT).grid(row=0, column=0, columnspan=4)

        ctk.CTkButton(master=self, text="Ricarica", command=self.refresh_matches).grid(row=1, column=0, columnspan=4)

        ctk.CTkLabel(master=self, text="Proprietario", text_color=TEXT_COLOR, font=FONT).grid(row=2, column=0, pady=10, padx=20, sticky="nwes")
        ctk.CTkLabel(master=self, text="Id", text_color=TEXT_COLOR, font=FONT).grid(row=2, column=1, pady=10, padx=20, sticky="nwes")
        ctk.CTkLabel(master=self, text="Stato", text_color=TEXT_COLOR, font=FONT).grid(row=2, column=2, pady=10, padx=20, sticky="nwes")

        self.matches = filter(lambda match: match["owner_id"] != utils.get_client_id(), utils.get_match_list())
        self.display_matches(self.matches)

    def add_game_row(self, row, owner, match_id, status, color):
        ctk.CTkLabel(master=self, text=owner, text_color=TEXT_COLOR, font=FONT).grid(row=row, column=0, pady=3, sticky="nwes")
        ctk.CTkLabel(master=self, text=match_id, text_color=TEXT_COLOR, font=FONT).grid(row=row, column=1, pady=3, sticky="nwes")
        ctk.CTkLabel(master=self, text=status, width=10, text_color=color, font=('Helvetica', 14)).grid(row=row, column=2, pady=3, sticky="nwes")
        if status == "CREAZIONE":
            ctk.CTkButton(master=self, text="Entra", textvariable=match_id, command=lambda: self.send_match_request(match_id)).grid(row=row, column=3, pady=3, sticky="nwes")

    def send_match_request(self,match_id):
        if controller.send_match_request(match_id, utils.get_client_id(), utils.get_client_username()):
            popup = Popup(self, "Info", "Richiesta inviata correttamente")
            popup.show()
        else:
            popup = Popup(self, "Info", "Match inesistente")
            popup.show()
            self.refresh_matches()

    def display_matches(self, match_list):
        row = 3
        for match in match_list:
            state_text, state_color = utils.get_match_state_info(int(match["match_state"]))
            self.add_game_row(row, match["owner_username"], match["match_id"], state_text, state_color)
            row += 1

    def refresh_matches(self):
        match_list_refreshed = controller.get_match_list()
        if match_list_refreshed:
            utils.switch_frame("Home")

class FrameYourMatches(ctk.CTkScrollableFrame):
    def __init__(self, master, *args, **kwargs):
        super().__init__(
            master=master,
        )

        self.your_matches_label = ctk.CTkLabel(master=self, text="Le tue partite", text_color=TEXT_COLOR, font=FONT)
        self.your_matches_label.grid(row=0, column=0, columnspan=2)
        
        self.matches = list(filter(lambda match: match["owner_id"] == utils.get_client_id(), utils.get_match_list()))
        self.display_matches(self.matches)

        self.create_match_btn = ctk.CTkButton(master=self, text="Crea Partita", command=self.create_match)
        self.create_match_btn.grid(row=len(self.matches)+2, column=0, columnspan=2, sticky="nsew")
        
    def add_game_row(self, row, match_id):
        ctk.CTkLabel(master=self, text=match_id, width=10, text_color=TEXT_COLOR, font=FONT).grid(row=row, column=0, pady=3, sticky="nwes")
        ctk.CTkButton(master=self, text="X", width=5, text_color="red", command=lambda: self.delete_match(match_id)).grid(row=row, column=1, pady=3, sticky="nwes")

    def display_matches(self, match_list):
        row = 1
        for match in match_list:
            self.add_game_row(row, match["match_id"])
            row += 1

    def create_match(self):
        match_created = controller.create_match()
        if match_created: 
            utils.switch_frame("Home")

    def delete_match(self, match_id):
        match_deleted = controller.delete_match(match_id)
        if not match_deleted:
            popup = Popup(self, "info", "Il match non ti appartiene più")
            popup.show()
            controller.get_match_list()
        utils.delete_match_notifications(match_id)
        utils.switch_frame("Home")

class FrameNotifications(ctk.CTkScrollableFrame):
    def __init__(self, master, *args, **kwargs):
        super().__init__(
            master=master, 
            height=40, 
        )
        self.notifications_count = 0
        self.notifications_label = ctk.CTkLabel(master=self, text="Notifiche", text_color=TEXT_COLOR, font=FONT)
        self.notifications_label.grid(row=0, column=0, columnspan=3)

        self.refresh_requests()

    def add_request(self, row, username: str, match_id: int, guest_id: int):
        msg = f"Il giocatore {username} vuole unirsi alla partita {match_id}"
        ctk.CTkLabel(master=self, text=msg, text_color=TEXT_COLOR, font=FONT).grid(row=row, column=0, padx=10, pady=10, sticky="nwes")
        ctk.CTkButton(master=self, text="V", text_color="green", font=FONT, command=lambda: self.send_match_response(match_id, guest_id, username, 1)).grid(row=row, column=1, padx=10, pady=10, sticky="nwes")
        ctk.CTkButton(master=self, text="X", text_color="red", font=FONT, command=lambda: self.send_match_response(match_id, guest_id, username, 0)).grid(row=row, column=2, padx=10, pady=10, sticky="nwes")

    def refresh_requests(self):
        notifications_list = utils.get_notifications_list()

        if len(notifications_list) != self.notifications_count:
            for widget in self.winfo_children():
                if widget != self.notifications_label:
                    widget.destroy()
            
            ctk.CTkLabel(master=self, text="Notifiche", text_color=TEXT_COLOR, font=FONT).grid(row=0, column=0, columnspan=3)

            row = 1
            for n in notifications_list:
                self.add_request(row, n["guest_username"], n["match_id"], n["guest_id"])
                row += 1

            self.notifications_count = len(notifications_list)
            
        self.after(5000, self.refresh_requests)

    def send_match_response(self, match_id: int, guest_id: int, guest_username: str, answer: int):
        match_data = controller.send_match_response(match_id, guest_id, guest_username, answer)
        utils.delete_notification(match_id, guest_id, guest_username, answer)
        if match_data == {} or "info" in match_data:
            self.refresh_requests()
            return
        if "info2" in match_data:
            popup = Popup(self, "info", "Giocatore disconnesso")
            popup.show()
            self.refresh_requests()
            return
        if answer == 1:
            utils.start_match(match_data)
