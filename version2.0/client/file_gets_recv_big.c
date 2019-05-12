#include "file.h"
#include "trans_ahead_cli.h"

int file_gets_recv_big(pFileInfo_t pfi, int sockFd, int fileSize)
{
    int ret;

    int fd = open(pfi->filePath, O_CREAT | O_RDWR, 0666);//must can read and write
    ERROR_CHECK(fd, -1, "open");
    int break_point = pfi->fileSize;
    ftruncate(fd, fileSize);
    char *pMap = (char*)mmap(NULL, fileSize, PROT_WRITE | PROT_READ, MAP_SHARED, fd, 0);

    ret = trans_recv_cycle(sockFd, pMap + break_point, fileSize - break_point);
    ERROR_CHECK(ret, -1, "trans_recv_cycle");

    munmap(pMap, fileSize);
    close(fd);

    if (fileSize == ret - break_point) {
        return 0;
    } else {
        return -1;
    }
}

