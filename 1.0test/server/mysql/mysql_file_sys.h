#ifndef __mysql_file_sys_h__
#define __mysql_file_sys_h__
#include <func.h>

#define TABLE_SZ 32
#define CD_PTH_SZ 32
#define EN_NM_SZ 64
#define QUERY_SZ 200

typedef struct {
    char buf[QUERY_SZ];
} Query_t, *pQuery_t;

typedef struct {
    const char* server;
    const char* user;
    const char* password;
    const char* database;
    MYSQL *conn;
} Mysql_t, *pMysql_t;

typedef struct {
    MYSQL_RES *res;
    MYSQL_ROW row;
} Result_t, *pResult;

typedef struct {
    char table[TABLE_SZ];
    int userID;
    int codePath[CD_PTH_SZ];
    int pathPointer;
    char entryName[EN_NM_SZ];
    int entryType;
    int opsCode;
    MYSQL *conn;
} FileSysInfo_t, *pFileSysInfo_t;

int mysqlInit(pMysql_t pMy);

char file_sys_type(int entryType);

int file_sys_entry_exist(pFileSysInfo_t pfs);

int file_sys_mkdir(pFileSysInfo_t pfs);

int file_sys_rmdir(pFileSysInfo_t pfs);

int file_sys_dir_empty(pFileSysInfo_t pfs);

int file_sys_ls(pFileSysInfo_t pfs);

int file_sys_cd(pFileSysInfo_t pfs);
#endif
