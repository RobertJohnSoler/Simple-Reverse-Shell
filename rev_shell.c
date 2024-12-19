#include <stdio.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <unistd.h>


void startWinsock();
SOCKET startSocket();
void connectToServer(struct sockaddr_in *serv_addr, SOCKET client_socket,  const char* server_ip);
void closeSocket(SOCKET client_socket);
void sendMsg(SOCKET client_socket, const char *msg);
void receiveCommand(SOCKET client_socket, char *rsp);

int main(){
    
    char cmd_buff[1024];
    char cwd[1024];

    SOCKET client_socket;
    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(8080);
    const char* server_ip = "127.0.0.1";

    startWinsock();
    client_socket = startSocket();
    connectToServer(&serv_addr, client_socket, server_ip);
    
    getcwd(cwd, 1024);
    sendMsg(client_socket, cwd);
    receiveCommand(client_socket, cmd_buff);
    printf("%s", cmd_buff);

    return 1;
}

void startWinsock() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("WSAStartup failed\n");
        exit(EXIT_FAILURE);
    }
}

SOCKET startSocket() {
    SOCKET client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket < 0) {
        printf("\n Socket creation error \n");
        WSACleanup();
        exit(EXIT_FAILURE);
    } else {
        printf("Socket created. \n");
    }
    return client_socket;
}

void connectToServer(struct sockaddr_in *serv_addr, SOCKET client_socket, const char* server_ip) {

    if (inet_pton(AF_INET, server_ip, &serv_addr->sin_addr) <= 0) {
        printf("\nInvalid address/ Address not supported \n");
        closesocket(client_socket);
        WSACleanup();
        exit(EXIT_FAILURE);
    } 
    if (connect(client_socket, (struct sockaddr *)serv_addr, sizeof(*serv_addr)) < 0) {
        printf("\nConnection Failed. \n");
        printf("%i", client_socket);
        closesocket(client_socket);
        WSACleanup();
        exit(EXIT_FAILURE);
    } else{
        printf("Client connected to server.\n");
    }
}

void sendMsg(SOCKET client_socket, const char *msg) {
    int sent = send(client_socket, msg, strlen(msg), 0);
    if (sent == -1){
        printf("Error sending message. The connection must have been cut.\n");
    } else{
        printf("%s sent, status = %i \n", msg, sent);
    }
}

void closeSocket(SOCKET client_socket) {
    closesocket(client_socket);
    WSACleanup();
}

void receiveCommand(SOCKET client_socket, char *rsp){
    int received = recv(client_socket, rsp, 1024, 0);
    if (received < 1){
        printf("Error receiving message.\n");
    }
}