extern char *g_1f80[];
extern f08ab1(), f08b73();

/* all that is known of the registers that are parts of the register r forgotten */
f08dc6(r)
register r;
{
    register char *p;

    for (p = g_1f80[r]; p[1];) {
        f08ab1(*p);
        f08b73(*p++);
    }
}
