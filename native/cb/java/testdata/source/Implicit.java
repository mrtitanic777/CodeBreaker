package t;

public class Implicit {
    private int a = 3;
    private Foo b = new Foo();
    private long c;

    {
        a = a + 1;
    }

    public int a() {
        return a;
    }
}
