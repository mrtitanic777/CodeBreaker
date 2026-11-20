public class Shape0 {
    public int size;
    public int scale = 2;
    protected String label = "s";
    public static int count;

    public Shape0() {
        this(1);
    }

    public Shape0(int s) {
        this(s, 3);
    }

    public Shape0(int s, int k) {
        super();
        this.size = s * k;
        this.scale += k;
    }

    {
        count++;
    }

    public int area() { return size * scale; }
    public String describe() { return label + ":" + size; }
    public static Shape0 make(int n) { return new Shape0(n); }
}
