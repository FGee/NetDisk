#include "mysql_file_sys.h"

int file_sys_gets(pFileSysInfo_t pfs)
{
    int ret;

    pfs->entryType = 1;
    ret = file_sys_entry_exist(pfs);

    if (0 == ret) {
#ifdef PUTS_ERROR
        printf("gets: the file is not exist, return value: 450\n");
#endif
        return 450;
    } else if (1 == ret) {
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
