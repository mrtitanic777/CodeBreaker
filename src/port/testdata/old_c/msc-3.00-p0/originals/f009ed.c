#include <stdio.h>
extern FILE *g_0232;

f009ed(p)
register long *p;
{
    if (*p <= 127 && *p >= -127)
        putc(*(char *)p, g_0232);
    else {
        putc(0x80, g_0232);
        putc(((char *)p)[0], g_0232);
        putc(((char *)p)[1], g_0232);
        putc(((char *)p)[2], g_0232);
        putc(((char *)p)[3], g_0232);
    }
}
