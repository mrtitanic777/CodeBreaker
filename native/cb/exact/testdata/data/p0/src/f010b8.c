extern char *g_06fe;
extern char *f01033();
extern f00081();

char *
f010b8(n, v, pool)
int n, v, pool;
{
    char *p;

    if (n < 2)
        f00081(g_06fe, 320);
    if ((p = f01033(pool, n)) == 0)
        return 0;
    *(int *)p = v;
    return p;
}
