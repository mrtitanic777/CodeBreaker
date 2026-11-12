#include <stdio.h>
#include "incl.h"
extern FILE *g_0a44;
extern int g_0a42, g_09d8;
extern char *g_09d4;
extern struct incl g_09de[];
extern f00f49();

f01e3e()
{
    int line;

    if (g_0a42 == -1)
        return EOF;
    fclose(g_0a44);
    g_09d4 = g_09de[g_0a42].name;
    line = g_09d8;
    g_09d8 = g_09de[g_0a42].line;
    if (--g_0a42 < 0) {
        g_09d8 = line;
        return EOF;
    }
    f00f49();
    g_0a44 = g_09de[g_0a42].fp;
    return 0;
}
