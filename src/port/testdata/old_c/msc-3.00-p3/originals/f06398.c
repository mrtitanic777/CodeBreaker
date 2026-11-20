#include "psym.h"
#include "tflags.h"
#include "recfmt.h"
extern struct recfmt g_091a[];
extern char *g_1dba;
extern char f0308c();
extern long f03191();
extern int f067e8();
extern struct init *f06170();
extern f030b6(), f03de2(), f017cb();

/* (open: the original's "|=" on the bit field keeps the value in a temporary; release 3.00 the pointer too) */

/* the next initialized variable read from the data file: fn1 for it and its place, fn2 for each of its
   initializers; the variable */
struct psym *
f06398(fn1, fn2)
int (*fn1)(), (*fn2)();
{
    char ch;
    register struct psym *s;
    register char *f;
    long off;
    int id;
    struct tflags *f2;

    for (f = g_091a[0].fmt; *f; f++)
        switch (*f) {
        case 'd':
            id = f030b6();
            s = f067e8(id, 0x3a20);
            f2 = (struct tflags *)&s->se;
            f2->used |= 1;
            break;
        case 'm':
            break;
        case 'o':
            off = f03191();
            break;
        case 'x':
            (*fn1)(s, off);
            while ((int)(ch = f0308c()) != 7) {
                (*fn2)(f06170(ch));
                f03de2(2);
            }
            break;
        default:
            f017cb(g_1dba, 663);
            break;
        }
    return s;
}
