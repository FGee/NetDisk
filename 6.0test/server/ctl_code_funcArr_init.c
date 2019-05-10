#include "ctl_code.h"

int ctl_code_funcArr_init(pCtl_code_t* arrCtl)
{
    bzero(arrCtl, sizeof(pCtl_code_t) * CTL_CODE_SZ);

    arrCtl[1] = ctl_code1;
    arrCtl[2] = ctl_code2;
    arrCtl[3] = ctl_code3;
    arrCtl[4] = ctl_code4;

    return 0;
}
