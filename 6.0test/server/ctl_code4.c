#include "ctl_code.h"

int ctl_code4(pTrain_t ptrain, pConnNode_t pconn, pConnQue_t pcq, pMysql_t pmysql, pEpoll_t pep)
{
    int ret;
    memcpy(pconn->pUser->ciphertext, ptrain->buf, ptrain->dataLenth);
    ret = usr_sys_verify(pconn->pUser);

    if (1 == ret) {
        ret = file_sys_info_init(&pconn->pFs, pmysql);

#ifdef EPOLL
        memcpy(pconn->pUser->token, ptrain->buf + 100, 100);
        for (unsigned long i = 0; i < strlen((char*)pconn->pUser->token); i++) {
            printf("%02x", pconn->pUser->token[i]);
        }
        printf("\n");
#endif

        bzero(ptrain, sizeof(Train_t));//send salt to client
        ptrain->dataLenth = sizeof(char);
        ptrain->ctl_code = -4;
        ptrain->buf[0] = 's';
        ret = trans_send_cycle(pconn->newfd, ptrain, sizeof(Train_t));
        ERROR_CHECK(ret, -1, "trans_send_cycle");
    } else if (0 == ret) {
        bzero(ptrain, sizeof(Train_t));//send salt to client
        ptrain->dataLenth = sizeof(char);
        ptrain->ctl_code = -4;
        ptrain->buf[0] = 'f';
        ret = trans_send_cycle(pconn->newfd, ptrain, sizeof(Train_t));
        ERROR_CHECK(ret, -1, "trans_send_cycle");
    }

    return 0;
}
