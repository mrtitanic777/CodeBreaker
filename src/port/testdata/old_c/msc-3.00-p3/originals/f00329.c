#include "opnd.h"
extern struct opnd *g_2622, *g_2624;
extern char g_00e6[];

/* whether the first operand is the accumulator and the second memory */
f00329()
{
    if (g_2622->kind == 1 && !g_2622->size && g_00e6[g_2624->kind] == 3)
        return 1;
    return 0;
}
