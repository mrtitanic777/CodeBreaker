#include "seg.h"
extern int g_1186;              /* (the segments so far) */
extern struct seg *g_11bc;
extern char g_1240[];
extern int g_2f88;
extern char *g_123c;
extern f048bd(), f040fb(), f04127(), f04274(), f017cb();

/* (open: the original computes the attribute byte in words and truncates it at the end (sub ah, ah); release
   3.00 does it in bytes when the whole is cast) */

/* the SEGDEF record of the segment s */
f04b73(s)
struct seg *s;
{
    f048bd(0x98, 0);
    if (s->num != g_1186 && s != g_11bc)
        f017cb(g_123c, 523);
    g_1186++;
    s->grp = (int *)g_1240;
    s->size = 0;
    f040fb((unsigned char)(s->len & 0xffff0000L ? 2 : 0) | s->align << 5 | s->combine << 2);
    f04127((int)s->len);
    f04274(s->nameidx);
    f04274(s->clsidx);
    f04274(g_2f88);
}
