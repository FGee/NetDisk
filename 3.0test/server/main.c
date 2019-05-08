#include "ahead.h"
#include "tcp_init.h"
#include "conn_que.h"
#include "trans_ahead.h"
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

    Mysql_t my;
    ret = mysqlInit(&my);
    ERROR_CHECK(ret, -1, "mysqlInit");

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
    int newfd;
    struct sockaddr_in clientaddr;// record ip and port for log sys
    memset(&clientaddr, 0, sizeof(struct sockaddr));
    socklen_t addrlen = sizeof(struct sockaddr);
    Train train;

    while (1) {
        readyNum = epoll_wait(epfd, evs, epollNum, -1);
        for (int i = 0; i < readyNum; ++i) {
            if (EPOLLIN == evs[i].events && sockFd == evs[i].data.fd) {
                if (cq.size <= cq.max_conn) {
                    newfd = accept(sockFd, (struct sockaddr*)&clientaddr, &addrlen);
                    ERROR_CHECK(newfd, -1, "accept"); // care this when signal

                    event.events = EPOLLIN;
                    event.data.fd = newfd;
                    ret = epoll_ctl(epfd, EPOLL_CTL_ADD, newfd, &event);
                    ERROR_CHECK(ret, -1, "epoll_ctl");
                    
                    ret = conn_que_insert(&cq, newfd);
                    ERROR_CHECK(ret, -1, "conn_que_insert");

                    ++epollNum;
                    ++connNum;
                }
            }
            if (EPOLLIN == evs[i].events) {
                pConnNode_t pconn = NULL;
                ret = conn_que_exist(&cq, &pconn, evs[i].data.fd);
                if (1 == ret) {
//#ifdef EPOLL
                    printf("pconn->newfd %d; evs[i].data.fd %d\n", pconn->newfd, evs[i].data.fd);
//#endif
                    bzero(&train, sizeof(train));
                    ret = trans_recv_cycle(pconn->newfd, &train, sizeof(train));
                    ERROR_CHECK(ret, -1, "trans_recv_cycle");
#ifdef EPOLL                    
                    printf("%d %s\n", train.dataLenth, train.buf);
#endif
                    if (1 == train.ctl_code) {//produce client's register 
                        pUserInfo_t pui;
                        usr_sys_usr_init(&pui);
                        pui->conn = my.conn;
                        pconn->pUser = pui;
                        memcpy(pui->userName, train.buf, train.dataLenth);

                        bzero(&train, sizeof(train));//send salt to client
                        train.dataLenth = strlen(pui->salt);
                        train.ctl_code = 2;
                        memcpy(train.buf, pui->salt, train.dataLenth);
                        ret = trans_send_cycle(pconn->newfd, &train, sizeof(train));
                        ERROR_CHECK(ret, -1, "trans_send_cycle");
                    } else if (3 == train.ctl_code) {//recv client's ciphertext from client and send success
                        printf("ciphertext %s\n", train.buf);
                        memcpy(pconn->pUser->ciphertext, train.buf, train.dataLenth);
                        ret = usr_sys_register(pconn->pUser);
                        
                        bzero(&train, sizeof(train));//send salt to client
                        train.dataLenth = strlen("register success!");
                        train.ctl_code = 4;
                        memcpy(train.buf, "register success!", train.dataLenth);
                        ret = trans_send_cycle(pconn->newfd, &train, sizeof(train));
                        ERROR_CHECK(ret, -1, "trans_send_cycle");
                    }
                }
            }
        }
    }

    return 0;
}
