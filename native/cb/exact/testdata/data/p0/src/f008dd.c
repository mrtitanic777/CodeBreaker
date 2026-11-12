#include <stdio.h>
extern FILE *g_0232;

f008dd(p)
register char *p;
{
    putc(p[0], g_0232);
    putc(p[1], g_0232);
}
