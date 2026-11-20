#include "tokval.h"
extern f02acb(), f011b3(), f00c74(), f00143();

/* (open: the original's third pass did not turn this loop - its test stays at the top; release 3.00's does) */
f01555(v)
register union tokval *v;
{
    register int x;
    int tok;
    char *name;

    for (x = 0; x < 1000; x++) {
        if ((tok = f02acb(v)) != 0x101)
            return tok;
        name = v->s;
        if (!f011b3(name))
            return f00c74(name);
    }
    f00143("recursively defined macro `%s'", name);
}
