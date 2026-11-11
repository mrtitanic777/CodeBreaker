#include "targ.h"
#include "seg.h"
extern struct seg *g_11bc;
extern char *f04316();
extern struct seg *f040a4();
extern f04d99();

/* the PUBDEF record of the symbol s, if it is public */
f05bac(s)
register struct targ *s;
{
    if (s->kind == 2) {
        if (s->t10 == 5)
            f04d99(s, g_11bc);
        else if (s->t10 == 3) {
            s->t6 = (int)f04316(s);
            f04d99(s, g_11bc);
        }
    } else if (!s->kind && (s->flags & 0xf00) == 0x100 && (s->flags & 0xf0) != 0x60)
        f04d99(s, f040a4(s));
}
