#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

/*
*Author:icepan
*Date: 2020/8/13-下午8:56 
*Description:
*/

int main() {

    int server_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_addr.sin_port = htons(10000);
    bind(server_socket, (struct sockaddr *) &server_addr, sizeof(server_addr));
    listen(server_socket, SOMAXCONN);

    while (1) {
        struct sockaddr_in client_addr;
        socklen_t client_addr_size = sizeof(client_addr);
        int client_socket = accept(server_socket, (struct sockaddr *) &client_addr,&client_addr_size);
        char msg[1024] = "";
        int len = read(client_socket,msg, sizeof(msg));
        printf("%s\n",msg);
        write(client_socket,msg,len);
        close(client_socket);
        memset(msg,0, sizeof(msg));
    }
    close(server_socket);
}
