public abstract class Animal implements Walker {
    protected int legs = 4;
    static int[] table = new int[]{1, 2, 3};
    abstract String name();
    public int walk(int n) { return legs * n; }
}
