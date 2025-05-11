from typing import Tuple

import core.globals as globals
import core.controller as controller
from models import MatchState, MATCH_STATE_INFO, Match
from gui.frames.match_frame import MatchFrame
from gui.popups import Popup

def get_client_id() -> int:
    return globals.client.client_id

def get_client_username() -> str:
    return globals.client.client_username

def get_match_list() -> list:
    return globals.match_list

def get_current_match() -> Match:
    return globals.current_match

def get_notifications_list() -> list:
    return globals.client.notifications_list

def switch_frame(frame):
    globals.app.switch_frame(frame)

def get_match_state_info(match_state: int) -> Tuple[str, str]:
    try:
        state = MatchState(match_state)
        return MATCH_STATE_INFO.get(state, ("SCONOSCIUTO", "red"))
    except ValueError:
        return ("Stato non valido", "red")

def delete_notification(match_id: int, guest_id: int, guest_username: str, answer: int):
        notifications_list = get_notifications_list()
        notifications_list.remove({
            "match_id": match_id,
            "guest_id": guest_id,
            "guest_username": guest_username
        })
        if answer == 0:
            return
        
        notifications_list = list(filter(
            lambda n: n["match_id"] != match_id, notifications_list
        ))

def start_match(match_data: dict):
    current_match = Match(
        match_data["match_id"],
        match_data["owner_id"],
        match_data["owner_username"],
        match_data["guest_id"],
        match_data["guest_username"],
        match_data["turn"]
    )
    globals.current_match = current_match
    print(f"[DEBUG] Current match set: {globals.current_match}")
    print("Switching frame...")
    switch_frame(MatchFrame)
    
def set_turn(turn_data: dict):

    if turn_data["match_outcome"] == "ongoing":
        globals.app.current_frame.grid_frame.refresh_grid(turn_data["x_coord"], turn_data["y_coord"])
    elif turn_data["match_outcome"] == "draw":
        pass
    elif turn_data["match_outcome"] == "win":
        popup = Popup(globals.app.current_frame, "info", "Hai vinto! >:)")
        popup.show()
        print("[DEBUG] Client won, switching frame...")
        globals.app.load_home_page()
    elif turn_data["match_outcome"] == "lose":
        popup = Popup(globals.app.current_frame, "info", "Hai perso! <:(")
        popup.show()
        print("[DEBUG] Client lost, switching frame...")
        globals.app.load_home_page()
    