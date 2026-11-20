extern int g_0f06;
extern f03731(), f00167();

/* the digits at s in the base as a long; one too big is said so and made 7FFFFFFFh (g_0f06 set) */
long
f0363f(s, base)
register char *s;
int base;
{
    long val, prev;

    g_0f06 = prev = val = 0;
    while (*s) {
        val *= base;
        val += f03731(*s);
        s++;
        if (base == 10) {
            if (val < prev) {
                s--;
                break;
            }
        } else if (*s)
            if (base == 8) {
                if (val & 0xe0000000L)
                    break;
            } else if (val & 0xf0000000L)
                break;
        prev = val;
    }
    if (*s) {
        f00167(1, "constant too big");
        g_0f06 = 1;
        val = 0x7fffffffL;
    }
    return val;
}
