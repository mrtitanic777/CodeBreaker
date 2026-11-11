#include "macro.h"
extern struct mac *g_0ad6[];
extern f005c1();

struct mac *
f0208c(name)
register char *name;
{
    register struct mac *m;
    int h;

    h = f005c1(name);
    for (m = g_0ad6[(unsigned)h % 47]; m; m = m->next)
        if (h == m->hash && !strcmp(name, m->name))
            break;
    return m;
}
