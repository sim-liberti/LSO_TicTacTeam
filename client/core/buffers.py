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

    def __init__(self, owner_id: int):
        self.sig = Signal.SIG_CREATE_NEW_MATCH
        self.owner_id = owner_id

    def serialize(self):
        return json.dumps({
            'sig': self.sig.value,
            'new_match': {
                'owner_id': self.owner_id
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

    def __init__(self, match_id: int, guest_id: int):
        self.sig = Signal.SIG_GUEST_REQUEST
        self.match_id = match_id
        self.guest_id = guest_id

    def serialize(self):
        return json.dumps({
            'sig': self.sig.value,
            'guest_request': {
                'match_id': self.match_id,
                'guest_id': self.guest_id,
            }
        }, default=str)
    
class GuestResponseBuffer():
    sig: Signal
    match_id: int
    guest_id: int
    asnw: int

    def __init__(self, match_id: int, guest_id: int, asnw: int):
        self.sig = Signal.SIG_GUEST_RESPONSE
        self.match_id = match_id
        self.guest_id = guest_id
        self.asnw = asnw

    def serialize(self):
        return json.dumps({
            'sig': self.sig.value,
            'guest_response': {
                'match_id': self.match_id,
                'guest_id': self.guest_id,
                'asnw': self.asnw
            }
        }, default=str)