class Match:
    owner_id: int
    guest_id: int
    board: list

    def __init__(self, owner_id, guest_id, board):
        self.owner_id = owner_id
        self.guest_id = guest_id
        self.board = [["" for _ in range(3)] for _ in range(3)]