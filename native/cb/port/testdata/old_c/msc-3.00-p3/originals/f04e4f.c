extern f048bd(), f0418c();

/* the LEDATA record of the n bytes at p in seg */
f04e4f(seg, n, p)
char *p;
{
    f048bd(0xa0, seg);
    f0418c(p, n);
}
