#include "getflags.h"
extern char *g_1f78;
extern f003cd(), f00577();

f0030c(t, arg, func, farg)
register struct flagdef *t;
char *arg;
char *(*func)();
int farg;
{
    register char *p;
    char *s;

    if (!arg)
        return 0;
    g_1f78 = arg;
    for (; t->flags; t++) {
        p = t->pat;
        s = arg;
        for (;;)
            switch (*p) {
            default:
                if (*s++ == *p++)
                    continue;
                goto nomatch;
            case '#':
                if (!*s) {
                    f003cd(t, (*func)(), farg);
                    return t->val;
                }
                f003cd(t, s, farg);
                return t->val;
            case '*':
                if (!*s || !f00577(p, s, t->flags))
                    goto nomatch;
                f003cd(t, s, farg);
                return t->val;
            case '\0':
                if (*s)
                    goto nomatch;
                if (t->flags & 0x40) {
                    f003cd(t, (*func)(), farg);
                    return t->val;
                }
                f003cd(t, 0, farg);
                return t->val;
            case '-':
                if (*s != '-')
                    goto nomatch;
                s++;
                p++;
                continue;
            }
nomatch:
        ;
    }
    return 0;
}
