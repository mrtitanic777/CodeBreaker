#include <stdio.h>
#include <ctype.h>
#include "outf.h"
extern printf();

/* the len bytes of the string s in the listing as DB lines, printable runs quoted */
f02472(s, len)
register unsigned char *s;
{
    int quoted;
    int c;
    register n;

    quoted = n = 0;
    printf("\tDB\t");
    while (len--) {
        if (n > 60) {
            if (quoted) {
                PUTOUT('\'');
                quoted = 0;
            }
            printf("\n\tDB\t");
            n = 0;
        }
        c = *s++;
        if (isprint(c) || c == ' ') {
            if (!quoted) {
                if (n) {
                    printf(", ");
                    n += 2;
                }
                PUTOUT('\'');
                quoted = 1;
                n++;
            }
            if (c == '\'') {
                PUTOUT('\'');
                n++;
            }
            PUTOUT(c);
            n++;
        } else {
            if (quoted) {
                PUTOUT('\'');
                quoted = 0;
                n++;
            }
            if (n) {
                printf(",  ");
                n += 2;
            }
            if (c < 0x10 || c > 0x9f)
                printf("0%xH", c);
            else
                printf("%xH", c);
            n += 4;
        }
    }
    if (quoted)
        PUTOUT('\'');
    PUTOUT('\n');
}
