#include "targ.h"
extern char *f04316();
extern f04c3e(), f04d5d();

/* the external records of the symbol s: EXTDEF (and COMDEF) as it needs */
f0582d(s)
register struct targ *s;
{
    int save;
    int ext;
    int u;                      /* (not used) */

    switch (s->kind) {
    case 2:
        ext = s->t10 == 4;
        if (s->t10 == 3) {
            save = s->t6;
            s->t6 = (int)f04316(s);
            f04d5d(s);
            s->t6 = save;
        } else if (s->t10 == 5)
            f04d5d(s);
        break;
    case 0:
        ext = (s->flags >> 8 & 0xf) == 3 || (s->flags >> 8 & 0xf) == 2;
        if ((s->flags & 0xf00) == 0x300)
            f04c3e(s);
        break;
    default:
        return;
    }
    if (ext)
        f04d5d(s);
}
