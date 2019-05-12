#include "thread.h"
//#include "trans_ahead.h"
#include "fs_ahead.h"
#include "work_que.h"

int thread_puts_doing_ser(pNode_t pget)
{
    int ret;
    Train_t train;

    bzero(&train, sizeof(train));
    ret = trans_recv_cycle(pget->newfd_put_get, &train, sizeof(train));
    bzero(pget->pconn->pFs->entryName, sizeof(pget->pconn->pFs->entryName));
    memcpy(pget->pconn->pFs->entryName, train.buf, 100);
    pget->pconn->pFs->entryType = 1;
    ret = file_sys_entry_exist(pget->pconn->pFs);
    if (1 == ret) {
        bzero(&train, sizeof(train));
        train.ctl_code = -211;
        ret = trans_send_cycle(pget->newfd_put_get, &train, sizeof(Train_t));
        ERROR_CHECK(ret, -1, "trans_send_cycle");
        return -1;
    }


    bzero(pget->pconn->pFs->md5sum, sizeof(pget->pconn->pFs->md5sum));
    printf("pget->pconn->pFs %p\n", pget->pconn->pFs);
    memcpy(pget->pconn->pFs->md5sum, train.buf + 200, strlen(train.buf + 200));
    printf("pget->pconn->pFs %p\n", pget->pconn->pFs);
    printf("before md5 exist\n");
    printf("pget->pconn->pFs %p\n", pget->pconn->pFs);
    printf("pget->pconn->pFs->md5sum %s\n", pget->pconn->pFs->md5sum);
    ret = file_sys_md5_exist(pget->pconn->pFs);
    pget->pconn->pFs->fileSize = *(int*)(train.buf + 100);
    printf("fileSize %d\n", pget->pconn->pFs->fileSize);
    if (ret >= 1) {
        ret = file_sys_create_file(pget->pconn->pFs);

        bzero(&train, sizeof(train));
        train.ctl_code = -212;
        ret = trans_send_cycle(pget->newfd_put_get, &train, sizeof(Train_t));
        ERROR_CHECK(ret, -1, "trans_send_cycle");
    }

    if (pget->pconn->pFs->fileSize > BIG_FILE_SZ) {
        bzero(&train, sizeof(train));
        train.ctl_code = -213;
        ret = trans_send_cycle(pget->newfd_put_get, &train, sizeof(Train_t));
        ERROR_CHECK(ret, -1, "trans_send_cycle");

        ret = thread_puts_recv_big(pget);

    } else {
        bzero(&train, sizeof(train));
        train.ctl_code = -214;
        ret = trans_send_cycle(pget->newfd_put_get, &train, sizeof(Train_t));
        ERROR_CHECK(ret, -1, "trans_send_cycle");

        ret = thread_puts_recv_small(pget);

    }
    if (-1 == ret) {
        printf("thread_puts_doing: recvbig error\n");

        bzero(&train, sizeof(train));
        train.ctl_code = -215;
        ret = trans_send_cycle(pget->newfd_put_get, &train, sizeof(Train_t));
        ERROR_CHECK(ret, -1, "trans_send_cycle");

        return -1;
    } else {
        bzero(&train, sizeof(train));
        train.ctl_code = -216;
        ret = trans_send_cycle(pget->newfd_put_get, &train, sizeof(Train_t));
        ERROR_CHECK(ret, -1, "trans_send_cycle");

        printf("pget->pconn->pFs->md5sum %s\n", pget->pconn->pFs->md5sum);
        ret = file_sys_create_file(pget->pconn->pFs);

        return 0;
    }
}
