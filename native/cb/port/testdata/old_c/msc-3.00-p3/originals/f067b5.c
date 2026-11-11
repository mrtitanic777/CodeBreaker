#include "hsym.h"

/* the symbol numbered num in the table tab, 0 if none */
struct hsym *
f067b5(num, tab)
register num;
struct hsym **tab;
{
    register struct hsym *p;

    for (p = tab[num % 64]; ; p = p->next)
        if (!p || p->num == num)
            return p;
}
