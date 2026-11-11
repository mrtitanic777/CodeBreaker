#include "opnd.h"
#include "ins.h"
#include "psym.h"
#include "lab.h"
extern int g_1e48;
extern struct ins *g_0c60;      /* (the function's items) */
extern struct psym *g_0cc4;     /* (the function) */
extern int g_0c4e, g_0954, g_0c8a;
extern long g_1e4a;
extern char *g_1e46;
extern char f0308c();
extern char *f0475f();
extern long f03c8b();
extern struct ins *f06a04();
extern f06a6f(), f06028(), f06ff5(), f017cb();

/* (NOROOM 1,2,3 takes it from 269 differing bytes to 67: the compiler of 3.00 was short of room here too) */
/* (open: the original moves the "too large" block after the switch's dispatch and shares the "return g_0cc4";
   release 3.00 keeps the block in place and copies the return) */

/* the function's items read into memory, each after the one before (each label knowing where it is placed,
   each jump entered as a reference): its symbol; 0 when it is too large to keep (it is then done as it is) */
struct psym *
f06adc()
{
    register struct ins *p;
    char kind;
    struct ins *last;

    g_1e48 = 0;
    last = g_0c60;
    g_2fb8 = 0;
    if (!last)
        f017cb(g_1e46, 243);
    g_0cc4 = g_0c60->kind == 3 ? (struct psym *)g_0c60->op : 0;
    g_0954 = g_0c4e;
    for (;;) {
        if ((kind = f0308c()) == -1)
            return g_0cc4;
        if (!f06a6f(kind)) {
            g_1e4a = f03c8b(kind, f0475f(g_0cc4));
            return 0;
        }
        p = f06a04(kind);
        f06028(p, kind, g_0c4e);
        switch (kind) {
        case 0:
            if ((p->op == 0x3d || p->op == 0x3f) && p->a->kind == 0xa)
                f06ff5(p, p->a->sym);
            break;
        case 1:
            if (((struct lab *)p->op)->def)
                f017cb(g_1e46, 274);
            ((struct lab *)p->op)->def = p;
            break;
        case 7:
            if (!((struct lab *)p->op)->def)
                f017cb(g_1e46, 279);
            if (!((struct lab *)p->op)->refs)
                f06ff5(p, p->op);
            f06ff5(p, *(int *)&p->size);
            break;
        case 3:
            if (g_0cc4)
                f017cb(g_1e46, 295);
            g_0cc4 = (struct psym *)p->op;
            break;
        case 6:
            last->next = p;
            p->prev = last;
            return g_0cc4;
        case 2:
            g_0c8a = p->op;
            break;
        case 8:
            break;
        default:
            f017cb(g_1e46, 324);
            break;
        }
        if (!last)
            f017cb(g_1e46, 330);
        p->prev = last;
        last->next = p;
        last = p;
    }
}
