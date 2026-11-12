/* MFLAGS: -Oat -FPi */
#include <stdio.h>
extern FILE *g_0234;

double
f007c2()
{
    float f;

    fread(&f, 4, 1, g_0234);
    return f;
}
