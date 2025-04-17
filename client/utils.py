from enum import Enum
from dataclasses import dataclass, asdict
import json

class Segnale(Enum):
    LISTA_PARTITE = 0
    NUOVA_PARTITA = 1
    NUOVA_MOSSA = 2
    GESTISCI_GUEST = 3
    GESTISCI_PAREGGIO = 4
    CANCELLA_PARTITA = 5

class BufferListaPartite:
    sig: Segnale

    def __init__(self):
        self.sig = Segnale.LISTA_PARTITE

    def serialize(self):
        return json.dumps({
            'segnale': self.sig.value
        }, default=str)

class BufferNuovaPartita:
    sig: Segnale
    id_owner: int

    def __init__(self, id_owner: int):
        self.sig = Segnale.NUOVA_PARTITA
        self.id_owner = id_owner

    def serialize(self):
        return json.dumps({
            'segnale': self.sig.value,
            'nuova_partita': {
                'id_owner': self.id_owner
            }
        }, default=str)

class BufferRichiestaPartita():
    sig: Segnale
    id_partita: int
    id_guest: int

    def __init__(self, id_partita: int, id_guest: int):
        self.sig = Segnale.GESTISCI_GUEST
        self.id_partita = id_partita
        self.id_guest = id_guest

    def serialize(self):
        return json.dumps({
            'segnale': self.sig.value,
            'gestisci_guest': {
                'id_partita': self.id_partita,
                'id_guest': self.id_guest,
            }
        }, default=str)