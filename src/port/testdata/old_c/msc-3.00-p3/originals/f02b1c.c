#include "targ.h"
#include "seg.h"
extern struct seg *g_11be;
extern int g_07c8;              /* (the listing is in _BSS for the EXTRNs) */
extern f02b75(), printf();

/* the EXTRN line of the symbol s, if it is one */
f02b1c(s)
register struct targ *s;
{
    if (!g_07c8) {
        printf("%s\tSEGMENT\n", g_11be->name);
        g_07c8 = 1;
    }
    if (s->kind == 2 && s->t10 == 4 || !s->kind && ((s->flags & 0xf00) == 0x300 || (s->flags & 0xf00) == 0x200))
        f02b75(s);
}
