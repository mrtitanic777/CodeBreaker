#include "psym.h"
extern char g_0ca0;
extern int g_0cc0, g_0cc2;
extern struct psym g_1dbc, g_1252;
extern struct {
    char *name;
    int x;
} g_1198[];                     /* (the floating point helpers' names) */
extern int g_3604[];            /* (their EXTDEF indexes) */
extern f04d5d(), f0582d(), f06988();

/* the EXTDEF records: the floating point ones first, then the symbols' */
f058d4()
{
    int i;
    struct psym t;
    struct psym *p;

    p = &t;
    if (g_0ca0) {
        f04d5d(&g_1dbc);
        p->cls = 0;
        p->kind = 2;
        *(char *)&p->se = 2;
        p->s3 = 0;
        p->flags = 0;
        if (g_0cc0 || g_0cc2)
            for (i = 5; i--; ) {
                p->s6 = (int)g_1198[i].name;
                f04d5d(p);
                g_3604[i] = p->s8;
            }
    }
    f04d5d(&g_1252);
    f06988(f0582d, 0x3a20);
}
