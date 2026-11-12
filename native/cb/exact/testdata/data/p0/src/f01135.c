extern char *f01033();

char *
f01135(s, n)
char *s;
int n;
{
    char *p;

    p = f01033(n, strlen(s) + 1);
    strcpy(p, s);
    return p;
}
