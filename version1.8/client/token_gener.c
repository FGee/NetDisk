#include "token.h"

int token_gener(pTokenInfo_t pti)
{
    unsigned char string[48] = {0};
    time_t now;
    time(&now);
    char *timeStr;
    timeStr = ctime(&now);
    memcpy(string, pti->userName, strlen(pti->userName));
    memcpy(string, timeStr, strlen(timeStr));
    size_t len = strlen(pti->userName) + strlen(timeStr);

    unsigned char* md5 =  MD5(string, len, NULL);
    ERROR_CHECK(md5, NULL, "MD5");
    memcpy(pti->token, md5, strlen((char*)md5));

    return 0;

}
