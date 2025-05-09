from core.connection import ClientConnection
from . import globals
from . import buffers
import customtkinter as ctk

from typing import Tuple

def login(username: str) -> Tuple[bool, str]:
    try:
        globals.client = ClientConnection(globals.HOST, globals.PORT)
        globals.client.connect()
        if not globals.client.is_connected:
            raise Exception("Connection Refused")

        globals.client.first_msg_received.wait()
        globals.client.client_username = username
    except Exception as e:
        return (False, e)
    
    return (True, "")

def get_match_list() -> bool:
    buffer = buffers.MatchListBuffer()
    globals.client.send_message(buffer.serialize())
    response = globals.client.wait_response(timeout=150)
    if response:
        globals.match_list = response["match_list"]
        return True

    return False

def create_match() -> bool:
    buffer = buffers.CreateNewMatchBuffer(globals.client.client_id, globals.client.client_username)
    globals.client.send_message(buffer.serialize())
    response = globals.client.wait_response(timeout=150)
    if response:
        globals.match_list = response["match_list"]
        return True

    return False

def delete_match(match_id: int) -> bool:
    buffer = buffers.DeleteMatchBuffer(match_id)
    globals.client.send_message(buffer.serialize())
    response = globals.client.wait_response(timeout=150)
    if response:
        globals.match_list = response["match_list"]
        return True

    return False

def send_match_request(match_id: int, guest_id: int, guest_username: str) -> bool:
    buffer = buffers.GuestRequestBuffer(match_id, guest_id, guest_username)
    globals.client.send_message(buffer.serialize())
    response = globals.client.wait_response(timeout=150)
    if response:
        return True

    return False

def send_match_response(match_id: int, guest_id: int, guest_username: str, answer: int) -> dict:
    buffer = buffers.GuestResponseBuffer(match_id, guest_id, guest_username, answer)
    globals.client.send_message(buffer.serialize())
    response = globals.client.wait_response(timeout=150)
    if response is not None and response != {}:
        return response["match_data"]
    return {}

def send_draw_response(answ: int) -> bool:
    buffer = buffers.HandleDrawBuffer(globals.current_match, globals.player_id, answ)
    globals.client.send_message(buffer.serialize())
    response = globals.client.wait_response(timeout=150)
    if response:
        return True
    
    return False
    
def send_move():
    # Se arriva un messaggio con le coordinate
    # chiamo refresh_grid in globals.app.current_frame.refresh_grid(x, y1)
    pass


