#include "targ.h"
extern char *f0475f();
extern printf();

/* the PUBLIC line of the symbol s, if it is one */
f02ad6(s)
register struct targ *s;
{
    if (s->kind == 2 && s->t10 == 5 || !s->kind && (s->flags & 0xf00) == 0x100 && (s->flags & 0xf0) != 0x60)
        printf("PUBLIC  %s\n", f0475f(s));
}
