import java.util.*;
import java.util.function.*;
import java.util.stream.*;

public class Modern {
    record Point(int x, int y) {
        int dist2() { return x * x + y * y; }
    }

    sealed interface Shape permits Sq, Circ {}
    record Sq(int side) implements Shape {}
    record Circ(int r) implements Shape {}

    enum Color { RED, GREEN, BLUE }

    static int area(Shape s) {
        if (s instanceof Sq q) return q.side() * q.side();
        if (s instanceof Circ c) return 3 * c.r() * c.r();
        return 0;
    }

    static String name(Color c) {
        return switch (c) {
            case RED -> "r";
            case GREEN -> "g";
            default -> { String b = "b"; yield b + b; }
        };
    }

    static int sum(List<Integer> xs) {
        int s = 0;
        for (int v : xs) s += v;
        return s;
    }

    static int squares(int n) {
        return IntStream.rangeClosed(1, Math.abs(n) % 50).map(x -> x * x).sum();
    }

    static String words(String s) {
        return switch (s) { case "a", "b" -> "ab"; default -> "other"; };
    }

    public static void main(String[] args) {
        List<Integer> xs = new ArrayList<>();
        for (int i = 0; i < 6; i++) xs.add(i * i);
        Function<Integer, Integer> twice = v -> v * 2;
        System.out.println(sum(xs) + " " + twice.apply(21) + " " + squares(10));
        System.out.println(new Point(3, 4) + " " + new Point(3, 4).dist2() + " " + new Point(1, 2).equals(new Point(1, 2)));
        System.out.println(area(new Sq(4)) + " " + area(new Circ(2)) + " " + name(Color.BLUE) + " " + words("b"));
        System.out.println(xs.stream().filter(v -> v % 2 == 0).map(String::valueOf).collect(Collectors.joining(",")));
        Map<String, Integer> m = new HashMap<>();
        for (String w : "x y x z y x".split(" ")) m.merge(w, 1, Integer::sum);
        System.out.println(m);
    }
}
