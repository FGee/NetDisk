#include "func_cli.h"
#define SLICE_SIZE 4096

int main(int argc, char** argv)
{
    int sockFd;
    sockFd = socket(AF_INET, SOCK_STREAM, 0);
    ERROR_CHECK(sockFd, -1, "socket");

    struct sockaddr_in ser_addr;
    bzero(&ser_addr, sizeof(ser_addr));
    ser_addr.sin_family = AF_INET;
    ser_addr.sin_port = htons(atoi("2000"));
    ser_addr.sin_addr.s_addr = inet_addr("192.168.198.131");
    int ret;

    ret = connect(sockFd, (struct sockaddr*)&ser_addr, sizeof(struct sockaddr_in));
    ERROR_CHECK(ret, -1, "conect");

    printf("conect success\n");

    while (1);
    
    return 0;
}
