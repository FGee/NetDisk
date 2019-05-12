#include "file.h"
#include "trans_ahead_cli.h"

int file_puts_send_big(pFileInfo_t pfi, int sockFd)
{
    int ret;

    int fd = open(pfi->filePath, O_RDWR);//must can read and write
    ERROR_CHECK(fd, -1, "open");
    int fileSize = pfi->fileSize;
    ftruncate(fd, fileSize);
    char *pMap = (char*)mmap(NULL, fileSize, PROT_READ, MAP_SHARED, fd, 0);

    ret = trans_send_cycle(sockFd, pMap, fileSize);
    ERROR_CHECK(ret, -1, "trans_send_cycle");

    munmap(pMap, fileSize);
    close(fd);

    if (fileSize == ret) {
        return 0;
    } else {
        return -1;
    }
}

