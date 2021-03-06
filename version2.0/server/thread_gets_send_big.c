#include "trans_ahead.h" 
#include "work_que.h"

int thread_gets_send_big(pNode_t pget, int break_point)
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
    char *pMap = (char*)mmap(NULL, fileSize, PROT_WRITE | PROT_READ, MAP_SHARED, fd, 0);

    ret = trans_send_cycle(newfd_put_get, pMap + break_point, fileSize - break_point);
    ERROR_CHECK(ret, -1, "trans_recv_cycle");

    munmap(pMap, fileSize);
    close(fd);

    if (fileSize - break_point == ret) {
        return 0;
    } else {
        return -1;
    }
}

