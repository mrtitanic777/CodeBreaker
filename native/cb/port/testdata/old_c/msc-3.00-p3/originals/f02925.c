#include "seg.h"
extern char *g_0c76;            /* (the source's name) */
extern char g_3a0a[];           /* (the processor options) */
extern struct seg *g_11bc, *g_11c0, *g_11c2, *g_11be;  /* (_TEXT, _DATA, CONST, _BSS) */
extern char *g_0c6a;            /* (the data's group) */
extern char *g_0c6c;
extern int g_123e;              /* (a code group) */
extern char *g_0c7a, *g_0c7c;
extern char g_0ca0;
extern int g_1dbc, g_07c8;
extern f02ad6(), f02b1c(), f02b75(), f06988(), f028ab(), f022d9(), f01840(), printf();

/* the listing's head (its title, segments, groups and ASSUME), the data and the variables' room */
f02925()
{
    char *s;

    printf("\tTITLE   %s\n\n", g_0c76);
    for (s = g_3a0a; *s; s++)
        if (*s == '2')
            printf("\t.286p\n");
    printf("%s\tSEGMENT  BYTE PUBLIC '%s'\n", g_11bc->name, g_11bc->cls);
    printf("%s\tENDS\n", g_11bc->name);
    printf("%s\tSEGMENT  WORD PUBLIC '%s'\n", g_11c0->name, g_11c0->cls);
    printf("%s\tENDS\n", g_11c0->name);
    printf("%s\tSEGMENT  WORD PUBLIC '%s'\n", g_11c2->name, g_11c2->cls);
    printf("%s\tENDS\n", g_11c2->name);
    printf("%s\tSEGMENT  WORD PUBLIC '%s'\n", g_11be->name, g_11be->cls);
    printf("%s\tENDS\n", g_11be->name);
    g_0c6a = "DGROUP";
    printf("%s\tGROUP\t%s,\t%s", g_0c6a, g_11c0->name, g_11c2->name);
    if (!g_0c6c)
        printf(",\t%s", g_11be->name);
    printf("\n");
    if (g_123e)
        printf("%s\tGROUP\t%s\n", g_0c7a, g_0c7c);
    printf("\tASSUME  CS: %s, DS: %s, SS: %s, ES: %s\n", g_123e ? g_0c7a : g_0c7c, g_0c6c ? g_0c6c : g_0c6a, g_0c6a,
        g_0c6a);
    f06988(f02ad6, 0x3a20);
    f06988(f02b1c, 0x3a20);
    if (g_0ca0)
        f02b75(&g_1dbc);
    if (g_07c8)
        printf("%s\tENDS\n", g_11be->name);
    f028ab();
    f022d9();
    f01840(1);
}
