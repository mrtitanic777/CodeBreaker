/* The C library mapping (java_libc): string literals, variadic arguments, a comparator given to qsort (by name, and through a
   variable), a JDK method, an MSVC-style stream, and a function the runtime does not have (a gap, not a refusal of the program). */
extern int printf(const char *fmt, ...);
extern double sqrt(double x);
extern unsigned long long strlen(const char *s);
extern void qsort(void *base, unsigned long long n, unsigned long long size, int (*cmp)(const void *, const void *));
extern int mystery(int x);

static int cmp_int(const void *a, const void *b) { return *(const int *)a - *(const int *)b; }

int use_mystery(int v) { return mystery(v); }

int main(void) {
    int v[4] = {3, 1, 2, 0};
    int (*f)(const void *, const void *) = cmp_int;
    qsort(v, 4, sizeof(int), cmp_int);
    qsort(v, 4, sizeof(int), f);
    printf("%d %s %f\n", v[0], "txt", sqrt(2.0));
    return (int)strlen("hello");
}
