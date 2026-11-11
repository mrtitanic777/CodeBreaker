extern int g_05ec;
extern char *g_05ee[];
extern char *f0291d();
extern f01d6e(), f00143();

f028ca(name)
char *name;
{
    register int i, n;
    char *file;

    n = g_05ec;
    for (i = 19; i >= n; i--) {
        file = f0291d(g_05ee[i], name);
        if (f01d6e(file, 0))
            return 0x16f;
    }
    f00143("cannot find `%s'", name);
    return 0x16f;
}
