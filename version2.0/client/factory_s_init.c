#include "factory.h"

int factory_s_init(pFactory_t pfs)
{
    int ret;

    ret = factoryInit(&pfs[0]);
    ERROR_CHECK(ret, -1, "factoryInit");
    ret = factoryInit(&pfs[1]);
    ERROR_CHECK(ret, -1, "factoryInit");

    ret = factoryStart(&pfs[0], thread_puts);
    ERROR_CHECK(ret, -1, "factoryStart");
    ret = factoryStart(&pfs[1], thread_gets);
    ERROR_CHECK(ret, -1, "factoryStart");

    return 0;
}
