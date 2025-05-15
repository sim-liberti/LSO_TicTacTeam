from core.connection import ClientConnection
from . import globals
from . import buffers

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
    
    return True if response else False

def send_match_response(match_id: int, guest_id: int, guest_username: str, answer: int) -> dict:
    buffer = buffers.GuestResponseBuffer(match_id, guest_id, guest_username, answer)
    globals.client.send_message(buffer.serialize())
    response = globals.client.wait_response(timeout=150)
    if response is not None and response != {}:
        return response["match_data"]
    return {}

def send_draw_response(answ: int) -> dict:
    buffer = buffers.HandleDrawBuffer(globals.current_match.match_id, globals.client.client_id, answ)
    globals.client.send_message(buffer.serialize())
    response = globals.client.wait_response(timeout=150)

    if response is not None and response != {}:
        return response["draw_result"]

    return {}

def make_move(match_id: int, player_id: int, symbol: int, x_coord: int, y_coord: int, turn: int) -> dict:
    buffer = buffers.MakeMoveBuffer(match_id, player_id, symbol, x_coord, y_coord, turn)
    globals.client.send_message(buffer.serialize())
    response = globals.client.wait_response(timeout=150)

    return response["turn"]
