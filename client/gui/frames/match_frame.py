import customtkinter as ctk

from core import controller
from models import Match
import utils

class TrisGridFrame(ctk.CTkFrame):
    page_title = "Partita"

    def __init__(self, master, match, client_id, **kwargs):
        super().__init__(master, **kwargs)

        self.match :Match = match
        self.client_id: int = client_id
        self.symbol: str = "O" if self.client_id == self.match.owner_id else "X"
        self.is_my_turn: bool = False
        if ((self.symbol == "O" and self.match.turn == 0) or
            (self.symbol == "X" and self.match.turn == 1)):
           self.is_my_turn = True
        self.board: list = self.match.board
        self.buttons: list = []

        for row in range(3):
            row_buttons = []
            for col in range(3):
                button = ctk.CTkButton(
                    master=self,
                    text="",
                    width=100,
                    height=100,
                    font=("Arial", 24),
                    command=lambda r=row, c=col: self.on_button_click(r, c)
                )
                button.grid(row=row, column=col, padx=5, pady=5)
                button.configure(state="normal" if self.is_my_turn else "disabled")
                row_buttons.append(button)
            self.buttons.append(row_buttons)

    def on_button_click(self, row: int, col: int):
        response = controller.make_move(self.match.match_id, self.client_id, 1 if self.symbol=="O" else 2, row, col, self.match.turn)
        if response["match_outcome"] == "ongoing":
            if self.board[row][col] == "":
                self.buttons[row][col].configure(text=self.symbol)
            for row in range(3):
                for col in range(3):
                    self.buttons[row][col].configure(state="disabled")
            self.is_my_turn = False
            self.match.turn = (self.match.turn + 1) % 2
        else:
            utils.set_turn(response)

    def refresh_grid(self, row: int, col: int):
        symbol = "O" if self.symbol == "X" else "X"
        self.board[row][col] = symbol
        self.buttons[row][col].configure(text=symbol)

        self.is_my_turn = True
        self.match.turn = (self.match.turn + 1) % 2
        for row in range(3):
            for col in range(3):
                if self.board[row][col] == "":
                    self.buttons[row][col].configure(state="normal")

    def handle_game_ended(self):
        pass

class MatchFrame(ctk.CTkFrame):
    def __init__(self, master, **kwargs):
        super().__init__(master, **kwargs)

        self.match = utils.get_current_match()
        self.client_id = utils.get_client_id()
        self.title_label = ctk.CTkLabel(self, text=f"{self.match.owner_username} Vs {self.match.guest_username}", font=("Arial", 24))
        ctk.CTkLabel(self, text=f"Giocatore: {utils.get_client_username()} ({utils.get_client_id()})").pack(pady=10)
        ctk.CTkLabel(self, text=f"Turno iniziale: {self.match.turn}").pack(pady=10)
        self.title_label.pack(pady=10)

        self.grid_frame = TrisGridFrame(self, self.match, self.client_id)
        self.grid_frame.pack(pady=10)

    def show_draw_frame(self):
        for row in range(3):
            for col in range(3):
                self.grid_frame.buttons[row][col].configure(state="disabled")

        draw_frame = ctk.CTkFrame(self)
        ctk.CTkLabel(draw_frame, text="Pareggio! Giochi ancora?").pack()
        close_button_yes = ctk.CTkButton(draw_frame, text="Si", command=lambda: self.send_draw(1))
        close_button_yes.pack()
        close_button_no = ctk.CTkButton(draw_frame, text="No", command=lambda: self.send_draw(2))
        close_button_no.pack()
        draw_frame.pack()

    def send_draw(self, choice: int):
        import utils
        response = controller.send_draw_response(choice)
        if not response["restart"]:
            utils.load_home_page()
        else:
            utils.start_match()

class TrisApp(ctk.CTk):
    def __init__(self):
        super().__init__()

        self.title("Match")
        self.geometry("400x450")

        self.main_frame = MatchFrame(self)
        self.main_frame.pack(padx=20, pady=20, fill="both", expand=True)

if __name__ == "__main__":
    ctk.set_appearance_mode("dark")
    ctk.set_default_color_theme("dark-blue")
    app = TrisApp()
    app.mainloop()
