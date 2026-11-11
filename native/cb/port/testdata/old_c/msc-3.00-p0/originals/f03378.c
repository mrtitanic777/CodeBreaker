#include <ctype.h>
#include "tokval.h"
extern int g_05d4, g_0e56, g_0f06;
extern int (*g_09da)(), (*g_09dc)();
extern char g_3270[];
extern long f0363f();
extern char *f03a85();
extern f0350b(), f0010e(), f03710(), f036f5();

/* (open: the original loads AX from DI again at the first loop's body; release 3.00 knows they are equal) */
/* the digits of a number in the base, after its first character (and 0x or 0): its token */
f03378(base, v)
int base;
union tokval *v;
{
    register char *p;
    register int c;
    int x4, x3;                 /* (not used) */
    int tok;

    tok = 0x103;
    p = g_3270;
    if (base == 10) {
        while (isdigit(c = (*g_09da)()))
            *p++ = c;
        if (c == '.' || c == 'E' || c == 'e') {
            (*g_09dc)(c);
            *p = 0;
            return f0350b(g_3270, v);
        }
    } else if (base == 16) {
        if (g_05d4) {
            *p++ = '0';
            *p++ = 'x';
        }
        while (isdigit(c = (*g_09da)()) || isxdigit(c))
            *p++ = c;
    } else if (base == 8) {
        if (g_05d4)
            *p++ = '0';
        while (isdigit(c = (*g_09da)())) {
            if (c > '7')
                f0010e("bad octal number `%c'", c);
            *p++ = c;
        }
    }
    if (!g_0e56 && (c == 'L' || c == 'l')) {
        tok = 0x104;
        if (g_05d4)
            *p++ = c;
    } else
        (*g_09dc)(c);
    *p = 0;
    if (g_05d4) {
        v->str.n = strlen(g_3270) + 1;
        v->str.f4 = 0;
        v->s = f03a85(g_3270, v->str.n, 3);
        if (base == 16)
            tok = 0x105;
        else if (base == 8)
            tok = 0x106;
    } else {
        v->l = f0363f(g_3270, base);
        if (g_0f06)
            tok = 0x104;
        else if (tok != 0x104)
            tok = base == 10 ? f03710(v->l) : f036f5(v->l);
    }
    return tok;
}
