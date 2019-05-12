#include "log_reg.h"
#include "trans_ahead_cli.h"
#include "token.h"

int login_register(const int sockFd, pTokenInfo_t pti)
{
    int ret;
    Train_t train;
LOGIN_REGIS:
    printf("\n\n\n1. Login\n\n\n2. Register\n\n\nPlease input the number: ");
    fflush(stdout);
    char select[20] = {0};
    scanf("%s", select);
    fflush(stdin);

    if (!strcmp(select, "2")) {
ACCOUNT_EXIST:
        bzero(&train, sizeof(train));
        bzero(pti->userName, NM_SZ);
        printf("\nPlease input the user name to register:\n");
        scanf("%s", train.buf);
        fflush(stdin);
        memcpy(pti->userName, train.buf, strlen(train.buf));
        train.dataLenth = strlen(train.buf);
        train.ctl_code = 1;
        ret = trans_send_cycle(sockFd, &train, sizeof(train));
        ERROR_CHECK(ret, -1, "trans_send_cycle");

        bzero(&train, sizeof(train));
        ret = trans_recv_cycle(sockFd, &train, sizeof(train));
        ERROR_CHECK(ret, -1, "trans_recv_cycle");
        //printf("salt %s\n", train.buf + 1);

        if ('e' == train.buf[0]) {
            printf("\nThe user name is exist, input again\n");
            goto ACCOUNT_EXIST;
        }

        char ciphertext[50] = {0};
TWO_DIFF:
        printf("\nPlease input the password to register:\n");
        char passwd[20] = {0};
        scanf("%s", passwd);
        fflush(stdin);
        printf("\nPlease input again:\n");
        char passwd_verify[20] = {0};
        scanf("%s", passwd_verify);
        fflush(stdin);
        if (strcmp(passwd, passwd_verify)) {
            printf("\nTwice input is different, input again\n");
            goto TWO_DIFF;
        }
        strcpy(ciphertext, crypt(passwd, train.buf + 1));
        //printf("%s\n", ciphertext);

        bzero(&train, sizeof(train));
        train.dataLenth = strlen(ciphertext);
        train.ctl_code = 2;
        memcpy(train.buf, ciphertext, train.dataLenth);

        ret = trans_send_cycle(sockFd, &train, sizeof(train));
        ERROR_CHECK(ret, -1, "trans_send_cycle");

        bzero(&train, sizeof(train));
        ret = trans_recv_cycle(sockFd, &train, sizeof(train));
        ERROR_CHECK(ret, -1, "trans_recv_cycle");
        if ('s' == train.buf[0]) {
            printf("\nRegister success!\n");
            goto LOGIN_REGIS;
        }
    } else if (!strcmp(select, "1")) {
ACCOUNT_NAME:
        bzero(&train, sizeof(train));
        bzero(pti->userName, NM_SZ);
        printf("\nPlease input the user name:\n");
        scanf("%s", train.buf);
        fflush(stdin);
        memcpy(pti->userName, train.buf, strlen(train.buf));
        train.dataLenth = strlen(train.buf);
        train.ctl_code = 3;
        ret = trans_send_cycle(sockFd, &train, sizeof(train));
        ERROR_CHECK(ret, -1, "trans_send_cycle");

        bzero(&train, sizeof(train));
        ret = trans_recv_cycle(sockFd, &train, sizeof(train));
        ERROR_CHECK(ret, -1, "trans_recv_cycle");
        //printf("salt %s\n", train.buf);
        if ('n' == train.buf[0]) {
            printf("\nAccount name not exist, input again\n");
            goto ACCOUNT_NAME;
        }

        char* passwd;
        char saltBuf[40] = {0};
        memcpy(saltBuf, train.buf + 1, strlen(train.buf) + 1);
PASSWD_ERROR:
        bzero(&train, sizeof(train));
        memcpy(train.buf + 1, saltBuf, strlen(saltBuf));
        passwd = getpass("\nPlease input the password:\n");
        //printf("%s\n", train.buf);
        //printf("%s\n", crypt(passwd, train.buf + 1));
        char ciphertext[50] = {0};
        strcpy(ciphertext, crypt(passwd, train.buf + 1));
        //printf("%s\n", ciphertext);

        bzero(&train, sizeof(train));
        train.dataLenth = strlen(ciphertext);
        train.ctl_code = 4;
        memcpy(train.buf, ciphertext, train.dataLenth);
        bzero(pti->token, TK_SZ);
        ret = token_gener(pti);
#ifdef LOGIN
        printf("token: ");
        for (unsigned long i = 0; i < strlen((char*)pti->token); i++) {
            printf("%02x", pti->token[i]);
        }
        printf("\n");
#endif
        memcpy(train.buf + 100, pti->token, 100);//send the token
        ret = trans_send_cycle(sockFd, &train, sizeof(train));
        ERROR_CHECK(ret, -1, "trans_send_cycle");

        bzero(&train, sizeof(train));
        ret = trans_recv_cycle(sockFd, &train, sizeof(train));
        ERROR_CHECK(ret, -1, "trans_recv_cycle");
        if ('s' == train.buf[0]) {
            printf("\nLOGIN SUCCESS\n");
        } else if ('f' == train.buf[0]) {
            printf("LOGIN FAIL\n");
            goto PASSWD_ERROR;
        }
    } else {
        printf("\nSELECT ERROR, please input again\n");
        goto LOGIN_REGIS;
    }

    return 0;
}
