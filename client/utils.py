import core.globals as globals

def get_client_id() -> int:
    return globals.client.client_id

def get_client_username() -> str:
    return globals.client.username

def get_match_list():
    return globals.match_list

def switch_frame(frame):
    globals.app.switch_frame(frame)

