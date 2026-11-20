#include "opnd.h"
#include "ins.h"
#include "lab.h"
#include "psym.h"
#include "fcon.h"
extern int g_0c86, g_0c88;      /* (to optimize at all; to ...) */
extern int g_3162;              /* (the passes over the function) */
extern int g_3aa0;              /* (something was changed) */
extern struct ins *g_0c60;
extern struct ins *g_3570;      /* (the item looked at next) */
extern int g_1fe6, g_1fe8, g_1ffa, g_2004;      /* (the counts of each change) */
extern struct psym *g_0cc4;     /* (the function) */
extern char *g_1f80[];          /* (for each register, the halves of it) */
extern int g_0c8a;              /* (the source line) */
extern char *g_1f7e;
extern struct ins *f06df2(), *f06e2c();
extern f08af3(), f07120(), f070da(), f08ebc(), f073d4(), f0762a(), f07939(), f07667(), f077b5(), f076de();
extern f07523(), f07410(), f08816(), f0863d(), f00306(), f086c5(), f0879b(), f0854c(), f017cb();

/* (NOROOM 7,10,15 takes it from 1312 differing bytes to 888: the compiler of 3.00 was short of room here too) */
/* (open: 1,657 bytes for 1,663 - the original tests the label loop at its head, moves the switch's end and
   the loop's step up among the cases, and shares the tails of the and/or cases; release 3.00 does not) */

/* the post-optimizer's peephole passes over the function's items, again while any changes one */
f07c12()
{
    register struct ins *p;
    struct opnd *rhs;
    struct opnd *dest;
    register struct ins *q;
    int half;
    struct fcon *fp;
    struct opnd *t;

    if (!g_0c86)
        return;
    g_3162 = 0;
    do {
        g_3162++;
        g_3aa0 = 0;
        for (p = g_0c60; p; p = g_3570) {
            g_3570 = p->next;
            switch (p->kind) {
            case 1:
                f08af3(0);
                if (!((struct lab *)p->op)->refs) {
                    f07120(p);
                    g_1fe6++;
                } else
                    while ((q = f06df2(p))->kind == 1) {
                        f070da(p->op, q->op);
                        f07120(q);
                        g_1fe8++;
                    }
                break;
            case 0:
                dest = p->a;
                rhs = p->b;
                switch (p->op) {
                case 0x3d:
                    if (dest->kind == 10) {
                        f073d4(p);
                        if (!f0762a(p) && !f07939(p) && !f07667(p) && !f077b5(p)) {
                            f076de(p, 0);
                            f07523(p);
                            f07410(p);
                        }
                    }
                    break;
                case 0x3f:
                    if (!f0762a(p))
                        f076de(p, p->size);
                    break;
                case 0x14:
                    if (rhs->kind == 11)
                        rhs->kind = 2;
                    if (rhs->kind == 2 && !rhs->val && f08816(p)) {
                        f07120(p);
                        if (!g_3aa0)
                            f017cb(g_1f7e, 571);
                        g_2004++;
                    }
                    break;
                case 1:
                    if (dest->kind == 1 && dest->size == 0) {
                        if (rhs->kind == 11)
                            rhs->kind = 2;
                        else if (rhs->kind == 2)
                            f0863d((int)rhs->val, p);
                        break;
                    }
                    t = dest->kind == 1 && f00306(dest) ? rhs : rhs->kind == 1 && f00306(rhs) ? dest : 0;
                    if (t) {
                        for (fp = g_200a; fp; fp = fp->next)
                            if (fp->con == t->sym)
                                break;
                        if (fp)
                            t->sym = fp->var;
                    }
                    break;
                case 2:
                case 4:
                    if (dest->kind == 1 && (dest->size == 6 || dest->size == 7)
                        && !((long)(1 << dest->size) & *(long *)&g_0cc4->s12)) {
                        f07120(p);
                        g_2004++;
                    }
                    break;
                case 0x17:
                    if (!f086c5(p) && dest->kind == 1 && rhs->kind == 2 && !rhs->val) {
                        p->op = 0x2f;
                        p->b = dest;
                        g_2004++;
                        g_3aa0 = 1;
                    }
                    break;
                case 0x2d:
                    if (dest->kind == 1 && dest->size == 0 && rhs->kind == 2 && rhs->val <= 0xff)
                        if ((q = f06e2c(p))->op == 0x24)
                            f07120(q);
                    if (rhs->kind == 2 && rhs->val <= 0x7f && dest->kind == 1 && dest->size == 12)
                        if ((q = f06df2(p))->op == 0x24) {
                            f07120(q);
                            p->size = 2;
                            dest->size = 0;
                        }
                    if (rhs->kind == 2 && !rhs->val)
                        if (dest->kind == 1) {
                            p->op = 0x14;
                            p->b = dest;
                            g_3aa0 = 1;
                        } else if (f08816(p)) {
                            p->op = 1;
                            g_3aa0 = 1;
                        }
                    if (!f08816(p))
                        break;
                    if (rhs->kind == 2 && rhs->val == 0xffffL) {
                        f07120(p);
                        break;
                    }
                    if (half = f0879b(p, 0xff)) {
                        if (half != 1)
                            break;
                        rhs->val <<= 8;
                    }
                    rhs->val &= 0xff;
                    if (dest->kind == 1)
                        dest->size = g_1f80[dest->size][half];
                    else if (half == 1)
                        dest->val++;
                    p->size = 1;
                    g_3aa0 = 1;
                    break;
                case 0x2f:
                case 0x30:
                    if (!f08816(p))
                        break;
                    if (rhs->kind == 2 && !rhs->val) {
                        f07120(p);
                        break;
                    }
                    if (half = f0879b(p, 0)) {
                        if (half != 1)
                            break;
                        rhs->val <<= 8;
                    }
                    rhs->val &= 0xff;
                    if (dest->kind == 1)
                        dest->size = g_1f80[dest->size][half];
                    else if (half == 1)
                        dest->val++;
                    p->size = 1;
                    g_3aa0 = 1;
                    break;
                case 0x11:
                    if (f08816(p) && f0879b(p, 0) == 1) {
                        rhs->val = (rhs->val >> 8) & 0xff;
                        if (dest->kind == 1)
                            dest->size = g_1f80[dest->size][1];
                        else
                            dest->val++;
                        p->size = 1;
                        g_3aa0 = 1;
                    }
                    if (g_0c88 && p->a->kind == 1 && p->a->size == 4 && !(q = f06df2(p))->kind)
                        if (q->op == 0x46 || q->op == 1 && q->a->kind == 1 && q->a->size == 4)
                            f07120(p);
                    break;
                case 0x25:
                    if (!(q = f06df2(p))->kind && q->op == 1 && q->a->kind == 1 && q->a->size == 2)
                        f07120(p);
                    break;
                case 0x2b:
                case 0x2c:
                    q = f06df2(p);
                    rhs = p->b;
                    if (!q->kind && q->op == 0x2a && f0854c(p->a, q->a) && rhs->kind == 2 && f0854c(rhs, q->b)) {
                        f07120(q);
                        p->op = 0x2d;
                        rhs->val = (-1L << rhs->val) & 0xffff;
                    }
                    break;
                }
                if (p->kind != -1)
                    f08ebc(p);
                break;
            case 7:
                f076de(p, 0);
                break;
            case 2:
                while ((q = p->next) && q->kind == 2) {
                    f07120(p);
                    p = q;
                    g_1ffa++;
                }
                g_0c8a = p->op;
                break;
            case 3:
                f08af3(0);
                break;
            }
        }
    } while (g_3aa0);
}
