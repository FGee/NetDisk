#include "ctl_code.h"

int ctl_code17(pTrain_t ptrain, pConnNode_t pconn, pConnQue_t pcq, pMysql_t pmysql, pEpoll_t pep, pFactory_t pf)
{
    int ret;
    bzero(pconn->pFs->entryName, sizeof(pconn->pFs->entryName));
    memcpy(pconn->pFs->entryName, ptrain->buf, ptrain->dataLenth);
    pconn->pFs->entryType = 1;
    ret = file_sys_entry_exist(pconn->pFs);
    if (1 == ret) {
        ret = file_sys_md5_exist(pconn->pFs);
        if (1 == ret) {
            ret = file_sys_rm(pconn->pFs);
            char ser_file_lib[128] = {0};
            strcpy(ser_file_lib, "/home/shane/NetDisk/ser_file_lib");
            sprintf(ser_file_lib, "%s/%s", ser_file_lib, pconn->pFs->md5sum);
            ret = unlink(ser_file_lib);
        }
        ret = file_sys_rm(pconn->pFs);
    }

    return 0;
}
