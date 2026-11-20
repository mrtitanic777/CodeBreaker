#include "opnd.h"
#include "ins.h"
#include "fcon.h"
#include "targ.h"
extern struct ins *g_0c60;
extern char *g_1f7e;
extern struct fcon *f03e86();
extern f017cb();

/* (open: the original puts np in AX to store it and reuses it, and tests the loop only at its end;
   release 3.00 stores DI and tests the loop at its head too) */

/* the floating point constants' loads (67h) moved to the head of the function, one load of a constant kept
   for all the variables it goes to */
f07ad0()
{
    struct fcon *fc;
    struct ins *head;
    struct fcon *cell;
    register struct ins *p;
    register struct ins *np;

    fc = 0;
    cell = 0;
    for (p = head = g_0c60; p; p = np) {
        np = p->next;
        if (p->kind || p->op != 0x67)
            continue;
        (p->prev->next = np)->prev = p->prev;
        if (p->a->kind != 3)
            f017cb(g_1f7e, 343);
        ((struct targ *)p->a->sym)->flags |= 0x2000;
        if (p->b->kind == 9) {
            for (fc = g_2008; fc; fc = fc->next)
                if (((struct targ *)fc->con)->t12 == ((struct targ *)p->b->sym)->t12 && ((struct targ *)p->b->sym)->t12)
                    break;
            if (fc && (cell = f03e86(0, 6))) {
                cell->var = fc->var;
                cell->con = p->a->sym;
                cell->next = g_200a;
                g_200a = cell;
            }
        }
        if (fc && cell)
            continue;
        if (p->b->kind == 9) {
            cell = f03e86(0, 6);
            cell->con = p->b->sym;
            cell->var = p->a->sym;
            cell->next = g_2008;
            g_2008 = cell;
        }
        p->next = head->next;
        p->prev = head;
        p->next->prev = p;
        head->next = p;
        head = p;
    }
}
