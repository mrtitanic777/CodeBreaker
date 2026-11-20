import java.util.*;
import java.util.function.*;

public class Mix {
    enum Color { RED, GREEN(5), BLUE;
        private final int w;
        Color() { this(1); }
        Color(int w) { this.w = w; }
        int weight() { return w; }
    }

    interface Shape { double area(); }

    static final int LIMIT = 10;
    static int counter;
    static long total = 5L;
    static String label;
    static { label = "mix"; counter = 3; }

    class Inner {
        int v;
        Inner(int v) { this.v = v + base; }
        int get() { return v * 2; }
    }

    int base = 4;

    static class Box<T> {
        T item;
        Box(T item) { this.item = item; }
        T get() { return item; }
    }

    static int fib(int n) { return n < 2 ? n : fib(n - 1) + fib(n - 2); }

    static int sw(int k) {
        switch (k) {
            case 1: return 10;
            case 2:
            case 3: k += 5; break;
            default: k = -1;
        }
        return k;
    }

    static String name(String s) {
        switch (s) {
            case "a": return "alpha";
            case "b": return "beta";
            default: return "other";
        }
    }

    static int[][] grid(int n) {
        int[][] g = new int[n][n];
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++) g[i][j] = i * j;
        return g;
    }

    static long mix(long a, int b, double c, char ch, byte by) {
        long r = a << (b & 7);
        r ^= (int) c;
        r += ch + by;
        r >>>= 1;
        return r % 1000 + (b / 3);
    }

    static void thrower(int x) throws Exception {
        if (x > 2) throw new IllegalStateException("big " + x);
        if (x == 2) throw new Exception("two");
    }

    public static void main(String[] args) {
        Mix m = new Mix();
        Mix.Inner in = m.new Inner(3);
        System.out.println(in.get() + " " + fib(10) + " " + sw(1) + sw(3) + sw(9) + " " + name("b") + name("z"));
        for (Color c : Color.values()) System.out.println(c + ":" + c.weight() + ":" + c.ordinal());
        Shape s = new Shape() { public double area() { return 2.5 * LIMIT; } };
        System.out.println(s.area());
        int[][] g = grid(4);
        int sum = 0;
        for (int[] row : g) for (int x : row) sum += x;
        System.out.println(sum + " " + mix(1234567L, 5, 7.9, 'x', (byte) 3) + " " + label + counter + total);
        Box<String> b = new Box<>("boxed");
        System.out.println(b.get().length() + b.get());
        for (int i = 0; i < 4; i++) {
            try {
                thrower(i);
                System.out.println("ok " + i);
            } catch (IllegalStateException e) {
                System.out.println("ise " + e.getMessage());
            } catch (Exception e) {
                System.out.println("ex " + e.getMessage());
            } finally {
                counter++;
            }
        }
        Function<Integer, Integer> twice = x -> x * 2;
        BiFunction<Integer, Integer, Integer> add = Integer::sum;
        Supplier<String> sup = () -> label + counter;
        System.out.println(twice.apply(21) + " " + add.apply(3, 4) + " " + sup.get());
        StringBuilder sb = new StringBuilder();
        outer:
        for (int i = 0; i < 5; i++) {
            int j = 0;
            while (true) {
                if (j > i) continue outer;
                if (i == 4) break outer;
                sb.append(i).append(',').append(j).append(' ');
                j++;
            }
        }
        System.out.println(sb.toString());
        Object o = b;
        if (o instanceof Box) System.out.println("box");
        double d = 1.0 / 3;
        float f = 2.5f;
        System.out.println(d + " " + f + " " + (long) d + " " + Math.max(3, 4) + " " + Integer.MAX_VALUE);
        int k = 0;
        do { k += 3; } while (k < 10);
        char c = (char) ('a' + k);
        System.out.println(k + " " + c + " " + (k > 5 && c != 'z' || k == 0));
    }
}
