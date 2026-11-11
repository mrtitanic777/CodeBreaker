#include "opnd.h"

/* the register r can be known to hold the operand o (o does not go through r): 1 if so */
f0950c(r, o)
struct opnd *o;
{
    int k;

    if ((k = o->kind) == 4 || k == 5 || k == 6 || k == 7) {
        if (o->size == r)
            return 0;
        if ((k == 6 || k == 7) && o->o12 == r)
            return 0;
    }
    return 1;
}
