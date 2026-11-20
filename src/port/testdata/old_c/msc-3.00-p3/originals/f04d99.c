#include "targ.h"
extern char *f0475f();
extern f048bd(), f041e1(), f04127(), f0405a(), f04274();

/* the PUBDEF record of the symbol s (in the segment seg) */
f04d99(s, seg)
struct targ *s;
{
    f048bd(0x90, seg);
    f041e1(f0475f(s));
    f04127(s->off);
    f04274(f0405a(s));
}
