#include <stdio.h>
#include "outf.h"
extern int g_0c9e;
extern long g_0ca8;
extern printf();

/* the n bytes at p of the code in the listing, with the place (if the listing has the code) */
f02158(n, p, jmp)
unsigned char *p;
{
    register unsigned char *b;
    register i;

    if (g_0c9e) {
        printf("\t*** %06lx\t", g_0ca8);
        b = p;
        if (jmp && (*b & 0xf0) == 0x70 && n == 5)
            n = 2;
        for (i = 0; i < n; i++)
            printf("%02x ", *b++);
        i = "\3\3\3\2\2\2\1\1"[n];      /* (the tabs after so many bytes) */
        while (i--)
            PUTOUT('\t');
    } else
        printf("\t");
}
