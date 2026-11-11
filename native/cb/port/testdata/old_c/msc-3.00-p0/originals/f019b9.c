#include "tokval.h"
extern int g_091e, g_05d4;
extern long g_091a;
extern char *g_2e00;
extern int (*g_09da)(), (*g_09dc)();
extern f03a28(), f01555(), f03784(), f0363f(), f01b28(), f01989(), f019a0();

f019b9(want)
int want;
{
    register int c;
    register char *p;
    union tokval v;
    int (*fnextc)(), (*putback)();     /* (their places in the frame follow from the names) */

    if (g_091e == 0x16f) {
        c = f03a28();
        (*g_09dc)(c);
        if (c == '\n')
            return 0;
        g_091e = f01555(&v);
        p = v.s;
        if (!g_05d4 && (g_091e == 0x103 || g_091e == 0x104)) {
            g_091e = 0x103;
            g_091a = v.l;
        } else if (g_091e == 0x103) {
            if (*p == '\'') {
                p++;
                if (*p == '\\') {
                    p++;
                    fnextc = g_09da;
                    putback = g_09dc;
                    g_09da = f01989;
                    g_09dc = f019a0;
                    g_2e00 = p;
                    g_05d4 = !g_05d4;
                    g_091a = f03784();
                    g_05d4 = !g_05d4;
                    g_09da = fnextc;
                    g_09dc = putback;
                } else if (*p == '\'')
                    g_091a = 0;
                else
                    g_091a = *p;
            } else
                g_091a = f0363f(p, 10);
        } else if (g_091e == 0x105) {
            p += 2;
            g_091a = f0363f(p, 16);
            g_091e = 0x103;
        } else if (g_091e == 0x106) {
            p++;
            g_091a = f0363f(p, 8);
            g_091e = 0x103;
        } else if (g_091e == 0x104) {
            g_091a = f0363f(p, 10);
            g_091e = 0x103;
        } else if (g_091e == 0x101) {
            g_091a = f01b28(p);
            g_091e = 0x103;
        }
    }
    if (want != g_091e)
        return 0;
    g_091e = 0x16f;
    return 1;
}
