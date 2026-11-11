#include <stdio.h>
extern FILE *g_0952;

f03366(p)
register char *p;
{
    putc(p[0], g_0952);
    putc(p[1], g_0952);
}
