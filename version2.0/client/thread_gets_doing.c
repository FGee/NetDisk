#include "thread.h"
#include "file.h"

int thread_gets_doing(pFactory_t pf, pNode_t pget, int sockFd)
{
    int ret;
    Train_t train;

    bzero(&train, sizeof(train));
    train.ctl_code = 22;
    train.dataLenth = 100;
    memcpy(train.buf, pget->ptokenInfo->token, train.dataLenth);
    ret = trans_send_cycle(sockFd, &train, sizeof(train));
    //#ifdef LOGIN
    printf("token: ");
    for (unsigned long i = 0; i < strlen((char*)pget->ptokenInfo->token); i++) {
        printf("%02x", pget->ptokenInfo->token[i]);
    }
    printf("\n");
    //#endif


    FileInfo_t fileInfo;
    bzero(&fileInfo, sizeof(FileInfo_t));
    strcpy(fileInfo.filePath, pget->ptokenInfo->pathName);//copy file initial path
    strcpy(fileInfo.fileName, pget->ptokenInfo->fileName);//copy fileName of task

    ret = file_exist(&fileInfo);
    if (0 == ret) {
        sprintf(fileInfo.filePath, "%s/%s", fileInfo.filePath, fileInfo.fileName);
    }
    printf("token path %s\n", pget->ptokenInfo->pathName);
    printf("fileInfor filePath %s\n", fileInfo.filePath);
    printf("fileName %s\n", fileInfo.fileName);
    printf("fileSize %d\n", fileInfo.fileSize);


    //printf("%s %d\n", fileInfo.fileName, fileInfo.fileSize);
    bzero(&train, sizeof(train));
    train.ctl_code = 221;
    memcpy(train.buf, fileInfo.fileName, strlen(fileInfo.fileName)); 
    memcpy(train.buf + 100, (char*)&fileInfo.fileSize, sizeof(int));
    ret = trans_send_cycle(sockFd, &train, sizeof(train));

    bzero(&train, sizeof(train));
    int real_fileSize = 0;
    ret = trans_recv_cycle(sockFd, &train, sizeof(train));
    real_fileSize = *(int*)train.buf;

    if (-221 == train.ctl_code) {
        return -1;
    } else if (-222 == train.ctl_code) {
        real_fileSize = *(int*)train.buf;
        ret = file_gets_recv_big(&fileInfo, sockFd, real_fileSize);
    } else if (-223 == train.ctl_code) {
        ret = file_gets_recv_small(&fileInfo, sockFd, real_fileSize);
    } else if (-224 == train.ctl_code) {
        printf("server send file error\n");
        return -1;
    } else if (-225 == train.ctl_code) {
        printf("file download success\n");
    }
}

