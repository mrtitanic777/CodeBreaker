#include "opnd.h"
#include "ins.h"
#include "targ.h"
#include "seg.h"
extern struct ins *g_264a;
extern char g_264c;
extern struct seg *g_11be;      /* (DGROUP) */
extern char *g_0306;
extern char *f03dbb(), *f0475f();
extern f020d5(), f017cb(), printf();

/* (open: of the shared "add sp, 4" tails the original keeps the last copy, release 3.00 the first) */

/* the operand p in the listing */
f01eb6(p)
struct opnd *p;
{
    int kind;
    register cnt;
    struct targ *t;
    register char *b;

    kind = p->kind;
    if (p->o2) {
        if (kind == 1 || kind == 0xf || kind == 9)
            f017cb(g_0306, 474);
        printf("%s:", f03dbb(p->o2, 2));
    }
    switch (kind) {
    case 1:
        printf("%s", f03dbb(p->size, g_264a->size));
        break;
    case 5:
    case 7:
        printf("%s", f0475f(p->sym));
    case 4:
    case 6:
        f020d5(p);
        break;
    case 0xf:
        printf("SEG %s", g_11be->name);
        break;
    case 9:
        printf("SEG %s", f0475f(p->sym));
        break;
    case 8:
        t = (struct targ *)p->sym;
        printf(t->kind == 2 || (t->flags & 0xf0) == 0x60 && t->t12 != 4 || (t->flags & 0xf0) == 0x20 && t->t12 != 2
            ? "OFFSET " : "OFFSET DGROUP:");
    case 3:
    case 0xa:
        printf("%s", f0475f(p->sym));
        if (p->val)
            printf("%s%ld", p->val > 0 ? "+" : "", p->val);
        break;
    case 0x10:
        printf("%d", (int)p->val);
        break;
    case 2:
    case 0xb:
        printf("%d", g_264c == 1 ? (unsigned char)p->val : (int)p->val);
        break;
    case 0xd:
        printf("\tDB\t");
        cnt = p->size;
        b = (char *)&p->val;
        while (cnt--)
            printf("0%xH, ", (unsigned char)*b++);
        break;
    default:
        f017cb(g_0306, 548);
        break;
    }
}
