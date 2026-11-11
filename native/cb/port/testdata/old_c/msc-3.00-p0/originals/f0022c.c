extern f00ce3();

f0022c(d, f, args)
register char *d, *f;
int *args;
{
    int base;
    int longf;

    for (longf = 0; *f; longf = 0) {
        if (*f == '%') {
again:
            switch (f[1]) {
            case 'l':
                longf = 1;
                f++;
                goto again;
            case 's':
                if (*args) {
                    strcpy(d, *args);
                    d += strlen(*args);
                }
                f += 2;
                args++;
                break;
            case 'd':
            case 'x':
                base = f[1] == 'd' ? 10 : 16;
                if (longf) {
                    d += f00ce3(*(long *)args, d, base);
                    args += 2;
                } else {
                    d += f00ce3((long)*args, d, base);
                    args++;
                }
                f += 2;
                break;
            case 'c':
                *d++ = *args;
                args++;
                f += 2;
                break;
            default:
                *d++ = *f++;
                break;
            }
        } else
            *d++ = *f++;
    }
    *d = 0;
}
