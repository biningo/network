#include <stdio.h>
#include <unistd.h>
#include <sys/epoll.h>

/*
*Author:icepan
*Date: 2020/8/13-下午8:40 
*Description:
*/

int main() {

    struct epoll_event evReq[10];
    struct epoll_event evRsp[10];

    evReq[0].data.fd = STDIN_FILENO;
    evReq[0].events = EPOLLIN;

    evReq[1].data.fd = STDOUT_FILENO;
    evReq[1].events = EPOLLOUT;


    //创建句柄
    int epollFd = epoll_create(10);


    //添加
    epoll_ctl(epollFd, EPOLL_CTL_ADD, evReq[0].data.fd, evReq);
    epoll_ctl(epollFd, EPOLL_CTL_ADD, evReq[1].data.fd, evReq + 1);

    int time_out = 5000;

    int retval = epoll_wait(epollFd, evRsp, 10, time_out);

    if (retval < 0) {
        printf("epoll error\n");
        close(epollFd);
        return -1;
    }

    if (retval == 0) {
        printf("time out\n");
        close(epollFd);
        return -1;
    }


    // 无需遍历所有句柄，只需直接取结果就行，如下evRsp[i]对应的句柄一定是"就绪的(active)"
    for (int i = 0; i < retval; ++i) {
        printf("active fd is %d\n", evRsp[i].data.fd);
    }
    close(epollFd);
    return 0;
}