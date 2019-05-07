#include "mysql_file_sys.h"

char file_sys_type(int entryType)                                   
{
    switch (entryType) {
    case 0:
        return 'd';
        break;
    case 1:
        return 'f';
        break;
    default:
        return '\0';
    }
}
