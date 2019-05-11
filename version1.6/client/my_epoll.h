#ifndef __MY_EPOLL_H__
#define __MY_EPOLL_H__
#include <func.h>

typedef struct {
    int epfd;
    int eventCapacity;
    int moniterNum;
    int readyNum;
    struct epoll_event event;
} Epoll_t, *pEpoll_t;

int my_epoll_init(pEpoll_t pep);


#endif
