extern char *g_1054[], *g_106c[];      /* the registers' names: words, bytes */

/* the name of the register r (of size sz) */
char *
f03dbb(r, sz)
char r, sz;
{
    return sz == 1 ? g_106c[r] : g_1054[r];
}
