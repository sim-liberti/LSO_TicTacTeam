from typing import Tuple

import core.globals as globals
from models import MatchState, MATCH_STATE_INFO

def get_client_id() -> int:
    return globals.client.client_id

def get_client_username() -> str:
    return globals.client.client_username

def get_match_list() -> list:
    return globals.match_list

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
