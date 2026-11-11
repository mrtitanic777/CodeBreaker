#include "opnd.h"
#include "recfmt.h"
/* what each kind of operand has in the file, its size */
extern struct recfmt g_1010[];
extern int g_0954;
extern struct opnd g_1ddc;      /* (an empty operand) */
extern char *g_1dba;
extern char f0308c();
extern long f03191();
extern struct opnd *f03e86();
/* (open: the original puts the upper half of the switch's compare tree after the cases, falling into the
   default; release 3.00 puts both halves before them) */
extern f03dad(), f00057(), f030b6(), f0006d(), f067f8(), f032ec(), f00002(), f017cb();

/* the next operand read from the file f, into p (a new one if 0) */
struct opnd *
f05e92(f, p)
register f;
register struct opnd *p;
{
    char kind;
    char *fmt;
    int x;

    g_0954 = f;
    kind = f0308c();
    if (p)
        *p = g_1ddc;
    else if (!(p = f03e86(2, g_1010[kind].n)))
        f03dad();
    for (fmt = g_1010[kind].fmt; *fmt; fmt++)
        switch (*fmt) {
        case 'b':
        case 'r':
            p->size = f0308c();
            break;
        case 'd':
        case 'o':
        case 'v':
            p->val = f03191();
            break;
        case 'g':
            p->o2 = f0308c();
            break;
        case 'i':
            p->o12 = f0308c();
            break;
        case 'm':
            p->kind = f00057(kind);
            break;
        case 's':
            x = f030b6();
            f0006d(f067f8(x), p, kind);
            break;
        case 'w':
            p->o13 = f0308c();
            break;
        case '#':
            if ((p->size = f032ec(&p->val, 10)) > 10)
                f017cb(g_1dba, 226);
            break;
        default:
            f017cb(g_1dba, 231);
            break;
        }
    f00002(kind, p);
    return p;
}
