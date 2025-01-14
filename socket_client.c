#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <stdio.h>
#include <string.h>


int main()
{
    int status, client_fd;
    struct sockaddr_in serv_addr;
    const char* hello = "Hello from my C client! \n";
    char buffer[1024] = { 0 };
    WSADATA wsaData;
    const char* server_ip = "127.0.0.1"; // change this to your server machine's IP address if you are trying to test sockets across separate machines.

    // start winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("WSAStartup failed\n");
        return 1;
    }

    // initialize socket
    if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        WSACleanup();
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(8080);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, server_ip, &serv_addr.sin_addr) <= 0) {
        printf("\nInvalid address/ Address not supported \n");
        closesocket(client_fd);
        WSACleanup();
        return -1;
    }

    // connect to server
    if ((status = connect(client_fd, (struct sockaddr*)&serv_addr,sizeof(serv_addr))) < 0) {
        printf("\nConnection Failed \n");
        closesocket(client_fd);
        WSACleanup();
        return -1;
    }

    // send message to server
    printf("Sending hello message from the C client to the server... \n");
    send(client_fd, hello, strlen(hello), 0);
    printf("Hello message sent\n");

    // closing the connected socket
    closesocket(client_fd);
    WSACleanup();
    printf("Client socket closed.");
    return 0;
}