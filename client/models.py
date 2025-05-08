from enum import Enum

class MatchState(Enum):
    MATCH_STATE_CREATING   = 0
    MATCH_STATE_WAITING    = 1
    MATCH_STATE_ONGOING    = 2
    MATCH_STATE_DRAW       = 3
    MATCH_STATE_COMPLETED  = 4

MATCH_STATE_INFO: dict = {
    MatchState.MATCH_STATE_CREATING: ("CREAZIONE", "green"),
    MatchState.MATCH_STATE_WAITING: ("IN ATTESA", "green"),
    MatchState.MATCH_STATE_ONGOING: ("IN CORSO", "yellow"),
    MatchState.MATCH_STATE_DRAW: ("PAREGGIO", "red"),
    MatchState.MATCH_STATE_COMPLETED: ("TERMINATA", "red"),
}

class Match:
    owner_id: int
    guest_id: int
    board: list

    def __init__(self, owner_id, guest_id, board):
        self.owner_id = owner_id
        self.guest_id = guest_id
        self.board = [["" for _ in range(3)] for _ in range(3)]