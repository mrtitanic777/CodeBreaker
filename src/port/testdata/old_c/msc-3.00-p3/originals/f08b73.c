#include "opnd.h"
#include "regc.h"
extern f0955e();

/* what the registers are known to hold forgotten where it is the register r or goes through it */
f08b73(r)
{
    register i;
    register struct regc *c;
    struct regc *next;

    for (i = 0; i < 20; i++)
        for (c = g_1fa8[i]; c; c = next) {
            next = c->next;
            switch (c->o->kind) {
            case 6:
            case 7:
                if (c->o->o12 == r) {
                    f0955e(c, i);
                    break;
                }
            case 1:
            case 4:
            case 5:
                if (c->o->size == r)
                    f0955e(c, i);
                break;
            }
        }
}
