extern char *g_00c0;
extern f017cb();

/* the field (bits 3-4) of the segment register r (8 up) */
f002e1(r)
{
    if (r < 8)
        f017cb(g_00c0, 271);
    return (r & 3) << 3;
}
