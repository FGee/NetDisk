#include "factory.h"
//#include "trans_ahead.h"
#include "fs_ahead.h"
#include "work_que.h"
#include "thread.h"

int thread_gets_doing_ser(pNode_t pget)
{
    int ret;
    Train_t train;

    bzero(&train, sizeof(train));
    ret = trans_recv_cycle(pget->newfd_put_get, &train, sizeof(train));
    bzero(pget->pconn->pFs->entryName, sizeof(pget->pconn->pFs->entryName));
    memcpy(pget->pconn->pFs->entryName, train.buf, strlen(train.buf));
    pget->pconn->pFs->entryType = 1;
    ret = file_sys_entry_exist(pget->pconn->pFs);
    if (0 == ret) {
        bzero(&train, sizeof(train));
        train.ctl_code = -221;
        ret = trans_send_cycle(pget->newfd_put_get, &train, sizeof(Train_t));
        ERROR_CHECK(ret, -1, "trans_send_cycle");
        return -1;
    }

    printf("pget->pconn->pFs %p\n", pget->pconn->pFs);
    printf("before md5 exist\n");
    int break_point = *(int*)(train.buf + 100);//break point download
    printf("fileSize %d\n", pget->pconn->pFs->fileSize);

    bzero(&train, sizeof(train));
    memcpy(train.buf, &pget->pconn->pFs->fileSize, sizeof(int));
    if (pget->pconn->pFs->fileSize > BIG_FILE_SZ) {
        train.ctl_code = -222;
        ret = trans_send_cycle(pget->newfd_put_get, &train, sizeof(Train_t));
        ERROR_CHECK(ret, -1, "trans_send_cycle");

        ret = thread_gets_send_big(pget, break_point);

    } else {
        train.ctl_code = -223;
        ret = trans_send_cycle(pget->newfd_put_get, &train, sizeof(Train_t));
        ERROR_CHECK(ret, -1, "trans_send_cycle");

        ret = thread_gets_send_small(pget, break_point);

    }
    if (-1 == ret) {
        printf("thread_puts_doing: recvbig error\n");

        bzero(&train, sizeof(train));
        train.ctl_code = -224;
        ret = trans_send_cycle(pget->newfd_put_get, &train, sizeof(Train_t));
        ERROR_CHECK(ret, -1, "trans_send_cycle");

        return -1;
    } else {
        bzero(&train, sizeof(train));
        train.ctl_code = -225;
        ret = trans_send_cycle(pget->newfd_put_get, &train, sizeof(Train_t));
        ERROR_CHECK(ret, -1, "trans_send_cycle");

        return 0;
    }
}

