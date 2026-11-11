#include "opnd.h"
#include "ins.h"
#include "regc.h"
#include "optab.h"
extern struct opnd *g_1fd8, *g_1fe2;    /* (the constants 0 and -1) */
extern int g_1ff8, g_3aa0;
extern struct ins *f06df2(), *f06e2c();
extern struct opnd *f06e7b();
extern f08bdd(), f08af3(), f09657(), f07120(), f08dfc(), f08e99(), f095d6(), f08e6e(), f08816(), f08bfe();
extern f0968e(), f00306(), f0950c();

/* (NOROOM 3,14 takes it from 1169 differing bytes to 982: the compiler of 3.00 was short of room here too) */
/* (open: the original merges the cases' like tails (the calls of f08bdd, f07120, f095d6, f08bfe) into one
   copy each; release 3.00 merges fewer) */

/* what the registers are known to hold brought up to date for the item p (and p made simpler by it where
   it can be: taken out, its operands made registers that hold them) */
f08ebc(p)
register struct ins *p;
{
    register struct opnd *pa;   /* (p's operands, their kinds, their registers) */
    int n;
    int rse;
    struct opnd *no;
    int o;
    struct opnd *pb;
    int ka;
    char sz;
    int rde;
    int es;                     /* (the segment register a les or lds loads) */
    struct regc *tmp;
    int kb;
    struct ins *next;

    sz = p->size;
    o = p->op;
    if (n = g_17b2[o].nops) {
        pa = p->a;
        ka = pa->kind;
        rde = pa->size;
        if (--n) {
            pb = p->b;
            kb = pb->kind;
            rse = pb->size;
        }
    }
    switch (o) {
    case 0x32:
    case 0x33:
    case 0x34:
        f08bdd(1);
        break;
    case 0x35:
        f08bdd(6);
        f08bdd(7);
        break;
    case 0x3c:
        f08af3(1);
        break;
    case 2:
        next = f06df2(p);
        if (next->kind == 0 && next->op == 4 && next->a->kind == 1 && f09657(pa, next->a->size)) {
            f07120(p);
            f07120(next);
            break;
        }
        if (ka != 1 && (rse = f08dfc(pa, 2)) >= 0)
            f08e99(pa, rse);
        f08bdd(4);
        break;
    case 4:
        f08bdd(4);
        if (ka == 1) {
            next = f06e2c(p);
            if (!next->kind && next->op == 2) {
                f08bdd(rde);
                f095d6(rde, next->a);
                break;
            }
        }
        f08bfe(pa, sz);
        break;
    case 0x17:
        if ((rde = f08e6e(pa, sz)) >= 0)
            f08e99(pa, rde);
        if ((rse = f08e6e(pb, sz)) >= 0)
            f08e99(pb, rse);
        break;
    case 0x14:
    case 0x30:
        if (ka == 1) {
            if (kb != 1 && (rse = f08e6e(pb, sz)) >= 0) {
                f08e99(pb, rse);
                kb = 1;
            }
            if (kb == 1 && rde == rse) {
                if (f09657(g_1fd8, rde)) {
                    f07120(p);
                    break;
                }
                f08bdd(rde);
                f095d6(rde, g_1fd8);
                break;
            }
        }
        f08bfe(pa, sz);
        break;
    case 0x2d:
    case 0x2f:
        if (kb != 1 && (rse = f08e6e(pb, sz)) >= 0) {
            f08e99(pb, rse);
            kb = 1;
        }
        if (f08816(p)) {
            if (kb == 1) {
                if (ka == 1 && rde == rse) {
                    f07120(p);
                    break;
                }
                if (f09657(o == 0x2f ? g_1fd8 : g_1fe2, rse)) {
                    f07120(p);
                    break;
                }
            }
            if (ka == 1 && f09657(o == 0x2f ? g_1fd8 : g_1fe2, rde)) {
                p->op = 1;
                g_3aa0 = 1;
            }
        }
        if (ka == 1 && kb == 1 && rde == rse)
            break;
        f08bfe(pa, sz);
        break;
    case 0x11:
    case 0x12:
    case 0x15:
        if (ka == 1 && (rse = f08e6e(pb, sz)) >= 0)
            f08e99(pb, rse);
    case 0x13:
    case 0x16:
    case 0x18:
    case 0x26:
    case 0x27:
    case 0x28:
    case 0x29:
    case 0x2a:
    case 0x2b:
    case 0x2c:
    case 0x31:
        f08bfe(pa, sz);
        break;
    case 0x24:
        f08bdd(16);
        break;
    case 0x1d:
    case 0x1e:
    case 0x20:
    case 0x21:
        if (ka != 1 && (rde = f08e6e(pa, sz)) >= 0)
            f08e99(pa, rde);
        f08bdd(0);
        if (sz != 2)
            break;
    case 0x25:
        f08bdd(2);
        break;
    case 0x1f:
        if (ka != 1 && (rde = f08e6e(pa, sz)) >= 0)
            f08e99(pa, rde);
        f08bdd(rse);
        break;
    case 6:
        tmp = g_1fa8[rse];
        g_1fa8[rse] = g_1fa8[rde];
        g_1fa8[rde] = tmp;
        break;
    case 1:
        if (ka == 1) {
            if (f09657(pb, rde)) {
                f07120(p);
                g_1ff8++;
                break;
            }
            f08bfe(pa, sz);
            if (kb == 1) {
                f095d6(rde, pb);
                f0968e(rde, rse);
                f095d6(rse, pa);
                break;
            }
            if ((rse = f00306(pa) ? f08e6e(pb, sz) : f08dfc(pb, sz)) >= 0) {
                f08e99(pb, rse);
                f0968e(rde, rse);
                break;
            }
            if (kb == 2 && !pb->val) {
                p->op = 0x30;
                f08e99(pb, rde);
                f095d6(rde, g_1fd8);
                break;
            }
            f095d6(rde, pb);
            break;
        }
        if (kb == 1) {
            if (f09657(pa, rse)) {
                f07120(p);
                g_1ff8++;
                break;
            }
            f08bfe(pa, sz);
        } else {
            f08bfe(pa, sz);
            if ((rse = f08dfc(pb, sz)) < 0)
                break;
            f08e99(pb, rse);
        }
        f095d6(rse, pa);
        break;
    case 10:
        f08bdd(rde);
        break;
    case 11:
    case 12:
        es = o == 11 ? 11 : 8;
        pb->val += 2;
        if (f09657(pb, es)) {
            pb->val -= 2;
            if (f09657(pb, rde)) {
                f07120(p);
                break;
            }
            p->op = 1;
            f08bdd(rde);
            f095d6(rde, pb);
            break;
        }
        pb->val -= 2;
        no = f06e7b(pb->kind);
        switch (pb->kind) {
        case 6:
        case 7:
            no->o12 = pb->o12;
            no->o13 = pb->o13;
        case 3:
        case 5:
            no->sym = pb->sym;
            no->o10 = pb->o10;
        }
        no->o1 = pb->o1;
        no->o2 = pb->o2;
        no->size = pb->size;
        no->val = pb->val + 2;
        if (f09657(pb, rde)) {
            p->op = 1;
            f08e99(pa, es);
            p->b = no;
        } else {
            f08bdd(rde);
            f095d6(rde, pb);
        }
        f08bdd(es);
        if (f0950c(rde, no))
            f095d6(es, no);
        break;
    }
}
