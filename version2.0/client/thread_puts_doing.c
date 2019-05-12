#include "thread.h"
#include "file.h"

int thread_puts_doing(pFactory_t pf, pNode_t pget, int sockFd)
{
    int ret;
    Train_t train;

    bzero(&train, sizeof(train));
    train.ctl_code = 21;
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
    printf("token path %s\n", pget->ptokenInfo->pathName);
    printf("fileInfor filePath %s\n", fileInfo.filePath);
    printf("fileName %s\n", fileInfo.fileName);
    printf("fileSize %d\n", fileInfo.fileSize);


    //printf("%s %d\n", fileInfo.fileName, fileInfo.fileSize);
    if (0 == ret) {
        return -1;
    }
    ret = file_md5(fileInfo.filePath, fileInfo.fileMD5);
    printf("fileMD5 %s\n", fileInfo.fileMD5);
    bzero(&train, sizeof(train));
    train.ctl_code = 211;
    memcpy(train.buf, fileInfo.fileName, strlen(fileInfo.fileName)); 
    memcpy(train.buf + 100, (char*)&fileInfo.fileSize, sizeof(int));
    memcpy(train.buf + 200, fileInfo.fileMD5, strlen(fileInfo.fileMD5));
    ret = trans_send_cycle(sockFd, &train, sizeof(train));

    bzero(&train, sizeof(train));
    ret = trans_recv_cycle(sockFd, &train, sizeof(train));
    if (-211 == train.ctl_code) {
        return -1;
    } else if (-212 == train.ctl_code) {
        return 0;
    } else if (-213 == train.ctl_code) {
        ret = file_puts_send_big(&fileInfo, sockFd);
    } else if (-214 == train.ctl_code) {
        ret = file_puts_send_small(&fileInfo, sockFd);
    } else if (-215 == train.ctl_code) {
        printf("server recv file error\n");
        return -1;
    } else if (-216 == train.ctl_code) {
        printf("file download success\n");
    }
}
