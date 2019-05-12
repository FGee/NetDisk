#include "ctl_code.h"

int ctl_code22(pTrain_t ptrain, pConnNode_t pconn, pConnQue_t pcq, pMysql_t pmysql, pEpoll_t pep, pFactory_t facts)
{ 
    int ret;
    pConnNode_t pconn_usr = NULL;
    unsigned char token_recv[100];
    memcpy(token_recv, ptrain->buf, 100);
    ret = conn_que_token_exist(pcq, &pconn_usr, token_recv);//find the usr count existed
    printf("pconn_usr %p\n", pconn_usr);

    if (0 == ret) {
        //close the newfd
        //delete from conn_que
        //cancel epoll
    } else if (1 == ret) {
        pFactory_t pf = &facts[1];
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
            printf("here\n");

            pthread_cond_signal(&pf->cond);
        } else {
            printf("the size of task queue is more than capacity\n");
        }
    }

    return 0;
}
