#include <stdio.h>
extern int g_05d6;
extern f0391c(), f01fd0();

/* after a '/': a comment, skipped (echoed under -C) */
f03949()
{
    register int c;

    if (!f0391c('*'))
        return 0;
    if (g_05d6) {
        printf("/*");
        for (;;) {
            if ((c = f01fd0()) == '*' && f0391c('/')) {
                printf("*/");
                break;
            }
            putc(c, stdout);
        }
    } else
        while (!((c = f01fd0()) == '*' && f0391c('/')))
            ;
    return 1;
}
