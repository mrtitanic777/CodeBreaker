/* MFLAGS: -Oat -Gs -FPi */
#include <stdio.h>
extern FILE *g_0954;
extern double _fac;              /* (the library's floating accumulator: the number read) */
extern fread();

/* (open: the original's fwait comes after the "mov ax, &_fac", release 3.00's before it) */

/* a float read from the file: the double it is */
double *
f0325c()
{
    float f;

    fread(&f, 4, 1, g_0954);
    return (_fac = f, &_fac);
}
