#include "work_que.h"

int thread_gets_send_small(pNode_t pget, int break_point)
{
    int ret;
    Train_t train;
    int newfd_put_get = pget->newfd_put_get;
    char ser_file_lib[128] = {0};
    strcpy(ser_file_lib, "/home/shane/NetDisk/ser_file_lib");

    sprintf(ser_file_lib, "%s/%s", ser_file_lib, pget->pconn->pFs->md5sum);

    int fd = open(ser_file_lib, O_RDWR, 0666);//must can read and write
    ERROR_CHECK(fd, -1, "open");
    //int fileSize = pget->pconn->pFs->fileSize;

    ret = lseek(fd, break_point, SEEK_SET);

    while (0 != (train.dataLenth = read(fd, train.buf, sizeof(train.buf)))) {
        train.ctl_code = 666;
        ret = trans_send_cycle(newfd_put_get, &train, sizeof(train));
        ERROR_CHECK(ret, -1, "trans_recv_cycle");
        bzero(&train, sizeof(Train_t));
    }
    train.ctl_code = 2333;
    ret = trans_send_cycle(newfd_put_get, &train, sizeof(train));
    ERROR_CHECK(ret, -1, "trans_recv_cycle");

    close(fd);

    return 0;
}


