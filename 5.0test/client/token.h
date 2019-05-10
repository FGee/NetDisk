#ifndef __TOKEN_H__
#define __TOKEN_H__
#include <func.h>

typedef struct {                                                                                                                              
    char userName[32];
    unsigned char token[40];
} TokenInfo_t, *pTokenInfo_t;

int token_gener(pTokenInfo_t* ppti); 

#endif
