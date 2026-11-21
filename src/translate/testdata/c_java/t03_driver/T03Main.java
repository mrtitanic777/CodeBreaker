// The t03 classes have no main of their own: this calls every method across the values that take each path, so their translations can be held to the JVM.
public class T03Main {
    public static void main(String[] args) {
        Base a = new Base(5);
        Base b = new Base(5, 4);
        System.out.println("base " + a.size + " " + a.scale + " " + b.size + " " + b.scale);
        Sub s = new Sub(6);
        System.out.println("sub " + s.size + " " + s.scale + " " + s.extra + " " + Sub.build(9));

        System.out.println("statics " + Calc.total + " " + Calc.table.length + " " + Calc.table[1]);
        Calc c = new Calc();
        int[] values = {-9, -1, 0, 1, 2, 3, 7, 8, 15, 16, 40, 2147483647, -2147483648};
        for (int v : values) {
            System.out.println(v + ": fill " + c.fill(v) + " total " + Calc.total + " label " + c.label(v) + " guarded " + c.guarded(v)
                    + " cleanup " + c.cleanup(v) + " search " + c.search(v) + " sums " + Calc.sums(v) + " tabled " + Calc.tabled(v)
                    + " made " + Calc.made(v) + " count " + c.count(v));
        }
        long[] longs = {0L, 5L, 1000L, 1001L, 123456789012345L, -1L, Long.MIN_VALUE, Long.MAX_VALUE};
        for (long l : longs) {
            System.out.println(l + " mix " + Calc.mix(l, 3) + " " + Calc.mix(l, -7));
        }
        float[] floats = {0.0F, 1.5F, -1.5F, 3.5F, 7.25F, -100.125F, 1.0E10F};
        for (float f : floats) {
            System.out.println(f + " wrap " + Calc.wrap(f));
        }
        System.out.println("sum " + Calc.sum() + " " + Calc.sum(4) + " " + Calc.sum(1, 2, 3));
    }
}
