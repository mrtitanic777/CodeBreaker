#include "opnd.h"
#include "ins.h"
#include "psym.h"
extern int g_0c82;              /* (the post-optimizer is on) */
extern struct psym *g_0cc4;
extern long g_1e4a, g_0ca8, g_0cb4;
extern struct item *g_0c60;
extern struct ins g_0cc6;
extern int g_0c4e;
extern int g_3572;
extern struct ins *f03d65();
extern f02c53(), f03d3b(), f06988(), f03ab2(), f03de2(), f06028(), f01756();

/* the function name too large to be kept: its items done as they are read, the item c first */
f03c8b(c, name)
char c;
char *name;
{
    register struct ins *p;

    if (g_0c82) {
        f02c53(0, "function `%s' too large for post-optimizer", name);
        f06988(f03d3b, &g_3572);
    }
    g_0cc4->place = g_1e4a;
    for (p = g_0c60; p; p = p->next)
        f03ab2(p);
    f03de2(2);
    p = &g_0cc6;
    f06028(p, (unsigned char)c, g_0c4e);
    while (f03ab2(p), p->kind != 6)
        if (!(p = f03d65(g_0c4e, 0)))
            f01756("Unexpected end of file read\n", 0);
    g_0cb4 = g_0ca8;
}
