#include "opnd.h"
#include "ins.h"
#include "lab.h"
extern char *g_1e46;
extern f07247(), f017cb();

/* every jump to the label lab made a jump to p */
f070da(p, lab)
register struct ins *p;
struct lab *lab;
{
    struct ref *next;
    register struct ref *r;

    for (r = lab->refs; r; r = next) {
        next = r->next;
        f07247(r->ins, p);
    }
    if (lab->refs)
        f017cb(g_1e46, 1080);
}
