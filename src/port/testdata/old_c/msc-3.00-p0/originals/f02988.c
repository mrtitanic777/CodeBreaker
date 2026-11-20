#include "tokval.h"
extern f02acb();

f02988(v)
register union tokval *v;
{
    register int tok;

    do
        tok = f02acb(v);
    while (tok == 0x16f || tok == 0x16a);
}
