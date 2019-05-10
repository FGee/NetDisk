#include "my_epoll.h"

int my_epoll_init(pEpoll_t pep)
{
    bzero(pep, sizeof(Epoll_t));
    pep->epfd = epoll_create(1);
    
    return 0;
}
