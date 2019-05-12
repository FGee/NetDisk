#include "trans_ahead.h" 
#include "work_que.h"

int thread_puts_recv_big(pNode_t pget)
{
    int ret;
    int newfd_put_get = pget->newfd_put_get;
    char ser_file_lib[128] = {0};
    strcpy(ser_file_lib, "/home/shane/NetDisk/ser_file_lib");

    sprintf(ser_file_lib, "%s/%s", ser_file_lib, pget->pconn->pFs->md5sum);

    printf("md5 file %s\n", ser_file_lib);

    int fd = open(ser_file_lib, O_CREAT | O_RDWR, 0666);//must can read and write
    ERROR_CHECK(fd, -1, "open");
    int fileSize = pget->pconn->pFs->fileSize;
    ftruncate(fd, fileSize);
    char *pMap = (char*)mmap(NULL, fileSize, PROT_WRITE, MAP_SHARED, fd, 0);

    ret = trans_recv_cycle(newfd_put_get, pMap, fileSize);
    ERROR_CHECK(ret, -1, "trans_recv_cycle");

    munmap(pMap, fileSize);
    close(fd);

    if (fileSize == ret) {
        return 0;
    } else {
        return -1;
    }
}
