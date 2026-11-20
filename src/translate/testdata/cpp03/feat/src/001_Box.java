public class Box {
    public int x;
    public int y;
    public Box(int a, int b) { x = a; y = b; }
    public void touch() {}
    public static Box create(int a) { return new Box(a, a); }
    public static Box make(int a) { return new Box(a, a); }
}
