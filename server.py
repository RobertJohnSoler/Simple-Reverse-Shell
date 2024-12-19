import socket

def start_server():
    s = socket.socket()
    print("")
    print("Socket created.")
    s.bind(('0.0.0.0', 8080))
    print("Socket binded to 8080")
    print("")
    s.listen(5)
    print("Socket is listening...")

    c, addr = s.accept()
    print("Got connection from reverse shell in ", addr)

if __name__ == "__main__":
    start_server()