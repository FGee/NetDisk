#include "fs_ahead.h"

int file_sys_entry_exist(pFileSysInfo_t pfs)
{
    int ret;
    Query_t query;
    bzero(&query, sizeof(Query_t));

    char charType = '\0';
    charType = file_sys_type(pfs->entryType);
    if ('\0' == charType) {
#ifdef ENTRY_EXIST_ERROR
        printf("entry_exist: no this entry type, return val -50\n");
#endif
        return -50;
    }

    sprintf(query.buf, "select * from %s where userID=%d and" \
           " precode=%d and fileName='%s' and fileType='%c';", 
           pfs->table, pfs->userID, 
           pfs->codePath[pfs->pathPointer], pfs->entryName, charType);

#ifdef ENTRY_EXIST
    printf("entry_exist: ");
    puts(query.buf);
    putchar('\n');
#endif

    Result_t result;
    bzero(&result, sizeof(Result_t));

    ret = mysql_query(pfs->conn, query.buf);
    if (ret)
    {
#ifdef ENTRY_EXIST_ERROR
        printf("entry_exist: return val -60\n");
#endif
        printf("Error making query:%s\n", mysql_error(pfs->conn));
        return -60;
    } else {
        result.res = mysql_use_result(pfs->conn);
        if (result.res) {
            if (NULL != (result.row = mysql_fetch_row(result.res))) {
#ifdef ENTRY_EXIST
                printf("entry_exist: the entry exist\n");
#endif
                mysql_free_result(result.res);
                return 1;
            } else {
//#ifdef ENTRY_EXIST
                printf("entry_exist: the entry is not exist\n");
//#endif
                mysql_free_result(result.res);
                return 0;
            }
        } else {
#ifdef ENTRY_EXIST_ERROR
        printf("entry_exist: mysql_use_result error, return val -70\n");
#endif
            return -70;
        }
    }
}
