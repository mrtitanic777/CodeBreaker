#include "opnd.h"
#include "ins.h"
#include "seg.h"
#include "rec.h"
#include "optab.h"
extern struct seg *g_11bc;
extern long g_0ca8;
extern int g_3606, g_3608, g_360a, g_360c;     /* (the floating point fixups' externals) */
extern int g_3604[];
extern int g_0c9a, g_0c8a, g_3366;
extern char *g_123c;
extern struct init *f04345();
extern f05131(), f04e4f(), f05347(), f05383(), f04dd6(), f017cb();
extern struct init *f05c07();

/* (NOROOM 9 takes it from 283 differing bytes to 255: the compiler of 3.00 was short of room here too) */
/* (open: of the shared f05383 calls the original keeps the last copies, release 3.00 the first; the original
   also loads init again from the frame) */

/* the n bytes at buf of the item p put in the object, with their fixups */
f0596d(p, n, buf)
struct ins *p;
register n;
char *buf;
{
    char no;
    int op;
    char *init;
    int k;

    switch (p->kind) {
    case 7:
        g_11bc->size = g_0ca8;
        f04e4f(g_11bc, n, buf);
        f05347(g_3368 - 8);
        break;
    case 0:
        op = p->op;
        no = g_17b2[op].nops;
        if (op == 0x67) {
            init = f04345(p);
            f05c07(init);
            f05131(p->b, p->size, g_3368 - 8, 0x67);
            break;
        }
        g_11bc->size = g_0ca8;
        f04e4f(g_11bc, n, buf);
        g_11bc->size += n;
        if (((unsigned char)g_360e[5] << 8) + (unsigned char)g_360e[4] + g_3368 - 6 != g_11bc->size)
            f017cb(g_123c, 1538);
        if (op == 0x68)
            f05383(g_3368 - n - 6, g_3604);
        else if (g_17b2[op].flags & 1)
            if (!p->a)
                f05383(g_3368 - n - 6, g_3606);
            else
                switch (k = p->a->o2) {
                default:
                    f017cb(g_123c, 1565);
                    break;
                case 8:
                    f05383(g_3368 - n - 6, g_3608);
                    break;
                case 0xa:
                    f05383(g_3368 - n - 6, g_360a);
                    f05383(g_3368 - n - 5, g_360c);
                    break;
                case 0:
                    f05383(g_3368 - n - 6, g_3606);
                    break;
                }
        if (no)
            f05131(p->a, p->size, g_3368 - n - 6, op);
        if (no == 2)
            f05131(p->b, p->size, g_3368 - n - 6, op);
        break;
    case 1: case 3: case 4: case 5: case 6: case 8:
        break;
    case 2:
        if (g_0c9a)
            f04dd6(g_0c8a, g_11bc->size, g_3366 >= 20);
        break;
    }
}
