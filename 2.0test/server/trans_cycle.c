#include "trans_ahead.h"

int trans_send_cycle(int newfd, const void *buf, int lenth)
{
    int ret;
    int total = 0;
    while (total < lenth) {
        ret = send(newfd, (const char*)buf + total, lenth - total, 0);
        ERROR_CHECK(ret, -1, "send");
        if (0 == ret) {
#ifdef TRANS_CYCLE
            printf("send_cycle: client disconnect when trans\n");
#endif
            return -500;
        }

        total += ret;
    }

    return 0;
}

int trans_recv_cycle(int newfd, void *buf, int lenth)
{
    int ret;
    int total = 0;
    while (total < lenth) {
        ret = recv(newfd, (char*)buf + total, lenth - total, 0);
        ERROR_CHECK(ret, -1, "recv");
        if (0 == ret) {
#ifdef TRANS_CYCLE
            printf("recv_cycle: client disconnect when trans\n");
#endif
            return -550;
        }

        total += ret;
    }

    return 0;
}
