#include "ahead.h"
#include "tcp_init.h"
#include "conn_que.h"
#include "fs_ahead.h"

int main(int argc, char ** argv)
{
    const char* ip = "192.168.198.131";
    const char* port = "2000";

    int ret;

    int sockFd;
    ret = tcpInit(&sockFd, ip, port);
    ERROR_CHECK(ret, -1, "tcpInit");

    ConnQue_t cq;
    memset(&cq, 0, sizeof(cq));
    int max_conn = 10;
    ret = conn_que_init(&cq, max_conn);

    int epfd;
    epfd = epoll_create(1);

    int eventNum = 1 + max_conn;

    struct epoll_event event, evs[eventNum];
    memset(evs, 0, sizeof(struct epoll_event) * eventNum);
    bzero(&event, sizeof(event));
    event.events = EPOLLIN;
    event.data.fd = sockFd;
    ret = epoll_ctl(epfd, EPOLL_CTL_ADD, sockFd, &event);
    ERROR_CHECK(ret, -1, "epoll_ctl");

    int epollNum = 1;
    int readyNum = 0;
    int connNum = 0;
    struct sockaddr_in clientaddr;// record ip and port for log sys
    memset(&clientaddr, 0, sizeof(struct sockaddr));
    socklen_t addrlen = sizeof(struct sockaddr);

    while (1) {
        readyNum = epoll_wait(epfd, evs, epollNum, -1);
        for (int i = 0; i < readyNum; ++i) {
            if (EPOLLIN == evs[i].events && sockFd == evs[i].data.fd) {
                if (cq.size <= cq.max_conn) {
                    ConnNode_t newConn;
                    bzero(&newConn, sizeof(newConn));

                    newConn.newfd = accept(sockFd, (struct sockaddr*)&clientaddr, &addrlen);
                    ERROR_CHECK(newConn.newfd, -1, "accept"); // care this when signal

                    ret = conn_que_insert(&cq, &newConn);
                    ERROR_CHECK(ret, -1, "conn_que_insert");

                    event.events = EPOLLIN;
                    event.data.fd = newConn.newfd;
                    ret = epoll_ctl(epfd, EPOLL_CTL_ADD, newConn.newfd, &event);
                    ERROR_CHECK(ret, -1, "epoll_ctl");

                    ++epollNum;
                    ++connNum;
                }
                if (EPOLLIN == evs[i].events) {
                    pConnNode_t pcur = NULL;
                    ret = conn_que_exist(&cq, &pcur, evs[i].data.fd);
                    if (1 == ret) {

                    }
                }
            }
        }
    }

    return 0;
}

