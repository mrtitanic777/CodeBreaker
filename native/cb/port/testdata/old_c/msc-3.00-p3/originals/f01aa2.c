#include "opnd.h"
#include "ins.h"
#include "psym.h"
#include "optab.h"
extern struct ins *g_264a;      /* (the instruction being listed) */
extern int g_2652;              /* (its operator) */
extern char g_264c;             /* (its width) */
extern struct opnd *g_264e, *g_2650;    /* (its operands) */
extern struct psym *g_2654;     /* (the procedure) */
extern struct ins g_030a;       /* (a jump, for a long conditional jump's second half) */
extern long g_0ca8;
extern int g_117c, g_117e;     /* (far calls: two flags) */
extern char *g_106c[];          /* (MD.c's: the byte registers' names, then the conditions') */
extern char g_0318[];           /* (each condition's opposite) */
extern char *g_0328[];          /* (the 8087 instructions' names, from 69h) */
extern int g_0380[];            /* (their widths) */
extern char *g_0306;
extern char *f0475f(), *f03dbb();
extern f01840(), f02158(), f01e23(), f01eb6(), f017cb(), printf();
extern struct init *f04345();
extern struct init *f02666();

/* the instruction p in the listing (a, b, c: the bytes' column) */
f01aa2(p, a, b, c)
struct ins *p;
{
    int nops;
    int x;

    g_264a = p;
    g_2652 = p->op;
    g_264c = p->size;
    nops = g_17b2[g_2652].nops;
    g_264e = nops > 0 ? p->a : 0;
    g_2650 = nops > 1 ? p->b : 0;
    if (g_2652 == 0x67) {
        f01840(3);
        printf("%s\t", f0475f(g_264e->sym));
        x = f04345(p);
        if (g_2650->kind == 9)
            printf("SEG %s ", f0475f(g_2650->sym));
        else
            f02666(x);
        f01840(1);
        return;
    }
    f02158(a, b, c);
    switch (g_2652) {
    case 0x3f:
        if (a == 5 && c) {
            printf("j%s\t$+3\n", g_106c[8 + g_0318[g_264c]]);
            g_030a.a = g_264e;
            g_0ca8 += 2;
            f01aa2(&g_030a, 3, b + 2, 0);
            g_0ca8 -= 2;
            return;
        }
        printf("j%s\t", g_106c[8 + g_264c]);
        break;
    case 0x1f:
        printf("imul\t");
        break;
    case 0x38:
        printf("lods%c", (char)(p->size == 1 ? 'b' : 'w'));
        break;
    default:
        if (g_2652 >= 0x69 && g_2652 <= 0x94) {
            printf("%s	", g_0328[g_2652 - 0x69]);
            g_264c = g_0380[g_2652 - 0x69];
        } else
            printf("%s	", g_17b2[g_2652].name);
        break;
    }
    switch (g_2652) {
    case 0x57:
        printf("%s", f03dbb(g_264e->size, 2));
        break;
    case 0x3e:
        if (g_264e->val)
            f01eb6(g_264e);
        printf("\n%s\tENDP\n", f0475f(g_2654));
        break;
    case 7:
    case 8:
        if (g_264e->kind == 2)
            f01e23(g_264e);
        break;
    case 0x3d:
        if (g_264e->kind == 0xa) {
            if (g_264c == 1)
                printf("SHORT ");
        } else
            printf("WORD PTR ");
        f01eb6(g_264e);
        break;
    case 0x3c:
        if (g_117c || g_117e)
            printf("FAR PTR ");
        f01eb6(g_264e);
        break;
    case 0x1f:
        printf("%s,", f03dbb(g_2650->size, 2));
        f01e23(g_264e);
        printf(",%ld", g_2650->val);
        break;
    case 0x26: case 0x27: case 0x28: case 0x29: case 0x2a: case 0x2b: case 0x2c:
        f01e23(g_264e);
        g_264a->size = 1;
        printf(",");
        f01e23(g_2650);
        g_264a->size = g_264c;
        break;
    case 0x13:
    case 0x16:
        f01e23(g_264e);
        break;
    default:
        switch (g_17b2[g_2652].nops) {
        case 0:
            break;
        case 1:
            f01e23(g_264e);
            break;
        case 2:
            f01e23(g_264e);
            printf(",");
            f01e23(g_2650);
            break;
        default:
            f017cb(g_0306, 431);
            break;
        }
        break;
    }
    printf("\n");
}
