#ifndef __THREAD_H__
#define __THREAD_H__
#include "factory.h"
#include "work_que.h"

int thread_puts_doing(pFactory_t pf, pNode_t pget, int sockFd);

int thread_gets_doing(pFactory_t pf, pNode_t pget, int sockFd);

#endif
