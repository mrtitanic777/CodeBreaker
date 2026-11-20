#include "targ.h"
extern int g_0954;              /* (the file read now) */
extern int g_0c5e, g_0c52;      /* (the files of the initialized data: near, far) */
extern int g_2666;
extern char *g_0306;
extern char f0308c();
extern struct psym *f06398();
extern f027e1(), rewind(), f017cb(), printf();

/* the initialized data in the listing, from both its files */
f028ab()
{
    char c;
    register struct targ *s;

    g_0954 = 0;
    do {
        g_0954 = !g_0954 ? g_0c5e : g_0c52;
        rewind(g_0954);
        while ((c = f0308c()) != -1) {
            if ((int)c == 9)
                continue;
            if ((int)c)
                f017cb(g_0306, 1060);
            s = f06398(f027e1, &g_2666);
            if (s->t10 & 1)
                printf("\tEVEN\n");
        }
    } while (g_0954 != g_0c52);
}
