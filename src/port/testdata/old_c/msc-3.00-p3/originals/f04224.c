extern int g_3368;
extern f040fb(), f04837();

/* n zero bytes put in records */
f04224(n)
register n;
{
    int k;

    while (n) {
        k = n > 1000 ? 1000 : n;
        n = n - k;
        if (k + g_3368 > 1000)
            f04837();
        while (k--)
            f040fb(0);
    }
}
