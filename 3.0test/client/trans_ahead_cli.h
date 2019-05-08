#ifndef __TRANS_AHEAD_CLI_H__
#define __TRANS_AHEAD_CLI_H__
#include "ahead_cli.h"
#define TRAIN_BUF_SZ 1000

typedef struct {
    int dataLenth;
    int ctl_code;
    char buf[TRAIN_BUF_SZ];
} Train;

int trans_send_cycle(int newfd, const void *buf, int lenth);

int trans_recv_cycle(int newfd, void *buf, int lenth);

#endif
