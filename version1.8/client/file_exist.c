#include "file.h"
#include <func.h>

int file_exist(pFileInfo_t pfi)
{
    int ret;
    struct dirent *pdirInfo;
    DIR *pdir;
    pdir = opendir(pfi->filePath);
    while (NULL != (pdirInfo = readdir(pdir))) {
        if (!strcmp(pfi->fileName, pdirInfo->d_name)) {
            struct stat statBuf;
            sprintf(pfi->filePath, "%s/%s", pfi->filePath, pdirInfo->d_name);
            ret = stat(pfi->filePath, &statBuf);
            pfi->fileSize = statBuf.st_size;
            printf("file exist\n");

            return 1;
        }
    }

    printf("file not exist\n");
    return 0;
}

