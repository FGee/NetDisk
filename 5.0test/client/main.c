#include "ahead_cli.h"
#include "tcp_cli.h"
#include "token.h"
#include "trans_ahead_cli.h"
#include "log_reg.h"
#include "cmd.h"

int main(int argc, char** argv)
{

    int sockFd;
    int ret;

    ret = tcp_init(&sockFd);

    TokenInfo_t tokenInfo;
    bzero(&tokenInfo, sizeof(tokenInfo));

    ret = login_register(sockFd, &tokenInfo);

    int epfd;
    epfd = epoll_create(1);

    int eventNum = 2;

    struct epoll_event event, evs[eventNum];
    memset(evs, 0, sizeof(struct epoll_event) * eventNum);
    bzero(&event, sizeof(event));
    event.events = EPOLLIN;
    event.data.fd = sockFd;
    ret = epoll_ctl(epfd, EPOLL_CTL_ADD, sockFd, &event);
    ERROR_CHECK(ret, -1, "epoll_ctl");
    event.data.fd = STDIN_FILENO;
    ret = epoll_ctl(epfd, EPOLL_CTL_ADD, STDIN_FILENO, &event);
    ERROR_CHECK(ret, -1, "epoll_ctl");

    int readyNum = 0;
    int epollNum = eventNum;
    char cmdBuf[64] = {0}; 
    Cmd_t cmd;
    Train_t train;
    while (1) {
        readyNum = epoll_wait(epfd, evs, epollNum, -1);
        for (int i = 0; i < readyNum; ++i) {
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
