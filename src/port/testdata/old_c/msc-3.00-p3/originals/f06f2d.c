#include "psym.h"
extern struct psym g_2fba;      /* (the label being made) */
extern int g_336a;              /* (the labels so far) */
extern struct psym *g_3572[];
extern char *g_1e46;
extern struct psym *f06824();
extern f017cb();

/* a new label of the function */
struct psym *
f06f2d()
{
    int x;                      /* (not used) */
    register struct psym *s;

    s = &g_2fba;
    s->kind = 1;
    if (g_336a >= 0x7fff)
        f017cb(g_1e46, 844);
    s->s4 = g_336a++;
    s->s8 = -1;
    s->s6 = 0;
    s->s3 = 0x20;
    return f06824(s, g_3572);
}
