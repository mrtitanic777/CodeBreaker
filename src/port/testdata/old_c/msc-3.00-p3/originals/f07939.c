#include "opnd.h"
#include "ins.h"
#include "lab.h"
extern int g_3aa0, g_2006;
extern char *g_1e46;
extern struct ins *f06e2c(), *f06df2();
extern f0762a(), f06c80(), f017cb();

/* (open: the original takes b->next from AX after storing it, release 3.00 loads it again) */

/* the block at the jump p's label (after an unconditional jump, up to the next one) moved to just after p, the
   jump p then taken out (code motion): whether it was */
f07939(p)
register struct ins *p;
{
    struct ins *b;
    register struct ins *lab;

    lab = ((struct lab *)p->a->sym)->def;
    b = f06e2c(lab);
    if (b->kind || b->op != 0x3d)
        return 0;
    for (b = f06df2(lab); b; b = f06df2(b)) {
        if (b->kind == 6)
            return 0;
        if (b->kind == 0 && b->op == 0x3d)
            break;
    }
    if (b == p)
        return 0;
    lab->prev->next = b->next;
    b->next->prev = lab->prev;
    b->next = p->next;
    b->next->prev = b;
    p->next = lab;
    lab->prev = p;
    if (!f0762a(p))
        f017cb(g_1e46, 2105);
    f06c80();
    g_3aa0 = 1;
    g_2006++;
    return 1;
}
