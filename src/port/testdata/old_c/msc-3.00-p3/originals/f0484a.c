#include "rec.h"
#include "rseg.h"
#include "sub.h"
extern struct rseg *g_11b6;
extern f04a37(), f04274(), f04127(), f040fb();

/* the record put out and a new one of the type t begun for seg */
f0484a(t, seg)
char t;
struct rseg *seg;
{
    f04a37();
    g_11b6 = seg;
    g_360e[0] = t;
    g_3368 = 3;
    switch ((unsigned char)t) {
    case 0xa0:
        f04274(seg->idx);
        f04127(seg->off);
        break;
    case 0x9a:
        f040fb(seg->sub->s0);
        break;
    case 0x90:
    case 0x94:
        f04274(seg->sub->s2);
        f04274(seg->idx);
        break;
    }
}
