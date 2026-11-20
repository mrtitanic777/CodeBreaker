#include <stdio.h>
#include "psym.h"
#include "getflags.h"
/* main.c: the third pass */
#include "outf.h"
/* an item read (f03d65) */
struct item {
    struct item *next;
};
extern int g_3364;
extern char **g_356c;
extern struct flagdef g_0d04[];  /* (its options) */
extern char *g_0c7e;
extern int g_0ca2, g_0c72, g_0c62, g_0c78, g_0c74, g_0c66, g_0c82, g_0c90;
extern char *g_0c68;            /* (-il: the intermediate files' name) */
extern int g_0c9e, g_0c9c, g_0c80;
extern int g_0c58, g_0c4e, g_0c54, g_0c56, g_0c52, g_0c5e;
extern FILE *g_0c5a;
extern struct item *g_0c60;
extern char *g_0c4c;
extern long g_0ca8, g_1e4a;
extern int g_35f2[];
extern char *f037ba();
extern struct ins *f03d65();
extern f02de5(), f02c34(), f01836(), f0183b(), f04446(), f017cb(), f018be(), f06768(),
    f01756(), f053c7(), f02925(), f0674e(), f079ed(), f03ab2(), f03de2(), f05b45(), f018e0(), f03672(), exit();
extern FILE *f03606();
extern struct psym *f06adc();

main(argc, argv)
char **argv;
{
    int flag;
    int i;
    register struct item *p;

    g_3364 = argc;
    g_356c = argv;
    while (flag = f02de5(g_0d04, f037ba(), f037ba, 0))
        if (flag == 5)
            f02c34("unrecognized flag in P3 = '%s'", g_0c7e);
    if (g_0ca2)
        f01836(g_0ca2);
    if (g_0c72) {
        g_0c9e = 1;
        if (!freopen(g_0c72, "wt", stdout))
            f02c34("cannot open listing file `%s'", g_0c72);
    } else if (g_0c62) {
        g_0c9c = 1;
        if (!freopen(g_0c62, "wt", stdout))
            f02c34("cannot open assembly language output file `%s'", g_0c72);
    }
    g_0c80 = g_0c78 ? 1 : 0;
    f04446(g_0c66);
    if (g_0c74)
        if (!g_0c9c && !g_0c9e && !g_0c72)
            f017cb(g_0c4c, 251);
        else if (!(g_0c58 = fopen(g_0c74, "rt")))
            f02c34("cannot open source file: `%s'", g_0c74);
    g_0c4e = f03606(g_0c68, "pr", "r", "ail");
    g_0c54 = f03606(g_0c68, "gs", "r", "glob sil");
    if (g_0c9c || g_0c9e)
        f018be();
    f06768();
    fclose(g_0c54);
    g_0c56 = f03606(g_0c68, "ls", "r", "loc sil");
    g_0c52 = f03606(g_0c68, "in", "r", "data init");
    g_0c5e = f03606(g_0c68, "st", "r", "str init");
    if (g_0c80) {
        if (!(g_0c5a = fopen(g_0c78, "w")))
            f01756("problems opening object file %s\n", g_0c78);
        f053c7();
    }
    if (g_0c9c || g_0c9e)
        f02925();
    g_0ca8 = 0;
    while (g_0c60 = f03d65(g_0c4e, 1)) {
        f0674e(g_0c56);
        if (f06adc()) {
            if (g_0c82)
                f079ed();
            for (p = g_0c60; p; p = p->next)
                f03ab2(p);
        }
        g_1e4a = g_0ca8;
        f03de2(2);
        f03de2(3);
        for (i = 0; i <= 8; )
            g_35f2[i++] = 0;
    }
    if (g_0c80)
        f05b45();
    if (g_0c9c || g_0c9e)
        f018e0();
    if (g_0ca2)
        f0183b();
    if (g_0c80)
        f03672(g_0c5a);
    if (g_0c9c | g_0c9e)
        f03672(stdout);
    exit(g_0c90);
}
