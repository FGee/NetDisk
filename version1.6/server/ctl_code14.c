#include "ctl_code.h"

int ctl_code14(pTrain_t ptrain, pConnNode_t pconn, pConnQue_t pcq, pMysql_t pmysql, pEpoll_t pep, pFactory_t pf)
{
    int ret;
    pconn->pFs->opsCode = 1;
    ret = file_sys_cd(pconn->pFs);

    return 0;
}
