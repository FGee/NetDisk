#ifndef __CTL_CODE_H__
#define __CTL_CODE_H__
#include "trans_ahead.h"
#include "conn_que.h"
#include "fs_ahead.h"
#include "my_epoll.h"

#define CTL_CODE_SZ 20

typedef int (*pCtl_code_t)(pTrain_t ptrain, pConnNode_t pconn, pConnQue_t pcq, pMysql_t pmysql, pEpoll_t pep);

typedef pCtl_code_t* ppCtl_code_t; 

int ctl_code1(pTrain_t ptrain, pConnNode_t pconn, pConnQue_t pcq, pMysql_t pmysql, pEpoll_t pep);

int ctl_code2(pTrain_t ptrain, pConnNode_t pconn, pConnQue_t pcq, pMysql_t pmysql, pEpoll_t pep);

int ctl_code3(pTrain_t ptrain, pConnNode_t pconn, pConnQue_t pcq, pMysql_t pmysql, pEpoll_t pep);

int ctl_code4(pTrain_t ptrain, pConnNode_t pconn, pConnQue_t pcq, pMysql_t pmysql, pEpoll_t pep);

int ctl_code_funcArr_init(pCtl_code_t* arrCtl);

#endif
