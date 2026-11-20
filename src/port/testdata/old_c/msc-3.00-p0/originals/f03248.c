#include "tokval.h"
extern int g_05d4, g_09d8;
extern char *g_09d4;
extern char g_3042[];
extern char *f03a85();

/* an identifier that may be __LINE__ or __FILE__ */
f03248(v)
union tokval *v;
{
    register char *s;

    s = v->s;
    if (*s == '_') {
        if (!strcmp(s, "__LINE__")) {
            if (g_05d4) {
                sprintf(g_3042, "%d", g_09d8);
                v->str.n = strlen(g_3042) + 1;
                v->str.f4 = 0;
                v->s = f03a85(g_3042, v->str.n, 3);
            } else
                v->l = g_09d8;
            return 0x103;
        }
        if (!strcmp(s, "__FILE__")) {
            v->str.n = strlen(g_09d4) + 1;
            v->str.f4 = 0;
            v->s = f03a85(g_09d4, v->str.n, 0);
            return 0x108;
        }
    }
    return 0x101;
}
