import customtkinter as ctk

from .frames.home_frame import HomeFrame
from .frames.login_frame import LoginFrame

class App(ctk.CTk):
    def __init__(self):
        ctk.set_default_color_theme("dark-blue")
        super().__init__()
        self.geometry("900x700")

        self.current_frame = None
        self.switch_frame(LoginFrame)

    def load_home_page(self):
        if self.current_frame:
            self.current_frame.destroy()
        self.current_frame = HomeFrame(self)
        self.current_frame.pack(fill="both", expand=True)

    def switch_frame(self, frame):
        if self.current_frame:
            self.current_frame.destroy()
        self.current_frame = frame(self)
        self.current_frame.pack(fill="both", expand=True)