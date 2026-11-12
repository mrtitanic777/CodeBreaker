#include <stdio.h>
extern FILE *g_0234;

f00613()
{
    register char *p;
    int w;

    p = (char *)&w;
    p[0] = getc(g_0234);
    p[1] = getc(g_0234);
    return w;
}
