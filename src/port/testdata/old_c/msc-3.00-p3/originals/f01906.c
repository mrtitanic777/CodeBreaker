#include "opnd.h"
#include "ins.h"
#include "psym.h"
extern int g_0c9e;              /* (the listing has the code's bytes) */
extern long g_0ca8;             /* (the place in the code) */
extern struct psym *g_2654;     /* (the procedure) */
extern struct ins *g_264a;
extern char *f0475f();
extern f01aa2(), f02158(), printf(), exit();

/* the item p in the listing */
f01906(p, a, b, c)
struct ins *p;
{
    switch (p->kind) {
    case 0:
        f01aa2(p, a, b, c);
        break;
    case 1:
        if (g_0c9e && ((struct psym *)p->op)->place != g_0ca8) {
            printf("%s offset mismatch - %1ld vs. %1ld\n", "LABEL", g_0ca8, ((struct psym *)p->op)->place);
            exit(1);
        }
        if (g_0c9e)
            printf("\t\t\t\t\t");
        printf("%s:\n", f0475f(p->op));
        break;
    case 2:
        printf("; Line %d\n", p->op);
        break;
    case 3:
        if (g_0c9e && ((struct psym *)p->op)->place != g_0ca8) {
            printf("%s offset mismatch - %1ld vs. %1ld\n", "ENTRY", g_0ca8, ((struct psym *)p->op)->place);
            exit(1);
        }
        g_2654 = (struct psym *)p->op;
        if (((struct psym *)p->op)->cls == 5)
            printf("\tPUBLIC\t%s\n", f0475f(g_2654));
        printf("%s\tPROC %s\n", f0475f(g_2654), ((struct psym *)p->op)->flags & 4 ? "FAR" : "NEAR");
        break;
    case 7:
        g_264a = p;
        f02158(a, b, 0);
        printf("\tDW\t%s\n", f0475f(*(int *)&p->size));
        break;
    case 8:
        printf(";\t%s\n", p->op);
        break;
    }
}
