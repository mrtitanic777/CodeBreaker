extern char *g_10ce;
extern char *f03e86();
extern f017cb();

char *
f03f02(n, v, pool)
int n, v, pool;
{
    char *p;

    if (n < 2)
        f017cb(g_10ce, 320);
    if ((p = f03e86(pool, n)) == 0)
        return 0;
    *(int *)p = v;
    return p;
}
