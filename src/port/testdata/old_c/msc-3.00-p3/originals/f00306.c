#include "opnd.h"

/* the segment register (8-0bh) p is, else 0 */
f00306(p)
struct opnd *p;
{
    if (p->kind == 1 && p->size >= 8 && p->size <= 0xb)
        return p->size;
    return 0;
}
