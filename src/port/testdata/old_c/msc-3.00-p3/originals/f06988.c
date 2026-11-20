#include "hsym.h"

/* fn for each symbol of the table tab */
f06988(fn, tab)
int (*fn)();
struct hsym **tab;
{
    register i;
    register struct hsym *p;

    for (i = 0; i < 64; i++)
        for (p = tab[i]; p; p = p->next)
            (*fn)(p);
}
