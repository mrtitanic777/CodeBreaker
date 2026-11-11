#include "seg.h"
extern char *g_0c7c, *g_0c7a, *g_0c6c, *g_0c76;
extern int g_117c, g_117e, g_123e, g_11ba;
extern struct seg *g_11be, *g_11bc;
extern char g_2edc[];
extern char *g_116e[];
extern char f0308c();
extern char *f03f79();
extern long f03191();
extern char *f03f02();
extern f046bf(), f02cf4(), f03dad(), f0310f(), f032b8(), f017cb();

/* the segments: the code's and its group's names, and the far data's segments read from the symbol file */
f04591()
{
    char name[40];
    struct seg *sp;
    struct seg *q;
    register n;

    if (g_0c7c)
        f046bf(g_0c7c);
    else if (g_117c || g_117e) {
        f02cf4(g_2edc, "%s_TEXT", g_0c76);
        f046bf(g_2edc);
        g_0c7c = g_2edc;
    } else
        g_0c7c = "_TEXT";
    if (g_0c7a)
        f046bf(g_0c7a);
    else
        g_0c7a = "IGROUP";
    if (g_0c6c)
        g_11be->name = g_0c6c;
    g_11bc->name = g_0c7c;
    g_123e = 0;
    g_11ba = n = f0308c();
    sp = 0;
    while (n--) {
        if (!(sp = f03f02(0x18, sp, 0)))
            f03dad();
        sp->num = f0310f();
        f032b8(name, 40);
        f046bf(sp->name = f03f79(name, 0));
        sp->len = f03191();
    }
    if (!g_11b8)
        f017cb(g_116e[0], 651);
    for (q = g_11b8; q->next; q = q->next)
        ;
    q->next = sp;
}
