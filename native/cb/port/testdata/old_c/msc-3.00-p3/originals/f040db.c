#include "cell.h"
extern f04e6c();

/* f04e6c for each of the list p */
f040db(p)
struct cell *p;
{
    for (; p; p = p->next)
        f04e6c(p);
}
