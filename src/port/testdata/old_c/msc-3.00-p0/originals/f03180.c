#include <stdio.h>
#include <ctype.h>
extern int g_05d4;
extern int (*g_09da)(), (*g_09dc)();
extern f01cd2(), f01c90(), f031f7();

/* at a line's start in the source file: the blanks (echoed under -E), then a # line or not */
/* (open: the original's third pass turned this loop, whose test has a label in it; release 3.00's does not) */
f03180()
{
    register int c;

    if (g_09da != f01cd2)
        return 0x16f;
    while ((isspace(c = f01c90()) || !c) && c != '\n' && c != EOF)
        if (g_05d4)
            putc(c, stdout);
    if (c == '#')
        return f031f7();
    (*g_09dc)(c);
    return 0x16f;
}
