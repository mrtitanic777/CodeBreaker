#include "opnd.h"
#include "regc.h"
extern int g_0c92;
extern f08ab1(), f0955e();

/* what the registers are known to hold forgotten: all, or (if all is 0) all but SI and DI, and of those what
   is not a constant (nor a ...) */
f08af3(all)
{
    register reg;
    struct regc *next;
    register struct regc *c;

    for (reg = 0; reg < 20; reg++)
        if (!all || reg != 6 && reg != 7)
            f08ab1(reg);
    if (all)
        for (reg = 6;; reg = 7) {
            for (c = g_1fa8[reg]; c; c = next) {
                next = c->next;
                switch (c->o->kind) {
                case 2:
                    break;
                case 4:
                case 5:
                    if (g_0c92 && c->o->size == 5)
                        break;
                default:
                    f0955e(c, reg);
                }
            }
            if (reg == 7)
                break;
        }
}
