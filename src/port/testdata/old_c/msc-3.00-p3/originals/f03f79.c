extern char *f03e86();

char *
f03f79(s, n)
char *s;
int n;
{
    char *p;

    p = f03e86(n, strlen(s) + 1);
    strcpy(p, s);
    return p;
}
