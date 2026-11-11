#include "recfmt.h"
extern struct recfmt g_0074[];
extern f03f41();

/* whether there is room for an item of the kind kind (and its operands) */
f06a6f(kind)
char kind;
{
    int ex;
    int size;

    size = g_0074[(unsigned char)kind].n;
    ex = 0;
    switch ((unsigned char)kind) {
    case 0:
        ex = 4;
        size += 0x1c;
        break;
    case 8:
        size += 0x80;
        break;
    case 7:
        ex = 8;
        break;
    }
    if (!f03f41(size, 2))
        return 0;
    return f03f41(ex, 3);
}
