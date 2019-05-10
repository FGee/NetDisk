#include "fs_ahead.h"

int mysqlInit(pMysql_t pMy)
{
    bzero(pMy, sizeof(Mysql_t));
    memcpy(pMy->server, "localhost", strlen("localhost"));
    memcpy(pMy->user, "root", strlen("root"));
    memcpy(pMy->password, "456456", strlen("456456"));
    memcpy(pMy->database, "netDisk", strlen("netDisk"));
    memcpy(pMy->user_table, "user", strlen("user"));
    memcpy(pMy->fs_table, "filesys", strlen("filesys"));
    memcpy(pMy->log_table, "log", strlen("log"));
    pMy->conn = mysql_init(NULL);
    if (!mysql_real_connect \
        (pMy->conn, pMy->server, pMy->user, pMy->password, pMy->database, 0, NULL, 0)) {
            printf("Error connecting to database:%s\n", mysql_error(pMy->conn));
            return -1;
    } else {
#ifdef MYSQL_DEBUG
        printf("Connected...\n");
#endif
    }

    return 0;
}
