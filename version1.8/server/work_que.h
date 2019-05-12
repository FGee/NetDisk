#ifndef __WORK_QUE_H__
#define __WORK_QUE_H__
#include "conn_que.h"

typedef struct node {
    //char fileName[32];
    int newfd_put_get;
    pConnNode_t pconn;
    struct node *pNext;
} Node_t, *pNode_t;

typedef struct {
    pNode_t phead;
    pNode_t ptail;
    pthread_mutex_t que_mutex;//the mutex of que
    int size;
    int capacity;
} Que_t, *pQue_t;


int queInit(pQue_t pq, int cap);
int queInsert(pQue_t pq, pNode_t pnew);
int queGet(pQue_t pq, pNode_t *pget);

int thread_puts_recv_big(pNode_t pget);
int thread_puts_recv_small(pNode_t pget);
int thread_puts_doing_ser(pNode_t pget);
#endif

