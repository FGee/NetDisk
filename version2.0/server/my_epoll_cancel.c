#include "my_epoll.h"
#include "trans_ahead.h"
#include "conn_que.h"
#include "fs_ahead.h"

int my_epoll_cancel(pEpoll_t pep, pConnQue_t pcq, pConnNode_t pdel, pMysql_t pmysql)
{
    int ret;

    pep->event.events = EPOLLIN;
    pep->event.data.fd = pdel->newfd;
    printf("here111111\n");
    ret = epoll_ctl(pep->epfd, EPOLL_CTL_DEL, pdel->newfd, &pep->event);
    printf("here111111\n");
    ERROR_CHECK(ret, -1, "epoll_ctl");

    --pep->moniterNum;
    
    ret = conn_que_delete(pcq, pdel);
    ERROR_CHECK(ret, -1, "conn_que_delete");

    return 0;
}


