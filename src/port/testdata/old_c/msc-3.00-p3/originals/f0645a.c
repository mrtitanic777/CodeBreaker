#include "psym.h"
#include "sflags.h"
#include "recfmt.h"
#define FL(s)   ((struct sflags *)&(s)->se)
/* what each kind of symbol has in the file */
extern struct recfmt g_2096[];
extern int g_0954;
extern struct psym g_1dea;      /* (an empty symbol) */
extern int g_0c9c, g_0c9e, g_0c56, g_336a;
extern char *g_0c76;
extern long g_0ca4, g_0cac, g_0cb0, g_0cb4;
extern char g_0ca0;
extern char *g_1dba;
extern char f0308c();
extern long f03191();
extern char *f03f79(), *f0475f();
extern f030b6(), f0310f(), f032b8(), f067f8(), f018cc(), f04591(), f017cb();
extern struct psym *f06824();

/* (NOROOM 1,2,3,4 takes it from 410 differing bytes to 227: the compiler of 3.00 was short of room here too) */
/* (open: of the cases' shared tails the original keeps others than release 3.00) */

/* the symbols read from the file f (to a kind 3): each entered (in the table of the function's or the
   globals') */
f0645a(f)
{
    int n;
    char p;
    char cls;
    char str[128];
    register char *fmt;
    register struct psym *s;
    struct psym ps;
    char v;
    char skind;
    char kd;

    g_0954 = f;
    s = &ps;
    while ((skind = f0308c()) != -1) {
        if (skind == 3)
            break;
        fmt = g_2096[skind].fmt;
        ps = g_1dea;
        for (; *fmt; fmt++)
            switch (*fmt) {
            case 'a':
                n = f0308c();
                if (skind == 2)
                    s->flags = n;
                else
                    *(char *)&s->s14 = n;
                break;
            case 'b':
                cls = f0308c();
                FL(s)->cls = cls;
                break;
            case 'c':
                v = f0308c();
                FL(s)->frame = v;
                break;
            case 'd':
            case 'e':
            case 'o':
                s->place = f03191();
                break;
            case 'f':
                kd = f0308c();
                s->se = kd;
                break;
            case 'g':
                s->s12 = f0308c();
                break;
            case 'k':
                s->s4 = f030b6();
                break;
            case 'l':
                s->cls = f0310f();
                break;
            case 'm':
                s->l1a = f03191();
                break;
            case 'N':
                f032b8(str, 0x7f);
                if (str[0])
                    s->s6 = (int)f03f79(str, 0);
                if ((g_0c9c || g_0c9e) && str[0] && s->s3 && !f067f8(s->s4))
                    f018cc(f0475f(s), str);
                break;
            case 'n':
                s->s3 = f0308c();
                break;
            case 'p':
                p = f0308c();
                FL(s)->lo = p;
                break;
            case 'r':
                *(long *)&s->s12 = f03191();
                break;
            case 's':
                s->l16 = f03191();
                break;
            case 't':
                s->kind = skind;
                break;
            case 'u':
                *(char *)&s->se = f0308c();
                break;
            case 'v':
                v = f0308c();
                s->cls = v;
                break;
            case 'B':
                g_0ca4 = f03191();
                break;
            case 'C':
                g_0cac = f03191();
                break;
            case 'D':
                g_0cb0 = f03191();
                break;
            case 'F':
                g_0ca0 = f0308c();
                break;
            case 'K':
                g_336a = f030b6();
                break;
            case 'M':
                f032b8(str, 0x80);
                if (!g_0c76)
                    g_0c76 = f03f79(str, 0);
                break;
            case 'S':
                f04591();
                break;
            case 'T':
                g_0cb4 = f03191();
                break;
            default:
                f017cb(g_1dba, 879);
                break;
            }
        if (skind == 3)
            f017cb(g_1dba, 883);
        if (skind != 4)
            f06824(s, f == g_0c56 ? 0x3572 : 0x3a20);
    }
}
