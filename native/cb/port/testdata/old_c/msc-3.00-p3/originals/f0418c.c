extern int g_3368;
extern f040fb(), f04837();

/* the n bytes at p put in records (a record ended when it would be too long) */
f0418c(p, n)
register char *p;
register n;
{
    int k;

    while (n) {
        k = n > 1000 ? 1000 : n;
        n = n - k;
        if (k + g_3368 > 1000)
            f04837();
        while (k--)
            f040fb(*p++);
    }
}
