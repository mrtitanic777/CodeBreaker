extern f005e4(), f0066f(), f00b85();

f0085b(p, n)
register char *p;
int n;
{
    register int len;
    int i;

    len = f0066f();
    f00b85(n >= len);
    for (i = 0; i < len; i++)
        *p++ = f005e4();
    return len;
}
