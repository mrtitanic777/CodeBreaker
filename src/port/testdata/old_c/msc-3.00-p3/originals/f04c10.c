#include "seg.h"
extern f048bd(), f040fb(), f04274();

/* the GRPDEF record of the group g (its segment s) */
f04c10(s, g)
struct seg *s;
{
    s->grp = (int *)g;
    f048bd(0x9a, s);
    f040fb(0xff);
    f04274(s->num);
}
