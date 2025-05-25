from typing import Tuple

import core.globals as globals
import core.controller as controller
from models import MatchState, MATCH_STATE_INFO, Match
from gui.popups import Popup, DrawPopup

def get_client_id() -> int:
    return globals.client.client_id

def get_client_username() -> str:
    return globals.client.client_username

def get_client_ip() -> str:
    import socket
    return socket.gethostbyname(socket.gethostname())

def is_ip_valid(addr: str) -> bool:
    import socket

    if not ":" in addr:
        return False
    ip, port = addr.split(":")[0], addr.split(":")[1]
    if not port.isnumeric():
        return False
    if ip == "localhost":
        return True
    
    try:
        socket.inet_aton(ip)
        return True
    except socket.error:
        return False

    return False

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

def delete_match_notifications(match_id: int):
    globals.client.notifications_list = list(filter(
        lambda n: n["match_id"] != match_id, globals.client.notifications_list
    ))

def start_match(match_data: dict = {}):
    if match_data != {}:
        current_match = Match(
            match_data["match_id"],
            match_data["owner_id"],
            match_data["owner_username"],
            match_data["guest_id"],
            match_data["guest_username"],
            match_data["turn"]
        )
        globals.current_match = current_match
    else:
        globals.current_match.board = [["" for _ in range(3)] for _ in range(3)]

    switch_frame("Match")
    
def set_turn(turn_data: dict):
    globals.app.current_frame.grid_frame.refresh_grid(turn_data["x_coord"], turn_data["y_coord"])

    if turn_data["match_outcome"] == "draw":
        globals.app.current_frame.show_draw_frame()
    elif turn_data["match_outcome"] == "win":
        popup = Popup(globals.app, "info", "Hai vinto! >:)")
        popup.show()
        switch_frame("Home")
    elif turn_data["match_outcome"] == "lose":
        match=get_current_match()
        delete_match_notifications(match.match_id)
        popup = Popup(globals.app, "info", "Hai perso! <:(")
        popup.show()
        switch_frame("Home")



def force_win():
    popup = Popup(globals.app, "info", "L'avversario si è disconnesso, hai vinto!")
    popup.show()
    switch_frame("Home")
    