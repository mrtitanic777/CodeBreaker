public class Marked {
    void m(Empty1 e, int x) {}
    void call() { m((Empty1) null, 1); this.m(null, 2); }
    int sum(int... xs) { int r = 0; for (int i = 0; i < xs.length; i++) r += xs[i]; return r; }
    int va() { return sum() + sum(1) + sum(1, 2, 3) + sum(new int[]{4, 5}) + fmt("a", 1, "b") + fmt("z"); }
    int fmt(String f, Object... args) { return args.length; }
}
