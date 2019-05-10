#include "ctl_code.h"

int ctl_code_funcArr_init(pCtl_code_t* arrCtl)
{
    bzero(arrCtl, sizeof(pCtl_code_t) * CTL_CODE_SZ);

    arrCtl[1] = ctl_code1;
    arrCtl[2] = ctl_code2;
    arrCtl[3] = ctl_code3;
    arrCtl[4] = ctl_code4;
    arrCtl[11] = ctl_code11;
    arrCtl[12] = ctl_code12;
    arrCtl[13] = ctl_code13;
    arrCtl[14] = ctl_code14;
    arrCtl[15] = ctl_code15;
    arrCtl[16] = ctl_code16;
    arrCtl[17] = ctl_code17;

    return 0;
}
