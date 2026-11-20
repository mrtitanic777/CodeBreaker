/* The idiomatic tier's names: a struct tag that is also a global's name, a Java keyword as a field name, an anonymous struct. */
struct counter { int v; int new; };
int counter = 3;
struct { int a; } anon_var;

int main(void) {
    struct counter c = {counter, 2};
    anon_var.a = c.v + c.new;
    return anon_var.a;
}
