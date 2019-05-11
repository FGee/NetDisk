#include "token.h"

int token_gener(pTokenInfo_t* ppti)
{
    unsigned char string[48] = {0};
    time_t now;
    time(&now);
    char *timeStr;
    timeStr = ctime(&now);
    memcpy(string, (*ppti)->userName, strlen((*ppti)->userName));
    memcpy(string, timeStr, strlen(timeStr));
    size_t len = strlen((*ppti)->userName) + strlen(timeStr);

    unsigned char* md5 =  MD5(string, len, NULL);
    ERROR_CHECK(md5, NULL, "MD5");
    memcpy((*ppti)->token, md5, strlen((char*)md5));

    return 0;

}
