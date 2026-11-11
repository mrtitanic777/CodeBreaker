#include "attr.h"
extern char *g_00dc;
extern f01717(), f017cb();

/* the size of the operand a's addressing: its mode byte and displacement */
f01684(a)
struct attr *a;
{
    register n;

    n = 1;
    switch (a->a0) {
    case 1:
        break;
    case 3: case 5: case 7: case 8: case 9: case 15: case 16:
        n += 2;
        break;
    case 4:
        if (!a->l4 && a->a2 == 5) {
            n++;
            break;
        }
    case 6:
        n += a->l4 ? f01717(2, a->l4) : 0;
        break;
    default:
        f017cb(g_00dc, 523);
        break;
    }
    return n;
}
