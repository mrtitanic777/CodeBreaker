extern f033c4(), f0332c();

f035c5(n, s)
int n;
register char *s;
{
    f033c4(&n);
    while (--n >= 0)
        f0332c(s++);
}
