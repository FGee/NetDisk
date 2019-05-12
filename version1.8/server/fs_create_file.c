#include "fs_ahead.h" 

int file_sys_create_file(pFileSysInfo_t pfs)
{
    int ret;

    Query_t query;
    bzero(&query, sizeof(Query_t));

    char charType = 'f';

    sprintf(query.buf, "insert into %s (userID, precode, fileName, fileType, fileSize, md5sum)" \
            " values (%d, %d, '%s', '%c', '%d', '%s');", pfs->table, pfs->userID, 
            pfs->codePath[pfs->pathPointer], pfs->entryName, charType, pfs->fileSize, pfs->md5sum);

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

