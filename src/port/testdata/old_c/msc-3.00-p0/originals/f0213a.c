#include "macro.h"
extern struct mac *g_0ad6[];
extern f005c1();

f0213a(name)
char *name;
{
    register struct mac *m, *pm;
    unsigned bucket;
    unsigned h;

    h = f005c1(name);
    bucket = h % 47;
    pm = 0;
    for (m = g_0ad6[bucket]; m; pm = m, m = m->next)
        if (h == m->hash && !strcmp(name, m->name)) {
            if (!pm)
                g_0ad6[bucket] = m->next;
            else
                pm->next = m->next;
            return 1;
        }
    return 0;
}
