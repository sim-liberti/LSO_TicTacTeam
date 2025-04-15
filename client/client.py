import socket

client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

server_address = ('localhost', 8080)
client_socket.connect(server_address)

data = client_socket.recv(1024)
print("Messaggio ricevuto dal server:", data.decode())

client_socket.close()