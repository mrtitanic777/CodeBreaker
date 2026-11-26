package t;

public class Plain extends Base {
    public static final int LIMIT = 16;
    public static String NAME = "plain";
    public static Foo shared = new Foo();
    public static Foo made = Foo.make(3);
    public static int computed = compute(2);
    public static int[] table = new int[]{1, 2, 3};
    public static byte[] raw = new byte[]{1, 2};
    public static Foo[] made = new Foo[]{new Foo(), null};   // objects: filled in initStatics()
    public static int[][] grid = new int[][]{{1, 2}, {3}};
    public static Foo[] gapItems = new Foo[]{gapNull, null};
    public static int gapInit = gapMakeIt(1);
    public static long counter;
    private static Foo unset;
    public Foo owner;
    private int count;
    protected float scale;
    private byte tag;
    private boolean flag;
    private Foo[] items;
    private Vec3D pos;
    private String label;
    private List<Foo> list;

    static {
        counter = 1;
    }

    public Plain() {
        items = new Foo[4];
        owner = new Foo();
    }

    public Plain(int count) {
        this.count = count;
    }

    public Plain(int count, String label) {
        this(count);
        this.label = label;
    }

    public Plain(int a, int b, int c) {
        super();
    }

    public Plain(Gap g) {
    }

    public void tick() {
        count++;
    }

    public int size() {
        return count;
    }

    public Foo find(Foo f, String s) {
        return f;
    }

    public void gapMethod() {
        count = 0;
    }

    public static Plain make() {
        return new Plain();
    }
}
