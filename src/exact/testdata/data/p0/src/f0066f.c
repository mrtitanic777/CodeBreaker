#include <stdio.h>
extern FILE *g_0234;

f0066f()
{
    char c;
    int w;

    if ((c = getc(g_0234)) != (char)0x80)
        return c;
    {
        register char *p;

        p = (char *)&w;
        p[0] = getc(g_0234);
        p[1] = getc(g_0234);
    }
    return w;
}
