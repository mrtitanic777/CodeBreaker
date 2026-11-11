#include "opnd.h"
#include "ins.h"
#include "psym.h"
#include "recfmt.h"
/* what each kind of item has in the file */
extern struct recfmt g_0074[];
extern int g_0954, g_0c8a;
extern struct ins g_0cc6;
extern struct opnd g_2f9c, g_2faa;      /* (its operands) */
extern char *g_1dba;
extern char f0308c();
extern char *f03f79();
extern int f067e8();
/* (NOROOM 3 takes it from 189 differing bytes to 108: the compiler of 3.00 was short of room here too) */
/* (open: the original puts the upper half of the switch's compare tree after the cases; release 3.00 puts it
   before them) */
extern f05fdd(), f032b8(), f030b6(), f067f8(), f0310f(), f000d9(), f017cb();

/* the item of kind kind read from the file f into p */
f06028(p, kind, f)
register struct ins *p;
register f;
{
    int x1;                     /* (not used) */
    char tname[128];
    int x2;                     /* (not used) */
    char ch;
    char *fmt;
    int x;

    g_0954 = f;
    p->kind = kind;
    fmt = g_0074[kind].fmt;
    if (p == &g_0cc6) {
        p->a = &g_2f9c;
        p->b = &g_2faa;
    }
    for (; *fmt; fmt++)
        switch (*fmt) {
        case 'n':
            f05fdd(p, f);
            break;
        case 's':
            f032b8(tname, 128);
            p->op = (int)f03f79(tname, 2);
            break;
        case 'z':
            p->size = f0308c();
            break;
        case 'C':
            x = f030b6();
            *(int *)&p->size = f067f8(x);
            break;
        case 'N':
            g_0c8a = p->op = f0310f();
            break;
        case 'O':
            p->op = (unsigned char)(ch = f0308c());
            break;
        case 'P':
            x = f030b6();
            p->op = (int)f067e8(x, 0x3a20);
            if (((struct psym *)p->op)->cls != 1 && ((struct psym *)p->op)->cls != 3
                && ((struct psym *)p->op)->cls != 5)
                f017cb(g_1dba, 396);
            break;
        case 'L':
        case 'T':
            x = f030b6();
            p->op = f067f8(x);
            break;
        default:
            f017cb(g_1dba, 406);
            break;
        }
    f000d9(p);
}
