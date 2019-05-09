#include "fs_ahead.h"

char* genRandomString(int lenth)
{
    char *str = (char*)calloc(lenth + 1, sizeof(char));
    if (NULL == str) {
        perror("calloc");
        return NULL;
    }
    int i, flag;

    srand(time(NULL));
    for (i = 0; i < lenth; i++) {
        flag = rand()%3;
        switch (flag) {
        case 0:
            str[i] = rand() % 26 + 'a';
            break;
        case 1:
            str[i] = rand() % 26 + 'A';
            break;
        case 2:
            str[i] = rand() % 10 + '0';
            break;
        }
    }
#ifdef USR
    printf("genRandomString: %s\n", str);
#endif
    return str;
}

int usr_sys_usr_init(pUserInfo_t* ppui, const pMysql_t pmysql)
{
    *ppui = (pUserInfo_t)calloc(1, sizeof(UserInfo_t));
    
    (*ppui)->conn = pmysql->conn;
    memcpy((*ppui)->table, pmysql->user_table, strlen(pmysql->user_table));   
    char *psalt = genRandomString(RANDOM_LENTH);
    strcpy((*ppui)->salt, psalt);
    free(psalt);

    return 0;
}
