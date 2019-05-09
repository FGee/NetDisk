#include "ahead_cli.h"
#include "tcp_cli.h"
#include "token.h"
#include "trans_ahead_cli.h"
#include "log_reg.h"

int main(int argc, char** argv)
{

    int sockFd;
    int ret;

    ret = tcp_init(&sockFd);

    TokenInfo_t tokenInfo;
    bzero(&tokenInfo, sizeof(tokenInfo));

    ret = login_register(sockFd, &tokenInfo);

    Train train; 
    bzero(&train, sizeof(train));

    while (1);

    return 0;
}
