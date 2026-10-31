import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;
import java.util.NoSuchElementException;

// Generic classes and methods, bounded type variables, a generic interface, Iterable with an anonymous
// Iterator, a generic stack, inner (non-static) classes.
public class Generics {
    static class Box<T> {
        private T value;

        Box(T value) { this.value = value; }

        T get() { return value; }

        void set(T v) { value = v; }

        <R> Box<R> map(java.util.function.Function<T, R> f) { return new Box<>(f.apply(value)); }
    }

    record Pair<A, B>(A first, B second) {
        Pair<B, A> swap() { return new Pair<>(second, first); }
    }

    interface Visitor<R> {
        R visitNumber(int n);
        R visitText(String s);
    }

    static class Stack<T> implements Iterable<T> {
        private final List<T> items = new ArrayList<>();

        void push(T t) { items.add(t); }

        T pop() {
            if (items.isEmpty()) throw new NoSuchElementException("empty stack");
            return items.remove(items.size() - 1);
        }

        boolean isEmpty() { return items.isEmpty(); }

        public Iterator<T> iterator() {
            return new Iterator<T>() {
                int i = items.size() - 1;

                public boolean hasNext() { return i >= 0; }

                public T next() { return items.get(i--); }
            };
        }
    }

    class Counter {
        int count;

        void hit() { count++; total++; }
    }

    int total;

    static <T extends Comparable<T>> T max(List<T> xs) {
        T best = xs.get(0);
        for (T x : xs) if (x.compareTo(best) > 0) best = x;
        return best;
    }

    static <T> int countMatching(List<T> xs, java.util.function.Predicate<T> p) {
        int n = 0;
        for (T x : xs) if (p.test(x)) n++;
        return n;
    }

    static int boxed(int v) {
        Box<Integer> b = new Box<>(v);
        b.set(b.get() + 1);
        Box<String> s = b.map(x -> "n" + x);
        return s.get().length() + b.get();
    }

    static String pairText(int a, String b) {
        Pair<Integer, String> p = new Pair<>(a, b);
        return p.swap().toString() + p.first();
    }

    static int maxOf(int a, int b) {
        List<Integer> xs = new ArrayList<>();
        xs.add(a);
        xs.add(b);
        xs.add(a ^ b);
        return max(xs);
    }

    static String maxWord(String a, String b) {
        List<String> xs = List.of(a, b, "m");
        return max(xs);
    }

    static int stackSum(int n) {
        Stack<Integer> st = new Stack<>();
        for (int i = 0; i < n % 40; i++) st.push(i * i);
        int sum = 0;
        for (int v : st) sum = sum * 3 + v;
        return sum;
    }

    static String visit(int n) {
        Visitor<String> v = new Visitor<>() {
            public String visitNumber(int k) { return "#" + k; }
            public String visitText(String s) { return "'" + s + "'"; }
        };
        return n % 2 == 0 ? v.visitNumber(n) : v.visitText("t" + n);
    }

    int hits(int n) {
        Counter a = new Counter();
        Counter b = this.new Counter();
        for (int i = 0; i < n % 30; i++) {
            a.hit();
            if (i % 3 == 0) b.hit();
        }
        return a.count * 100 + b.count + total * 10000;
    }

    static int innerHits(int n) {
        return new Generics().hits(n);
    }

    public static void main(String[] args) {
        System.out.println(boxed(41) + " " + pairText(7, "seven") + " " + maxOf(12, 5) + " " + maxWord("pear", "apple"));
        Stack<String> st = new Stack<>();
        st.push("a");
        st.push("b");
        st.push("c");
        for (String s : st) System.out.print(s);
        System.out.println(" " + st.pop() + st.pop() + " " + st.isEmpty());
        try {
            st.pop();
            st.pop();
        } catch (NoSuchElementException e) {
            System.out.println("caught: " + e.getMessage());
        }
        System.out.println(stackSum(10) + " " + visit(4) + " " + visit(5) + " " + innerHits(10));
        List<String> words = List.of("alpha", "beta", "gamma", "delta", "epsilon");
        System.out.println(countMatching(words, w -> w.length() > 4) + " " + countMatching(words, w -> w.contains("l")));
    }
}
