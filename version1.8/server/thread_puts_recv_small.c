#include "work_que.h"

int thread_puts_recv_small(pNode_t pget)
{
    int ret;
    Train_t train;
    int newfd_put_get = pget->newfd_put_get;
    char ser_file_lib[128] = {0};
    strcpy(ser_file_lib, "/home/shane/NetDisk/ser_file_lib");

    sprintf(ser_file_lib, "%s/%s", ser_file_lib, pget->pconn->pFs->md5sum);

    printf("md5 file %s\n", ser_file_lib);

    int fd = open(ser_file_lib, O_CREAT | O_RDWR, 0666);//must can read and write
    ERROR_CHECK(fd, -1, "open");
    int fileSize = pget->pconn->pFs->fileSize;

    int total_write = 0;
    while (1) {
        bzero(&train, sizeof(Train_t));
        ret = trans_recv_cycle(newfd_put_get, &train, sizeof(train));
        ERROR_CHECK(ret, -1, "trans_recv_cycle");
        if (666 == train.ctl_code) {
            ret = write(fd, train.buf, train.dataLenth);
            ERROR_CHECK(ret, -1, "write");

            total_write += ret;
        } else if (2333 == train.ctl_code) {
            printf("finish puts\n");
            break;
        }
    }
    close(fd);
    if (total_write != fileSize) {
        printf("puts file fail\n");
        return -1;
    } else {
        printf("finish puts file\n");
        return 0;
    }
}

