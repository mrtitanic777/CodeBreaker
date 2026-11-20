#include "opnd.h"
#include "targ.h"
#include "optab.h"
/* an instruction as the third pass keeps it */
struct ins {
    int i0, i2;
    int kind;                   /* 0 an instruction, 7 a word of a label's place */
    int op;
    int size;                   /* (its low byte the width; for kind 7 the label) */
    struct opnd *a, *b;         /* its operands */
};
extern char *g_262a;            /* the record the instruction is put in */
extern struct ins *g_2614;
extern int g_262c;              /* (its operator) */
extern struct optab *g_261a;
extern char g_2620;             /* (its width) */
extern int g_2628, g_262e, g_2630, g_261e, g_2616;
extern char *g_2626;
extern char g_261c;             /* (the opcode) */
extern struct opnd *g_2622, *g_2624;
extern char g_2618, g_2619;
extern char g_00e6[];
extern char *g_0cc4;
extern char *g_00c0;
extern f001c9(), f002e1(), f00306(), f003c6(), f0053e(), f00383(), f00219(), f00329(), f00356(), f00102(), f0012a(),
    f005b6(), f00f8a(), f017cb(), abs();

/* (NOROOM 4 takes it from 1118 differing bytes to 1067: the compiler of 3.00 was short of room here too) */
/* (open: 2,172 bytes for 2,171 - of the cases' shared tails release 3.00 keeps the first copies, the original
   the last; the original also moves the long-jump test of 3fh after the switch's jumps) */

