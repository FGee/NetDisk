#include "ctl_code.h"

int ctl_code17(pTrain_t ptrain, pConnNode_t pconn, pConnQue_t pcq, pMysql_t pmysql, pEpoll_t pep, pFactory_t pf)
{
    int ret;
    memcpy(pconn->pFs->entryName, ptrain->buf, ptrain->dataLenth);
    ret = file_sys_rm(pconn->pFs);

    return 0;
}
