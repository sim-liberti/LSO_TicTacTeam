import os
from prettytable import PrettyTable

import core.globals as globals

def print_menu():
    print("\n======== MENU =========")
    print(" (r) reload match list")
    print(" (n) create new match")
    print(" (q) exit")

def print_message(client, choice: str, data: dict):
    os.system('cls' if os.name == 'nt' else 'clear')

    if choice == "r" or choice == "n":
        print_match_list()

def print_match_list():
    partite = globals.match_list
    table = PrettyTable()
    table.field_names = ["Match ID", "Owner ID", "MATCH STATUS"]
    for p in partite:
        table.add_row([p["match_id"], p["owner_id"], ""])
    print(table)

# cli.print_message(client, "r", globals.match_list)

#     while(client.is_connected):
#         cli.print_menu()
#         choice = input("> ").strip().lower()

#         if choice == "r":
#             buffer = buffers.MatchListBuffer()
#             client.send_message(buffer.serialize())
#             response = client.wait_response(timeout=150)
#             if response:
#                 globals.match_list = response["match_list"]
#                 cli.print_message(client, choice, response)

#         if choice == "n":
#             buffer = buffers.CreateNewMatchBuffer(globals.socket_id)
#             client.send_message(buffer.serialize())
#             response = client.wait_response(timeout=150)
#             if response:
#                 globals.match_list = response["match_list"]
#                 cli.print_message(client, choice, response)

#         if choice == "q":
#             client.disconnect()
#             return
