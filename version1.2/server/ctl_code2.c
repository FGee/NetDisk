#include "ctl_code.h"

int ctl_code2(pTrain_t ptrain, pConnNode_t pconn, pConnQue_t pcq, pMysql_t pmysql, pEpoll_t pep)
{
    int ret;
    memcpy(pconn->pUser->ciphertext, ptrain->buf, ptrain->dataLenth);
    ret = usr_sys_register(pconn->pUser);

    bzero(ptrain, sizeof(Train_t));
    ptrain->dataLenth = sizeof(char);
    ptrain->ctl_code = -2;
    ptrain->buf[0] = 's';
    ret = trans_send_cycle(pconn->newfd, ptrain, sizeof(Train_t));
    ERROR_CHECK(ret, -1, "trans_send_cycle");

    return 0;
}
