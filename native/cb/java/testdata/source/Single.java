package t;

public class Single {
    static Single instance = new Single();
    static Foo other = new Foo();
    private int v;

    static Single getInstance() {
        return instance;
    }

    static Single getInstance2() {
        return instance;
    }

    static Single notPure() {
        v2();
        return instance;
    }

    int get() {
        return v;
    }
}
