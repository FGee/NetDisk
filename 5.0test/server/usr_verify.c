#include "fs_ahead.h"

int usr_sys_verify(const pUserInfo_t pui)
{
    int ret;
    Query_t query;
    sprintf(query.buf, "select ciphertext from %s where userName='%s';",
            pui->table, pui->userName);

#ifdef USR_VERIFY
    printf("usr_verify: %s\n", query.buf);
#endif

    Result_t result;
    bzero(&result, sizeof(Result_t));

    ret = mysql_query(pui->conn, query.buf);
    if (ret)
    {
        printf("Error making query:%s\n", mysql_error(pui->conn));
        return -700;
    } else {
        result.res = mysql_use_result(pui->conn);
        if (result.res) {
            if (NULL != (result.row = mysql_fetch_row(result.res))) {
                if (!strcmp(pui->ciphertext, result.row[0])) {
                    pui->userID = atoi(result.row[0]);
#ifdef USR_VERIFY
                    printf("usr_verify: success\n");
#endif
                    mysql_free_result(result.res);
                    return 1;
                } else {
#ifdef USR_VERIFY
                printf("usr_verify: fail\n");
#endif
                    mysql_free_result(result.res);
                    return 0;
                }
            }
        } else {
#ifdef USR_VERIFY_ERROR
            printf("usr_verify: mysql_use_result error, return val -710\n");
#endif
            return -710;
        }
    }

    return 0;
}

