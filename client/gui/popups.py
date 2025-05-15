import customtkinter as ctk

from core import controller

FONT = ("Helvetica", 18)
TEXT_COLOR = "white"

class Popup:
    def __init__(self, master, title, message):
        self.master = master
        self.active_popup = None
        self.title = title
        self.message = message

    def show(self):
        if self.active_popup is None or not self.active_popup.winfo_exists():
            self.active_popup = ctk.CTkToplevel(self.master)
            self.active_popup.title(self.title)
            self.active_popup.geometry("300x150")  # Dimensioni predefinite
            self.active_popup.resizable(False, False)

            label = ctk.CTkLabel(self.active_popup, text=self.message, font=FONT)
            label.pack(padx=20, pady=20)

            close_button = ctk.CTkButton(self.active_popup, text="Chiudi", command=self.close)
            close_button.pack(pady=10)

            # Imposta il popup come modale (disabilita l'interazione con la finestra principale)
            self.active_popup.update()
            self.active_popup.grab_set()
            self.master.wait_window(self.active_popup)
        else:
            self.active_popup.focus() # Porta in primo piano se già esiste

    def close(self):
        if self.active_popup:
            self.active_popup.destroy()
            self.active_popup = None

class DrawPopup(Popup):
    def __init__(self, master, title, message):
        super().__init__(master, title, message)

    def show(self):
        if self.active_popup is None or not self.active_popup.winfo_exists():
            self.active_popup = ctk.CTkToplevel(self.master)
            self.active_popup.title(self.title)
            self.active_popup.geometry("300x150")
            self.active_popup.resizable(False, False)

            label = ctk.CTkLabel(self.active_popup, text=self.message, font=FONT)
            label.grid(row=0, column=0, columnspan=2, padx=20, pady=20)

            close_button_yes = ctk.CTkButton(self.active_popup, text="Si", command=lambda: self.send_draw(1))
            close_button_yes.grid(row=1, column=0, pady=10)

            close_button_no = ctk.CTkButton(self.active_popup, text="No", command=lambda: self.send_draw(2))
            close_button_no.grid(row=1, column=1, pady=10)

            self.active_popup.update()
            self.active_popup.grab_set()
            self.master.wait_window(self.active_popup)

    def send_draw(self, choice: int):
        import utils
        response = controller.send_draw_response(choice)
        if not response["restart"]:
            utils.load_home_page()
        else:
            utils.start_match()

        self.close()