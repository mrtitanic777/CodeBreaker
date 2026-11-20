#include "hsym.h"
extern f02cbf();

/* the table tab's chains shown (to debug) */
f069b8(tab)
struct hsym **tab;
{
    register i;
    register struct hsym *p;

    for (i = 0; i < 64; i++) {
        p = tab[i];
        f02cbf("%d : 0x%x : ", i, p);
        for (; p; p = p->next)
            f02cbf("K=%d ", p->num);
        f02cbf("\n");
    }
}
