#include "psym.h"
extern char *g_0306;
extern char *f0475f();
extern f017cb(), printf();

/* the EXTRN line of the symbol s: its size or its distance */
f02b75(s)
register struct psym *s;
{
    char *t;

    printf("EXTRN\t%s", f0475f(s));
    switch (s->kind) {
    default:
        f017cb(g_0306, 1185);
        break;
    case 2:
        printf(":%s\n", s->flags & 4 ? "FAR" : "NEAR");
        break;
    case 0:
        switch (s->cls) {
        case 1:
        default:
            t = "BYTE";
            break;
        case 2:
            t = "WORD";
            break;
        case 4:
            t = "DWORD";
            break;
        case 8:
            t = "QWORD";
            break;
        }
        printf(":%s\n", t);
        break;
    }
}
