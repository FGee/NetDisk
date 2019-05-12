#ifndef __FILE_H__
#define __FILE_H__
typedef struct {
    char filePath[128];
    char fileName[32];
    int fileSize;
    char fileMD5[32];
} FileInfo_t, *pFileInfo_t;

int file_exist(pFileInfo_t pfi);

int file_md5(const char *file_path, char *md5_str);

int file_puts_recv_big(pFileInfo_t pfi, int sockFd);

int file_puts_recv_small(pFileInfo_t pfi, int sockFd);
#endif
