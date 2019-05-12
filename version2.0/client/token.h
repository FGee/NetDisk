#ifndef __TOKEN_H__
#define __TOKEN_H__
#include <func.h>
#define NM_SZ 64
#define PATH_SZ 128
#define TK_SZ 100

typedef struct {                                                                                                                              
    char userName[NM_SZ];
    char fileName[NM_SZ];
    char pathName[PATH_SZ];
    unsigned char token[TK_SZ];
} TokenInfo_t, *pTokenInfo_t;

int token_gener(pTokenInfo_t pti); 

#endif
