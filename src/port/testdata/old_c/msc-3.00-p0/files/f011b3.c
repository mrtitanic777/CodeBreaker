#include "macro.h"
struct macro {	/* (as struct mac is: its bytes) */
    struct mac *f0;
    char *f2;
    int f4;
    struct btok *body;
    char nparams;
};
extern int g_082a, g_25fc;
extern char *g_25fe;
extern struct arg *g_3772;
extern char g_2600[];          /* the actual parameters' text (2K) */
extern int (*g_09dc)();
extern struct macro *f0208c();
extern f00143(), f03a28(), f00167(), f01406(), f0129a();

f011b3(name)
char *name;
{
    register int c, np;
    int u, x;                   /* (not used) */
    struct macro *m;

    if ((m = f0208c(name)) == 0)
        return 0;
    if (g_082a++ > 1000)
        f00143("compiler limit : possibly a recursively defined macro");
    else if (m->nparams == 0) {
        f01406(m->body, 0, 0);
        return 1;
    }
    if ((c = f03a28()) != '(') {
        (*g_09dc)(c);
        f00167(1, "macro `%s' requires parameters", name);
        f01406(m->body, 0, 0);
        return 1;
    }
    np = m->nparams;
    if (np == -1)
        np = 0;
    g_25fc = 0;
    g_25fe = g_2600;
    while (f0129a())
        ;
    if (g_25fc > np)
        f00167(1, "too many actual parameters for macro `%s'", name);
    else if (g_25fc < np)
        f00167(1, "not enough actual parameters for macro `%s'", name);
    f01406(m->body, g_3772, g_25fc);
    g_3772 = 0;
    return 1;
}
