#include "mysql_file_sys.h"

int file_sys_ls(pFileSysInfo_t pfs)
{
    int ret;
    Query_t query;
    bzero(&query, sizeof(Query_t));

    sprintf(query.buf, "select fileName,fileType,fileSize from %s"
            " where userID=%d and precode=%d", 
            pfs->table, pfs->userID, pfs->codePath[pfs->pathPointer]);

#ifdef LS
    printf("ls: %s\n", query.buf);
#endif

    Result_t result;
    bzero(&result, sizeof(Result_t));

    ret = mysql_query(pfs->conn, query.buf);
    if (ret)
    {
#ifdef LS_ERROR
        printf("entry_exist: return val -200\n");
#endif
        printf("Error making query:%s\n", mysql_error(pfs->conn));
        return -200;
    } else {
        result.res = mysql_use_result(pfs->conn);
        if (result.res) {
            while (NULL != (result.row = mysql_fetch_row(result.res))) {
                for(unsigned i = 0; i < mysql_num_fields(result.res); i++)
                {
                    printf("%-8s", result.row[i]);
                }
                printf("\n");                
            }
        } else {
#ifdef LS_ERROR
            printf("ls: mysql_use_result error, return val -210\n");
#endif
            return -210;
        }
    }
#ifdef LS
    printf("ls: ls success, return val 0\n");
#endif
    mysql_free_result(result.res);
    return 0;
}
