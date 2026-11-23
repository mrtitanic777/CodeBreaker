import java.util.HashMap;
import java.util.HashSet;
import java.util.LinkedHashSet;
import java.util.Map;
import java.util.TreeSet;

// What running a whole game client showed: arrays of a program's classes copied, a raw map's missing number, a hash-printing object, and a big sorted set.
public class ClientKit {
    static class Thing implements Comparable<Thing> {
        final int id;
        Thing(int id) { this.id = id; }
        @Override public int hashCode() { return id; }
        @Override public int compareTo(Thing o) { return Integer.compare(id, o.id); }
    }

    static class Named extends Thing {
        Named(int id) { super(id); }
        @Override public String toString() { return "named " + id; }
    }

    @SuppressWarnings({"rawtypes", "unchecked"})
    public static void main(String[] args) {
        Thing[] from = {new Thing(1), new Named(2), new Thing(3)};
        Thing[] into = new Thing[5];
        System.arraycopy(from, 0, into, 1, 3);
        System.arraycopy(into, 1, into, 2, 3);
        System.out.println(into[0] + " " + into[1].id + " " + into[2].id + " " + into[3] + " " + into[4].id);
        Object[] wide = new Object[2];
        System.arraycopy(from, 1, wide, 0, 2);
        System.out.println(wide[0] + " " + ((Thing) wide[1]).id);
        System.out.println(new Thing(255));

        Map raw = new HashMap();
        raw.put("a", Integer.valueOf(7));
        Integer found = (Integer) raw.get("a");
        Integer missing = (Integer) raw.get("b");
        System.out.println(found + " " + missing + " " + (missing == null));
        try {
            int bad = (Integer) raw.get("b");
            System.out.println(bad);
        } catch (NullPointerException e) {
            System.out.println("npe");
        }

        HashSet<String> linked = new LinkedHashSet<>();
        linked.add("z");
        linked.add("a");
        linked.add("m");
        System.out.println(linked);

        TreeSet<Thing> sorted = new TreeSet<>();
        for (int i = 0; i < 20000; i++) sorted.add(new Thing((i * 7919) % 20011));
        long sum = 0;
        while (sorted.size() > 10000) {
            Thing first = sorted.first();
            sum += first.id;
            sorted.remove(first);
        }
        System.out.println(sum + " " + sorted.first().id + " " + sorted.size());
        System.out.println("abcabc".indexOf("c", 3) + " " + "abc".indexOf("", 2) + " " + "abc".indexOf("z", 1));
    }
}
