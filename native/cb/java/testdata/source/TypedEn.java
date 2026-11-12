package t;

public enum TypedEn {
    A(1, "a"), B(2, "b"), NULL(3, "n");

    private int level;
    private String label;
    static int COUNT = 0;
    static Foo fx = new Foo();

    static {
        COUNT = 1;
    }

    TypedEn(int l, String s) {
        level = l;
        label = s;
    }

    int getLevel() {
        return level;
    }

    Foo make() {
        return new Foo();
    }
}
