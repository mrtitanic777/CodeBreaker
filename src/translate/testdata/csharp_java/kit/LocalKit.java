import java.util.ArrayList;
import java.util.Comparator;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.function.Function;
import java.util.function.Supplier;

// What the C# translation must do as Java does with classes declared in methods: anonymous classes of the program's interfaces and abstract
// classes and of the library's functional interfaces, local classes with constructors, the locals they copy in, the instances around them, and
// one made inside another. Then Set.of and Map.of, used in the ways whose result Java fixes.
public class LocalKit {
    interface Shape {
        double area();
        default String label() { return "shape " + area(); }
    }

    abstract static class Greeter {
        final String who;
        int count;
        Greeter(String who) { this.who = who; }
        abstract String word();
        String greet() { count++; return word() + ", " + who + " #" + count; }
    }

    private int total = 10;
    private final String tag = "kit";

    int add(int by) { total += by; return total; }

    Shape square(double side) {
        return new Shape() {
            @Override public double area() { return side * side + total; }
        };
    }

    Greeter greeter(String word, int extra) {
        return new Greeter("world") {
            private final String loud = word + "!";
            @Override String word() { add(extra); return loud + tag + total; }
        };
    }

    Runnable counter(List<String> log, String name) {
        return new Runnable() {
            int runs = 0;
            @Override public void run() { runs++; log.add(name + runs + ":" + add(runs)); }
        };
    }

    final Supplier<String> held = new Supplier<String>() {
        @Override public String get() { return tag + total; }
    };

    static final Function<Integer, Integer> TWICE = new Function<Integer, Integer>() {
        @Override public Integer apply(Integer x) { return x * 2; }
    };

    static Comparator<String> byLength(boolean reversed) {
        return new Comparator<String>() {
            @Override public int compare(String a, String b) {
                int c = Integer.compare(a.length(), b.length());
                if (c == 0) c = a.compareTo(b);
                return reversed ? -c : c;
            }
        };
    }

    int localClasses(int start) {
        final int step = start * 2;
        class Tally {
            int value;
            Tally() { this(start); }
            Tally(int value) { this.value = value + step; }
            Tally next() { return new Tally(value + total); }
            int read() { int step2 = step + 1; return value + step2; }
        }
        class Maker {
            Tally make() { return new Tally(); }
        }
        Tally t = new Maker().make().next().next();
        return t.read();
    }

    static String nested(String first) {
        String second = first + "2";
        Supplier<Supplier<String>> outer = new Supplier<Supplier<String>>() {
            @Override public Supplier<String> get() {
                String third = second + "3";
                return new Supplier<String>() {
                    @Override public String get() { return first + "/" + second + "/" + third; }
                };
            }
        };
        return outer.get().get();
    }

    static String shadow(int x) {
        Shape s = new Shape() {
            @Override public double area() { int y = x + 1; return y; }
            @Override public String label() { int x = 40; return "own " + (x + 2); }
        };
        return s.area() + " " + s.label();
    }

    static void collections() {
        Set<String> set = Set.of("a", "b", "c");
        System.out.println(set.size() + " " + set.contains("b") + " " + set.contains("z"));
        System.out.println(Set.of("only"));
        Map<String, Integer> map = Map.of("one", 1, "two", 2);
        System.out.println(map.size() + " " + map.get("two") + " " + map.containsKey("three") + " " + map.getOrDefault("x", 9));
        System.out.println(Map.of("solo", 5));
        System.out.println(Set.<String>of().isEmpty() + " " + Map.<String, String>of().isEmpty());
    }

    public static void main(String[] args) {
        LocalKit kit = new LocalKit();
        Shape sq = kit.square(3);
        System.out.println(sq.area());
        kit.add(5);
        System.out.println(sq.area());
        Greeter g = kit.greeter("hello", 2);
        System.out.println(g.greet());
        System.out.println(g.greet());
        List<String> log = new ArrayList<>();
        Runnable r = kit.counter(log, "r");
        r.run();
        r.run();
        System.out.println(log + " " + kit.total);
        System.out.println(kit.held.get() + " " + TWICE.apply(21));
        List<String> words = new ArrayList<>(List.of("pear", "fig", "apple", "kiwi", "plum"));
        words.sort(byLength(false));
        System.out.println(words);
        words.sort(byLength(true));
        System.out.println(words);
        System.out.println(kit.localClasses(3));
        System.out.println(nested("n"));
        System.out.println(shadow(1));
        collections();
    }
}
