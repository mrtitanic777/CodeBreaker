#include "opnd.h"
#include "ins.h"
#include "lab.h"
extern char *g_1e46;
extern f017cb();

/* (open: block motion - the original moves the loop's "not this one" step after the code that follows) */

/* the reference of the jump p to the label lab given back (all of them if p is 0) */
f07042(lab, p)
struct lab *lab;
register struct ins *p;
{
    struct ref *prev;
    register struct ref *r;

    if (!p) {
        if (!(r = prev = lab->refs))
            return;
        while (prev->next)
            prev = prev->next;
        lab->refs = 0;
    } else {
        prev = 0;
        if (!lab->refs)
            return;
        for (r = lab->refs; r; prev = r, r = r->next)
            if (r->ins == p) {
                if (prev)
                    prev->next = r->next;
                else
                    lab->refs = r->next;
                prev = r;
                break;
            }
    }
    if (!r)
        f017cb(g_1e46, 1031);
    prev->next = g_2fb8;
    g_2fb8 = r;
}
