import customtkinter as ctk

import utils

class TrisGridFrame(ctk.CTkFrame):
    def __init__(self, master, match, client_id, **kwargs):
        super().__init__(master, **kwargs)

        self.match = match
        self.client_id = client_id
        self.symbol = "O" if self.client_id == self.match.owner_id else "X"
        self.is_my_turn = False
        if ((self.symbol == "O" and self.match.turn == 0) or
            (self.symbol == "X" and self.match.turn == 1)):
           self.is_my_turn = True
        self.board = self.match.board
        self.buttons = []

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
        self.is_my_turn = False
        for row in range(3):
            for col in range(3):
                self.board[row][col].configure(state="disabled")

        if self.board[row][col] == "":
            self.buttons[row][col].configure(text=self.symbol)

        # controller.make_move()

    def refresh_grid(self, row: int, col: int):
        symbol = "O" if self.symbol == "X" else "X"
        self.board[row][col].configure(text=symbol)

        self.is_my_turn = True
        for row in range(3):
            for col in range(3):
                if self.board[row][col] == "":
                    self.board[row][col].configure(state="normal")

class MatchFrame(ctk.CTkFrame):
    def __init__(self, master, **kwargs):
        super().__init__(master, **kwargs)

        self.match = utils.get_current_match()
        self.client_id = utils.get_client_id()
        self.title_label = ctk.CTkLabel(self, text=f"{self.match.owner_username} Vs {self.match.guest_username}", font=("Arial", 24))
        self.title_label.pack(pady=10)

        self.grid_frame = TrisGridFrame(self, self.match, self.client_id)
        self.grid_frame.pack(pady=10)

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