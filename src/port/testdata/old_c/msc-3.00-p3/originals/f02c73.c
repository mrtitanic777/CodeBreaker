extern char *g_0c70;
extern int g_0c8a;
extern f02cbf(), f02d08();

f02c73(prefix, fmt, args)
char *prefix, *fmt;
int *args;
{
    char buf[100];

    f02cbf("%s(%d) : ", g_0c70, g_0c8a);
    if (prefix)
        f02cbf(prefix);
    f02d08(buf, fmt, args);
    f02cbf(buf);
    f02cbf("\n");
}
