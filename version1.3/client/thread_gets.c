#include "factory.h"
#include "tcp_cli.h"

void* thread_gets(void *pArg)
{
    int ret;
    int sockFd;
    ret = tcp_init(&sockFd);

    while (1);

}
