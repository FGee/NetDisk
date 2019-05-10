#include "my_epoll.h"

int my_epoll_init(pEpoll_t pep, int max_conn)
{
    bzero(pep, sizeof(Epoll_t));
    pep->epfd = epoll_create(1);
    pep->eventCapacity = 1 + max_conn;
    
    return 0;
}
