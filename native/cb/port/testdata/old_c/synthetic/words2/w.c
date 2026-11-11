int table[8];
int keep;
void f(n)
int n;
{
    register int a, *sp, b[4];
    unsigned *fp;
    sp = table;
    fp = sp;
    fp = (unsigned *)sp[1];
    a = keep;
}
void caller()
{
    int q[3];
    f(q);
}