/* the bytes of the instruction p put in buf: their number */
f006d7(p, buf)
struct ins *p;
char *buf;
{
    int jd;                     /* (a jump's displacement) */
    int v;
    int pre;                    /* (where a segment prefix goes) */
    char sr;

    g_262a = buf;
    if (p->kind == 7) {
        g_261e = 0;
        f001c9(2, ((struct targ *)p->size)->off);
        return g_261e;
    }
    if (p->kind)
        return 0;
    g_2614 = p;
    g_262c = p->op;
    g_261a = &g_17b2[g_262c];
    g_2620 = p->size;
    g_262e = g_2628 = 0;
    g_261e = g_2630 = 1;
    g_2616 = g_261a->nops;
    g_2626 = g_261a->bytes;
    g_261c = *g_2626;
    pre = (int)(g_2622 = g_2624 = 0);
    if (g_261a->flags & 1 && g_262c != 0x68) {
        g_262a[g_262e++] = 0x9b;
        g_2630++;
        pre = 1;
    }
    if (g_2616) {
        g_2618 = (g_2622 = g_2614->a)->kind;
        if (g_2622->size >= 0xc)
            g_2622->size -= 0xc;
        if (g_2616 == 2) {
            g_2619 = (g_2624 = g_2614->b)->kind;
            if (g_2624->size >= 0xc)
                g_2624->size -= 0xc;
        }
        if (g_2622->o2 || g_2624 && g_2624->o2) {
            g_261e = 2;
            g_262e++;
            g_2630++;
            g_262a[pre] = f002e1((g_2622->o2 ? g_2622 : g_2624)->o2) | 0x26;
        }
    }
    switch (g_262c) {
    case 0x3f:
        g_261c |= g_2614->size;
        jd = f005b6();
        g_2620 = g_0cc4[0xf] && abs(jd) <= 0x7f ? 1 : 2;
        if (g_2620 == 2) {
            g_261c ^= 1;
            g_262a[g_261e++] = 3;
            g_262a[g_261e++] = 0xe9;
        }
        f001c9(g_2620, jd);
        break;
    case 0x41:
    case 0x42:
    case 0x43:
        f001c9(1, f005b6());
        break;
    case 0x1e:
        if (g_2618 == 2) {
            g_261c = g_2626[1];
            g_262a[g_261e++] = 0xc0;
            if (f001c9(-1, (int)g_2622->val))
                g_261c |= 2;
            break;
        }
        f003c6(g_2622, g_261a->ext);
        f00102();
        break;
    case 0x1f:
        f003c6(g_2622, (g_2624->size & 7) << 3);
        if (f001c9(-1, (int)g_2624->val))
            g_261c |= 2;
        break;
    case 0x18:
    case 0x1d:
    case 0x20:
    case 0x21:
    case 0x31:
        f003c6(g_2622, g_261a->ext);
        f00102();
        break;
    case 0xa:
    case 0xb:
    case 0xc:
        f0053e();
        break;
    case 0x11:
    case 0x12:
    case 0x14:
    case 0x15:
    case 0x17:
        f00383();
        break;
    case 0x26: case 0x27: case 0x28: case 0x29: case 0x2a: case 0x2b: case 0x2c:
        g_261c = *g_2626;
        f003c6(g_2622, g_261a->ext);
        if (g_2619 == 1) {
            if (g_2624->size != 1)
                f017cb(g_00c0, 653);
            g_261c = g_2626[1];
        } else {
            if (g_2619 != 2)
                f017cb(g_00c0, 658);
            if (g_2624->val != 1) {
                f001c9(1, (int)g_2624->val);
                g_261c = g_2626[2];
            }
        }
        f00102();
        break;
    case 0x57:
        g_261c = f002e1(g_2622->size) | 0x26;
        break;
    case 2:
    case 4:
        switch (g_2618) {
        case 1:
            if (!(sr = f00306(g_2622))) {
                g_261c = g_2626[1] | g_2622->size;
                break;
            }
            g_261c = (sr & 3) << 3 | g_2626[2];
            break;
        case 2:
        case 0xb:
            g_261c = g_2626[3];
            if (g_2618 == 2 && f001c9(-1, (int)g_2622->val))
                g_261c |= 2;
            break;
        case 8:
        case 9:
        case 0xf:
            g_261c = g_2626[3];
            f001c9(2, f0012a(g_2622));
            break;
        default:
            f003c6(g_2622, g_261a->ext);
            break;
        }
        break;
    case 1:
        if (g_2628 = (sr = f00306(g_2622)) ? 5 : (sr = f00306(g_2624)) ? 6 : 0) {
            g_261c = g_2626[g_2628];
            f003c6(g_2628 == 5 ? g_2624 : g_2622, f002e1(sr));
        } else if (g_00e6[g_2619] == 2) {
            if (g_2618 == 1) {
                g_261c = (g_2620 == 2 ? 1 : 0) << 3 | g_2626[2] | g_2622->size;
                switch (g_2619) {
                case 2:
                case 0xb:
                    v = (int)g_2624->val;
                    break;
                case 8:
                    v = f0012a(g_2624);
                    break;
                case 9:
                case 0xf:
                    v = 0;
                    break;
                }
                f001c9(g_2620, v);
            } else {
                f00219();
                f00102();
            }
        } else if (f00329()) {
            g_261c = g_2626[3];
            f001c9(2, f0012a(g_2624));
            f00102();
        } else if (f00356()) {
            g_261c = g_2626[4];
            f001c9(2, f0012a(g_2622));
            f00102();
        } else {
            g_261c = *g_2626;
            f00219();
        }
        break;
    case 0x2d:
    case 0x2e:
    case 0x2f:
    case 0x30:
        g_261c = *g_2626;
        f00219();
        break;
    case 7:
    case 8:
        if (g_2618 == 2) {
            g_262a[g_2630] = g_2622->val;
            g_2628 = 1;
        }
        g_261c = g_2626[g_2628];
        f00102();
        break;
    case 6:
        if (g_2618 == 1 && g_2619 == 1 && (!g_2622->size || !g_2624->size)) {
            g_261c = (!g_2622->size ? g_2624 : g_2622)->size | g_2626[1];
            break;
        }
        f0053e();
        f00102();
        break;
    case 0x35: case 0x36: case 0x37: case 0x38: case 0x39: case 0x3a: case 0x3b:
        f00102();
        break;
    case 0x22:
    case 0x23:
    case 0x55:
        g_262a[g_261e++] = g_261a->ext;
        break;
    case 0x13:
    case 0x16:
        if (g_2618 == 1 && g_2620 == 2) {
            g_261c = g_2626[1] | g_2622->size;
            break;
        }
        g_2616 = 1;
        g_261c |= g_2620 == 2 ? 1 : 0;
        f003c6(g_2622, g_261a->ext);
        break;
    case 0x3c:
        if (g_2618 == 0xa) {
            if (g_2620 == 4)
                g_2628 = 2;
            else {
                if (g_2620 != 2)
                    f017cb(g_00c0, 850);
                if (((struct targ *)g_2622->sym)->kind != 2)
                    f017cb(g_00c0, 851);
            }
            v = 0;
            f001c9(g_2620, v);
        } else {
            g_2628 = g_2620 == 4 ? 3 : 1;
            f003c6(g_2622, g_2620 == 4 ? 0x18 : 0x10);
        }
        g_261c = g_2626[g_2628];
        break;
    case 0x3d:
        if (g_2618 == 0xa) {
            g_2628 = g_2620 - 1;
            f001c9(g_2620, g_2620 == 4 ? 0 : f005b6());
        } else {
            g_2628 = g_2620 == 4 ? 4 : 2;
            f003c6(g_2622, g_2620 == 4 ? 0x28 : 0x20);
        }
        g_261c = g_2626[g_2628];
        break;
    case 0x3e:
        if (!g_2622->val) {
            if (g_2620 == 4)
                g_261c = g_2626[2];
            break;
        }
        g_261c = g_2626[g_2620 == 4 ? 3 : 1];
        f001c9(2, (int)g_2622->val);
        break;
    case 0x47:
        if (g_2622->val == 3)
            g_261c = g_2626[1];
        else
            g_262a[g_261e++] = g_2622->val;
        break;
    case 0x45:
        f001c9(2, (int)g_2622->val);
        f001c9(1, (int)g_2624->val);
        break;
    case 0x68:
        g_262a[g_262e++] = 0x90;
        g_261c = 0x9b;
        g_261e = 2;
        break;
    case 0x67:
        g_261e = 0;
        break;
    default:
        if (g_261a->size == 1)
            break;
        if (g_261a->flags & 1) {
            if (g_261a->nops == 1)
                f003c6(g_2622, g_261a->ext);
            else {
                g_262a[g_262e + 1] = g_261a->ext;
                g_261e = 3;
            }
            g_261c = *g_2626 | 0xd8;
        } else
            f017cb(g_00c0, 960);
        break;
    }
    g_262a[g_262e] = g_261c;
    if (g_2616) {
        f00f8a(1, (unsigned char)g_262a[g_2630]);
        if (g_2616 == 2)
            f00f8a(2, (unsigned char)g_262a[g_2630]);
    }
    return g_261e;
}
