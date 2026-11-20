f03045(pat, s, flags)
char *pat, *s;
int flags;
{
    register char *p, *q;

    p = pat;
    q = s;
    if (!p[1])
        return 1;
    while (p[1])
        p++;
    while (q[1])
        q++;
    while (*q == *p) {
        q--;
        p--;
    }
    return pat == p && s <= q;
}
