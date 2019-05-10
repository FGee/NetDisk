#include "ahead_cli.h"
#include "tcp_cli.h"
#include "token.h"
#include "trans_ahead_cli.h"
#include "log_reg.h"
#include "cmd.h"
#include "my_epoll.h"

int main(int argc, char** argv)
{

    int sockFd;
    int ret;

    ret = tcp_init(&sockFd);

    TokenInfo_t tokenInfo;
    bzero(&tokenInfo, sizeof(tokenInfo));

    ret = login_register(sockFd, &tokenInfo);

    Epoll_t myep;
    ret = my_epoll_init(&myep);
    myep.eventCapacity = 5;
    struct epoll_event evs[myep.eventCapacity];
    memset(evs, 0, sizeof(struct epoll_event) * myep.eventCapacity);
    myep.event.events = EPOLLIN;
    myep.event.data.fd = sockFd;
    ret = epoll_ctl(myep.epfd, EPOLL_CTL_ADD, sockFd, &myep.event);
    ERROR_CHECK(ret, -1, "epoll_ctl");
    myep.event.data.fd = STDIN_FILENO;
    ret = epoll_ctl(myep.epfd, EPOLL_CTL_ADD, STDIN_FILENO, &myep.event);
    ERROR_CHECK(ret, -1, "epoll_ctl");

    myep.moniterNum = 2;
    char cmdBuf[64] = {0}; 
    Cmd_t cmd;
    Train_t train;
    while (1) {
        myep.readyNum = epoll_wait(myep.epfd, evs, myep.moniterNum, -1);
        for (int i = 0; i < myep.readyNum; ++i) {
            if (EPOLLIN == evs[i].events && sockFd == evs[i].data.fd) {

            }
            if (EPOLLIN == evs[i].events && STDIN_FILENO == evs[i].data.fd) {
                bzero(cmdBuf, sizeof(cmdBuf));
                bzero(&cmd, sizeof(Cmd_t));
                ret = read(STDIN_FILENO, cmdBuf, 64);
                //printf("%s\n", cmdBuf);
                int wrdNum = cmd_separate(cmdBuf, &cmd);
                if (-1 == wrdNum) {
                    printf("\nCommand illegal!!!, please input again\n");
                    goto CMD_ILLEGAL;
                }
                bzero(&train, sizeof(train));
                ret = cmd_load(&cmd, wrdNum, &train);
                if (-1 == ret) {
                    printf("\nCommand illegal!!!, please input again\n");
                    goto CMD_ILLEGAL;
                }
                ret = trans_send_cycle(sockFd, &train, sizeof(train));
                ERROR_CHECK(ret, -1, "trans_send_cycle");
CMD_ILLEGAL:
                ret = 0;//for goto can be use
            }
        }
    }

    return 0;
}
