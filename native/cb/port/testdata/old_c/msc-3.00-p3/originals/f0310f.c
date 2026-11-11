#include <stdio.h>
extern FILE *g_0954;

f0310f()
{
    char c;
    int w;

    if ((c = getc(g_0954)) != (char)0x80)
        return c;
    {
        register char *p;

        p = (char *)&w;
        p[0] = getc(g_0954);
        p[1] = getc(g_0954);
    }
    return w;
}
