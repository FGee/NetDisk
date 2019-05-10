#include "fs_ahead.h"

int file_sys_info_init(pFileSysInfo_t* ppsi, const pMysql_t pmysql)
{
    *ppsi = (pFileSysInfo_t)calloc(1, sizeof(FileSysInfo_t));
    (*ppsi)->conn = pmysql->conn;
    memcpy((*ppsi)->table, pmysql->fs_table, strlen(pmysql->fs_table));

    return 0;
}
