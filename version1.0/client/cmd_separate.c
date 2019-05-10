#include "cmd.h"

int cmd_separate(char *cmdBuf, pCmd_t pcmd)
{
    int wrd_cnt = 0;
    int cmd_cnt = 0;
    int obj_cnt = 0;
    int pre_is_space = true;
    int is_word = 0;
    for (int i = 0; cmdBuf[i] != '\n'; ++i) {
        if (pre_is_space && cmdBuf[i] != ' ') {
            pre_is_space = false;
            is_word = 1;
            ++wrd_cnt;
        }
        else if(!pre_is_space && cmdBuf[i] == ' ')
        {
            pre_is_space = true;
            is_word = 0;
        }
        if (is_word && 1 == wrd_cnt) {
            pcmd->command[cmd_cnt] = cmdBuf[i];
            ++cmd_cnt;
        }
        if (is_word && 2 == wrd_cnt) {
            pcmd->object[obj_cnt] = cmdBuf[i]; 
            ++obj_cnt;
        }
    }

    if (0 == wrd_cnt || wrd_cnt > 2) {
        return -1;
    } 
#ifdef DEBUG
    printf("%s\n", pcmd->command);
    printf("%s\n", pcmd->object);
#endif

    return wrd_cnt;
}
