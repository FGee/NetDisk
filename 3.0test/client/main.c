#include "ahead_cli.h"
#include "trans_ahead_cli.h"

int main(int argc, char** argv)
{
    int sockFd;
    sockFd = socket(AF_INET, SOCK_STREAM, 0);
    ERROR_CHECK(sockFd, -1, "socket");

    struct sockaddr_in ser_addr;
    bzero(&ser_addr, sizeof(ser_addr));
    ser_addr.sin_family = AF_INET;
    ser_addr.sin_port = htons(atoi("2000"));
    ser_addr.sin_addr.s_addr = inet_addr("192.168.198.131");
    int ret;

    ret = connect(sockFd, (struct sockaddr*)&ser_addr, sizeof(struct sockaddr_in));
    ERROR_CHECK(ret, -1, "conect");

    printf("conect success\n");
    Train train; 
    bzero(&train, sizeof(train));

    while (1) {
        bzero(&train, sizeof(train));
        scanf("%s", train.buf);
        fflush(stdin);
        train.dataLenth = strlen(train.buf);
        train.ctl_code = 1;
        ret = trans_send_cycle(sockFd, &train, sizeof(train));
        ERROR_CHECK(ret, -1, "trans_send_cycle");

        bzero(&train, sizeof(train));
        ret = trans_recv_cycle(sockFd, &train, sizeof(train));
        ERROR_CHECK(ret, -1, "trans_recv_cycle");
        printf("salt %s\n", train.buf);

        char passwd[20] = {0};
        char ciphertext[50] = {0};
        scanf("%s", passwd);
        fflush(stdin);
        strcpy(ciphertext, crypt(passwd, train.buf));
        printf("%s\n", ciphertext);

        bzero(&train, sizeof(train));
        train.dataLenth = strlen(ciphertext);
        train.ctl_code = 3;
        memcpy(train.buf, ciphertext, train.dataLenth);
        ret = trans_send_cycle(sockFd, &train, sizeof(train));
        ERROR_CHECK(ret, -1, "trans_send_cycle");

        bzero(&train, sizeof(train));
        ret = trans_recv_cycle(sockFd, &train, sizeof(train));
        ERROR_CHECK(ret, -1, "trans_recv_cycle");
        printf("%s\n", train.buf);
    }

    return 0;
}
