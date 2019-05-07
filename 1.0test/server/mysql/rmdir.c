#include "mysql_file_sys.h"

int file_sys_rmdir(pFileSysInfo_t pfs)
{
    int ret;

    pfs->entryType = 0;// if the dir is exist in the current dir 
    ret = file_sys_entry_exist(pfs);

    if (0 == ret) {
#ifdef RMDIR_ERROR
        printf("rmdir: the entry is not exist, return val 100");
#endif
        return 100; //to tell the user the dir is not exist
    } else if (1 == ret) {
        ret = file_sys_dir_empty(pfs);
        if (0 == ret) {
            printf("not empty , can not del\n");
#ifdef RMDIR_ERROR
            printf("rmdir: the entry is not empty, return val 110");
#endif
            return 110; // to tell the user the dir is not empty, not allow to del
        } else if (ret != 1) {
#ifdef RMDIR_ERROR
            printf("rmdir: dir_empty error, return val -1");
#endif
            return -1;
        } else if (1 == ret) {// create the dir
            Query_t query;
            bzero(&query, sizeof(Query_t));

            char charType = 'd';

            sprintf(query.buf, "delete from %s where userID=%d and"
                    " precode=%d and fileName='%s' and fileType='%c';", 
                    pfs->table, pfs->userID,
                    pfs->codePath[pfs->pathPointer], pfs->entryName, charType);

#ifdef RMDIR
            printf("rmdir: ");
            puts(query.buf);
            puchar('\n');
#endif

            ret = mysql_query(pfs->conn, query.buf);
            if (ret)
            {
#ifdef RMDIR_ERROR
                printf("rmdir: return value: \n");
#endif
                printf("Error making query:%s\n", mysql_error(pfs->conn));
                return -100;
            } else {
#ifdef RMDIR
                printf("rmdir: delete dir success\n");
#endif
                return 0;
            }
        } else {
#ifdef RMDIR_ERROR
            printf("rmdir: entry exist error, return val -1");
#endif
            return -1;
        }
    }
    return 0;
}
