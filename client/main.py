import customtkinter as ctk

import core.globals as globals

from gui.app import App

def main():
    ctk.set_appearance_mode("dark")
    ctk.set_default_color_theme("dark-blue")
    globals.app = App()
    globals.app.mainloop()

if __name__ == "__main__":
    main()
