#include "ctl_code.h"

int ctl_code15(pTrain_t ptrain, pConnNode_t pconn, pConnQue_t pcq, pMysql_t pmysql, pEpoll_t pep)
{
    int ret;
    pconn->pFs->opsCode = 0;
    memcpy(pconn->pFs->entryName, ptrain->buf, ptrain->dataLenth);
    ret = file_sys_cd(pconn->pFs);

    return 0;
}
