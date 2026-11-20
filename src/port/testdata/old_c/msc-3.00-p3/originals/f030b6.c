#include <stdio.h>
extern FILE *g_0954;

f030b6()
{
    register char *p;
    int w;

    p = (char *)&w;
    p[0] = getc(g_0954);
    p[1] = getc(g_0954);
    return w;
}
