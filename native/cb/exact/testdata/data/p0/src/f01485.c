#include "macro.h"
extern char *g_0828;
extern f00081();

char *
f01485(n, a)
register int n;
register struct arg *a;
{
    while (n--)
        a = a->next;
    if (!a)
        f00081(g_0828, 241);
    return a->text;
}
