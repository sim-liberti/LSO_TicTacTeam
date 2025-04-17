import json
from prettytable import PrettyTable

def stampa_lista_partite(lista_json: str):
    data = json.loads(lista_json)
    partite = data["lista_partite"]
    table = PrettyTable()
    table.field_names = ["ID Partita", "ID Owner", "Stato Partita"]
    for p in partite:
        table.add_row([p["id_partita"], p["id_owner"], p["stato_partita"]])
    print(table)
