#include "opnd.h"
#include "optab.h"
extern char *g_2626;            /* (the opcode's bytes) */
extern char g_261c, g_2618, g_2619, g_2620;
extern char g_00e6[];
extern struct opnd *g_2622;
extern struct optab *g_261a;
extern struct opnd *g_2624;
extern int g_262c;
extern char *g_00c0;
extern f003c6(), f001c9(), f0012a(), f0053e(), f00102(), f017cb();

/* (the instruction's opcode and its immediate operand put in the record) */
f00219()
{
    g_261c = *g_2626;
    if (g_00e6[g_2619] == 2) {
        if (g_2618 == 1 && !g_2622->size)
            g_261c = g_2626[2];
        else {
            f003c6(g_2622, g_261a->ext);
            g_261c = g_2626[1];
        }
        switch (g_2619) {
        case 2:
        case 0xb:
            f001c9(g_2620, (int)g_2624->val);
            break;
        case 8:
            f001c9(2, f0012a(g_2624));
            break;
        case 9:
        case 0xf:
            f001c9(2, 0);
            break;
        default:
            f017cb(g_00c0, 244);
            break;
        }
    } else {
        f0053e();
        switch (g_262c) {
        case 6:
        case 0x2e:
            break;
        default:
            if (g_2618 == 1)
                g_261c |= 2;
            break;
        }
    }
    f00102();
}
