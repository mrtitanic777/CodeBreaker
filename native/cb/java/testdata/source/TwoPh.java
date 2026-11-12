package t;

public class TwoPh extends TwoParent {
    private int extra = 5;
    private Foo helper;

    public TwoPh() {
        super();
    }

    public TwoPh(int hp) {
        super(hp);
    }

    public TwoPh(int a, int b) {
        this(a);
    }

    public TwoPh(Gap g) {
    }

    protected void init() {
        helper = new Foo();
    }
}
