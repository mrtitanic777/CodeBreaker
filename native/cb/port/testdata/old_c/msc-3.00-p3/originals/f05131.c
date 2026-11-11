#include "opnd.h"
#include "targ.h"
#include "fix.h"
extern struct cell *g_11ae;
extern int g_123e;
extern int g_2f84, g_2f8a, g_2f92;
extern char *g_123c;
extern char *f0475f();
extern struct cell *f03fec();
extern f04f85(), f02c04(), f04023(), f017cb();

/* (open: of the shared tails the original keeps the last copies, release 3.00 the first) */

/* the fixup the operand p (of an instruction op, the bytes from base, a far one if size 4) needs, if any */
f05131(p, size, base, op)
struct opnd *p;
{
    struct fix *f;
    struct targ *s;

    switch (p->kind) {
    case 0xa:
        if (op != 0x3c)
            return;
        break;
    case 0: case 1: case 2: case 4: case 6: case 0xb: case 0xc: case 0xd: case 0xe: case 0x10:
        return;
    case 3: case 5: case 7: case 8: case 9: case 0xf:
        break;
    default:
        f017cb(g_123c, 1021);
        break;
    }
    f = f03fec();
    f->seg = 1;
    f->loc5 = 0;
    f->loc = p->kind == 9 || p->kind == 0xf ? 2 : 1;
    f->off = p->o1 + base;
    f->p = 1;
    f->idx = 0;
    s = (struct targ *)p->sym;
    switch (p->kind) {
    case 0xf:
        f->fthread = 0;
        f->frame = 5;
        f->tthread = 1;
        f->target = g_2f8a;
        break;
    case 3: case 5: case 7: case 8: case 9:
        switch (s->kind) {
        case 0:
            f04f85(s, f);
            break;
        case 1:
        case 2:
            f->fthread = 0;
            f->frame = 5;
            if (s->kind == 1 && s->flags != 4) {
                if (g_123e) {
                    f->fthread = 1;
                    f->frame = g_2f84;
                }
                f->tthread = 1;
                f->target = g_2f92;
            } else {
                f->tthread = 0;
                f->target = 2;
                f->idx = s->t8;
            }
            break;
        default:
            f017cb(g_123c, 1082);
            break;
        }
        break;
    case 0xa:
        if (s->kind != 2)
            f017cb(g_123c, 1090);
        switch (size) {
        case 2:
            f->seg = 0;
            break;
        case 4:
            f->seg = 1;
            f->loc = 3;
            break;
        default:
            f017cb(g_123c, 1100);
            break;
        }
        f->fthread = 0;
        f->frame = 5;
        switch (s->t10) {
        case 2:
            f02c04("static procedure `%s' not found.", f0475f(s));
            f = g_11ae;
            g_11ae = f->next;
            f->next = 0;
            f04023(f);
            break;
        case 3: case 4: case 5:
            f->tthread = 0;
            f->target = 2;
            f->idx = s->t8;
            break;
        default:
            f017cb(g_123c, 1128);
            break;
        }
        break;
    }
}
