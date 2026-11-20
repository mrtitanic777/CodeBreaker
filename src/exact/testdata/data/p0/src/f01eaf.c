#include "macro.h"
#include "getflags.h"
extern struct btok g_0a8e;
extern char *f03b0d();
extern f01c68(), f01ff6(), f01d6e();
extern struct btok *f02817();

f01eaf(name, defs)
char *name;
register struct slots *defs;
{
    register char *s;
    char *eq;
    int i;

    f01c68(name);
    for (i = defs->top; i < 20; i++) {
        s = defs->v[i];
        if (eq = f03b0d(s, '=')) {
            *eq++ = 0;
            f01ff6(s, f02817(0x108, 0, eq, 0, 0), 0);
        } else
            f01ff6(s, &g_0a8e, 0);
    }
    f01d6e(name, 1);
}
