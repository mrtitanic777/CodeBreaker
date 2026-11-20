#include "opnd.h"
#include "targ.h"
#include "regc.h"
extern int g_0c92;
extern char *g_1f7e;
extern f08bdd(), f084cf(), f0955e(), f017cb();

/* (NOROOM 3 takes it from 241 differing bytes to 175: the compiler of 3.00 was short of room here too) */
/* (open: the original merges the cases' like tails across the switches (case 3's compare into case 16, the
   checks of g_0c92 into one); release 3.00 keeps them apart) */

/* what the registers are known to hold forgotten where it may be the operand o (of the width w), that is
   changed */
f08bfe(o, w)
struct opnd *o;
{
    register struct regc *c;
    int kind;
    struct regc *next;
    int rsize;
    register reg;

    if ((kind = o->kind) == 1) {
        f08bdd(o->size);
        return;
    }
    for (reg = 0; reg < 20; reg++)
        for (c = g_1fa8[reg]; c; c = next) {
            rsize = reg >= 12 ? 1 : 2;
            next = c->next;
            switch (kind) {
            case 3:
                switch (c->o->kind) {
                case 3:
                    if (f084cf(c->o, rsize, o, w))
                        f0955e(c, reg);
                    break;
                case 6:
                case 7:
                    if (c->o->o12 == 5)
                        break;
                case 4:
                case 5:
                    if (c->o->size != 5 && !g_0c92)
                        f0955e(c, reg);
                    break;
                }
                break;
            case 4:
            case 5:
                if (c->o->kind == 3 && ((struct targ *)c->o->sym)->flags & 0x2000)
                    break;
                switch (c->o->kind) {
                case 3:
                    if (!(((struct targ *)c->o->sym)->flags & 0x2000) && o->size != 5 && !g_0c92)
                        f0955e(c, reg);
                    break;
                case 4:
                case 5:
                    if (o->size == 5 && c->o->size == 5) {
                        if (f084cf(c->o, rsize, o, w))
                            f0955e(c, reg);
                    } else if (!g_0c92 || f084cf(c->o, rsize, o, w))
                        f0955e(c, reg);
                    break;
                case 6:
                case 7:
                    if (!g_0c92)
                        f0955e(c, reg);
                    break;
                }
                break;
            case 6:
            case 7:
                switch (c->o->kind) {
                case 3:
                    if (!(((struct targ *)c->o->sym)->flags & 0x2000) && o->size != 5 && o->o12 != 5 && !g_0c92)
                        f0955e(c, reg);
                    break;
                case 4:
                case 5:
                case 6:
                case 7:
                    if (!g_0c92)
                        f0955e(c, reg);
                    break;
                }
                break;
            case 16:
                if (f084cf(c->o, rsize, o, w))
                    f0955e(c, reg);
                break;
            default:
                f017cb(g_1f7e, 2044);
                break;
            }
        }
}
