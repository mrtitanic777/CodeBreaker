#include "opnd.h"
#include "ins.h"

/* which byte of the constant of p is m, the other's the one to change: 1 the low byte, 0 the high byte, -1
   neither (or p not on a register's halves or a byte in memory) */
f0879b(p, m)
struct ins *p;
{
    struct opnd *a;
    struct opnd *b;

    a = p->a;
    b = p->b;
    if ((a->kind == 1 && a->size >= 0 && a->size <= 3 || a->kind != 1 && p->size != 1) && b->kind == 2) {
        if ((unsigned char)b->val == m)
            return 1;
        if ((b->val & 0xff00) == (long)(m << 8))
            return 0;
    }
    return -1;
}
