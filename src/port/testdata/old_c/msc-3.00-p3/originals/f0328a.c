/* MFLAGS: -Oat -Gs -FPi */
#include <stdio.h>
extern FILE *g_0954;
extern double _fac;
extern fread();

/* (open: the original's fwait comes after the "mov ax, &_fac", release 3.00's before it) */

/* a double read from the file */
double *
f0328a()
{
    double d;

    fread(&d, 8, 1, g_0954);
    return (_fac = d, &_fac);
}
