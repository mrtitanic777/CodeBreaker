#include <stdio.h>
#include "seg.h"
#include "outf.h"
#include "cell.h"
#include "grp.h"
extern char g_3af2[];           /* (the suffix of segments' names: the time) */
extern char *g_0c76;            /* (the source's name) */
extern int g_0cb8;              /* (no library records) */
extern int g_1180;              /* (the memory model's letter) */
extern char *g_1248, *g_124a, *g_124c, *g_124e, *g_1250;       /* (the default libraries' names) */
extern int g_0cbc, g_0cc2, g_0cbe, g_0cc0;
extern char g_3a0a[];
extern int g_2f88;
extern char *g_0c6c, *g_0c6a, *g_0c7a;
extern struct grp *g_1244, *g_1246;     /* (DGROUP, the code's group) */
extern int g_1184, g_123e, g_11ba, g_1190, g_118e;
extern struct seg *g_11bc, *g_11be, *g_11c0, *g_11c2;
extern long g_0cb4, g_0cb0, g_0cac, g_0ca4;     /* (the segments' sizes) */
extern long g_1338;             /* (where the code's SEGDEF is in the object) */
extern FILE *g_0c5a;
extern int g_2f8c, g_2f8a, g_2f92, g_2f98, g_2f86, g_2f9a, g_2f84;
extern char *f03f02();
extern long ftell();
extern time(), f02cf4(), f04afe(), f04b16(), f04b4f(), strcmp(), f04b73(), f04c10(), f04f1d(), f058d4(), f03dad(),
    f04023(), f046e3();

/* the object's head: its name, its comments, the names, segments, groups and threads */
f053c7()
{
    int far;
    char tname[64];
    char b2[64];
    int i;
    long t;
    struct seg *sp;
    int x;                      /* (not used) */

    time(&t);
    f02cf4(g_3af2, "%lx", t);
    f02cf4(tname, "%s.c", g_0c76);
    f04afe(tname);
    if (!g_0cb8) {
        f04b16(0, "MS C");
        *g_1248 = g_1180;
        *g_124a = g_1180;
        if (g_0cbc || g_0cc2) {
            f04b16(0x9f, g_124e);
            f04b16(0x9f, g_124a);
        } else if (g_0cbe || g_0cc0) {
            f04b16(0x9f, g_1250);
            f04b16(0x9f, g_124a);
        } else {
            *g_124c = g_1180;
            f04b16(0x9f, g_124c);
        }
        f04b16(0x9f, g_1248);
        f04b16(0x9d, g_3a0a);
    }
    g_2f88 = f04b4f("");
    if (g_0c6c && !strcmp(g_0c6c, "_DATA"))
        g_0c6c = 0;
    g_0c6a = g_0c6c ? g_0c6c : "DGROUP";
    g_1244->idx = g_1184++;
    g_1244->nameidx = f04b4f(g_0c6a);
    if (g_123e) {
        g_1246->idx = g_1184++;
        g_1246->nameidx = f04b4f(g_0c7a);
    }
    g_11bc->len = g_0cb4;
    g_11bc->nameidx = f04b4f(g_11bc->name);
    g_11bc->clsidx = f04b4f(g_11bc->cls);
    g_11be->len = g_0cb0;
    g_11be->nameidx = f04b4f(g_11be->name);
    g_11be->clsidx = f04b4f(g_11be->cls);
    if (g_0c6c) {
        f02cf4(tname, "%s%s", g_0c6c, "_CONST");
        g_11c0->name = tname;
        f02cf4(b2, "%s%s", g_0c6c, "_BSS");
        g_11c2->name = b2;
    }
    g_11c0->len = g_0cac;
    g_11c0->nameidx = f04b4f(g_11c0->name);
    g_11c0->clsidx = g_11c0->nameidx;
    g_11c2->len = g_0ca4;
    g_11c2->nameidx = f04b4f(g_11c2->name);
    g_11c2->clsidx = f04b4f(g_11c2->cls);
    if (g_11ba || g_0c6c)
        far = f04b4f("FAR_DATA");
    if (g_0c6c) {
        g_11be->clsidx = far;
        g_11c0->clsidx = far;
        g_11c2->clsidx = far;
    }
    for (sp = g_11b8; sp; sp = sp->next)
        if (sp->num > 4) {
            sp->nameidx = f04b4f(sp->name);
            sp->cls = "FAR_DATA";
            sp->clsidx = far;
            sp->combine = 2;
            sp->align = 2;
        }
    for (sp = g_11b8; sp; sp = sp->next) {
        f04b73(sp);
        if (sp == g_11bc)
            g_1338 = ftell(g_0c5a);
    }
    f04c10(g_11c0, g_1244);
    f04c10(g_11c2, g_1244);
    f04c10(g_11be, g_1244);
    if (g_123e)
        f04c10(g_11bc, g_1246);
    g_2f8c = f04f1d(0, 0, g_11c0->num, g_1190++);
    g_2f8a = f04f1d(0, 0, g_11be->num, g_1190++);
    g_2f92 = f04f1d(0, 0, g_11bc->num, g_1190++);
    g_2f98 = f04f1d(0, 0, g_11c2->num, g_1190++);
    g_2f86 = f04f1d(0x40, 0, g_11bc->num, g_118e++);
    g_2f9a = f04f1d(0x40, 1, g_1244->idx, g_118e++);
    if (g_123e)
        g_2f84 = f04f1d(0x40, 1, g_1246->idx, g_118e++);
    f058d4();
    for (i = 0; i < 100; i++) {
        register struct cell *cl;

        if (!(cl = f03f02(0x14, 0, 0)))
            f03dad();
        !cl->next;
        f04023(cl);
    }
    f046e3();
}
