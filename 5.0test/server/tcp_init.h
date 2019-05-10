#ifndef __TCP_INIT_H__
#define __TCP_INIT_H__
#include "ahead.h"

#define MAX_CLIENT 20

int tcpInit(int *psockFd, const char *ip, const char *port);
#endif 
