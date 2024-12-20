#include <stdio.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <unistd.h>

int main(){
    const char* cmd_buff = "type README.md";
    char output_buff[1024];
    char cwd[1024];
    FILE *output;
    output = popen(cmd_buff, "r"); 
    while(1){
        char line[1024];
        char* read = fgets(line, 1024, output); 
        if (!read){
            break;
        }
        strcat(output_buff, line);
    }
    getcwd(cwd, 1024);
    strcat(output_buff, "cURR_dIR");
    strcat(output_buff, cwd);
    printf("output buffer: %s\n", output_buff);
    return 1;
}