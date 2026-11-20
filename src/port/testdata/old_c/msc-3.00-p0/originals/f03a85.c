extern char *f01033();

char *
f03a85(s, n, pool)
register char *s;
register int n;
int pool;
{
    char *p, *q;

    q = p = f01033(pool, n);
    while (n--)
        *q++ = *s++;
    return p;
}
