#include "macro.h"
extern struct mac *g_0ad6[];
extern struct mac *f0208c();
extern f020d9(), f00167(), f005c1();
extern char *f010b8();

f01ff6(name, body, np)
char *name;
struct btok *body;
char np;
{
    register struct mac *m;
    register int h;
    int u;

    if (m = f0208c(name)) {
        if (!f020d9(m->body, body))
            return;
        f00167(1, "`%s' : redefinition", name);
    } else {
        h = f005c1(name);
        m = g_0ad6[(unsigned)h % 47] = (struct mac *)f010b8(10, g_0ad6[(unsigned)h % 47], 0);
        m->name = name;
        m->hash = h;
    }
    m->body = body;
    m->nparams = np;
}
