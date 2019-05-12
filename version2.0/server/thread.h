#ifndef __THREAD_H__
#define __THREAD_H__
#include "work_que.h"

int thread_puts_recv_big(pNode_t pget);
int thread_puts_recv_small(pNode_t pget);
int thread_puts_doing_ser(pNode_t pget);

int thread_gets_doing_ser(pNode_t pget);
int thread_gets_send_big(pNode_t pget, int break_point);
int thread_gets_send_small(pNode_t pget, int break_point);

#endif
