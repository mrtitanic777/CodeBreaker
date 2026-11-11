#include <stdio.h>
extern f0022c();

f001db(fmt, args)
char *fmt;
{
    char buf[82];

    f0022c(buf, fmt, &args);
    fwrite(buf, strlen(buf), 1, stderr);
}
