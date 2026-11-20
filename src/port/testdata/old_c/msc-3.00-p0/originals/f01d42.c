#include <stdio.h>
extern FILE *g_0a44;
extern int g_09d8;
extern f01c46();

f01d42(c)
int c;
{
    if (c != EOF)
        ungetc(c, g_0a44);
    if (c == '\n') {
        g_09d8--;
        f01c46();
    }
}
