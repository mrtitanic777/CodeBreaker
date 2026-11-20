#include "opnd.h"
#include "attr.h"
#include "optab.h"
/* an instruction as the third pass keeps it */
struct ins {
    int i0, i2;
    int kind;
    int op;
    char size;
    char i9;
    struct opnd *a, *b;
};
extern struct attr g_00de;      /* (no operand) */
extern struct attr g_2632, g_263a;
extern f011c3();

/* the size of the instruction p (a segment prefix counted) */
f010f8(p)
register struct ins *p;
{
    int nops;
    int pre;
    register struct opnd *x;

    pre = 0;
    g_2632 = g_00de;
    g_263a = g_00de;
    if (nops = g_17b2[p->op].nops) {
        x = p->a;
        g_2632.a0 = x->kind;
        g_2632.a2 = x->size;
        g_2632.l4 = x->val;
        if (x->o2)
            pre = 1;
        if (nops == 2) {
            x = p->b;
            g_263a.a0 = x->kind;
            g_263a.a2 = x->size;
            g_263a.l4 = x->val;
            if (x->o2)
                pre = 1;
        }
    }
    return f011c3(p->op, p->size, nops ? &g_2632 : 0, nops == 2 ? &g_263a : 0) + pre;
}
