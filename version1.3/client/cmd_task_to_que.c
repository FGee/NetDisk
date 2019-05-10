#include "cmd.h"

int cmd_task_to_que(pFactory_t pf, pTokenInfo_t ptokenInfo)
{
    int ret;
    pQue_t pq = &pf->work_que;

    if (pq->size <= pq->capacity) {
        pNode_t pnew = (pNode_t)calloc(1, sizeof(Node_t));
        ERROR_CHECK(pnew, NULL, "calloc");
        pthread_mutex_lock(&pq->que_mutex);
        ret = queInsert(pq, pnew);
        ERROR_CHECK(ret, -1, "queInsert");
        pthread_mutex_unlock(&pq->que_mutex);

        pthread_cond_signal(&pf->cond);
    } else {
        printf("the size of task queue is more than capacity\n");
    }

    return 0;
}
