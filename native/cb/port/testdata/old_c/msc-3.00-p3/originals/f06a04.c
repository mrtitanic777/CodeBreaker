#include "opnd.h"
#include "ins.h"
#include "recfmt.h"
extern struct ins *g_35f2[];    /* (the free items of each kind) */
extern struct recfmt g_0074[];
extern char *f03f02();

/* a new item of the kind kind: a free one, else from the pool */
struct ins *
f06a04(kind)
char kind;
{
    struct ins *p;

    if (p = g_35f2[(unsigned char)kind]) {
        g_35f2[(unsigned char)kind] = p->prev;
        p->next = 0;
        p->prev = 0;
        if (!kind) {
            p->a = 0;
            p->b = 0;
        }
    } else
        p = f03f02(g_0074[(unsigned char)kind].n, 0, 2);
    return p;
}
