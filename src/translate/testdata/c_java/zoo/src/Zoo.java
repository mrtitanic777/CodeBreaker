import java.util.*;

public class Zoo {
    interface Animal { String sound(); default String greet() { return name() + " says " + sound(); } String name(); }

    abstract static class Base implements Animal {
        protected final String name;
        static int made;
        Base(String n) { name = n; made++; describe(); }
        public String name() { return name; }
        void describe() {}
    }

    static class Dog extends Base {
        int barks;
        Dog(String n) { super(n); }
        public String sound() { return "woof"; }
        void describe() { barks = 1; }
    }

    static class Cat extends Base {
        Cat(String n) { super(n); }
        public String sound() { return "meow"; }
    }

    record Pair(String a, int b) {}

    static class Empty extends RuntimeException {
        Empty(String m) { super(m); }
    }

    static int pick(int k) {
        if (k < 0) throw new Empty("negative " + k);
        return k * 3;
    }

    static int sumTo(int n) {
        int s = 0;
        for (int i : new int[] {1, 2, 3, n}) s += i;
        return s;
    }

    public static void main(String[] args) {
        List<Animal> zoo = new ArrayList<>();
        zoo.add(new Dog("rex"));
        zoo.add(new Cat("tom"));
        for (Animal a : zoo) System.out.println(a.greet());
        System.out.println(Base.made + " " + ((Dog) zoo.get(0)).barks + " " + new Pair("x", 2) + " " + sumTo(4));
        try {
            pick(-2);
        } catch (Empty e) {
            System.out.println("caught " + e.getMessage());
        } finally {
            System.out.println("finally");
        }
        Map<String, Integer> m = new LinkedHashMap<>();
        m.put("one", 1);
        m.put("two", 2);
        System.out.println(m + " " + pick(5) + " " + String.format("%5.2f|%d", 3.14159, 42));
        Comparator<String> byLen = (x, y) -> x.length() - y.length();
        List<String> ws = new ArrayList<>(List.of("ccc", "a", "bb"));
        ws.sort(byLen);
        System.out.println(ws);
    }
}
