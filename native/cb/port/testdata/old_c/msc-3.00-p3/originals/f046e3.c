#include "targ.h"
extern int g_0954, g_0c5e, g_0c52;
extern char *g_116e[];
extern char f0308c();
extern struct psym *f06398();
extern f04559(), f03de2(), rewind(), f017cb(), f05c07();

/* the room of the initialized data: each variable's segment made as long as its end */
f046e3()
{
    char c;
    register struct targ *s;

    g_0954 = 0;
    do {
        g_0954 = !g_0954 ? g_0c5e : g_0c52;
        rewind(g_0954);
        while ((c = f0308c()) != -1) {
            if ((int)c && (int)c != 9)
                f017cb(g_116e[0], 687);
            if ((int)c != 9) {
                s = f06398(f04559, f05c07);
                f03de2(2);
            }
        }
    } while (g_0954 != g_0c52);
}
