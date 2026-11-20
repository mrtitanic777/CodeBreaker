public class Circle0 extends Shape0 {
    public double r = 1.5;
    public int[] pts;

    public Circle0(double r) {
        super((int) r);
        this.r = r;
    }

    public Circle0() {
        super();
    }

    public Circle0(int a, int b) {
        this((double) (a + b));
        this.pts = new int[]{a, b};
    }

    public int area() { return super.area() + (int) (r * r); }
    public String describe() { return super.describe() + "c" + this.label + super.label; }
    public int total() { return this.size + super.size + Shape0.count + Circle0.count; }
}
