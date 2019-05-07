#include "fs_ahead.h"

int file_sys_rm(pFileSysInfo_t pfs)
{
    int ret;

    pfs->entryType = 1;// if the file is exist in the current dir 
    ret = file_sys_entry_exist(pfs);

    if (0 == ret) {
#ifdef RM_ERROR
        printf("rm: the file is not exist, return val 350\n");
#endif
        return 350; //to tell the user the file is not exist
    } else if (1 == ret) {
        Query_t query;
        bzero(&query, sizeof(Query_t));

        char charType = 'f';

        sprintf(query.buf, "delete from %s where userID=%d and"
                " precode=%d and fileName='%s' and fileType='%c';", 
                pfs->table, pfs->userID,
                pfs->codePath[pfs->pathPointer], pfs->entryName, charType);

#ifdef RM
        printf("rm: %s\n", query.buf);
#endif

        ret = mysql_query(pfs->conn, query.buf);
        if (ret)
        {
#ifdef RM_ERROR
            printf("rm: return value: -350\n");
#endif
            printf("Error making query:%s\n", mysql_error(pfs->conn));
            return -350;
        }
    }

#ifdef RM
            printf("rm: delete file success\n");
#endif
    return 0;
}
