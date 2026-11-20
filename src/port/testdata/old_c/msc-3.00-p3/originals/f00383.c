#include "opnd.h"
extern struct opnd *g_2622, *g_2624;
extern char g_261c, g_2618, g_2619, g_2620;
extern int g_261e;
extern f00219(), f01739();

/* (the instruction with an immediate word made the short form, sign-extended, when the value fits a byte) */
f00383()
{
    f00219();
    if ((g_2618 != 1 || g_2622->size) && g_2619 == 2 && g_2620 == 2 && f01739((int)g_2624->val)) {
        g_261e--;
        g_261c |= 2;
    }
}
