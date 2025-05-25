import customtkinter as ctk
import tkinter as tk
from PIL import Image, ImageTk

from .frames.home_frame import HomeFrame
from .frames.login_frame import LoginFrame
from .frames.match_frame import MatchFrame

import os, platform

class App(ctk.CTk):
    def __init__(self):
        ctk.set_default_color_theme("dark-blue")
        super().__init__()
        self.geometry("900x700")
        self.title("APP")

        # self.frames = {}
        # for f in (LoginFrame, HomeFrame, MatchFrame):
        #     page_name = f.__name__
        #     frame = f(parent=self.container, controller=self)
        #     self.frames[page_name] = frame
       
        script_dir = os.path.dirname(os.path.abspath("./tic-tac-toe.png"))
        icon_path = os.path.join(script_dir, "gui/tic-tac-toe.png")

        try:
            image = Image.open(icon_path).convert("RGB")
            self.icon_img = ImageTk.PhotoImage(image)
            self.wm_iconphoto(True, self.icon_img)
            print("Icona caricata")
        except Exception as e:
            print(f"Errore icona: {e}")
    
        self.current_frame = None
        self.switch_frame("Login")

    def switch_frame(self, frame):
        if self.current_frame:
            self.current_frame.destroy()
        if frame=="Login":
            self.current_frame = LoginFrame(self)
        if frame=="Home":
            self.current_frame = HomeFrame(self)
        if frame=="Match":
            self.current_frame = MatchFrame(self)
        #self.current_frame = frame(self)
        #self.title(self.current_frame.page_title)
        self.current_frame.pack(fill="both", expand=True)
    
    # def show_frame(self, page_name):
    #     frame = self.frames[page_name]
    #     frame.tkraise()
    #     self.title(frame.page_title) 