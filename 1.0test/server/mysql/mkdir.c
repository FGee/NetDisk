#include "mysql_file_sys.h"

int file_sys_mkdir(pFileSysInfo_t pfs)
{
    int ret;

    pfs->entryType = 0;
    ret = file_sys_entry_exist(pfs);

    if (1 == ret) {
#ifdef MKDIR_ERROR
        printf("mkdir: the entry exist, return value: 10\n");
#endif
        return 10;
    } else if (0 == ret) {
        Query_t query;
        bzero(&query, sizeof(Query_t));

        char charType = 'd';

        sprintf(query.buf, "insert into %s (userID, precode, fileName, fileType)" \
                " values (%d, %d, '%s', '%c');", pfs->table, pfs->userID, 
                pfs->codePath[pfs->pathPointer], pfs->entryName, charType);

#ifdef MKDIR
        printf("mkdir: ");
        puts(query.buf);
        puchar('\n');
#endif

        ret = mysql_query(pfs->conn, query.buf);
        if (ret)
        {
#ifdef MKDIR_ERROR
        printf("mkdir: return value: -10\n");
#endif
            printf("Error making query:%s\n", mysql_error(pfs->conn));
            return -10;
        } else {
#ifdef MKDIR
            printf("mkdir: create dir success\n");
#endif
            return 0;
        }
    }
            return 0;
}
