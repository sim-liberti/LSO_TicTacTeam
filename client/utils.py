from enum import Enum
import json

class Segnale(Enum):
    NUOVA_PARTITA = 1
    NUOVA_MOSSA = 2
    GESTISCI_GUEST = 3
    GESTISCI_PAREGGIO = 4
    CANCELLA_PARTITA = 5

class BufferNuovaPartita:
    sig: Segnale
    id_owner: int

    def serialize(self):
        return json.dumps(
            self, 
            default=lambda o: o.__dict__, 
            sort_keys=True, 
            indent=4
        )
