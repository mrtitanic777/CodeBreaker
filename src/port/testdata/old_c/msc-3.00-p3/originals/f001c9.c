extern char *g_262a;            /* (the record being made) */
extern int g_261e;              /* (its length) */
extern f01739();

/* the value v put in the record in n bytes (-1: one if it fits in a byte, else two); whether it took one */
f001c9(n, v)
register n;
register v;
{
    int one;

    one = 0;
    if (n == -1)
        if (one = f01739(v))
            n = 1;
        else
            n = 2;
    while (n) {
        g_262a[g_261e++] = v;
        v >>= 8;
        n--;
    }
    return one;
}
