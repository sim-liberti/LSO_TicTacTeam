from enum import Enum
import json

class Signal(Enum):
    SIG_GET_MATCH_LIST    = 0  # Richiesta o invio della lista partite
    SIG_CREATE_NEW_MATCH  = 1  # Creazione nuova partita
    SIG_MAKE_MOVE         = 2  # Invio di una nuova mossa
    SIG_GUEST_REQUEST     = 3  # Notifica al proprietario che un guest vuole entrare
    SIG_GUEST_RESPONSE    = 4  # Risposta del proprietario alla richiesta guest
    SIG_HANDLE_DRAW       = 5  # Gestione della richiesta di pareggio
    SIG_DELETE_MATCH      = 6  # Cancellazione della partita
    SIG_DISCONNECTION_MSG = 7  # Invio messaggio di disconnessione

class MatchListBuffer:
    sig: Signal

    def __init__(self):
        self.sig = Signal.SIG_GET_MATCH_LIST

    def serialize(self):
        return json.dumps({
            'sig': self.sig.value
        }, default=str)

class CreateNewMatchBuffer:
    sig: Signal
    owner_id: int
    owner_username: str

    def __init__(self, owner_id: int, owner_username: str):
        self.sig = Signal.SIG_CREATE_NEW_MATCH
        self.owner_id = owner_id
        self.owner_username = owner_username

    def serialize(self):
        return json.dumps({
            'sig': self.sig.value,
            'new_match': {
                'owner_id': self.owner_id,
                'owner_username': self.owner_username
            }
        }, default=str)

class DeleteMatchBuffer:
    sig: Signal
    match_id: int

    def __init__(self, match_id: int):
        self.sig = Signal.SIG_DELETE_MATCH
        self.match_id = match_id

    def serialize(self):
        return json.dumps({
            'sig': self.sig.value,
            'delete_match': {
                'match_id': self.match_id
            }
        })

class GuestRequestBuffer():
    sig: Signal
    match_id: int
    guest_id: int
    guest_username: str

    def __init__(self, match_id: int, guest_id: int, guest_username: str):
        self.sig = Signal.SIG_GUEST_REQUEST
        self.match_id = match_id
        self.guest_id = guest_id
        self.guest_username = guest_username

    def serialize(self):
        return json.dumps({
            'sig': self.sig.value,
            'guest_request': {
                'match_id': self.match_id,
                'guest_id': self.guest_id,
                'guest_username': self.guest_username
            }
        }, default=str)
    
class GuestResponseBuffer():
    sig: Signal
    match_id: int
    guest_id: int
    guest_username: str
    owner_answ: int

    def __init__(self, match_id: int, guest_id: int, guest_username: str, owner_answ: int):
        self.sig = Signal.SIG_GUEST_RESPONSE
        self.match_id = match_id
        self.guest_id = guest_id
        self.guest_username = guest_username
        self.owner_answ = owner_answ

    def serialize(self):
        return json.dumps({
            'sig': self.sig.value,
            'guest_response': {
                'match_id': self.match_id,
                'guest_id': self.guest_id,
                'guest_username': self.guest_username,
                'owner_answ': self.owner_answ
            }
        }, default=str)

class MakeMoveBuffer():
    sig: Signal
    match_id :int
    player_id :int
    symbol :int
    x_coord :int
    y_coord :int
    turn :int

    def __init__(self, match_id: int, player_id: int, symbol: int, x_coord: int, y_coord: int, turn: int):
        self.sig = Signal.SIG_MAKE_MOVE
        self.match_id = match_id
        self.player_id = player_id
        self.symbol = symbol
        self.x_coord = x_coord
        self.y_coord = y_coord
        self.turn = turn

    def serialize(self):
        return json.dumps({
            'sig': self.sig.value,
            'make_move': {
                'match_id': self.match_id,
                'player_id': self.player_id,
                'symbol': self.symbol,
                'x_coord': self.x_coord,
                'y_coord': self.y_coord,
                'turn': self.turn
            }
        }, default=str)

class HandleDrawBuffer():
    sig: Signal
    match_id: int
    player_id: int
    answ: int

    def __init__(self, match_id: int, player_id: int, answ: int):
        self.sig = Signal.SIG_HANDLE_DRAW
        self.match_id = match_id
        self.player_id = player_id
        self.answ = answ

    def serialize(self):
        return json.dumps({
            'sig': self.sig.value,
            'handle_draw': {
                'match_id': self.match_id,
                'player_id': self.player_id,
                'answ': self.answ
            }
        }, default=str)