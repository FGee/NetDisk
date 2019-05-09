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

    Mysql_t mysql;
    ret = mysqlInit(&mysql);
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
#ifdef EPOLL
                    printf("pconn->newfd %d; evs[i].data.fd %d\n", pconn->newfd, evs[i].data.fd);
#endif
                    bzero(&train, sizeof(train));
                    ret = trans_recv_cycle(pconn->newfd, &train, sizeof(train));
                    ERROR_CHECK(ret, -1, "trans_recv_cycle");
#ifdef EPOLL                    
                    printf("%d %s\n", train.dataLenth, train.buf);
#endif
                    if (1 == train.ctl_code) {//produce client's register 
                        pUserInfo_t pui;
                        usr_sys_usr_init(&pui, &mysql);
                        pconn->pUser = pui;
                        memcpy(pui->userName, train.buf, train.dataLenth);
                        ret = usr_sys_usrName_exist(pui);
                        if (0 == ret) {
                            bzero(&train, sizeof(train));//send salt to client
                            train.dataLenth = sizeof(char) + strlen(pui->salt);
                            train.ctl_code = -1;
                            train.buf[0] = 'n';
                            memcpy(train.buf + 1, pui->salt, train.dataLenth);
                            ret = trans_send_cycle(pconn->newfd, &train, sizeof(train));
                            ERROR_CHECK(ret, -1, "trans_send_cycle");
                        } else if (1 == ret) {
                            bzero(&train, sizeof(train));
                            train.dataLenth = sizeof(char);
                            train.ctl_code = -1;
                            train.buf[0] = 'e';
                            ret = trans_send_cycle(pconn->newfd, &train, sizeof(train));
                            ERROR_CHECK(ret, -1, "trans_send_cycle");

                            free(pui);
                            pui = NULL;
                            pconn->pUser = NULL;
                        }
                    } else if (2 == train.ctl_code) {//recv client's ciphertext from client and send success
                        memcpy(pconn->pUser->ciphertext, train.buf, train.dataLenth);
                        ret = usr_sys_register(pconn->pUser);

                        bzero(&train, sizeof(train));
                        train.dataLenth = sizeof(char);
                        train.ctl_code = -2;
                        train.buf[0] = 's';
                        ret = trans_send_cycle(pconn->newfd, &train, sizeof(train));
                        ERROR_CHECK(ret, -1, "trans_send_cycle");
                    } else if (6 == train.ctl_code) {
                        pUserInfo_t pui;
                        usr_sys_usr_init(&pui, &mysql);
                        pconn->pUser = pui;
                        memcpy(pui->userName, train.buf, train.dataLenth);
                        ret = usr_sys_usrName_exist(pui);
                        if (0 == ret) {
                            bzero(&train, sizeof(train));//send salt to client
                            train.dataLenth = sizeof(char);
                            train.ctl_code = -6;
                            train.buf[0] = 'n';
                            ret = trans_send_cycle(pconn->newfd, &train, sizeof(train));
                            ERROR_CHECK(ret, -1, "trans_send_cycle");

                            free(pui);
                            pui = NULL;
                            pconn->pUser = NULL;
                        } else if (1 == ret) {
                            bzero(&train, sizeof(train));//send salt to client
                            train.dataLenth = sizeof(char) + strlen(pui->salt);
                            train.ctl_code = -6;
                            train.buf[0] = 'e';
                            memcpy(train.buf + 1, pui->salt, train.dataLenth);
                            ret = trans_send_cycle(pconn->newfd, &train, sizeof(train));
                            ERROR_CHECK(ret, -1, "trans_send_cycle");
                        }
                    } else if (7 == train.ctl_code) {
                        memcpy(pconn->pUser->ciphertext, train.buf, train.dataLenth);
                        ret = usr_sys_verify(pconn->pUser);

                        if (1 == ret) {
                            ret = file_sys_info_init(&pconn->pFs, &mysql);

#ifdef EPOLL                    
                            memcpy(pconn->pUser->token, train.buf + 100, 100);
                            for (unsigned long i = 0; i < strlen((char*)pconn->pUser->token); i++) {
                                printf("%02x", pconn->pUser->token[i]);
                            }
                            printf("\n");
#endif

                            bzero(&train, sizeof(train));//send salt to client
                            train.dataLenth = sizeof(char);
                            train.ctl_code = -7;
                            train.buf[0] = 's';
                            ret = trans_send_cycle(pconn->newfd, &train, sizeof(train));
                            ERROR_CHECK(ret, -1, "trans_send_cycle");
                        } else if (0 == ret) {
                            bzero(&train, sizeof(train));//send salt to client
                            train.dataLenth = sizeof(char);
                            train.ctl_code = -7;
                            train.buf[0] = 'f';
                            ret = trans_send_cycle(pconn->newfd, &train, sizeof(train));
                            ERROR_CHECK(ret, -1, "trans_send_cycle");
                        }
                    }
                }
            }
        }
    }

    return 0;
}
