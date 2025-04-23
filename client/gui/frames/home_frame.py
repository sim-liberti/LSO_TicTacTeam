import customtkinter as ctk

FONT = ("Helvetica", 18)
TEXT_COLOR = "white"

class HomeFrame(ctk.CTkFrame):
    def __init__(self, master):
        super().__init__(master)

        # Layout a griglia 3x1
        self.grid_rowconfigure(1, weight=1)  # Zona centrale espandibile
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
            #fg_color="lightblue"
        )
        #Il Nickname deve essere ottenuto dalla TextBox della prima GUI
        self.header_label=ctk.CTkLabel(master=self, text="Benvenuto Nickname", text_color=TEXT_COLOR, font=("Helvetica",30))
        self.header_label.pack()

class FrameMatches(ctk.CTkScrollableFrame):
    def __init__(self, master, *args, **kwargs):
        super().__init__(
            master=master,
            #fg_color="lightgreen"
        )
        ctk.CTkLabel(master=self, text="Lista Partite Globali", text_color=TEXT_COLOR, font=FONT).grid(row=0, column=0, columnspan=3)
        ctk.CTkLabel(master=self, text="Proprietario", text_color=TEXT_COLOR, font=FONT).grid(row=1, column=0, pady=10, padx=20, sticky="nwes")
        ctk.CTkLabel(master=self, text="Id", text_color=TEXT_COLOR, font=FONT).grid(row=1, column=1, pady=10, padx=20, sticky="nwes")
        ctk.CTkLabel(master=self, text="Stato", text_color=TEXT_COLOR, font=FONT).grid(row=1, column=2, pady=10, padx=20, sticky="nwes")

        self.add_game_row(2, "Cristian", "017", "Terminata", "red")
        self.add_game_row(3, "Stefano", "017", "In Corso", "green")
        self.add_game_row(4, "Simone", "005", "Attesa", "orange")
        self.add_game_row(5, "Cristian", "032", "Terminata", "red")
        self.add_game_row(6, "Stefano", "014", "In Corso", "green")

    def add_game_row(self, row, owner, match_id, status, color):
        ctk.CTkLabel(master=self, text=owner, text_color=TEXT_COLOR, font=FONT).grid(row=row, column=0, pady=3, sticky="nwes")
        ctk.CTkLabel(master=self, text=match_id, text_color=TEXT_COLOR, font=FONT).grid(row=row, column=1, pady=3, sticky="nwes")
        ctk.CTkLabel(master=self, text=status, width=10, text_color=color, font=FONT).grid(row=row, column=2, pady=3, sticky="nwes")
        ctk.CTkButton(master=self, text="Entra", textvariable=match_id, command=lambda: self.send_request(match_id)).grid(row=row, column=3, pady=3, sticky="nwes")

    def send_request(self, match_id):
        print(f"Request Sent for: {match_id}")


class FrameYourMatches(ctk.CTkScrollableFrame):
    def __init__(self, master, *args, **kwargs):
        super().__init__(
            master=master,
            #fg_color="lightyellow"
        )
        self.your_matches_label=ctk.CTkLabel(master=self, text="Le tue partite", text_color=TEXT_COLOR, font=FONT)
        self.your_matches_label.grid(row=0, column=0, columnspan=2)
        self.create_game(1, "002")
        self.create_game(2, "005")
        
    #funzione creazione di un game
    def create_game(self, row, match_id):
        ctk.CTkLabel(master=self, text=match_id, width=10, text_color=TEXT_COLOR, font=FONT).grid(row=row, column=0, pady=3, sticky="nwes")
        ctk.CTkButton(master=self, text="X", width=5, text_color="red", command=lambda: self.delete_match(match_id)).grid(row=row, column=1, pady=3, sticky="nwes")

    def delete_match(self, match_id):
        print(f"Delete match: {match_id}")

 
class FrameNotifications(ctk.CTkScrollableFrame):
    def __init__(self, master, *args, **kwargs):
        super().__init__(
            master=master, 
            height=40, 
            #fg_color="lightgray"
        )
        self.notifications_label=ctk.CTkLabel(master=self, text="Notifiche", text_color=TEXT_COLOR, font=FONT)
        self.notifications_label.grid(row=0, column=0, columnspan=3)

        for i in range(5):
            self.add_request(i+1)

    def add_request(self, row, nickname="Nickname", match_id="ID"):
        msg = f"Il giocatore {nickname} vuole unirsi alla partita {match_id}"
        ctk.CTkLabel(master=self, text=msg, text_color=TEXT_COLOR, font=FONT).grid(row=row, column=0, padx=10, pady=10, sticky="nwes")
        ctk.CTkButton(master=self, text="V", text_color="green", font=FONT, command=lambda: self.accept_match(match_id)).grid(row=row, column=1, padx=10, pady=10, sticky="nwes")
        ctk.CTkButton(master=self, text="X", text_color="red", font=FONT, command=lambda: self.reject_match(match_id)).grid(row=row, column=2, padx=10, pady=10, sticky="nwes")

    def accept_match(self, match_id):
        print(f"Accept match: {match_id}")

    def reject_match(self, match_id):
        print(f"Reject match: {match_id}")