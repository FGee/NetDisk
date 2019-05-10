#include <func.h>
#include "factory.h"
#include "tcp_init.h"
#include "conn_que.h"
#include "trans_ahead.h"
#include "fs_ahead.h"
#include "my_epoll.h"
#include "ctl_code.h"

int main(int argc, char ** argv)
{
    const char* ip = "192.168.198.131";
    const char* port = "2000";

    int ret;
    Factory_t facts[2];
    bzero(facts, sizeof(Factory_t) * 2);
    facts[0].work_que.capacity = 10;
    facts[0].threadNum = 2;
    facts[1].work_que.capacity = 10;
    facts[1].threadNum = 2;
    ret = factory_s_init(facts);


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

    Epoll_t myep;
    ret = my_epoll_init(&myep, max_conn);

    struct epoll_event evs[myep.eventCapacity];
    memset(evs, 0, sizeof(struct epoll_event) * myep.eventCapacity);
    myep.event.events = EPOLLIN;
    myep.event.data.fd = sockFd;
    ret = epoll_ctl(myep.epfd, EPOLL_CTL_ADD, sockFd, &myep.event);
    ERROR_CHECK(ret, -1, "epoll_ctl");

    myep.moniterNum = 1;
    Train_t train;

    pCtl_code_t ctl_code[CTL_CODE_SZ];

    ret = ctl_code_funcArr_init(ctl_code);

    while (1) {
        myep.readyNum = epoll_wait(myep.epfd, evs, myep.moniterNum, -1);
        for (int i = 0; i < myep.readyNum; ++i) {
            if (EPOLLIN == evs[i].events && sockFd == evs[i].data.fd) {
                ret = my_epoll_conn(sockFd, &myep, &cq, NULL);
            }
            if (EPOLLIN == evs[i].events) {
                pConnNode_t pconn = NULL;
                ret = conn_que_exist(&cq, &pconn, evs[i].data.fd);
                if (1 == ret) {
                    bzero(&train, sizeof(train));
                    ret = trans_recv_cycle(pconn->newfd, &train, sizeof(train));
                    ERROR_CHECK(ret, -1, "trans_recv_cycle");
                    printf("ctl_code %d\n", train.ctl_code);
                    ret = ctl_code[train.ctl_code](&train, pconn, &cq, &mysql, &myep);
                }
            }
        }
    }

    return 0;
}
