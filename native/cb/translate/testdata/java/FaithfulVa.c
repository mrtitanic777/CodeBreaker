/* A variadic definition: its arguments past the named ones travel as boxed objects into the emulated memory. */
#include <stdarg.h>

static int total(int n, ...) {
    va_list ap;
    int s = 0;
    va_start(ap, n);
    for (int i = 0; i < n; i++) s += va_arg(ap, int);
    va_end(ap);
    return s;
}

int main(void) { return total(3, 1, 2, 3); }
