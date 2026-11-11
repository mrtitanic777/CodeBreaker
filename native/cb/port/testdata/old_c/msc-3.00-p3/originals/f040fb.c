extern char g_360e[];           /* (the record being made) */
extern int g_3368;              /* (its length) */
extern char *g_116e[];
extern f017cb();

/* the byte c put in the record */
f040fb(c)
char c;
{
    g_360e[g_3368++] = c;
    if (g_3368 > 0x3fc)
        f017cb(g_116e[0], 239);
}
