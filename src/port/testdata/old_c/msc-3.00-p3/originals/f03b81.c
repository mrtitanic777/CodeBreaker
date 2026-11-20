#include <stdio.h>
#include "outf.h"
extern int g_0c58;              /* (the source file) */
extern int g_0ee6;              /* (its line read next) */
extern f03c69(), fseek(), printf();

/* the source up to its line n in the listing, as comments */
f03b81(n)
register n;
{
    int x, y;                   /* (not used) */

    if (g_0ee6 - 1 > n) {
        fseek(g_0c58, 0L, 0);
        g_0ee6 = 1;
        do
            if (f03c69(g_0c58) == '\n')
                g_0ee6++;
        while (g_0ee6 < n);
        printf(";|*** ");
    } else {
        printf(";|*** ");
        while (g_0ee6 < n)
            if (PUTOUT(f03c69(g_0c58)) == '\n') {
                g_0ee6++;
                printf(";|*** ");
            }
    }
    while (PUTOUT(f03c69(g_0c58)) != '\n')
        ;
    g_0ee6++;
}
