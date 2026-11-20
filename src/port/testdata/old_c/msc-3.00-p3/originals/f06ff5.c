#include "opnd.h"
#include "ins.h"
#include "lab.h"
extern char *f03f02();
extern f03dad();

/* (open: the original reads r->next through BX, release 3.00 through DI) */

/* the jump p entered as a reference to the label lab */
f06ff5(p, lab)
struct ins *p;
register struct lab *lab;
{
    register struct ref *r;

    if (g_2fb8) {
        r = g_2fb8;
        g_2fb8 = r->next;
    } else if (!(r = f03f02(4, 0, 3)))
        f03dad();
    r->next = lab->refs;
    lab->refs = r;
    r->ins = p;
}
