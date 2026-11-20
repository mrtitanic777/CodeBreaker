#include "targ.h"
extern int g_1182;              /* (the EXTDEFs so far) */
extern char *f0475f();
extern f048bd(), f041e1(), f0405a(), f04274();

/* the EXTDEF record of the symbol s: its index kept */
f04d5d(s)
struct targ *s;
{
    f048bd(0x8c, 0);
    s->t8 = g_1182++;
    f041e1(f0475f(s));
    f04274(f0405a(s));
}
