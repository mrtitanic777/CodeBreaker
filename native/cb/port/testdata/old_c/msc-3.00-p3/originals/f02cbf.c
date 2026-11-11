#include <stdio.h>
extern f02d08();

f02cbf(fmt, args)
char *fmt;
{
    char buf[82];

    f02d08(buf, fmt, &args);
    fwrite(buf, strlen(buf), 1, stderr);
}
