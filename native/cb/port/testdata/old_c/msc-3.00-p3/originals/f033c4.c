#include <stdio.h>
extern FILE *g_0952;

f033c4(p)
register int *p;
{
    if (*p <= 127 && *p >= -127)
        putc(*(char *)p, g_0952);
    else {
        putc(0x80, g_0952);
        putc(((char *)p)[0], g_0952);
        putc(((char *)p)[1], g_0952);
    }
}
