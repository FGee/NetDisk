#include "conn_que.h"

int conn_que_init(pConnQue_t pcq, int max)
{
    bzero(pcq, sizeof(ConnQue_t));
    pcq->max_conn = max;

    return 0;
}

int conn_que_insert(pConnQue_t pcq, pConnNode_t pnew)
{
    if (NULL == pcq->ptail) {
        pcq->ptail = pnew;
        pcq->phead = pnew;
    } else {
        pcq->ptail->pNext = pnew;
        pcq->ptail = pnew;
    }
    ++pcq->size;

    return 0;
}

int conn_que_exist(pConnQue_t pcq, pConnNode_t *ppfind, int fdFind)
{
    pConnNode_t pcur = pcq->phead;
    while (NULL != pcur) {
        if (fdFind == pcur->newfd) {
            *ppfind = pcur;
            return 1;
        }
        pcur = pcur->pNext;
    }
    return 0;
}

int conn_que_delete(pConnQue_t pcq, pConnNode_t pdel)
{

    return 0;
}
