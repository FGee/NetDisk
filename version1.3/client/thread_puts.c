#include "factory.h"
#include "tcp_cli.h"

void* thread_puts(void *pArg)
{
    int ret;
    int sockFd;
    ret = tcp_init(&sockFd);

    while (1);
}
