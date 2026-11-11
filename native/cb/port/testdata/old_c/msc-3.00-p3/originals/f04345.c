#include "opnd.h"
#include "ins.h"
#include "targ.h"
#include "init.h"
#include "seg.h"
#include "tflags.h"
extern struct init g_2ece;      /* (the constant made) */
extern struct seg *g_11b4, *g_11c0;
extern char *g_116e[];
extern f017cb(), f03fc9();

/* (open: the original's "|=" on the bit field keeps the value in a temporary and loads the pointer again; release
   3.00 keeps the pointer in two temporaries) */

/* the constant of the floating point load p made an initializer (in _DATA) */
struct init *
f04345(p)
struct ins *p;
{
    char *dst;
    register struct opnd *q;
    struct tflags *flags;
    register struct init *ip;

    if (p->kind || p->op != 0x67)
        f017cb(g_116e[0], 413);
    if (((struct targ *)p->a->sym)->kind)
        f017cb(g_116e[0], 414);
    flags = (struct tflags *)&((struct targ *)p->a->sym)->flags;
    flags->used |= 1;
    ip = &g_2ece;
    ip->next = 0;
    ip->kind = 1;
    q = p->b;
    switch (q->kind) {
    default:
        f017cb(g_116e[0], 444);
        break;
    case 9:
        ip->w4 = 0x42;
        ip->v.w = 0;
        break;
    case 0xd:
        switch (q->size) {
        default:
            f017cb(g_116e[0], 438);
            break;
        case 4:
            ip->w4 = 0x34;
            dst = (char *)&ip->v;
            break;
        case 8:
            ip->w4 = 0x38;
            dst = (char *)&ip->v;
            break;
        }
        f03fc9(dst, &q->val, q->size);
        break;
    }
    g_11b4 = g_11c0;
    return ip;
}
