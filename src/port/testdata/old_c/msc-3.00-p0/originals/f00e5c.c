#include "tokval.h"
extern f01555(), f00ea0(), f00f89();

f00e5c()
{
    int tok;
    union tokval val;

    for (;;) {
        tok = f01555(&val);
        if (tok == 0x16b || tok == 0x16f)
            continue;
        f00ea0(tok, &val);
        if (tok == 0x169)
            return;
        f00f89(3);
    }
}
