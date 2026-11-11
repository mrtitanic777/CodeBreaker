#include "opnd.h"
#include "recfmt.h"
extern struct recfmt g_1010[];
extern struct opnd *f03e86();

/* a new operand of the kind kind */
struct opnd *
f06e7b(kind)
{
    register struct opnd *p;

    if (p = f03e86(2, g_1010[kind].n))
        p->kind = kind;
    return p;
}
