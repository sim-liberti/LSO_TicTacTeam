from gui.app import App
from core.connection import ClientConnection
from models import Match

HOST = 'localhost'
PORT = 8080

app: App = None
client: ClientConnection = None
match_list = []
current_match: Match = None
