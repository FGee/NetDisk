#ifndef __MY_EPOLL_H__
#define __MY_EPOLL_H__
#include <func.h>
#include "conn_que.h"
#include "fs_ahead.h"

typedef struct {
    int epfd;
    int eventCapacity;
    int moniterNum;
    int readyNum;
    struct epoll_event event;
} Epoll_t, *pEpoll_t;

int my_epoll_init(pEpoll_t pep, int max_conn);

int my_epoll_conn(int sockFd, pEpoll_t pep, pConnQue_t pcq, pMysql_t pmysql);

#endif
