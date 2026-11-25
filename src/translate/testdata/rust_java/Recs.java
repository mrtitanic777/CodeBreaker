import java.util.ArrayList;
import java.util.List;

public class Recs {
    record Point(int x, int y) {
        Point { if (x < 0) throw new IllegalArgumentException("negative x"); }
        int dist() { return Math.abs(x) + Math.abs(y); }
        static Point origin() { return new Point(0, 0); }
    }
    record Person(String name, double score, boolean admin) {}
    interface Shape { double area(); }
    record Circle(double r) implements Shape { public double area() { return 3 * r * r; } }
    record Sq(double s) implements Shape { public double area() { return s * s; } }

    public static void main(String[] args) {
        Point p = new Point(3, -4), q = new Point(3, -4);
        System.out.println(p + " " + p.x() + " " + p.dist() + " " + p.equals(q) + " " + (p == q) + " " + (p.hashCode() == q.hashCode()) + " " + Point.origin());
        Person a = new Person("Ann", 9.5, true);
        System.out.println(a + " " + a.name().length() + " " + a.equals(new Person("Ann", 9.5, true)) + " " + a.equals(new Person("Bob", 9.5, true)));
        List<Shape> shapes = new ArrayList<>();
        shapes.add(new Circle(2)); shapes.add(new Sq(3));
        double t = 0;
        for (Shape s : shapes) t += s.area();
        System.out.println(shapes + " " + t);
        try { new Point(-1, 0); } catch (IllegalArgumentException e) { System.out.println(e.getMessage()); }
    }
}
