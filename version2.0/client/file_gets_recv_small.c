#include "file.h"
#include "trans_ahead_cli.h"

int file_gets_recv_small(pFileInfo_t pfi, int sockFd, int fileSize)
{
    int ret;
    Train_t train;

    int fd = open(pfi->filePath, O_CREAT | O_RDWR | O_APPEND, 0666);//must can read and write
    ERROR_CHECK(fd, -1, "open");

    int break_point = pfi->fileSize;

    int total_write = 0;
    while (1) {
        bzero(&train, sizeof(Train_t));
        ret = trans_recv_cycle(sockFd, &train, sizeof(train));
        ERROR_CHECK(ret, -1, "trans_recv_cycle");
        if (666 == train.ctl_code) {
            ret = write(fd, train.buf, train.dataLenth);
            ERROR_CHECK(ret, -1, "write");

            total_write += ret;
        } else if (2333 == train.ctl_code) {
            printf("finish gets\n");
            break;
        }
    }

    close(fd);
    if (total_write != fileSize - break_point) {
        printf("gets file fail\n");
        return -1;
    } else {
        printf("finish gets file\n");
        return 0;
    }

    return 0;
}
