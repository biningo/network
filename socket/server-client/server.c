
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
/*
*Date: 2020/8/13-下午2:37
*Author:icepan
*Description:
*/

int main(){
    //插件套接字
    int serv_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    //套接字和端口绑定
    struct sockaddr_in serv_addr;
    //用0填充字段
    memset(&serv_addr, 0, sizeof(serv_addr));
    //使用IPv4
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); //将字符串转化为整数
    // 将主机的无符号短整形数转换成网络字节顺序
    serv_addr.sin_port = htons(10000);

    ////将套接字和IP 端口绑定
    bind(serv_socket, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

    //监听请求 由系统决定缓冲队列的长度
    //listen() 只是让套接字处于监听状态，并没有接收请求。接收请求需要使用 accept() 函数
    listen(serv_socket, SOMAXCONN);


    //接收客户端请求
    struct sockaddr_in clnt_addr;
    socklen_t clnt_addr_size = sizeof(clnt_addr);
    //获得client的socket
    int clnt_socket = accept(serv_socket, (struct sockaddr *)&clnt_addr, &clnt_addr_size);

    //发送数据
    char msg[] = "Hello,I'm server";
    write(clnt_socket, msg, sizeof(msg));

    close(serv_socket);
    close(clnt_socket);

    return 0;
}

