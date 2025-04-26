import customtkinter as ctk

class TrisGridFrame(ctk.CTkFrame):
    def __init__(self, master, **kwargs):
        super().__init__(master, **kwargs)

        self.symbol = "X"
        self.board = [["" for _ in range(3)] for _ in range(3)]
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
                row_buttons.append(button)
            self.buttons.append(row_buttons)

    def on_button_click(self, row: int, col: int):
        for row in range(3):
            for col in range(3):
                self.board[row][col].configure(state="disabled")

        if self.board[row][col] == "":
            self.buttons[row][col].configure(text=self.symbol)

    def refresh_grid(self, row: int, col: int):
        symbol = "O" if self.symbol == "X" else "X"
        self.board[row][col].configure(text=symbol)

        for row in range(3):
            for col in range(3):
                if self.board[row][col] == "":
                    self.board[row][col].configure(state="normal")

class MainFrame(ctk.CTkFrame):
    def __init__(self, master, **kwargs):
        super().__init__(master, **kwargs)

        self.title_label = ctk.CTkLabel(self, text="Owner Vs Guest", font=("Arial", 24))
        self.title_label.pack(pady=10)

        self.grid_frame = TrisGridFrame(self)
        self.grid_frame.pack(pady=10)

class TrisApp(ctk.CTk):
    def __init__(self):
        super().__init__()

        self.title("Match")
        self.geometry("400x450")

        self.main_frame = MainFrame(self)
        self.main_frame.pack(padx=20, pady=20, fill="both", expand=True)

if __name__ == "__main__":
    ctk.set_appearance_mode("dark")
    ctk.set_default_color_theme("dark-blue")
    app = TrisApp()
    app.mainloop()