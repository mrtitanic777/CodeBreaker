#include <stdio.h>
extern f02c34();

f035eb(ok)
int ok;
{
    if (!ok)
        f02c34("string too big for buffer\n", stderr);
}
