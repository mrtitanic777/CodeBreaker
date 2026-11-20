package feat;

import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import java.util.Random;
import java.util.TreeMap;
import java.util.function.Function;

/** Java semantics in C#: dispatch, construction order, text, library calls. */
public class Features {
    static class Resource implements AutoCloseable {
        public void close() { System.out.println("closed"); }
    }

    static abstract class Shape implements Comparable<Shape> {
        abstract double area();
        public int compareTo(Shape o) { return Double.compare(area(), o.area()); }
        @Override public String toString() { return "Shape(" + area() + ")"; }
    }

    static final class Square extends Shape {
        private final double side;
        Square(double side) { this.side = side; }
        double area() { return side * side; }
        @Override public boolean equals(Object o) { return o instanceof Square && ((Square) o).side == side; }
        @Override public int hashCode() { return (int) side; }
    }

    interface Greeter { String greet(String name); }

    // leaves the interface's method to its subclasses: C# needs it declared abstract here
    static abstract class Partial implements Greeter { }

    static class Polite extends Partial {
        public String greet(String name) { return "hello " + name; }
    }

    static class Base {
        int v = 1;
        Base() { System.out.println("base " + v); }
    }

    // Java runs w's initialiser after Base's constructor; C# before it: moved into the constructor
    static class Derived extends Base {
        int w = 2;
        { w += 1; }
        Derived() { super(); System.out.println("derived " + w); }
    }

    static final int LIMIT = 10;
    static final String NAME = "features";
    static int counter = compute();
    static { counter += 1; }

    static int compute() { return 41; }

    public static void main(String[] args) {
        StringBuilder sb = new StringBuilder("x");
        sb.append(1).append("a");
        List<Integer> xs = new ArrayList<>();
        xs.add(3);
        xs.add(4);
        Map<String, Integer> sorted = new TreeMap<>();
        sorted.put("b", 2);
        sorted.put("a", 1);
        for (Map.Entry<String, Integer> e : sorted.entrySet()) {
            System.out.println(e.getKey() + "=" + e.getValue());
        }
        Random rnd = new Random(7);
        double d = 2.5;
        float f = 1.5f;
        String s = null;
        System.out.println("d=" + d + " f=" + f + " s=" + s + " b=" + (xs.size() > 1));
        System.out.println(Integer.MAX_VALUE + " " + Math.PI + " " + rnd.nextInt(10));
        Function<Integer, Integer> inc = x -> x + 1;
        Runnable run = () -> System.out.println("run " + inc.apply(1));
        run.run();
        try (Resource r = new Resource()) {
            System.out.println(new Polite().greet("you") + new Square(2));
        }
        String a = "left";
        String b = sb.toString();
        System.out.println(a == b);
        if (args == null) {
            throw new IllegalStateException("never");
        }
        new Derived();
        System.out.println(counter + LIMIT + NAME.length());
    }
}
