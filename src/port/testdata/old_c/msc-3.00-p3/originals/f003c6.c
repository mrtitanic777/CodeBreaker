#include "opnd.h"
extern int g_261e, g_262e, g_2630;
extern char *g_262a;
extern char g_00e6[];           /* (the class of each kind of operand: 2 an immediate, 3 memory, 6 based) */
extern char g_00c2[];           /* (the r/m field of each base register) */
extern char *g_00c0;
extern f00306(), f0012a(), f001c9(), f017cb();

/* (open: of the shared "add sp, 4" tails the original keeps the last copy, release 3.00 the first) */

/* the ModR/M byte of the operand p with the register field r put in the record, its displacement after it */
f003c6(p, r)
struct opnd *p;
{
    long v;
    char k;
    char r2;
    char m;
    int b, x;                   /* (not used) */

    g_261e = g_262e + 2;
    switch (k = p->kind) {
    case 1:
        if (f00306(p))
            f017cb(g_00c0, 356);
        m = p->size | 0xc0;
        break;
    case 4:
    case 5:
    case 6:
    case 7:
        r2 = p->size;
        if (g_00e6[k] == 6) {
            m = p->o12 - 6;
            r2 -= 3;
            if (r2 & ~2 || m & ~1)
                f017cb(g_00c0, 369);
            m |= r2;
        } else {
            if (r2 < 3 || r2 > 7 || r2 == 4)
                f017cb(g_00c0, 373);
            m = g_00c2[r2];
        }
        if (k == 5 || k == 7) {
            m |= 0x80;
            f001c9(2, f0012a(p));
            break;
        }
        if ((v = p->val) || r2 == 5)
            m |= f001c9(-1, (int)v) ? 0x40 : 0x80;
        break;
    case 16:
        m = 6;
        f001c9(2, (int)p->val);
        break;
    case 3:
        m = 6;
        f001c9(2, f0012a(p));
        break;
    default:
        f017cb(g_00c0, 399);
        break;
    }
    g_262a[g_2630] = m | r;
}
