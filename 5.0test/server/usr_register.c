#include "fs_ahead.h"

int usr_sys_register(pUserInfo_t pui)
{
    int ret;
    Query_t query;
    bzero(&query, sizeof(Query_t));

    sprintf(query.buf, "insert into %s (userName, salt, ciphertext)"
            " values ('%s', '%s', '%s');", 
            pui->table,
            pui->userName, pui->salt, pui->ciphertext);

#ifdef REGISTER
    printf("register: %s\n", query.buf);
#endif

    ret = mysql_query(pui->conn, query.buf);
    if (ret)
    {
#ifdef REGISTER_ERROR
        printf("register: return value: -600\n");
#endif
        printf("Error making query:%s\n", mysql_error(pui->conn));
        return -600;
    }

#ifdef REGISTER
    printf("register: user register success\n");
#endif
    return 0;
}
