#include <stdio.h>
#include "rec.h"
#include "outf.h"
#include "cell.h"
extern FILE *g_0c5a;   /* (the object file) */
extern struct cell *g_11ae;     /* (the fixups waiting for the record) */
extern char *g_123c;
extern f04127(), f040db(), f04023(), f017cb();

/* the record put out: its length, its checksum; its fixups after it */
f04a37()
{
    register n;
    register char *p;
    char cs;
    int u;                      /* (not used) */
    struct cell *l;

    if (g_3368 < 0)
        return;
    n = g_3368;
    g_3368 = 1;
    f04127(n - 2);
    g_3368 = n;
    cs = 0;
    while (n--)
        cs += g_360e[n];
    g_360e[g_3368] = -cs;
    p = g_360e;
    do
        putc((unsigned char)*p++, g_0c5a);
    while (g_3368--);
    if (g_11ae) {
        if (g_360e[0] != (char)0xa0)
            f017cb(g_123c, 433);
        l = g_11ae;
        g_11ae = 0;
        f040db(l);
        f04023(l);
        f04a37();
    }
}
