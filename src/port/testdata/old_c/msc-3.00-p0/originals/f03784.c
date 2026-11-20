#include <ctype.h>
extern int g_05d4;
extern int (*g_09da)(), (*g_09dc)();
extern f03ada(), f03ac1();


/* the character a backslash escapes (its value; one over 255 is said so) */
f03784()
{
    register int n;
    char c;
    int mm;                     /* (mm, a, w not used) */
    long hval;
    int a;
    long ov;
    long l;
    int w;

    switch (c = (*g_09da)()) {
    case 'b':
        c = 8;
        break;
    case 'f':
        c = 0xc;
        break;
    case 'n':
        c = 0xa;
        break;
    case 'r':
        c = 0xd;
        break;
    case 't':
        c = 9;
        break;
    case 'v':
        c = 0xb;
        break;
    case '\\':
        c = '\\';
        break;
    default:
        if (c >= '0' && c <= '7') {
            for (n = 1, ov = f03ada(c); c = (*g_09da)(), n < 3; n++) {
                if (c < '0' || c > '7')
                    break;
                ov *= 8;
                ov += f03ada(c);
            }
            (*g_09dc)(c);
            l = ov;
        } else if (c == 'X' || c == 'x') {
            hval = 0;
            n = 1;
            c = (*g_09da)();
            while (n <= 2 && isxdigit(c)) {
                hval *= 16;
                hval += f03ada(c);
                c = (*g_09da)();
                n++;
            }
            (*g_09dc)(c);
            l = hval;
        } else
            break;
        if (!g_05d4) {
            c = l;
            if (l > 255)
                f03ac1(l);
        }
    }
    return c;
}
