extern char g_2620, g_261c;
extern char *g_00c0;
extern f017cb();

/* (g_2620 no more than 2; bit 1 of g_261c set when it is 2) */
f00102()
{
    if (g_2620 > 2)
        f017cb(g_00c0, 120);
    if (g_2620 >= 2)
        g_261c |= 1;
}
