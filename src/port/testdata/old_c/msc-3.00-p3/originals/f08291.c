#include "opnd.h"
#include "ins.h"
#include "lab.h"
extern int g_0c84;              /* (to make jumps short) */
extern int g_3162, g_3aa0;
extern struct ins *g_0c60;
extern long g_2fce;             /* (the bytes saved in this pass) */
extern long g_2fd2;             /* (the place of the item) */
extern long g_1e4a;             /* (the procedure's place as the second pass had it) */
extern long g_0cb4;
extern f083ef();

/* (open: the original tests the item loop only at its end; release 3.00 at its head too) */

/* the function's jumps made short where they reach, the places of its labels found again, while any is made
   short */
f08291()
{
    register struct ins *p;

    if (!g_0c84)
        return;
    g_3162 = 0;
    do {
        g_3162++;
        g_2fce = 0;
        g_2fd2 = g_1e4a;
        g_3aa0 = 0;
        for (p = g_0c60; p; p = p->next)
            switch (p->kind) {
            case 0:
                switch (p->op) {
                case 0x3d:
                    if (p->size == 4)
                        break;
                case 0x3f:
                    if (p->a->kind == 10)
                        f083ef(p);
                    break;
                }
                g_2fd2 += p->i9;
                if (p->op == 0x3f && p->a->o10 == 1)
                    g_2fd2 -= 3;
                break;
            case 7:
                g_2fd2 += 2;
                break;
            case 1:
            case 3:
                ((struct lab *)p->op)->place = g_2fd2;
                break;
            }
        g_0cb4 += g_2fce;
    } while (g_3aa0);
}
