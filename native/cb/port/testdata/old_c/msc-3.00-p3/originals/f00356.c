#include "opnd.h"
extern struct opnd *g_2622, *g_2624;
extern char g_00e6[];

/* whether the second operand is the accumulator and the first memory */
f00356()
{
    if (g_2624->kind == 1 && !g_2624->size && g_00e6[g_2622->kind] == 3)
        return 1;
    return 0;
}
