#include "my_epoll.h"
#include "trans_ahead.h"
#include "conn_que.h"
#include "fs_ahead.h"

int my_epoll_conn(int sockFd, pEpoll_t pep, pConnQue_t pcq, pMysql_t pmysql)
{
    int ret;
    if (pcq->size <= pcq->max_conn) {
        pConnNode_t pnew;
        pnew = (pConnNode_t)calloc(1, sizeof(ConnNode_t));
        socklen_t addrlen = sizeof(struct sockaddr);
        pnew->newfd = accept(sockFd, (struct sockaddr*)&pnew->clientAddr, &addrlen);
        ERROR_CHECK(pnew->newfd, -1, "accept"); // care this when signal

        pep->event.events = EPOLLIN;
        pep->event.data.fd = pnew->newfd;
        ret = epoll_ctl(pep->epfd, EPOLL_CTL_ADD, pnew->newfd, &pep->event);
        ERROR_CHECK(ret, -1, "epoll_ctl");

        ret = conn_que_insert(pcq, pnew);
        ERROR_CHECK(ret, -1, "conn_que_insert");

        ++pep->moniterNum;
    }

    return 0;
}

