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

int conn_que_token_exist(pConnQue_t pcq, pConnNode_t *ppfind, unsigned char *token_recv)
{
    pConnNode_t pcur = pcq->phead;
    while (NULL != pcur) {
        int i = 0;
        for (i = 0; i < 100; ++i) {
            if (pcur->pUser->token[i] != token_recv[i]) {
                break;
            }
        }
        if (100 == i) {
            *ppfind = pcur;
            printf("token verify success\n");
            return 1;
        } else {
            pcur = pcur->pNext;
        }
    }

    printf("token verify fail\n");
    return 0;
}

int conn_que_delete(pConnQue_t pcq, pConnNode_t pdel)
{
    pConnNode_t ppre = NULL;
    pConnNode_t pcur = NULL;
    ppre = pcur = pcq->phead;
    if (NULL == pcq->ptail) {
        printf("conn_que is empty\n");
        return -1;
    } else if (pdel == pcq->phead) {
        if (pdel == pcq->ptail) {
            pcq->phead = pcq->ptail = NULL;
        } else {
            pcq->phead = pcq->phead->pNext;
        }
    } else {
        while (NULL != pcur) {
            if (pdel == pcur) {
                ppre->pNext = pcur->pNext;
                break;
            }
            ppre = pcur;
            pcur = pcur->pNext;
        }
        if (pcur == pcq->ptail) {
            pcq->ptail = ppre;
        } 
        if (NULL == pcur) {
            printf("Don't find this Node in conn_que\n");
            return -1;
        }
    }
    free(pdel);
    --pcq->size;

    return 0;
}
