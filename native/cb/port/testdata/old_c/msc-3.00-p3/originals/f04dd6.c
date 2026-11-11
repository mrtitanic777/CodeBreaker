extern int g_3aa2[];            /* (the line numbers waiting: number and place) */
extern int g_3366;
extern struct seg *g_11bc;
extern f048bd(), f04127();

/* the line n at the place at kept (the LINNUM record of those waiting first, if out) */
f04dd6(n, at, out)
{
    int i;

    if (out) {
        f048bd(0x94, g_11bc);
        for (i = 0; i < g_3366; ) {
            f04127(g_3aa2[i++]);
            f04127(g_3aa2[i++]);
        }
        g_3366 = 0;
    }
    g_3aa2[g_3366++] = n;
    g_3aa2[g_3366++] = at;
}
