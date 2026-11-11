extern int g_09d6;
extern f0018c(), f00143();

f0010e(fmt, args)
char *fmt;
{
    f0018c(0, fmt, &args);
    if (++g_09d6 > 100)
        f00143("Error count exceeds %d; stopping compilation", 100);
}
