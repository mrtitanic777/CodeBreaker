#include "rec.h"
extern struct rseg *g_11b6;              /* (the record's segment) */
extern f0484a();

/* the record ended (and another of its type begun) */
f04837()
{
    f0484a(g_360e[0], g_11b6);
}
