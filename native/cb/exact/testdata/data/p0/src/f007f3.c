/* MFLAGS: -Oat -FPi */
#include <stdio.h>
extern FILE *g_0234;

double
f007f3()
{
    double d;

    fread(&d, 8, 1, g_0234);
    return d;
}
