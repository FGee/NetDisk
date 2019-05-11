#include "cmd.h"
#include "trans_ahead_cli.h"

int cmd_load(pCmd_t pcmd, int wrdNum, pTrain_t pt, pFactory_t pfs, pTokenInfo_t pto)
{
    if (!strcmp("mkdir", pcmd->command)) {
        if (2 != wrdNum) {
            return -1;
        }
        pt->ctl_code = 11;
        pt->dataLenth = strlen(pcmd->object);
        memcpy(pt->buf, pcmd->object, pt->dataLenth);
    } else if (!strcmp("rmdir", pcmd->command)) {
        if (2 != wrdNum) {
            return -1;
        }
        pt->ctl_code = 12;
        pt->dataLenth = strlen(pcmd->object);
        memcpy(pt->buf, pcmd->object, pt->dataLenth);
    } else if (!strcmp("ls", pcmd->command)) {
        if (1 != wrdNum) {
            return -1;
        }
        pt->ctl_code = 13;
    } else if (!strcmp("cd", pcmd->command)) {
        if (2 != wrdNum) {
            return -1;
        }
        if (!strcmp(pcmd->object, "..")) {
            pt->ctl_code = 14;
        } else {
            pt->ctl_code = 15;
            pt->dataLenth = strlen(pcmd->object);
            memcpy(pt->buf, pcmd->object, pt->dataLenth);
        }
    } else if (!strcmp("pwd", pcmd->command)) {
        if (1 != wrdNum) {
            return -1;
        }
        pt->ctl_code = 16;
    } else if (!strcmp("rm", pcmd->command)) {
        if (2 != wrdNum) {
            return -1;
        }
        pt->ctl_code = 17;
        pt->dataLenth = strlen(pcmd->object);
        memcpy(pt->buf, pcmd->object, pt->dataLenth);
    } else if (!strcmp("puts", pcmd->command)) {
        if (2 != wrdNum) {
            return -1;
        }
        //pt->dataLenth = strlen(pcmd->object;
        //memcpy(pt->buf, pcmd->object, pt->dataLenth);
        strcpy(pto->fileName, pcmd->object);//file name to puts thread
        cmd_task_to_que(&pfs[0], pto);
    } else if (!strcmp("gets", pcmd->command)) {
        if (2 != wrdNum) {
            return -1;
        }
        strcpy(pto->fileName, pcmd->object);//file name to gets thread
        cmd_task_to_que(&pfs[1], pto);
    } else {
        return -1;
    }

    return 0;
}
