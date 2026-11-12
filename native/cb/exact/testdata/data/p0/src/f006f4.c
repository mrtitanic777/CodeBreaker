#include <stdio.h>
extern FILE *g_0234;

long
f006f4()
{
    char c;
    long l;

    if ((c = getc(g_0234)) != (char)0x80)
        return c;
    {
        register char *p;

        p = (char *)&l;
        p[0] = getc(g_0234);
        p[1] = getc(g_0234);
        p[2] = getc(g_0234);
        p[3] = getc(g_0234);
    }
    return l;
}
