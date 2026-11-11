#include "opnd.h"
#include "ins.h"
#include "optab.h"
/* (an immediate operand's value in bytes) */
struct imm {
    char kind, o1, o2, size;
    char lo, hi;
    int high;
};
extern struct ins *g_0c60;
extern f06c80();

/* the byte instructions' registers made the byte registers (12 up), their immediates bytes */
f07a11()
{
    register struct ins *p;

    for (p = g_0c60; p; p = p->next)
        if (p->kind == 0 && p->size == 1)
            switch (g_17b2[p->op].nops) {
            case 2:
                if (p->b->kind == 2) {
                    ((struct imm *)p->b)->hi = 0;
                    ((struct imm *)p->b)->high = 0;
                }
                if (p->b->kind == 1)
                    p->b->size += 12;
            case 1:
                if (p->a->kind == 1)
                    p->a->size += 12;
            }
    f06c80();
}
