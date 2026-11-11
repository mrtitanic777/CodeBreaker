#include "opnd.h"
#include "targ.h"
extern struct opnd *g_2622;
extern int g_262c;              /* (the instruction) */
extern char g_2620;
extern int g_0ca8;              /* (the place of the instruction) */
extern char *g_0cc4;
extern char *g_00c0;
extern f017cb(), abs();

/* the displacement of the jump g_262c to its label, from the end of the instruction */
f005b6()
{
    register n;
    int d;
    register op;
    struct targ *t;

    t = (struct targ *)g_2622->sym;
    if (t->kind != 1 && t->kind != 2)
        f017cb(g_00c0, 460);
    switch (op = g_262c) {
    case 0x41:
    case 0x42:
    case 0x43:
        n = 2;
        break;
    case 0x3c:
        n = 3;
        break;
    case 0x3d:
        n = g_2620 == 2 ? 3 : 2;
        break;
    case 0x3f:
        n = g_0cc4[0xf] ? 2 : 5;
        break;
    default:
        f017cb(g_00c0, 482);
        break;
    }
    d = t->off - g_0ca8 - n;
    switch (op) {
    case 0x3c:
    case 0x3d:
        break;
    case 0x41:
    case 0x42:
    case 0x43:
        if (abs(d) > 0x7f)
            f017cb(g_00c0, 489);
        break;
    case 0x3f:
        if (g_0cc4[0xf] && abs(d) > 0x7f) {
            d -= op == 0x3d ? 1 : 3;
            if (abs(d) <= 0x7f)
                f017cb(g_00c0, 499);
        }
        break;
    default:
        f017cb(g_00c0, 504);
        break;
    }
    return d;
}
