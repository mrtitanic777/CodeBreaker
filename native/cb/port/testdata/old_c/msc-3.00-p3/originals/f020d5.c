#include "opnd.h"
extern char *f03dbb();
extern printf();

/* the based operand p in the listing: [base+disp][index] */
f020d5(p)
register struct opnd *p;
{
    printf("[%s", f03dbb(p->size, 2));
    if (p->val)
        printf("%s%ld", p->val > 0 ? "+" : "", p->val);
    printf("]");
    if (p->kind == 6 || p->kind == 7)
        printf("[%s]", f03dbb(p->o12, 2));
}
