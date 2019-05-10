#include "ctl_code.h"

int ctl_code3(pTrain_t ptrain, pConnNode_t pconn, pConnQue_t pcq, pMysql_t pmysql, pEpoll_t pep)
{
    int ret;
    pUserInfo_t pui;
    ret = usr_sys_usr_init(&pui, pmysql);
    pconn->pUser = pui;
    memcpy(pui->userName, ptrain->buf, ptrain->dataLenth);
    ret = usr_sys_usrName_exist(pui);
    if (0 == ret) {
        bzero(ptrain, sizeof(Train_t));//send salt to client
        ptrain->dataLenth = sizeof(char);
        ptrain->ctl_code = -3;
        ptrain->buf[0] = 'n';
        ret = trans_send_cycle(pconn->newfd, ptrain, sizeof(Train_t));
        ERROR_CHECK(ret, -1, "trans_send_cycle");

        free(pui);
        pui = NULL;
        pconn->pUser = NULL;
    } else if (1 == ret) {
        bzero(ptrain, sizeof(Train_t));//send salt to client
        ptrain->dataLenth = sizeof(char) + strlen(pui->salt);
        ptrain->ctl_code = -3;
        ptrain->buf[0] = 'e';
        memcpy(ptrain->buf + 1, pui->salt, ptrain->dataLenth);
        ret = trans_send_cycle(pconn->newfd, ptrain, sizeof(Train_t));
        ERROR_CHECK(ret, -1, "trans_send_cycle");
    }

    return 0;
}
