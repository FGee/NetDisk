#include "mysql_file_sys.h"

int main(int argc, char** argv)
{
    int ret;

    Mysql_t my;
    ret = mysqlInit(&my);
    ERROR_CHECK(ret, -1, "mysqlInit");

    FileSysInfo_t fsi;
    bzero(&fsi, sizeof(FileSysInfo_t));
    strcpy(fsi.table, "filesys");
    printf("input the userid: \n");
    scanf("%d", &fsi.userID);
    //fsi.codePath[1] = 3;
    //fsi.pathPointer = 0;
    //fsi.entryType = atoi(argv[5]);
    fsi.conn = my.conn;

    char instrBuf[20];

    while (1) {
        bzero (instrBuf, sizeof(instrBuf));
        scanf("%s %s", instrBuf, fsi.entryName);

        if (!strcmp("mkdir", instrBuf)) {
            ret = file_sys_mkdir(&fsi);
        } else if (!strcmp("rmdir", instrBuf)) {
            ret = file_sys_rmdir(&fsi);
        } else if (!strcmp("ls", instrBuf)) {
            ret = file_sys_ls(&fsi);
        } else if (!strcmp("cd", instrBuf)) {
            fsi.entryType = 0;
            if (!strcmp(fsi.entryName, "..")) {
                fsi.opsCode = 1;
            } else {
                fsi.opsCode = 0;
            }
            ret = file_sys_cd(&fsi);
        }
    }

    return 0;
}
