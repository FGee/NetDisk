#include "factory.h"
#include "tcp_cli.h"
#include "trans_ahead_cli.h"

void* thread_puts(void *pArg)
{
    int ret;
    pFactory_t pf = (pFactory_t)pArg;
    pQue_t pq = &pf->work_que;
    int sockFd;
    Train_t train;
    ret = tcp_init(&sockFd);

    pNode_t pget = NULL;
    int getSuccess = 0;

    while (1) {
        if (pf->startFlag || pq->size) {
            pthread_mutex_lock(&pq->que_mutex);
            pthread_cond_wait(&pf->cond, &pq->que_mutex);
            getSuccess = queGet(pq, &pget);
            pthread_mutex_unlock(&pq->que_mutex);
            if (getSuccess) {
                printf("do puts task\n");
                bzero(&train, sizeof(train));
                train.ctl_code = 21;
                train.dataLenth = strlen((char*)pget->ptokenInfo->token);
                memcpy(train.buf, pget->ptokenInfo->token, train.dataLenth);
                ret = trans_send_cycle(sockFd, &train, sizeof(train));
                
                while (1);
                //tranFile(pget->fd);
                //close(pget->fd); //print puts task finsh
                //free(pget);
                getSuccess = 0;
            }
        }
    }
}
