#include "ctl_code.h"
#include "my_epoll.h"

int ctl_code21(pTrain_t ptrain, pConnNode_t pconn, pConnQue_t pcq, pMysql_t pmysql, pEpoll_t pep, pFactory_t facts)
{
    int ret;
    pConnNode_t pconn_usr = NULL;
    unsigned char token_recv[100];
    memcpy(token_recv, ptrain->buf, 100);
//#ifdef EPOLL
printf("toekn_recv:");
        for (unsigned long i = 0; i < strlen((char*)token_recv); i++) {
            printf("%02x", token_recv[i]);
        }
        printf("\n");
// #endif

    ret = conn_que_token_exist(pcq, &pconn_usr, token_recv);//find the usr count existed
    //#ifdef EPOLL
    printf("token_usr:");
        for (unsigned long i = 0; i < strlen((char*)pconn_usr->pUser->token); i++) {
            printf("%02x", pconn_usr->pUser->token[i]);
        }
        printf("\n");
//boot#endif

    printf("pconn_usr %p\n pconn_pusr %p\n", pconn_usr, pconn_usr->pUser);
    printf("pconn_usr name %s\n", pconn_usr->pUser->userName);
    printf("pconn_usr->pFs %p\n", pconn_usr->pFs);
    if (0 == ret) {
        //close the newfd
        //delete from conn_que
        //cancel epoll
    } else if (1 == ret) {
        pFactory_t pf = &facts[0];
        pQue_t pq = &pf->work_que;

        if (pq->size <= pq->capacity) {
            pNode_t pnew = (pNode_t)calloc(1, sizeof(Node_t));
            ERROR_CHECK(pnew, NULL, "calloc");
            pnew->newfd_put_get = pconn->newfd;// task info
            pnew->pconn = pconn_usr;
            pthread_mutex_lock(&pq->que_mutex);
            ret = queInsert(pq, pnew);
            ERROR_CHECK(ret, -1, "queInsert");
            pthread_mutex_unlock(&pq->que_mutex);
            
            ret = my_epoll_cancel(pep, pcq, pconn, pmysql);
            
            pthread_cond_signal(&pf->cond);
            
        } else {
            printf("the size of task queue is more than capacity\n");
        }
    }

    return 0;
}
