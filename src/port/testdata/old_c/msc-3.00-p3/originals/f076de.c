#include "opnd.h"
#include "ins.h"
#include "lab.h"
#include "jt.h"
extern unsigned g_109c[];       /* (for each condition the conditions that imply it, a bit each) */
extern int g_3aa0, g_1ff4;
extern struct ins *f06df2();
extern f075f8(), f07247();

/* the jump p (of the condition cond) to a jump made a jump to where that one goes, as long as it can (jump
   chaining) */
f076de(p, cond)
struct ins *p;
{
    int again;
    int isi;
    register struct ins *t;
    int u;                      /* (not used) */
    struct ins *l;

    isi = p->kind == 0 ? 1 : 0;
    do {
        again = 0;
        l = ((struct jt *)(isi ? (char *)p->a : (char *)p))->lab->def;
        t = f06df2(l);
        if (t->kind || t->op != 0x3d && t->op != 0x3f || ((struct lab *)t->a->sym)->le == 4)
            return;
        if (isi && f075f8(p))
            return;
        if (t->op == 0x3d || isi && p->op != 0x3d && g_109c[cond] & 1 << t->size) {
            if (l->op == t->a->sym)
                return;
            f07247(p, t->a->sym);
            again = g_3aa0 = 1;
            g_1ff4++;
        }
    } while (again);
}
