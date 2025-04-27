from gui.app import App
from core.connection import ClientConnection

HOST = 'localhost'
PORT = 8080

app: App = None
client: ClientConnection = None
match_list = []
current_match = None
# socket_id = 4
# match_list = [
#     {
#         "match_id": 1,
#         "owner_id": 4,
#         "match_state": 1
#     },
#     {
#         "match_id": 2,
#         "owner_id": 5,
#         "match_state": 1
#     },
#     {
#         "match_id": 3,
#         "owner_id": 6,
#         "match_state": 1
#     },
#     {
#         "match_id": 4,
#         "owner_id": 7,
#         "match_state": 1
#     },
#     {
#         "match_id": 5,
#         "owner_id": 4,
#         "match_state": 1
#     },
#     {
#         "match_id": 6,
#         "owner_id": 8,
#         "match_state": 1
#     },
# ]