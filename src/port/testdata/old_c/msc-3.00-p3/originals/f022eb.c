/* MFLAGS: -Oat -Gs -FPi */
extern char g_2656[];           /* (the number made) */
extern char *ecvt();
extern strcpy(), strlen(), sprintf();

/* the floating point number d written with digits digits, as MASM takes it */
char *
f022eb(d, digits)
double d;
{
    int dec;
    int sign;
    char *s;
    char *t;

    s = ecvt(d, digits, &dec, &sign);
    t = g_2656;
    if (sign)
        *t++ = '-';
    if (dec < 0 || dec > digits) {
        *t++ = *s++;
        *t++ = '.';
        strcpy(t, s);
        t += strlen(s);
        *t++ = 'E';
        if (dec < 0) {
            *t++ = '-';
            sprintf(t, "%.2d", -dec + 1);
        } else {
            *t++ = '+';
            sprintf(t, "%.2d", dec - 1);
        }
    } else {
        while (dec-- > 0)
            *t++ = *s++;
        *t++ = '.';
        while (*s)
            *t++ = *s++;
        *t = 0;
    }
    return g_2656;
}
