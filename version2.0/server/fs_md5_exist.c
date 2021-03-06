#include "fs_ahead.h"

int file_sys_md5_exist(pFileSysInfo_t pfs)
{
    printf("go in md5_exist\n");
    int ret;
    Query_t query;
    bzero(&query, sizeof(Query_t));

    sprintf(query.buf, "select * from %s where fileType='f' and md5sum='%s';", 
            pfs->table, pfs->md5sum);

#ifdef ENTRY_EXIST
    printf("entry_exist: ");
    puts(query.buf);
    putchar('\n');
#endif

    Result_t result;
    bzero(&result, sizeof(Result_t));
    int md5_cnt = 0;
    

    ret = mysql_query(pfs->conn, query.buf);
    if (ret)
    {
        //#ifdef ENTRY_EXIST_ERROR
        printf("entry_exist: return val -60\n");
        //#endif
        printf("Error making query:%s\n", mysql_error(pfs->conn));
        return -60;
    } else {
        result.res = mysql_use_result(pfs->conn);
        if (result.res) {
            while (NULL != (result.row = mysql_fetch_row(result.res))) {
                ++md5_cnt;
            }
        } else {
#ifdef ENTRY_EXIST_ERROR
            printf("entry_exist: mysql_use_result error, return val -70\n");
#endif
            return -70;
        }
    }
    mysql_free_result(result.res);

    return md5_cnt;
}
