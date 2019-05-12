#include "fs_ahead.h"

int file_sys_ls(pFileSysInfo_t pfs)
{
    int ret;
    Train_t train;
    Query_t query;
    bzero(&query, sizeof(Query_t));

    sprintf(query.buf, "select fileName,fileType,fileSize from %s"
            " where userID=%d and precode=%d", 
            pfs->table, pfs->userID, pfs->codePath[pfs->pathPointer]);

#ifdef LS
    printf("ls: %s\n", query.buf);
#endif

    Result_t result;
    bzero(&result, sizeof(Result_t));

    ret = mysql_query(pfs->conn, query.buf);
    if (ret)
    {
#ifdef LS_ERROR
        printf("entry_exist: return val -200\n");
#endif
        printf("Error making query:%s\n", mysql_error(pfs->conn));
        return -200;
    } else {
        result.res = mysql_use_result(pfs->conn);
        if (result.res) {
                bzero(&train, sizeof(Train_t));
                //int pos = 0;
            while (NULL != (result.row = mysql_fetch_row(result.res))) {
                for(unsigned i = 0; i < mysql_num_fields(result.res); i++)
                {
                    //printf("%-8s", result.row[i]);
                    sprintf(train.buf, "%s%-8s", train.buf, result.row[i]);
                   // if (0 == i) {
                   //     strcpy(train.buf + pos, result.row[i]);
                   //     pos += 16;
                   // } else if (1 == i) {
                   //     strcpy(train.buf + pos, result.row[i]);
                   //     pos += 4;
                   // } else if (2 == i) {
                   //     strcpy(train.buf + pos, result.row[i]);
                   //     pos += 16;
                   // }
                }
                sprintf(train.buf, "%s\n", train.buf);
            }
                //printf("%s", train.buf);
                //fflush(stdout);
                train.dataLenth = strlen(train.buf);
                //printf("%d\n", train.dataLenth);
                ret = trans_send_cycle(pfs->newfd, &train, sizeof(Train_t));
                //printf("newfd %d\n", pfs->newfd);
                ERROR_CHECK(ret, -1, "trans_send_cycle");
        } else {
#ifdef LS_ERROR
            printf("ls: mysql_use_result error, return val -210\n");
#endif
            return -210;
        }
    }
#ifdef LS
    printf("ls: ls success, return val 0\n");
#endif
    mysql_free_result(result.res);
    return 0;
}
