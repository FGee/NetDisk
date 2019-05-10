#ifndef __FACTORY_H__
#define __FACTORY_H__
#include <func.h>
#include "work_que.h"

typedef struct {
    pthread_t *pthid;//store the start pointer point to thread id
    int threadNum;//the thread number which will to create
    pthread_cond_t cond;//the condition arugement that sync between main thread and child thread
    Que_t work_que;//store the task that main thread produce
    short startFlag;// the signal of factory start
} Factory_t, *pFactory_t;

typedef void* (*pThread_task_t)(void *pArg);

int factory_s_init(pFactory_t pfs);

int factoryInit(pFactory_t);

int factoryStart(pFactory_t, pThread_task_t);

int factoryDestroy(pFactory_t);

void* thread_puts(void *pArg);

void* thread_gets(void *pArg);

#endif


