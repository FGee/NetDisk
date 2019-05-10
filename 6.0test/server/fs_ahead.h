#ifndef __fs_ahead_h__
#define __fs_ahead_h__
#include <func.h>
#define TABLE_SZ 32
#define CD_PTH_SZ 32
#define EN_NM_SZ 64
#define QUERY_SZ 200

typedef struct {
    char buf[QUERY_SZ];
} Query_t, *pQuery_t;

typedef struct {
    char server[TABLE_SZ];
    char user[TABLE_SZ];
    char password[TABLE_SZ];
    char database[TABLE_SZ];
    char fs_table[TABLE_SZ];
    char user_table[TABLE_SZ];
    char log_table[TABLE_SZ];
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

int file_sys_info_init(pFileSysInfo_t* ppfs, const pMysql_t pmysql);

char file_sys_type(int entryType);

int file_sys_entry_exist(pFileSysInfo_t pfs);

int file_sys_mkdir(pFileSysInfo_t pfs);

int file_sys_rmdir(pFileSysInfo_t pfs);

int file_sys_dir_empty(pFileSysInfo_t pfs);

int file_sys_ls(pFileSysInfo_t pfs);

int file_sys_cd(pFileSysInfo_t pfs);

int file_sys_pwd(pFileSysInfo_t pfs); 

int file_sys_rm(pFileSysInfo_t pfs);

int file_sys_puts(pFileSysInfo_t pfs);

#define USR_NM_SZ 32
#define SALT_SZ 40
#define RANDOM_LENTH 32 //the salt
#define CIPTHER_SZ 32

typedef struct {
    char table[TABLE_SZ];
    int userID;
    char userName[USR_NM_SZ];
    char salt[SALT_SZ];
    char ciphertext[CIPTHER_SZ];
    MYSQL *conn;
    unsigned char token[40]; 
} UserInfo_t, *pUserInfo_t;

int usr_sys_usr_init(pUserInfo_t* ppui, const pMysql_t pmysql);

int usr_sys_register(pUserInfo_t pui);

int usr_sys_usrName_exist(const pUserInfo_t pui);

int usr_sys_verify(const pUserInfo_t pui);

#endif
