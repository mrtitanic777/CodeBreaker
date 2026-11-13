package sample.features;

import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import java.util.HashMap;

// One of each construct the Java front end lowers: what the converter's translator is given.
public class Features<T extends Comparable<T>> implements Runnable {
    static final int LIMIT = 16;
    private final List<T> items = new ArrayList<T>();
    private int[][] grid = new int[3][4];
    protected long counter;
    public String label = "a" + LIMIT + 'c' + 1.5f + 2.5 + true;

    enum Color { RED, GREEN(2) { int weight() { return 9; } }, BLUE; int weight() { return 1; } Color() { } Color(int w) { } }

    record Point(int x, double y, String name) { }

    interface Shape { double area(); }

    static class Base {
        Base() { this(1); }
        Base(int n) { }
        void greet() { }
    }

    static class Derived extends Base {
        Derived() { super(2); }
        @Override void greet() { super.greet(); }
    }

    public Features() { }

    public Features(int start) { this(); counter = start; }

    public void run() {
        int i = 0;
        i += 3;
        i++;
        --i;
        long big = 123456789012L;
        char c = 'x';
        double d = (double) i / 3;
        boolean flag = i > 2 && d < 4.0 || !(i == 0);
        String s = flag ? "yes" : "no";
        for (int k = 0, m = 10; k < m; k++, m--) {
            if (k == 5) continue;
            if (k == 8) break;
        }
        for (T item : items) {
            System.out.println(item);
        }
        outer:
        while (i < 100) {
            do {
                i += 7;
                if (i > 50) break outer;
            } while (i % 2 != 0);
        }
        switch (i) {
            case 1:
            case 2:
                i = 3;
                break;
            default:
                i = 0;
        }
        String word = switch (c) {
            case 'x' -> "ex";
            case 'y', 'z' -> { yield "why"; }
            default -> "other";
        };
        try (java.io.StringReader r = new java.io.StringReader("x")) {
            r.read();
        } catch (java.io.IOException | RuntimeException e) {
            throw new IllegalStateException(e);
        } finally {
            i = -i;
        }
        synchronized (this) {
            counter <<= 2;
        }
        Object o = s;
        if (o instanceof String str && str.length() > 1) {
            i = str.length();
        }
        Runnable task = () -> System.out.println("run");
        java.util.function.Function<Integer, Integer> twice = x -> x * 2;
        java.util.function.Supplier<List<String>> make = ArrayList::new;
        Shape unit = new Shape() {
            public double area() { return 1.0; }
        };
        Map<String, Integer> counts = new HashMap<String, Integer>();
        counts.put("a", twice.apply(i));
        int[] numbers = {1, 2, 3};
        int total = numbers.length + grid[1][2];
        Point p = new Point(1, 2.0, "p");
        assert total >= 0 : "negative";
        Class<?> k = String.class;
        grid[0][0] = ~total ^ (total >>> 2);
        Color color = Color.GREEN;
        label = this.label + super.toString();
    }

    <U> U first(List<U> list) { return list.get(0); }
}
