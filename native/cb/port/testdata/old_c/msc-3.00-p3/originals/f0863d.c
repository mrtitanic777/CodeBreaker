#include "opnd.h"
#include "ins.h"
extern int g_2004, g_3aa0;
extern struct ins *f06df2();

/* the "mov dx,n" after the item p (that leaves v in AX) made a cwd where n is AX's sign */
f0863d(v, p)
struct ins *p;
{
    struct ins *q;
    int val;

    v &= 0x8000;
    if ((q = f06df2(p))->kind == 0 && q->op == 1 && q->a->kind == 1 && q->a->size == 2 && q->b->kind == 2) {
        val = q->b->val;
        if (!v && !val || v && val == -1) {
            q->op = 0x25;
            q->a = q->b = 0;
            g_2004++;
            g_3aa0 = 1;
        }
    }
}
