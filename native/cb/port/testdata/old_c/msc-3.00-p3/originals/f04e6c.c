#include "fix.h"
extern f048bd(), f04127(), f040fb(), f04274();

/* the FIXUPP record of the fixup f */
f04e6c(f)
struct fix *f;
{
    char fixdat;
    int locat;

    fixdat = 0;
    f048bd(0x9c, 0);
    locat = (unsigned char)f->off << 8;
    locat |= (f->off & 0x300) >> 8;
    locat |= 0x80;
    locat |= f->seg << 6;
    locat |= f->loc5 << 5;
    locat |= f->loc << 2;
    f04127(locat);
    fixdat |= f->fthread << 7;
    fixdat |= f->frame << 4;
    fixdat |= f->tthread << 3;
    fixdat |= f->p << 2;
    fixdat += f->target;
    f040fb(fixdat);
    if (!f->tthread)
        f04274(f->idx);
}
