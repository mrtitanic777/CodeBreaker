#include "opnd.h"
extern f0854c();

/* the operands a (of the kind ka) and b (of kb) the same, a constant that is odd taken one less to match a
   register's half */
f084cf(a, ka, b, kb)
struct opnd *a;
struct opnd *b;
{
    struct opnd *cp;
    long save;
    int rv;

    cp = 0;
    if (ka == 2 && kb == 1)
        cp = b;
    else if (ka == 1 && kb == 2)
        cp = a;
    if (cp) {
        save = cp->val;
        if (cp->val & 1)
            cp->val--;
    }
    rv = f0854c(a, b);
    if (cp)
        cp->val = save;
    return rv;
}
