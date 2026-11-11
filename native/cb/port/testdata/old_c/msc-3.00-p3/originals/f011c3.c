#include "attr.h"
#include "optab.h"
extern char g_00e6[];           /* (the class of each kind of operand: 2 a constant, 3 memory) */
extern char *g_00dc;
extern f01684(), f01717(), f017cb();

/* (open: 1,194 bytes for 1,214 - the cases share tails ("n += f01684(a)", "n += w == 1 ? 1 : 2", ...); the original
   keeps the last copy of each and jumps to it, release 3.00 the first) */

/* the size of the instruction op of width w (1, 2 or 4) on the operands a and b (either may be 0) */
f011c3(op, w, a, b)
register struct attr *a;
struct attr *b;
{
    register n;
    int ka;
    int kb;

    if (a)
        ka = a->a0;
    if (b) {
        if (a->a0 == 12 || a->a0 == 14)
            f017cb(g_00dc, 241);
        if ((kb = b->a0) == 11) {
            kb = 2;
            b->l4 = 300;
        }
    }
    n = g_17b2[op].size;
    switch (op) {
    case 0x3f:
    case 0x40:
        n = 5;
        break;
    case 0x1e:
        if (ka == 2)
            n += f01717(2, a->l4) + 1;
        else
            n += f01684(a);
        break;
    case 0x1f:
        n += f01684(a) + f01717(2, b->l4);
        break;
    case 0x26: case 0x27: case 0x28: case 0x29: case 0x2a: case 0x2b: case 0x2c:
        if (!(kb == 2 ? b->l4 > 0 && b->l4 < 256 : kb == 1 && (b->a2 = 1)))
            f017cb(g_00dc, 326);
        n += f01684(a) + (kb == 2 && b->l4 != 1);
        break;
    case 0x11: case 0x12: case 0x14: case 0x15: case 0x17:
        if (g_00e6[kb] == 2) {
            if (ka == 1 && !a->a2)
                n += w == 1 ? 1 : 2;
            else
                n += f01684(a) + (kb == 2 ? f01717(w, b->l4) : 2);
        } else if (kb == 1)
            n += f01684(a);
        else
            n += f01684(b);
        break;
    case 0x2d: case 0x2e: case 0x2f: case 0x30:
        if (ka == 1) {
            if (kb == 2) {
                n += w == 1 ? 1 : 2;
                if (a->a2)
                    n++;
            } else
                n += f01684(b);
        } else
            n += f01684(a) + (kb == 2 ? (w == 1 ? 1 : 2) : 0);
        break;
    case 0x13:
    case 0x16:
        n += ka == 1 && w == 2 ? 0 : f01684(a);
        break;
    case 6:
        if (ka != 1)
            f017cb(g_00dc, 387);
        if (kb == 1 && w == 2 && (!a->a2 || kb == 1 && !b->a2))
            break;
        n += f01684(b);
        break;
    case 0xa: case 0xb: case 0xc: case 0x4a:
        if (ka != 1 || kb == 1)
            f017cb(g_00dc, 398);
        n += f01684(b);
        break;
    case 7:
    case 8:
        if (!(ka != 2 || a->l4 <= 255))
            f017cb(g_00dc, 404);
        n += ka == 2 ? 1 : 0;
        break;
    case 2:
        if (g_00e6[ka] == 2) {
            n += ka == 2 ? f01717(2, a->l4) : 2;
            break;
        }
    case 4:
        if (ka != 1)
            n += f01684(a);
        break;
    case 1:
        if (ka == 1 && !a->a2 && g_00e6[kb] == 3 || kb == 1 && !b->a2 && g_00e6[ka] == 3)
            n += 2;
        else if (g_00e6[kb] == 2) {
            if (ka != 1)
                n += f01684(a);
            n += w == 1 ? 1 : 2;
        } else
            n += f01684(ka == 1 ? b : a);
        break;
    case 0x3c:
    case 0x3d:
        if (!(op != 0x3c || ka != 12))
            f017cb(g_00dc, 440);
        if (ka == 10 || ka == 12)
            n += w == 4 ? 4 : w == 2 ? 2 : 1;
        else
            n += f01684(a);
        break;
    case 0x3e:
        if (ka != 2)
            f017cb(g_00dc, 448);
        if (a->l4)
            n += 2;
        break;
    case 0x47:
        if (ka != 2)
            f017cb(g_00dc, 454);
        if (a->l4 != 3)
            n++;
        break;
    case 0x56:
        f017cb(g_00dc, 465);
        break;
    default:
        if (!(g_17b2[op].flags & 1) || g_17b2[op].nops != 1)
            break;
    case 0x18: case 0x1d: case 0x20: case 0x21: case 0x31:
    case 0x58: case 0x59: case 0x5a: case 0x5b: case 0x5c: case 0x5d: case 0x5e: case 0x5f:
    case 0x60: case 0x61: case 0x62: case 0x63: case 0x64: case 0x65: case 0x66:
        n += f01684(a);
        break;
    }
    return n;
}
