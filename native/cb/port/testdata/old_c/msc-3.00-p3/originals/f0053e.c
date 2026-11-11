#include "opnd.h"
extern struct opnd *g_2622, *g_2624;
extern char g_2618, g_2619;
extern char *g_00c0;
extern f00306(), f003c6(), f017cb();

/* the ModR/M byte of a two-operand instruction: the register of one in the reg field, the other the r/m */
f0053e()
{
    if (g_2618 == 1) {
        if (f00306(g_2622))
            f017cb(g_00c0, 416);
        f003c6(g_2624, (g_2622->size & 7) << 3);
    } else {
        if (g_2619 != 1 || f00306(g_2624))
            f017cb(g_00c0, 421);
        f003c6(g_2622, (g_2624->size & 7) << 3);
    }
}
