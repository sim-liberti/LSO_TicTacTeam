from . import globals
from . import buffers

def get_match_list() -> bool:
    buffer = buffers.MatchListBuffer()
    globals.client.send_message(buffer.serialize())
    response = globals.client.wait_response(timeout=150)
    if response:
        globals.match_list = response["match_list"]
        return True

    return False

def create_match() -> bool:
    buffer = buffers.CreateNewMatchBuffer(globals.socket_id)
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

def send_match_request(match_id: int, guest_id: int) -> str:
    buffer = buffers.GuestRequestBuffer(match_id, guest_id)
    globals.client.send_message(buffer.serialize())
    response = globals.client.wait_response(timeout=150)
    if response:
        return globals.client.response_queue.get()

    return ""