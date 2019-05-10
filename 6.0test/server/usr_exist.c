#include "fs_ahead.h"

int usr_sys_usrName_exist(const pUserInfo_t pui)
{
    int ret;
    Query_t query;
    sprintf(query.buf, "select * from %s where userName='%s';",
            pui->table, pui->userName);

#ifdef USR_EXIST
    printf("usr_exist: %s\n", query.buf);
#endif

    Result_t result;
    bzero(&result, sizeof(Result_t));

    ret = mysql_query(pui->conn, query.buf);
    if (ret)
    {
        printf("Error making query:%s\n", mysql_error(pui->conn));
        return -650;
    } else {
        result.res = mysql_use_result(pui->conn);
        if (result.res) {
            if (NULL != (result.row = mysql_fetch_row(result.res))) {
                memcpy(pui->salt, result.row[2], strlen(result.row[2]));
#ifdef USR_EXIST
                printf("usr_exist: the usr name exist\n");
#endif
                mysql_free_result(result.res);
                return 1;
            } else {
#ifdef USR_EXIST
                printf("usr_exist: the user name is not exist\n");
#endif
                mysql_free_result(result.res);
                return 0;
            }
        } else {
#ifdef USR_EXIST_ERROR
            printf("usr_exist: mysql_use_result error, return val -660\n");
#endif
            return -660;
        }
    }
}

