#include "opnd.h"
#include "targ.h"
extern char *g_00c0;
extern f017cb();

/* (open: of the assertions' shared call the original keeps the last copy (the outer default's), release 3.00 an
   earlier one) */

/* the offset the operand f stands for */
f0012a(f)
struct opnd *f;
{
    int r;
    struct targ *t;

    t = (struct targ *)f->sym;
    switch (t->kind) {
    case 0:
        switch (t->flags >> 4 & 0xf) {
        case 2:
            switch (t->flags >> 8 & 0xf) {
            case 2:
            case 3:
                r = (int)f->val;
                break;
            case 1:
            case 4:
                r = t->off + (int)f->val;
                break;
            default:
                f017cb(g_00c0, 152);
                break;
            }
            break;
        case 4:
        case 6:
            r = t->off + (int)f->val;
            break;
        default:
            f017cb(g_00c0, 162);
            break;
        }
        break;
    case 1:
        r = t->off + (int)f->val;
        break;
    case 2:
        r = 0;
        break;
    default:
        f017cb(g_00c0, 173);
        break;
    }
    return r;
}
