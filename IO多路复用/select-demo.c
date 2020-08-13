#include <stdio.h>
#include <sys/select.h>
#include <unistd.h>

/*
*Author:icepan
*Date: 2020/8/13-下午3:32 
*Description:
*/


int main() {

    fd_set read_fds;
    FD_ZERO(&read_fds); //清空集合
    //注册标准输入
    FD_SET(STDIN_FILENO, &read_fds);

    fd_set write_fds;
    FD_ZERO(&write_fds);
    //注册标准输出
    FD_SET(STDOUT_FILENO, &read_fds);


    struct timeval tv;
    tv.tv_sec = 5;
    tv.tv_usec = 0;

    //nfds：注册的数量
    int retval = select(2,&read_fds,&write_fds,NULL,&tv);

    if (retval < 0)
    {
        printf("select error\n");
        return -1;
    }

    if (retval == 0)
    {
        printf("time out\n");
        return -1;
    }

    // 需要对所有句柄进行轮询遍历
    if(FD_ISSET(STDIN_FILENO, &read_fds))
    {
        printf("can read\n");
    }

    if(FD_ISSET(STDOUT_FILENO, &write_fds))
    {
        printf("can write\n");
    }


    return 0;

}