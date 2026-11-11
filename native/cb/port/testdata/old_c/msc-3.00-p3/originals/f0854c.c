#include "opnd.h"
extern char *g_1f7e;
extern f017cb();

/* (open: one jump - the original ends case 10 in the last copy of the compare's tail, release 3.00 in the first) */

/* the operands a and b the same */
f0854c(a, b)
register struct opnd *a;
register struct opnd *b;
{
    int k;

    if (a && b) {
        if ((k = a->kind) != b->kind)
            return 0;
        switch (k) {
        case 1:
            return a->size == b->size;
        case 16:
            if (a->val == b->val)
                return 1;
            return 0;
        case 6:
        case 7:
            if (a->o12 != b->o12)
                return 0;
        case 4:
        case 5:
            if (a->size != b->size)
                return 0;
        case 3:
        case 8:
        case 9:
            if (a->val != b->val || a->o2 != b->o2)
                return 0;
            if (k == 4 || k == 6)
                return 1;
        case 10:
            return a->sym == b->sym;
        case 2:
            return a->val == b->val;
        case 15:
            break;
        default:
            f017cb(g_1f7e, 1219);
            break;
        }
        return 1;
    }
    return a == b;
}
