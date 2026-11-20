/* MFLAGS: -Oat -Gs -FPi */
extern char *f022eb();
extern printf();

/* the double at d in the listing */
f0242e(d)
double *d;
{
    char *s;

    s = f022eb(*d, 16);
    printf("\tDQ\t0%lx%.8lxH    ;\t%s\n", ((long *)d)[0], ((long *)d)[1], s);
}
