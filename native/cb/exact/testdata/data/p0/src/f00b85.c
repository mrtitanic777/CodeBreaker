#include <stdio.h>
extern f00143();

f00b85(ok)
int ok;
{
    if (!ok)
        f00143("string too big for buffer\n", stderr);
}
