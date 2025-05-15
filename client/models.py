from enum import Enum

class MatchState(Enum):
    MATCH_STATE_CREATING   = 0
    MATCH_STATE_WAITING    = 1
    MATCH_STATE_ONGOING    = 2
    MATCH_STATE_DRAW       = 3
    MATCH_STATE_COMPLETED  = 4

MATCH_STATE_INFO: dict = {
    MatchState.MATCH_STATE_CREATING: ("CREAZIONE", "green"),
    MatchState.MATCH_STATE_WAITING: ("IN ATTESA", "yellow"),
    MatchState.MATCH_STATE_ONGOING: ("IN CORSO", "yellow"),
    MatchState.MATCH_STATE_DRAW: ("PAREGGIO", "red"),
    MatchState.MATCH_STATE_COMPLETED: ("TERMINATA", "red"),
}

class Match:
    match_id: int
    owner_id: int
    owner_username: str
    guest_id: int
    guest_username: str
    turn: int
    board: list

    def __init__(self, match_id, owner_id, owner_username, guest_id, guest_username, turn):
        self.match_id = match_id
        self.owner_id = owner_id
        self.owner_username = owner_username
        self.guest_id = guest_id
        self.guest_username = guest_username
        self.turn = turn
        self.board = [["" for _ in range(3)] for _ in range(3)]

    def __str__(self):
        return f"match_id: {self.match_id}\nowner_id: {self.owner_id}\nowner_username: {self.owner_username}\nguest_id: {self.guest_id}\nguest_username: {self.guest_username}\nturn: {self.turn}\nboard: {self.board}"