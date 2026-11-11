#include <ctype.h>
#include "tokval.h"
extern intptr_t g_05dc; extern int  g_05d4;
extern int (*g_09da)(), (*g_09dc)();
extern char g_3370[], g_3470[];
extern char *f03a85();
extern double atof();
extern f00167(), f0010e();

/* the rest of a floating constant (after its integer part, intpart, if any): its token */
f0350b(intpart, v)
char *intpart;
union tokval *v;
{
    register int c;
    register char *p;

    p = g_3370;
    c = (*g_09da)();
    if (g_05dc) {
        f00167(1, "float constant in a cross compilation");
        g_05dc = 0;
    }
    if (isdigit(c) || c == '.')
        do {
            *p++ = c;
            c = (*g_09da)();
        } while (isdigit(c));
    if (c == 'E' || c == 'e') {
        *p++ = c;
        c = (*g_09da)();
        if (c == '+' || c == '-') {
            *p++ = c;
            c = (*g_09da)();
        }
        if (!isdigit(c)) {
            f0010e("expected exponent value, not `%c'", c);
            *p++ = '0';
        } else
            while (isdigit(c)) {
                *p++ = c;
                c = (*g_09da)();
            }
    }
    (*g_09dc)(c);
    *p = 0;
    if (intpart)
        sprintf(g_3470, "%s%s", intpart, g_3370);
    else
        sprintf(g_3470, "%s", g_3370);
    if (g_05d4) {
        v->str.n = strlen(g_3470) + 1;
        v->str.f4 = 0;
        v->s = f03a85(g_3470, v->str.n, 3);
    } else
        v->d = atof(g_3470);
    return 0x107;
}
