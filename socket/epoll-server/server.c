#include <netinet/in.h>
#include <sys/epoll.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <zconf.h>
/*
*Author:icepan
*Date: 2020/8/21-下午4:08 
*Description:
*/



#define MAXEPOLL 10

int main() {

    int listen_fd;
    int conn_fd;
    int epoll_fd;
    int wait_fds;
    int i;
    int n;

    struct sockaddr_in serverAddr;
    struct sockaddr_in clientAddr;

    struct epoll_event ev;
    struct epoll_event events[MAXEPOLL];

    char buf[1024];

    socklen_t len = sizeof(struct sockaddr_in);
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serverAddr.sin_port = htons(10000);

    //建立套接字
    if ((listen_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1) {
        printf("socket error\n");
        return 1;
    }

    //绑定
    if (bind(listen_fd, (struct sockaddr *) &serverAddr, sizeof(serverAddr)) == -1) {
        printf("bind error\n");
        return 1;
    }

    //监听
    if (listen(listen_fd, SOMAXCONN) == -1) {
        printf("listen error\n");
        return 1;
    }

    //创建epoll
    epoll_fd = epoll_create(MAXEPOLL);

    //注册listen_fd 监听连接事件 有客户端连接则触发
    ev.events = EPOLLIN | EPOLLET;
    ev.data.fd = listen_fd;
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, listen_fd, &ev) < 0) {
        printf("epoll ctl error\n");
        return 1;
    }

    int cur_fds = 1; //当前一共有多少个正在监听的客户端 初始状态只有一个 listen_fd
    while (1) {

        //client_fds 告诉内核一共有多少个fd需要监听
        if ((wait_fds = epoll_wait(epoll_fd, events, cur_fds, -1)) == -1) {
            printf("epoll wait error\n");
            close(listen_fd);
            return 1;
        }



        //遍历已经触发的事件fd
        for (i = 0; i < wait_fds; i++) {

            //如果是client连接时间 并且没有超过epoll的最大值 则处理连接事件，获取client_fd，加入到epoll中
            if (events[i].data.fd == listen_fd && cur_fds < MAXEPOLL) {

                //接受请求
                if ((conn_fd = accept(listen_fd, (struct sockaddr *) &clientAddr, &len)) == -1) {
                    printf("accept error\n");
                    return 1;
                }
                printf("%s[%d]:连接了！", inet_ntoa(clientAddr.sin_addr), htonl(clientAddr.sin_port));

                //注册到epoll
                ev.data.fd = conn_fd;
                if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, conn_fd, &ev) == -1) {
                    printf("epoll ctl error\n");
                    return 1;
                }

                //监听的事件个数+1
                ++cur_fds;

            } else { //如果是已经连接的client的读事件 则读取数据

                n = read(events[i].data.fd, buf, sizeof(buf));

                //如果对方关闭了连接
                if (n <= 0) {
                    close(events[i].data.fd); //关闭fd
                    epoll_ctl(epoll_fd, EPOLL_CTL_DEL, events[i].data.fd, NULL); //删除epoll监听项
                    printf("%d:离开了", ev.data.fd);
                    --cur_fds;
                } else {
                    write(events[i].data.fd, buf, n); //回写
                }
            }


        }
    }


}