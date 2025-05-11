import socket, json, threading, time
from queue import Queue

from . import globals
from .buffers import Signal
import utils

class ClientConnection:
    response_queue: Queue
    notifications_list: list
    alert_queue: Queue

    client_id: int
    client_username: str

    def __init__(self, host='localhost', port=8080):
        self.host = host
        self.port = port
        self.client_socket = None
        self.is_connected = False
        self.receive_thread = None
        self.first_msg_received = threading.Event()

    def connect(self):
        try:
            self.client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            self.client_socket.connect((self.host, self.port))
            self.is_connected = True
            self.response_queue = Queue()
            self.notifications_list = list()
            self.alert_queue = Queue()
            print(f"[INFO] Connected to {self.host} on port {self.port}")

            self.receive_thread = threading.Thread(target=self.receive_messages)
            self.receive_thread.daemon = True
            self.receive_thread.start()

            self.notifications_thread = threading.Thread(target=self.wait_notifications)
            self.notifications_thread.daemon = True
            self.notifications_thread.start()
        except Exception as e:
            print(f"[ERROR] Could not connect to server: {e}")
            self.is_connected = False

    def send_message(self, message: str):
        if not self.is_connected:
            print("[ERROR] Not connected to server.")
            return
        
        try:
            self.client_socket.sendall(message.encode('utf-8'))
            
        except Exception as e:
            print(f"[ERROR] Exception caught while sending message: {e}")

    def wait_response(self, timeout: int = 5):
        t0 = time.time()
        while time.time() - t0 < timeout:
            if not self.response_queue.empty():
                return self.response_queue.get()
            time.sleep(0.1)
        return None

    def wait_notifications(self):
        if globals.current_match == None:
            pass

    def handle_alert(self):
        alert = self.alert_queue.get()
        if alert["sig"] == Signal.SIG_GUEST_RESPONSE.value:
            utils.start_match(alert["match_data"])
        elif alert["sig"] == Signal.SIG_MAKE_MOVE.value:
            print(f"[DEBUG] Turn received: {alert["turn"]}")
            utils.set_turn(alert["turn"])

    def receive_messages(self):
        while self.is_connected:
            try:
                data = self.client_socket.recv(1024)
                if not data:
                    self.disconnect()
                    print("[ERROR] Server disconnected")
                    break
                message = data.decode()
                self.handle_message(message)
            except Exception as e:
                print(f"[ERROR] Error receiving message: {e}")
                break

    def handle_message(self, message):
        try:
            message_data = json.loads(message)
            message_type = message_data["type"]
            message_content = message_data["content"]

            if not isinstance(message_data, dict):
                raise json.JSONDecodeError("Invlid object.", '', 0)
            
            if message_type == "first_connection":
                self.client_id = message_content["socket_id"]
                globals.match_list = message_content["match_list"]
                if not self.first_msg_received.is_set():
                    self.first_msg_received.set()
                
            if message_type == "notification":
                self.notifications_list.append(message_content)

            if message_type == "alert":
                self.alert_queue.put(message_content)
                self.handle_alert()

            if message_type == "response":
                self.response_queue.put(message_content)
            
        except Exception as e:
            print(f"[ERROR] Exception while decoding message: {e}")

    def disconnect(self):
        if self.client_socket:
            self.client_socket.close()
            self.is_connected = False
            print("[INFO] Connection closed.")