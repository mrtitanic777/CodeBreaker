#include "macro.h"
extern char g_3270[];
extern char *g_0828;
extern int (*g_09da)(), (*g_09dc)();
extern char *f01485();
extern f00081(), f014bb(), f014fb(), f0153c();

f01406(t, args, nargs)
register struct btok *t;
char *args;
int nargs;
{
    register int n;

    g_3270[0] = 0;
    for (; t; t = t->next)
        if (t->kind == 0x17a) {
            n = t->argno;
            if (nargs > n)
                strcat(g_3270, f01485(n, args));
        } else if (t->kind == 0x108)
            strcat(g_3270, t->text);
        else
            f00081(g_0828, 224);
    f014bb(g_3270);
    if (g_09da != f014fb) {
        g_09da = f014fb;
        g_09dc = f0153c;
    }
}
