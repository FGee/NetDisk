#include "mysql_file_sys.h"

int file_sys_dir_empty(pFileSysInfo_t pfs)
{
    int ret;
    Query_t query;
    bzero(&query, sizeof(Query_t));

    // read the code of dir from table 
    int code; //save the code of dir
    sprintf(query.buf, "select code from %s where userID=%d and" \
           " precode=%d and fileName='%s' and fileType='d';", 
           pfs->table, pfs->userID, 
           pfs->codePath[pfs->pathPointer], pfs->entryName);

#ifdef DIR_EMPTY
    printf("dir_empty: %s\n", query.buf);
#endif

    Result_t result;
    bzero(&result, sizeof(Result_t));

    ret = mysql_query(pfs->conn, query.buf);
    if (ret)
    {
#ifdef DIR_EMPTY_ERROR
    printf("dir_empty: query, return val -150");
#endif
        printf("Error making query:%s\n", mysql_error(pfs->conn));
        return -150;
    } else {
        result.res = mysql_use_result(pfs->conn);
        if (result.res) {
            result.row = mysql_fetch_row(result.res);
            code = atoi(result.row[0]);
            //printf("code %d\n", code);
        } else {
#ifdef DIR_EMPTY_ERROR
    printf("dir_empty: mysql_use_result, return val -160");
#endif
            return -160;
        }
        mysql_free_result(result.res);
    }

    //query to search the entry if exist which precode equal to code
    bzero(&query, sizeof(Query_t));

    sprintf(query.buf, "select * from %s where userID=%d and" \
           " precode=%d", pfs->table, pfs->userID, code);

#ifdef DIR_EMPTY
    printf("dir_empty: %s\n", query.buf);
#endif

    bzero(&result, sizeof(Result_t));

    ret = mysql_query(pfs->conn, query.buf);
    if (ret)
    {
#ifdef DIR_EMPTY_ERROR
    printf("dir_empty: query, return val -170");
#endif
        printf("Error making query:%s\n", mysql_error(pfs->conn));
        return -170;
    } else {
        result.res = mysql_use_result(pfs->conn);
        if (result.res) {
            if (NULL != (result.row = mysql_fetch_row(result.res))) {
#ifdef DIR_EMPTY
                printf("dir_empty: the entry is not empty\n");
#endif
                mysql_free_result(result.res);
                return 0;
            } else {
#ifdef DIR_EMPTY
                printf("dir_empty: the entry is empty\n");
#endif
                mysql_free_result(result.res);
                return 1;
            }
        } else {
#ifdef DIR_EMPTY_ERROR
    printf("dir_empty: mysql_use_result, return val -180");
#endif
            printf("Error mysql_use_result:%s\n", mysql_error(pfs->conn));
            return -180;
        }
    }
}
