#include "rec.h"
#include "rseg.h"
#include "sub.h"
#include "cell.h"
extern struct rseg *g_11b6;
extern struct seg *g_11bc;
extern struct cell *g_11b2;
extern char *g_123c;
extern f0484a(), f040fb(), f017cb();

/* (NOROOM 1,2,3,10 takes it from 222 differing bytes to 77: the compiler of 3.00 was short of room here too) */
/* (open: of the cases' shared "f0484a(...)" calls the original keeps the last copies (and leaves a dead jump
   where the first was), release 3.00 the first) */

/* a record of the type t for seg begun, unless the one being made can go on */
f048bd(t, seg)
char t;
struct rseg *seg;
{
    switch ((unsigned char)t) {
    case 0x80:
    case 0x88:
    case 0x8a:
    case 0x8e:
    case 0x98:
        f0484a(t, 0);
        break;
    case 0x8c:
    case 0x96:
    case 0x9c:
        if (t == g_360e[0] && g_3368 <= 1000)
            break;
        f0484a(t, 0);
        break;
    case 0xa0:
        if (t == g_360e[0] && seg == g_11b6 && g_3368 <= 1000 && g_11b2 && g_11b2->next) {
            if (((unsigned char)g_360e[5] << 8) + (unsigned char)g_360e[4] + g_3368 - 6 == seg->off)
                break;
            if (((unsigned char)g_360e[5] << 8) + (unsigned char)g_360e[4] + g_3368 - 5 == seg->off) {
                if (seg == g_11bc)
                    f017cb(g_123c, 360);
                f040fb(0);
                break;
            }
        }
        f0484a(t, seg);
        break;
    case 0x90:
        if (t == g_360e[0] && g_360e[3] == seg->sub->s2 && g_360e[4] == seg->idx)
            break;
        f0484a(t, seg);
        break;
    case 0x9a:
        if (t == g_360e[0] && g_360e[3] == seg->sub->s0)
            break;
        f0484a(t, seg);
        break;
    case 0x94:
        if (t == g_360e[0] && g_3368 <= 1000)
            break;
        f0484a(t, seg);
        break;
    default:
        f017cb(g_123c, 389);
        break;
    }
}
