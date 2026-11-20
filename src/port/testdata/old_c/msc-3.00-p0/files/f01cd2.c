#include <stdio.h>
extern FILE *g_0a44;
extern intptr_t g_05d8; extern int g_09d8;
extern char *g_09d4;
extern f01c46(), f01e3e();

f01cd2()
{
    register int c;

    for (;;) {
        switch (c = getc(g_0a44)) {
        case '\n':
            g_09d8++;
            f01c46();
        default:
            return c;
        case EOF:
            break;
        }
        if (f01e3e() == EOF)
            return EOF;
        if (g_05d8)
            printf("\n#line %d %s", g_09d8 + 1, g_09d4);
    }
}
