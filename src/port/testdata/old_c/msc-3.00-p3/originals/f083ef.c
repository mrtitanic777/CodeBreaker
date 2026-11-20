#include "opnd.h"
#include "ins.h"
extern long g_2fce;
extern int g_1ffc, g_1ffe, g_3aa0;
extern char *g_1f7e;
extern char f010f8();
extern f08378(), f017cb();

/* the jump p made short if its label is near enough */
f083ef(p)
register struct ins *p;
{
    register struct opnd *a;

    a = p->a;
    if (a->o10 == 1)
        return;
    if (a->o10 != 2)
        f017cb(g_1f7e, 1041);
    if (f08378(p) <= 0x7f) {
        if (p->op == 0x3d) {
            p->size = 1;
            g_1ffc++;
            g_2fce -= 1;
        } else {
            g_1ffe++;
            g_2fce -= 3;
        }
        a->o10 = 1;
        p->i9 = f010f8(p);
        g_3aa0 = 1;
    }
}
