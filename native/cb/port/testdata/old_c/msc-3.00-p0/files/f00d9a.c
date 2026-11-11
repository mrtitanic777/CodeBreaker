#include <stdio.h>
#include "getflags.h"
extern intptr_t g_0644; extern int g_3770,  g_05d8, g_05da, g_05d4, g_09d6;
extern char **g_3b74;
extern char *g_05d2, *g_0642;
extern struct flagdef g_0664[];
extern struct slots g_0616;
extern FILE *g_05e8, *g_0232;
extern char *f00d74(), *f00ba5();
extern f0030c(), f00143(), f01eaf(), f01bc7(), f00e5c(), f00c14();

main(argc, argv)
int argc;
char **argv;
{
    g_3770 = argc;
    g_3b74 = argv;
    while (f0030c(g_0664, f00d74(), f00d74, 0))
        ;
    if (g_0644)
        f00143("bad flag = %s\n", g_0644);
    if ((g_05d4 = g_05d8 || g_05da) == 0)
        g_05e8 = f00ba5(g_05d2, "pr", "w", "token file");
    g_0232 = g_05e8;
    if (g_0642)
        f01eaf(g_0642, &g_0616);
    else
        f00143("no input file specified");
    if (g_05d4)
        f01bc7();
    else
        f00e5c();
    f00c14(g_0232);
    exit(g_09d6);
}
