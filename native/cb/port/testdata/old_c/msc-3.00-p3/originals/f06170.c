/* MFLAGS: -Oat -Gs -FPi */
/* NOROOM: 1,2 */
#include "init.h"
#include "recfmt.h"
/* what each kind of initializer has in the file, its size */
extern struct recfmt g_091a[];
extern char *g_1dba;
extern char f0308c();
extern long f03191();
extern double *f0325c(), *f0328a();
extern char *f03f02();
extern struct opnd *f03e86();
extern f03dad(), f032ec(), f03fc9(), f0310f(), f030b6(), f067f8(), f017cb();

/* (NOROOM: the compiler of 3.00 had no room for the first two mergings of equal code here; they waited and came
   out the other way round) */

/* the next initializer of the kind kind read from the data file (a repetition: the items in it after it) */
struct init *
f06170(kind)
char kind;
{
    char c;
    int n;
    struct init *r;
    char s[0x202];
    char bits;
    int id;
    register struct init *init;
    register char *fmt;

    if (!(init = f03f02(g_091a[kind].n, 0, 2)))
        f03dad();
    for (fmt = g_091a[kind].fmt; *fmt; fmt++)
        switch (*fmt) {
        case 'a':
            ((char *)init)[0xa] = f0308c();
            break;
        case 'i':
            *(char *)&init->w4 = f0308c();
            break;
        case 'l':
            n = init->w4 = f032ec(s, 0x201);
            if (!(init->v.w = (int)f03e86(2, n)))
                f03dad();
            f03fc9(init->v.w, s, n);
            break;
        case 'n':
            *(long *)&init->w4 = f03191();
            break;
        case 'p':
            c = f0308c();
            *(char *)&init->w4 = (int)c == 1 ? 0x10 : (int)c == 2 ? 0x20 : (int)c == 3 ? 0x40 : 0x30;
            break;
        case 'r':
            init->w4 = f0310f();
            break;
        case 's':
            id = f030b6();
            init->w4 = f067f8(id);
            break;
        case 't':
            init->kind = kind;
            break;
        case 'f':
            init->v.l = f03191();
            break;
        case 'v':
            switch ((char)init->w4) {
            case 0x11:
            case 0x21:
                init->v.c = f0308c();
                break;
            case 0x14:
            case 0x24:
            case 0x44:
                init->v.l = f03191();
                break;
            case 0x34:
                init->v.f = *f0325c();
                break;
            case 0x38:
                init->v.d = *f0328a();
                break;
            case 0x12:
            case 0x22:
            case 0x42:
                init->v.w = f0310f();
                break;
            default:
                f017cb(g_1dba, 555);
                break;
            }
            break;
        case 'z':
            bits = f0308c();
            if ((int)kind == 2)
                ((char *)init)[0xb] |= bits;
            else
                *(char *)&init->w4 |= bits;
            break;
        default:
            f017cb(g_1dba, 571);
            break;
        }
    r = init;
    if ((int)kind == 5)
        while ((f0308c() == kind) != 6)         /* (so in the original: it never stops this way) */
            init = init->next = f06170(kind);
    return r;
}
