/* MFLAGS: -Oat -Gs -FPi */
extern char *f022eb();
extern printf();

/* the float at f in the listing */
f023f8(f)
float *f;
{
    char *s;

    s = f022eb((double)*f, 8);
    printf("\tDD\t0%lxH   ;\t%s\n", *(long *)f, s);
}
