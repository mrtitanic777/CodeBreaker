#include <stdio.h>
#include "incl.h"
extern FILE *g_0a44;
extern intptr_t g_05d8; extern int g_0a42, g_09d8;
extern char *g_09d4;
extern struct incl g_09de[];
extern char *f01135();
extern f00143(), f00f49(), f03180();

f01d6e(name, must)
char *name;
int must;
{
    if (!name)
        g_0a44 = stdin;
    else if ((g_0a44 = fopen(name, "rt")) == 0) {
        if (must == 1)
            f00143("cannot open `%s'", name);
        return 0;
    }
    if (++g_0a42 >= 10)
        f00143("too many include files\n");
    g_09de[g_0a42].fp = g_0a44;
    g_09de[g_0a42].line = g_09d8;
    g_09de[g_0a42].name = g_09d4;
    g_09d8 = 1;
    g_09d4 = f01135(name, 0);
    g_09de[g_0a42].iname = g_09d4;
    if (g_05d8)
        printf("#line 1 %s\n", g_09d4);
    f00f49();
    f03180();
    return 1;
}
