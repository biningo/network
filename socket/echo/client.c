#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

/*
*Author:icepan
*Date: 2020/8/13-下午9:10 
*Description:
*/
int main() {

    int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

//向服务器（特定的IP和端口）发起请求
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));           //每个字节都用0填充
    serv_addr.sin_family = AF_INET;                     //使用IPv4地址
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); //具体的IP地址
    serv_addr.sin_port = htons(10000);                  //端口

    connect(sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr));


    char msg[1024]="icepan";
    write(sock,msg, sizeof(msg));
    memset(msg,0, sizeof(msg));
    read(sock, msg, sizeof(msg) - 1);
    printf("%s\n", msg);
    close(sock);
    return 0;
}