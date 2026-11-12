f00ff1(p, n)
register long *p;
register int n;
{
    long zero;

    zero = 0;
    if ((n >>= 3) > 0)
        do {
            *p++ = zero;
            *p++ = zero;
        } while (--n > 0);
}
