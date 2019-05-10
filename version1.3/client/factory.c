#include "factory.h"

int factoryInit(pFactory_t pf)
{
    int ret;
    pf->pthid = (pthread_t*)calloc(pf->threadNum, sizeof(pthread_t));
    ERROR_CHECK(pf->pthid, NULL, "calloc");//heat space not enough

    ret = queInit(&pf->work_que, pf->work_que.capacity);
    ERROR_CHECK(ret, -1, "queInit");

    ret = pthread_cond_init(&pf->cond, NULL);

    return 0;
}

int factoryStart(pFactory_t pf, pThread_task_t pttFunc)
{
    //int ret;
    for (int i = 0; i < pf->threadNum; ++i) {
        pthread_create(pf->pthid + i, NULL, pttFunc, (void*)pf);
    }
    pf->startFlag = 1;

    return 0;
}

int factoryDestroy(pFactory_t pf)
{
    for (int i = 0; i < pf->threadNum; ++i) {//avoid deadlock at the condition args
        pthread_cond_signal(&pf->cond);
    }
    for (int i = 0; i < pf->threadNum; ++i) {
        pthread_join(pf->pthid[i], NULL);
    }
    pthread_mutex_destroy(&pf->work_que.que_mutex);
    pthread_cond_destroy(&pf->cond);

    return 0;
}
