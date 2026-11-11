extern int g_0c90;
extern f02c73(), f02c34();

f02c04(fmt, args)
char *fmt;
{
    f02c73(0, fmt, &args);
    if (++g_0c90 > 100)
        f02c34("Error count exceeds %d; stopping compilation", 100);
}
