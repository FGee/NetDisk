#include "mysql_file_sys.h"

int file_sys_cd(pFileSysInfo_t pfs)
{
    int ret;
    if (0 == pfs->opsCode) {
        pfs->entryType = 0;
        ret = file_sys_entry_exist(pfs);
        if (0 == ret) {
#ifdef CD_ERROR
            printf("cd: the dir is not exist in the current dir, return val 250\n");
#endif
                return 250;
        } else if (1 == ret) {

            Query_t query;
            bzero(&query, sizeof(Query_t));

            // read the code of dir from table
            int code; //save the code of dir
            sprintf(query.buf, "select code from %s where userID=%d and" \
                    " precode=%d and fileName='%s' and fileType='d';",
                    pfs->table, pfs->userID,
                    pfs->codePath[pfs->pathPointer], pfs->entryName);

#ifdef CD
            printf("cd: %s\n", query.buf);
#endif

            Result_t result;
            bzero(&result, sizeof(Result_t));

            ret = mysql_query(pfs->conn, query.buf);
            if (ret)
            {
#ifdef CD_ERROR
                printf("cd: query, return val -250");
#endif
                printf("Error making query:%s\n", mysql_error(pfs->conn));
                return -250;
            } else {
                result.res = mysql_use_result(pfs->conn);
                if (result.res) {
                    result.row = mysql_fetch_row(result.res);
                    code = atoi(result.row[0]);
                    //printf("code %d\n", code);
                } else {
#ifdef CD_ERROR
                    printf("cd: mysql_use_result, return val -260");
#endif
                    return -260;
                }
                mysql_free_result(result.res);
            }
            
            // change path
            ++pfs->pathPointer;
            pfs->codePath[pfs->pathPointer] = code;
#ifdef CD
            printf("cd: change to child dir success\n");
#endif
            return 0;
        }
    } else if (1 == pfs->opsCode) {
        if (0 == pfs->pathPointer) {
#ifdef CD_ERROR
            printf("cd: current dir is root dir, return val 260\n");
#endif
            return 260;
        } else {
            --pfs->pathPointer;
#ifdef CD
            printf("cd: change to parent dir success\n");
#endif
            return 1;
        }
    }

    return 0;
}
