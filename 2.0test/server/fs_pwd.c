#include "fs_ahead.h"

int file_sys_pwd(pFileSysInfo_t pfs) 
{
    int ret;
    char pathBuf[128] = { 0 };
    sprintf(pathBuf, "/");
    Query_t query;
    Result_t result;
    for (int i = 1; i <= pfs->pathPointer; ++i) {
        bzero(&query, sizeof(Query_t));

        sprintf(query.buf, "select fileName from %s"
                " where userID=%d and code=%d and fileType='d';", 
                pfs->table, pfs->userID, pfs->codePath[i]);

#ifdef PWD
        printf("pwd: %s\n", query.buf);
#endif

        bzero(&result, sizeof(Result_t));

        ret = mysql_query(pfs->conn, query.buf);
        if (ret)
        {
#ifdef PWD_ERROR
            printf("pwd: query, return val -300\n");
#endif
            printf("Error making query:%s\n", mysql_error(pfs->conn));
            return -300;
        } else {
            result.res = mysql_use_result(pfs->conn);
            if (result.res) {
                result.row = mysql_fetch_row(result.res);
                sprintf(pathBuf, "%s%s/", pathBuf, result.row[0]);
                //printf("code %d\n", code);
            } else {
#ifdef PWD_ERROR
                printf("pwd: mysql_use_result, return val -310\n");
#endif
                return -310;
            }
            mysql_free_result(result.res);
        }
    }
    printf("%s\n", pathBuf);
    
#ifdef PWD
            printf("pwd: print path success\n");
#endif
    return 0;
}
