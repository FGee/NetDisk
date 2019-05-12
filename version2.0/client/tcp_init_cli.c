#include "tcp_cli.h"

int tcp_init(int* psockFd)
{
    int ret;
    *psockFd = socket(AF_INET, SOCK_STREAM, 0);
    ERROR_CHECK(*psockFd, -1, "socket");

    struct sockaddr_in ser_addr;
    bzero(&ser_addr, sizeof(ser_addr));
    ser_addr.sin_family = AF_INET;
    ser_addr.sin_port = htons(atoi("2000"));
    ser_addr.sin_addr.s_addr = inet_addr("192.168.198.131");

    ret = connect(*psockFd, (struct sockaddr*)&ser_addr, sizeof(struct sockaddr_in));
    ERROR_CHECK(ret, -1, "conect");

    return 0;
}
