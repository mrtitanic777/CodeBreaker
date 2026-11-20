#include "macro.h"
extern int g_25fc;
extern char *g_25fe;
extern struct arg *g_3772, *g_3b76;
extern int (*g_09dc)();
extern char *f01033();
extern f03a28(), f01fd0(), f039e2(), f0391c(), f0010e();

extern char g_2600[];
#define AEND    (g_2600 + 0x800)

f0129a()
{
    register int c;
    register char *p;
    int depth;
    int c2;

    if ((c = f03a28()) == ')')
        return 0;
    depth = 0;
    g_25fc++;
    if (!g_3772)
        g_3772 = g_3b76 = (struct arg *)f01033(3, 4);
    else
        g_3b76 = g_3b76->next = (struct arg *)f01033(3, 4);
    p = g_3b76->text = g_25fe;
    for (;;) {
        if (c == ',' && depth == 0)
            break;
        switch (c) {
        case '"':
        case '\'':
            *p++ = c;
            for (;;) {
                if ((c2 = f01fd0()) == c)
                    break;
                if (c2 == '\\') {
                    if (f0391c('\n'))
                        continue;
                    *p++ = c2;
                    c2 = f01fd0();
                } else if (c2 == '\n') {
                    f0010e("newline in constant");
                    break;
                }
                *p++ = c2;
                if (p == AEND) {
                    f0010e("out of macro actual parameter space");
                    goto full;
                }
            }
            break;
        case '(':
            depth++;
            break;
        case ')':
            if (depth == 0)
                goto done;
            depth--;
            break;
        case '/':
            if (f039e2(c)) {
                c = f01fd0();
                continue;
            }
            break;
        case '\\':
            if (!f0391c('\n'))
                *p++ = c;
            c = f01fd0();
            break;
        }
        *p++ = c;
        if (p == AEND) {
            f0010e("out of macro actual parameter space");
full:
            *--p = 0;
            return 0;
        }
        c = f01fd0();
    }
done:
    if (c == ')')
        (*g_09dc)(c);
    *p = 0;
    g_25fe += strlen(g_3b76->text) + 1;
    return 1;
}
