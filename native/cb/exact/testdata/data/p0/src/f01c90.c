#include <stdio.h>
extern FILE *g_0a44;
extern int g_09d8;
extern f01c46();

f01c90()
{
    register int c;

    if ((c = getc(g_0a44)) == '\n') {
        g_09d8++;
        f01c46();
    }
    return c;
}
