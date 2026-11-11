#include "opnd.h"
#include "optab.h"
extern int g_262c;
extern struct optab *g_261a;
extern struct opnd *g_2622, *g_2624;
extern char g_00e6[];
extern f00356(), f00329(), f00f52();

/* (open: the original moves the tail's second test (n == 2 ...) after the cases) */

/* the place (from the opcode) of the operand n's bytes in the instruction, m its ModR/M byte: kept in it */
f00f8a(n, m)
register n;
unsigned char m;
{
    register at;

    at = 2;
    switch (g_262c) {
    case 2:
        if (g_00e6[g_2622->kind] == 2)
            at = 1;
        break;
    case 1:
        if (f00356() || f00329()) {
            at = 1;
            break;
        }
    case 0x11:
    case 0x12:
    case 0x14:
    case 0x15:
    case 0x17:
    case 0x2d:
    case 0x2e:
    case 0x2f:
    case 0x30:
        if (g_2622->kind == 1) {
            if (g_262c == 1 || !g_2622->size)
                if (g_00e6[g_2624->kind] == 2)
                    at = 1;
        } else if (g_00e6[g_2622->kind] == 3 || g_00e6[g_2622->kind] == 4 || g_00e6[g_2622->kind] == 6)
            if (g_00e6[g_2624->kind] == 2 && n == 2)
                at = f00f52(m) + 2;
        break;
    case 0x3c:
    case 0x3d:
        if (g_2622->kind == 0xa)
            at = 1;
        break;
    case 0x3e:
        at = 1;
        break;
    case 0x67:
        at = 0;
        break;
    default:
        if (g_261a->flags & 1)
            at++;
        break;
    }
    if (n == 1 && g_2622->o2 || n == 2 && (g_2622->o2 || g_2624->o2))
        at++;
    (n == 1 ? g_2622 : g_2624)->o1 = at;
}
