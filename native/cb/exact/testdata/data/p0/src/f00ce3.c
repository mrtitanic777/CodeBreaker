extern char g_0582[];

f00ce3(val, d, base)
long val;
register char *d;
int base;
{
    register char *p;
    int cnt;
    char buf[32];
    unsigned long x;

    cnt = 0;
    p = buf;
    *p++ = 0;
    if (val < 0 && base == 10) {
        *d++ = '-';
        x = -val;
        cnt++;
    } else
        x = val;
    do
        *p++ = g_0582[x % base];
    while ((x /= base) != 0);
    while ((*d++ = *--p) != 0)
        cnt++;
    return cnt;
}
