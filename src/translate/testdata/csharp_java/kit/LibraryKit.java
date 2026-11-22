import java.util.ArrayList;
import java.util.HashMap;
import java.util.Iterator;
import java.util.LinkedHashMap;
import java.util.List;
import java.util.Map;
import java.util.TreeMap;

// What the C# translation must do as Java does with the library programs lean on: String.format's rounding and flags, Map's default methods
// (which put a new key at the head of its bin), iterators, enums of objects, block arms of a switch, arrays of arrays.
public class LibraryKit {
    enum Planet {
        MERCURY(3.303e+23, 2.4397e6), EARTH(5.976e+24, 6.37814e6), MARS(6.421e+23, 3.3972e6);

        private final double mass, radius;
        private static int asked = 0;

        Planet(double mass, double radius) { this.mass = mass; this.radius = radius; }
        double gravity() { asked++; return 6.67300E-11 * mass / (radius * radius); }
        static int asked() { return asked; }
    }

    enum Level {
        LOW(1), MID, HIGH(9) ;
        final int score;
        Level() { this(5); }
        Level(int score) { this.score = score; }
        @Override public String toString() { return name().toLowerCase() + "/" + score; }
    }

    static class Range implements Iterable<Integer> {
        final int from, to;
        Range(int from, int to) { this.from = from; this.to = to; }
        @Override public Iterator<Integer> iterator() {
            return new Iterator<Integer>() {
                int at = from;
                @Override public boolean hasNext() { return at < to; }
                @Override public Integer next() { return at++; }
            };
        }
    }

    static void formats() {
        double[] values = {0.125, 1.005, 2.5, 1234567.891, -0.5, 0.0, -0.0, 1e-7, 99.995, 1e21};
        for (double v : values) {
            System.out.println(String.format("[%.2f][%.0f][%10.3f][%-10.1f][%+.1f][%e][%.3E]", v, v, v, v, v, v, v));
        }
        System.out.println(String.format("%d|%5d|%-5d|%05d|%+d|%,d|%(d|% d", 42, 42, 42, 42, 42, 1234567, -42, 42));
        System.out.println(String.format("%x|%X|%o|%#x|%08x|%x|%x", 255, 255, 8, 255, 255, -1, -1L));
        System.out.println(String.format("%s|%10s|%-10s|%.2s|%S|%b|%c|%%|%2$s %1$s", "abc", "def", "ghi", "jkl", "mno", true, 'z'));
        System.out.println(String.format("%s %s %s %s", 1.5, null, List.of(1, 2), 'c'));
        System.out.printf("%d items at %.2f%n", 3, 9.999);
        System.out.println("%s and %d".formatted("text", 7));
        System.out.println(String.format("%f %f %f", 1.0f, Double.NaN, Double.POSITIVE_INFINITY));
    }

    static void maps() {
        Map<String, Integer> counts = new HashMap<>();
        for (String w : "to be or not to be that is the question and or be".split(" ")) counts.merge(w, 1, Integer::sum);
        System.out.println(counts);
        Map<String, List<Integer>> groups = new HashMap<>();
        String[] names = {"be", "or", "to", "not", "that", "is", "ab", "ba", "Aa", "BB"};
        for (int i = 0; i < names.length; i++) groups.computeIfAbsent(names[i], k -> new ArrayList<>()).add(i);
        System.out.println(groups);
        Map<String, Integer> grown = new HashMap<>(2);
        for (int i = 0; i < 40; i++) grown.merge("key" + i * 7, i, Integer::sum);
        System.out.println(grown);
        Map<String, String> present = new HashMap<>();
        present.put("a", "1");
        System.out.println(present.putIfAbsent("a", "2") + " " + present.putIfAbsent("b", "3") + " " + present);
        System.out.println(present.computeIfPresent("a", (k, v) -> k + v) + " " + present.computeIfPresent("zz", (k, v) -> v) + " " + present);
        System.out.println(present.compute("c", (k, v) -> v == null ? "new" : v + "!") + " " + present.compute("c", (k, v) -> v == null ? "new" : v + "!"));
        System.out.println(present.compute("a", (k, v) -> null) + " " + present);
        Map<String, Integer> linked = new LinkedHashMap<>();
        for (String w : "c a b a c".split(" ")) linked.merge(w, 1, Integer::sum);
        System.out.println(linked);
        Map<String, Integer> tree = new TreeMap<>();
        for (String w : "c a b a c".split(" ")) tree.merge(w, 10, Integer::sum);
        System.out.println(tree);
    }

    static String describe(Level l) {
        return switch (l) {
            case LOW -> "small";
            case MID -> {
                String twice = l.name() + l.name();
                yield twice.length() > 3 ? twice : "short";
            }
            default -> {
                int n = 0;
                for (int i = 0; i < l.score; i++) n += i;
                yield "big" + n;
            }
        };
    }

    static void enums() {
        for (Planet p : Planet.values()) System.out.println(String.format("%s %d %.2f", p, p.ordinal(), p.gravity()));
        System.out.println(Planet.asked() + " " + Planet.valueOf("MARS").name() + " " + Planet.EARTH.compareTo(Planet.MARS));
        for (Level l : Level.values()) {
            switch (l) {
                case LOW: System.out.print("low "); break;
                case HIGH: System.out.print("high ");
                default: System.out.print("other ");
            }
            System.out.println(l + " " + describe(l));
        }
        try {
            Planet.valueOf("PLUTO");
        } catch (IllegalArgumentException ex) {
            System.out.println("no such planet");
        }
    }

    static void iterators() {
        int sum = 0;
        for (int i : new Range(1, 5)) sum += i;
        System.out.println(sum);
        Iterator<Integer> it = new Range(7, 9).iterator();
        while (it.hasNext()) System.out.print(it.next() + " ");
        System.out.println();
        List<String> list = new ArrayList<>(List.of("x", "y"));
        Iterator<String> walk = list.iterator();
        System.out.println(walk.hasNext() + " " + walk.next() + " " + walk.next() + " " + walk.hasNext());
    }

    static void arrays() {
        int[][] grid = new int[3][4];
        grid[1][2] = 5;
        System.out.println(grid.length + " " + grid[0].length + " " + grid[1][2]);
        String[][][] deep = new String[2][2][2];
        System.out.println(deep[1][1][1] + " " + deep[1][1].length);
        System.out.println(new String(new char[] {'h', 'i'}) + new String(new char[] {'a', 'b', 'c', 'd'}, 1, 2) + new String("s") + new String().length());
    }

    public static void main(String[] args) {
        formats();
        maps();
        enums();
        iterators();
        arrays();
    }
}
