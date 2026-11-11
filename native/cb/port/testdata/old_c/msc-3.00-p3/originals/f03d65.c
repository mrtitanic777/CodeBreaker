#include "opnd.h"
#include "ins.h"
extern int g_0954;
extern struct ins g_0cc6;       /* (the item read when it is not kept) */
extern f0308c(), f03dad(), f06028();
extern struct ins *f06a04();

/* the next item of the file f: a new one if keep, else read into g_0cc6; 0 at the end */
struct ins *
f03d65(f, keep)
{
    int c;
    register struct ins *p;

    p = 0;
    g_0954 = f;
    if ((c = f0308c()) != -1) {
        if (!keep)
            p = &g_0cc6;
        else if (!(p = f06a04(c)))
            f03dad();
        f06028(p, c, f);
    }
    return p;
}
