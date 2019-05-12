#include "file.h"
#include "trans_ahead_cli.h"

int file_puts_send_small(pFileInfo_t pfi, int sockFd)
{
    int ret;
    Train_t train;

    int fd = open(pfi->filePath, O_RDWR);//must can read and write
    ERROR_CHECK(fd, -1, "open");

    while (0 != (train.dataLenth = read(fd, train.buf, sizeof(train.buf)))) {
        train.ctl_code = 666;
        ret = trans_send_cycle(sockFd, &train, sizeof(train));
        ERROR_CHECK(ret, -1, "trans_recv_cycle");
        bzero(&train, sizeof(Train_t));
    }
    train.ctl_code = 2333;
    ret = trans_send_cycle(sockFd, &train, sizeof(train));
    ERROR_CHECK(ret, -1, "trans_recv_cycle");

    close(fd);

    return 0;
}


