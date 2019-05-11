#ifndef __CMD_H__
#define __CMD_H__
#include <func.h>
#include "trans_ahead_cli.h"
#include "factory.h"
#include "token.h"

typedef struct {
    char command[16];
    char object[32];
} Cmd_t, *pCmd_t;

int cmd_separate(char *cmdBuf, pCmd_t pcmd);

int cmd_load(pCmd_t pcmd, int wrdNum, pTrain_t pt, pFactory_t pfs, pTokenInfo_t pto);      

int cmd_task_to_que(pFactory_t pf, pTokenInfo_t ptokenInfo);

#endif
