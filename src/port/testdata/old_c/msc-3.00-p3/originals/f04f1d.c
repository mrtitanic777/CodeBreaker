extern char *g_123c;
extern f048bd(), f040fb(), f04274(), f017cb();

/* a THREAD subrecord: thread n of the kind d (frame or target) by method m, index idx; the thread */
f04f1d(n, d, idx, m)
char n, d;
{
    f048bd(0x9c, 0);
    if (m > 4)
        f017cb(g_123c, 789);
    f040fb(d << 2 | n | m);
    f04274(idx);
    return m;
}
