extern int g_3368;
extern f040fb(), f04837(), f0418c(), strlen();

/* the name s (no more than 40 characters) put in the record, its length first */
f041e1(s)
register char *s;
{
    register n;

    if ((n = strlen(s)) > 40) {
        n = 40;
        s[40] = 0;
    }
    if (g_3368 + n + 1 > 1000)
        f04837();
    f040fb(n);
    f0418c(s, n);
}
