/* Calls into C's library: the back end asks its library mapping (java_libc, a separate module) how each is written. */
extern int puts(const char *s);
extern int abs(int v);
extern int strlen_of(const char *s);

int main(void) {
    puts("hello");
    return abs(-3);
}
