package t;

public enum GapEn {
    GAPC(1), D(2);

    private int level;
    static Foo fx = gapMake();

    GapEn(int l) {
        level = l;
    }
}
