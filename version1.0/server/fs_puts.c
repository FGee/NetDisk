#include "fs_ahead.h"

int file_sys_puts(pFileSysInfo_t pfs)
{
    int ret;

    pfs->entryType = 1;
    ret = file_sys_entry_exist(pfs);

    if (1 == ret) {
#ifdef PUTS_ERROR
        printf("puts: the file exist, return value: 400\n");
#endif
        return 400;
    } else if (0 == ret) {
        Query_t query;
        bzero(&query, sizeof(Query_t));

        char charType = 'f';

        sprintf(query.buf, "insert into %s (userID, precode, fileName, fileType)" \
                " values (%d, %d, '%s', '%c');", pfs->table, pfs->userID, 
                pfs->codePath[pfs->pathPointer], pfs->entryName, charType);

#ifdef PUTS
        printf("puts: %s\n", query.buf);
#endif

        ret = mysql_query(pfs->conn, query.buf);
        if (ret)
        {
#ifdef PUTS_ERROR
        printf("puts: return value: -400\n");
#endif
            printf("Error making query:%s\n", mysql_error(pfs->conn));
            return -400;
        }
    }

#ifdef PUTS
            printf("puts: create the file in the table\n");
#endif
            return 0;
}
