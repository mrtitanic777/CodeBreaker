#include "seg.h"
extern char *g_0306;
extern f017cb(), printf();

/* the listing moved into the segment n (its ENDS and SEGMENT lines) */
f01840(n)
{
    struct seg *p;

    if (!n || g_03d8 && g_03d8->num == n)
        return;
    for (p = g_11b8; p; p = p->next)
        if (p->num == n)
            break;
    if (!p)
        f017cb(g_0306, 184);
    if (g_03d8)
        printf("%s      ENDS\n", g_03d8->name);
    printf("%s      SEGMENT\n", p->name);
    g_03d8 = p;
}
