import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

// What the C# translation must do as Java does with boxes: a box can be null, unboxing a null throws, == on boxes compares references.
public class BoxKit {
    static Integer field;
    Long own;
    Boolean flag;
    Double ratio;
    Character letter;

    static Integer maybe(boolean give) { return give ? 7 : null; }
    static int plus(Integer a, int b) { return a + b; }
    static String show(Object o) { return o == null ? "nothing" : "value:" + o; }

    static void nulls() {
        Integer a = null;
        System.out.println(a + " " + (a == null) + " " + field + " " + maybe(false) + " " + maybe(true));
        a = 5;
        System.out.println(a + 1);
        a = null;
        Long l = null;
        Double d = null;
        Boolean b = null;
        Character c = null;
        System.out.println(l + " " + d + " " + b + " " + c);
        BoxKit k = new BoxKit();
        System.out.println(k.own + " " + k.flag + " " + k.ratio + " " + k.letter);
        k.own = 9L;
        k.flag = true;
        k.ratio = 1.5;
        k.letter = 'q';
        System.out.println(k.own + " " + k.flag + " " + k.ratio + " " + k.letter);
        System.out.println(show(null) + " " + show(3) + " " + show(3L) + " " + show(2.5) + " " + show('x') + " " + show(true));
    }

    static void throwing() {
        Integer none = null;
        try {
            int x = none;
            System.out.println("no throw " + x);
        } catch (NullPointerException e) {
            System.out.println("npe assign");
        }
        try {
            System.out.println(none + 1);
        } catch (NullPointerException e) {
            System.out.println("npe add");
        }
        try {
            System.out.println(plus(none, 1));
        } catch (NullPointerException e) {
            System.out.println("npe argument");
        }
        Boolean no = null;
        try {
            if (no) System.out.println("yes");
        } catch (NullPointerException e) {
            System.out.println("npe condition");
        }
        Map<String, Integer> m = new HashMap<>();
        try {
            int v = m.get("missing");
            System.out.println(v);
        } catch (NullPointerException e) {
            System.out.println("npe map");
        }
        try {
            none++;
        } catch (NullPointerException e) {
            System.out.println("npe increment");
        }
        Double dn = null;
        try {
            double y = dn * 2;
            System.out.println(y);
        } catch (NullPointerException e) {
            System.out.println("npe double");
        }
        try {
            switch (none) {
                case 1: System.out.println("one"); break;
                default: System.out.println("other");
            }
        } catch (NullPointerException e) {
            System.out.println("npe switch");
        }
    }

    static void collections() {
        Map<String, Integer> m = new HashMap<>();
        m.put("a", 1);
        Integer got = m.get("zz");
        System.out.println(got + " " + m.get("a") + " " + m.containsKey("n") + " " + m);
        System.out.println(m.getOrDefault("q", 5));
        List<Integer> list = new ArrayList<>();
        list.add(1);
        list.add(3);
        int sum = 0;
        for (Integer x : list) {
            if (x != null) sum += x;
        }
        System.out.println(list + " " + sum);
    }

    static void identity() {
        Integer small1 = 100, small2 = 100, big1 = 1000, big2 = 1000;
        System.out.println((small1 == small2) + " " + big1.equals(big2) + " " + (big1 == 1000));
        Long l1 = 127L, l2 = 127L, l3 = 128L, l4 = 128L;
        System.out.println((l1 == l2) + " " + l3.equals(l4) + " " + l3.equals(128));
        Character c1 = 'a', c2 = 'a';
        Boolean t1 = true, t2 = true;
        System.out.println((c1 == c2) + " " + (t1 == t2));
        Object o1 = 5, o2 = 5L;
        System.out.println(o1.equals(o2) + " " + o1.equals(5) + " " + o1.hashCode() + " " + o2.hashCode());
        Integer x = 3;
        x++;
        x += 2;
        int y = x * x;
        System.out.println(x + " " + y + " " + x.compareTo(7) + " " + x.intValue() + " " + x.doubleValue());
    }

    public static void main(String[] args) {
        nulls();
        throwing();
        collections();
        identity();
    }
}
