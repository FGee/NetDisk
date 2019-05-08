#ifndef __TRANS_AHEAD_H__
#define __TRANS_AHEAD_H__
#include "ahead.h"
#define TRAIN_BUF_SZ 1000

typedef struct {
    int dataLenth;
    int ctl_code;
    char buf[TRAIN_BUF_SZ];
} Train;
#endif
