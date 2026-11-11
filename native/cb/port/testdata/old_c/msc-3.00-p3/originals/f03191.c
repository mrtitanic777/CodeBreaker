#include <stdio.h>
extern FILE *g_0954;

long
f03191()
{
    char c;
    long l;

    if ((c = getc(g_0954)) != (char)0x80)
        return c;
    {
        register char *p;

        p = (char *)&l;
        p[0] = getc(g_0954);
        p[1] = getc(g_0954);
        p[2] = getc(g_0954);
        p[3] = getc(g_0954);
    }
    return l;
}
