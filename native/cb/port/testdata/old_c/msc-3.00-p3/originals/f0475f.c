#include "targ.h"
extern char g_2f04[];           /* (the name made) */
extern char g_0308;
extern int g_0c94;              /* (names as they are) */
extern int g_0c8e;              /* (the names' length, if cut) */
extern char *f036d0();
extern f04805(), strcpy();

/* the name of the symbol s as the assembler and the object have it */
char *
f0475f(s)
register struct targ *s;
{
    register char *p;

    p = g_2f04;
    *p = 0;
    if (!s->t3) {
        *p++ = '_';
        *p++ = '_';
        strcpy(p, s->t6);
        if (s->kind == 2) {
            if (s->t10 == 3)
                *--p = ' ';
            else
                switch ((char)s->flags) {
                case 1:
                    p -= 2;
                    break;
                case 2:
                    break;
                case 3:
                    f04805(p);
                    break;
                default:
                    p--;
                    break;
                }
        } else if (s->kind == 1) {
            if (*p != g_0308)
                *--p = g_0308;
        } else if (g_0c94 || s->t14 & 1)
            f04805(p);
        else
            p--;
    } else
        f036d0(p, s->t4, s->t3, s->t6);
    if (g_0c8e)
        p[g_0c8e] = 0;
    return p;
}
