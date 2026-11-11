#include "opnd.h"
#include "ins.h"
extern int g_2002;
extern struct ins *f06e2c(), *f06df2();
extern f0854c(), f08876(), f071f1();

/* (open: 299 bytes for 301 - the original moves case 1 (it ends in a jump) after the second compare, release
   3.00 leaves it in place) */

/* the conditional jump j after the compare (or test) j's item before it and the setting p of the same register
   two items before that, if it goes the same way for what p sets: 1 if it always goes, 0 if it never (and taken
   out, to the label at *lab), 2 if not known, -1 if j is not a plain jump */
f08986(lab, p, j)
int *lab;
register struct ins *p;
register struct ins *j;
{
    int u;                      /* (not used) */
    int k;
    long l;
    long w;
    struct opnd *a;
    int x;                      /* (not used) */

    if (j->i9)
        return -1;
    j = f06e2c(j);
    if (f06e2c(f06e2c(j)) != p)
        return 2;
    if (p->kind || j->kind)
        return 2;
    switch (p->op) {
    case 0x14:
    case 0x30:
        if (!f0854c(p->a, p->b))
            return 2;
        l = 0;
        break;
    case 1:
        if (p->b->kind != 2)
            return 2;
        l = (int)p->b->val;
        break;
    default:
        return 2;
    }
    a = p->a;
    if ((k = j->op) == 0x17 || k == 0x2f || k == 0x2e) {
        if (!f0854c(a, j->a))
            return 2;
        if (k == 0x2f) {
            if (!f0854c(a, j->b))
                return 2;
            w = 0;
        } else {
            if (j->b->kind != 2)
                return 2;
            w = (int)j->b->val;
        }
    } else
        return 2;
    if (f08876(f06df2(j)->size, l, w))
        return 1;
    f071f1(j, *lab);
    g_2002++;
    return 0;
}
