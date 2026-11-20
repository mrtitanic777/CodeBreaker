package t;
public class Chainer {
    void a(List<Foo> x) {
        b(x);
    }
    void b(List<Foo> y) {
        c(y, 1);
        c(y);
    }
    void c(List<Foo> z) {
        z.add(null);
    }
    void c(List<Foo> z, int k) {
        d(foo(z), z);
    }
    void d(Object o, List<Foo> q) {
    }
}
