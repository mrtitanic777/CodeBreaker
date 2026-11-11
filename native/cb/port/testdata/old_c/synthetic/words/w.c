int stack[10];
void walk(n)
int n;
{
    int *sp;
    int *fp;
    sp = stack;
    fp = sp;
    fp = (int *)sp[2];
    sp[1] = (int)fp;
}
