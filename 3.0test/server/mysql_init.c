#include "fs_ahead.h"

int mysqlInit(pMysql_t pMy)
{
    bzero(pMy, sizeof(Mysql_t));
    pMy->server = "localhost";
    pMy->user = "root";
    pMy->password = "456456";
    pMy->database = "netDisk";
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
