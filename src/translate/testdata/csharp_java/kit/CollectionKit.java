import java.util.ArrayList;
import java.util.HashMap;
import java.util.HashSet;
import java.util.LinkedHashMap;
import java.util.LinkedHashSet;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.TreeMap;
import java.util.TreeSet;
import java.util.stream.Collectors;

// What the C# translation must do as Java does: walk a hash table in Java's order through growth, removal, collisions and every constructor;
// immutable lists; nested and inner classes reaching their enclosing instances.
public class CollectionKit {
    record Key(String name, int n) {}

    static class Collide {
        final int id;
        Collide(int id) { this.id = id; }
        @Override public int hashCode() { return 7; }
        @Override public boolean equals(Object o) { return o instanceof Collide && ((Collide) o).id == id; }
        @Override public String toString() { return "c" + id; }
    }

    private int total = 100;
    private String label = "outer";

    int bump(int by) { total += by; return total; }

    class Counter {
        int mine = total + 1;
        int next() { mine++; return bump(mine); }
        String who() { return label + ":" + CollectionKit.this.total; }

        class Deep {
            final int extra;
            Deep(int extra) { this.extra = extra; }
            Deep() { this(5); }
            int all() { return mine + total + extra + bump(1); }
            Counter parent() { return Counter.this; }
        }

        Deep deep() { return new Deep(); }
    }

    class Plain { int two() { return 2; } }

    Counter counter() { return new Counter(); }

    static void maps() {
        Map<String, Integer> words = new HashMap<>();
        String[] names = {"pear", "fig", "apple", "kiwi", "banana", "cherry", "date", "elder", "grape", "honey", "iris", "jade", "kale", "lime", "mango", "nut", "olive", "plum"};
        for (int i = 0; i < names.length; i++) {
            words.put(names[i], i);
            if (i % 5 == 0) System.out.println(words);
        }
        words.remove("kiwi");
        words.put("kiwi", 99);
        words.put("pear", -1);
        for (Map.Entry<String, Integer> e : words.entrySet()) System.out.print(e + " ");
        System.out.println();
        for (String k : words.keySet()) System.out.print(k + " ");
        System.out.println();
        for (int v : words.values()) System.out.print(v + " ");
        System.out.println();

        Map<Integer, String> ints = new HashMap<>(4);
        for (int i = 0; i < 40; i++) ints.put(i * 37 - 500, "v" + i);
        System.out.println(ints);
        System.out.println(new HashMap<>(ints).equals(ints) + " " + (new HashMap<>(ints).hashCode() == ints.hashCode()));
        Map<String, Integer> copy = new HashMap<>(words);
        System.out.println(copy);
        Map<String, Integer> tight = new HashMap<>(2, 0.5f);
        for (String n : names) tight.put(n, n.length());
        System.out.println(tight);

        Map<Collide, Integer> bin = new HashMap<>();
        for (int i = 0; i < 10; i++) bin.put(new Collide(i), i);   // (past eight in a bin, a table under 64 grows)
        bin.remove(new Collide(3));
        System.out.println(bin);
        Map<Key, String> keyed = new HashMap<>();
        for (int i = 0; i < 9; i++) keyed.put(new Key("k" + (i % 3), i), "x" + i);
        System.out.println(keyed.get(new Key("k1", 4)) + " " + keyed);
        Map<Long, Double> mixed = new HashMap<>();
        for (long i = 1; i < 9; i++) mixed.put(i * 5000000000L, i / 4.0);
        System.out.println(mixed);
        Map<String, String> nulls = new HashMap<>();
        nulls.put(null, "one");
        nulls.put("a", null);
        System.out.println(nulls + " " + nulls.containsKey(null) + " " + nulls.get("a") + " " + nulls.getOrDefault("b", "five"));

        Map<String, Integer> linked = new LinkedHashMap<>();
        for (String n : names) linked.put(n, n.length());
        linked.remove("apple");
        linked.put("fig", 0);
        linked.put("apple", 1);
        System.out.println(linked);
        Map<String, Integer> tree = new TreeMap<>();
        for (String n : new String[] {"b", "B", "a", "A", "_", "10", "9"}) tree.put(n, 1);
        System.out.println(tree);
    }

    static void sets() {
        Set<String> seen = new HashSet<>();
        for (String w : "the quick brown fox jumps over the lazy dog again and again until done".split(" ")) System.out.print(seen.add(w) + " ");
        System.out.println();
        System.out.println(seen);
        seen.remove("fox");
        for (String w : seen) System.out.print(w + ",");
        System.out.println();
        List<Integer> many = new ArrayList<>();
        for (int i = 0; i < 30; i++) many.add(i * i - 40);
        System.out.println(new HashSet<>(many));
        Set<Integer> small = new HashSet<>(3);
        for (int v : many) small.add(v);
        System.out.println(small);
        Set<String> order = new LinkedHashSet<>();
        for (String s : new String[] {"z", "a", "m", "a", "z", "b"}) order.add(s);
        order.remove("m");
        order.add("m");
        System.out.println(order);
        Set<String> sorted = new TreeSet<>();
        for (String s : new String[] {"b", "B", "a", "A"}) sorted.add(s);
        System.out.println(sorted);
        System.out.println(many.stream().map(x -> "n" + x).collect(Collectors.toSet()));
        System.out.println(many.stream().collect(Collectors.groupingBy(x -> Math.abs(x) % 7)));
        System.out.println(many.stream().collect(Collectors.groupingBy(x -> "g" + Math.abs(x) % 5, Collectors.counting())));
    }

    static void lists() {
        List<String> fixed = List.of("x", "y", "z");
        System.out.println(fixed + " " + fixed.size() + " " + fixed.get(1) + " " + fixed.contains("y"));
        List<Integer> none = List.of();
        System.out.println(none + " " + none.isEmpty());
        System.out.println(fixed.stream().map(String::toUpperCase).collect(Collectors.joining("+")));
    }

    static void inner() {
        CollectionKit a = new CollectionKit();
        CollectionKit b = new CollectionKit();
        b.total = 1000;
        Counter ca = a.counter();
        Counter cb = b.new Counter();
        System.out.println(ca.mine + " " + cb.mine + " " + ca.next() + " " + cb.next() + " " + a.total + " " + b.total);
        System.out.println(ca.who() + " " + cb.who());
        Counter.Deep d = ca.deep();
        Counter.Deep e = cb.new Deep(9);
        System.out.println(d.all() + " " + e.all() + " " + (d.parent() == ca) + " " + (e.parent() == cb) + " " + a.total + " " + b.total);
        System.out.println(a.new Plain().two());
        CollectionKit missing = null;
        try {
            missing.new Plain();
            System.out.println("no throw");
        } catch (NullPointerException ex) {
            System.out.println("null outer");
        }
    }

    public static void main(String[] args) {
        maps();
        sets();
        lists();
        inner();
    }
}
