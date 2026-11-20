#include <stdio.h>
extern int (*g_09da)();
extern f00143();

f01fd0()
{
    register int c;

    c = (*g_09da)();
    if (c == EOF)
        f00143("unexpected EOF");
    return c;
}
