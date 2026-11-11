#include <ctype.h>
extern int g_05d4;
extern int (*g_09da)(), (*g_09dc)();
extern char g_3042[];
extern f00167();

/* an identifier from its first character c (32 characters kept) */
char *
f0330d(c)
register int c;
{
    register char *p;

    p = g_3042;
    while (isalnum(c) || c == '_') {
        *p++ = c;
        c = (*g_09da)();
    }
    (*g_09dc)(c);
    *p = 0;
    if (!g_05d4 && strlen(g_3042) >= 32) {
        g_3042[31] = 0;
        f00167(1, "identifier truncated to `%s'", g_3042);
    }
    return g_3042;
}
