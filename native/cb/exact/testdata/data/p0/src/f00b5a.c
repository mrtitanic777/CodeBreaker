extern f00940(), f0089e();

f00b5a(n, s)
int n;
register char *s;
{
    f00940(&n);
    while (--n >= 0)
        f0089e(s++);
}
