#ifndef __CONN_QUE_H__
#define __CONN_QUE_H__
#include "fs_ahead.h"
#include <func.h>

typedef struct conn_node {
    int newfd;
    pFileSysInfo_t pFs;
    pUserInfo_t pUser;
    struct sockaddr_in clientAddr;
    struct conn_node *pNext;
} ConnNode_t, *pConnNode_t;

typedef struct {
    pConnNode_t phead;
    pConnNode_t ptail;
    int size;
    int max_conn; 
} ConnQue_t, *pConnQue_t;

int conn_que_init(pConnQue_t pcq, int max);

int conn_que_insert(pConnQue_t pcq, pConnNode_t pnew);

int conn_que_exist(const pConnQue_t pcq, pConnNode_t *ppfind, int fdFind);

int conn_que_token_exist(pConnQue_t pcq, pConnNode_t *ppfind, unsigned char *token);

int conn_que_delete(pConnQue_t pcq, pConnNode_t pdel);

#endif
