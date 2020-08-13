#include <stdio.h>
#include <unistd.h>
#include <poll.h>
/*
*Author:icepan
*Date: 2020/8/13-下午8:30 
*Description:
*/

int main(){
    struct pollfd pfd[2];
    int nfds=2;

    pfd[0].fd = STDIN_FILENO;
    pfd[0].events = POLLIN;

    pfd[1].fd = STDOUT_FILENO;
    pfd[1].events = POLLOUT;

    int time_out = 5000; //5s

    int reval = poll(pfd,nfds,time_out);

    if(reval<0){
        printf("pool error\n");
        return -1;
    }

    if(reval==0){
        printf("time out\n");
    }



    // 需要对所有句柄进行轮询遍历
    if(pfd[0].revents & POLLIN)
    {
        printf("can read\n");
    }

    if(pfd[1].revents & POLLOUT)
    {
        printf("can write\n");
    }

    return 0;
};