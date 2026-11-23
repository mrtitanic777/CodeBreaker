import java.util.ArrayList;
import java.util.List;

public class JumpKit {
    static final List<String> log = new ArrayList<>();

    interface Op<T> { T apply(T x); default T twice(T x) { return apply(apply(x)); } }
    interface Two { int add(int a, int b); }
    interface Sink { void take(String s); }
    interface Named extends Sink { default String name() { return "named"; } }

    static class R implements AutoCloseable {
        final String n; final boolean bad;
        R(String n, boolean bad) { this.n = n; this.bad = bad; }
        @Override public void close() { log.add("close " + n); if (bad) throw new IllegalStateException(n); }
    }

    static int twoFinallys() {
        for (int i = 0; i < 3; i++) {
            try {
                try {
                    if (i == 1) break;
                    log.add("body" + i);
                } finally {
                    log.add("in" + i);
                }
            } finally {
                log.add("out" + i);
            }
        }
        return log.size();
    }

    static int throwInFinallyOnReturn() {
        try {
            try {
                return 1;
            } finally {
                if (log != null) throw new RuntimeException("from finally");
            }
        } catch (RuntimeException e) {
            log.add(e.getMessage());
            return 2;
        }
    }

    static int conditionalOverride(boolean over) {
        try {
            throw new RuntimeException("x");
        } catch (RuntimeException e) {
            log.add("caught");
        } finally {
            if (over) return 9;
        }
        return 3;
    }

    static int breakInFinally() {
        int n = 0;
        while (true) {
            try {
                n++;
                throw new RuntimeException("dropped");
            } finally {
                if (n == 2) break;
                if (n < 2) continue;
            }
        }
        return n;
    }

    static int valueKept() {
        int v = 1;
        try {
            return v;
        } finally {
            v = 50;
            log.add("v" + v);
        }
    }

    static String switchInTry(int k) {
        String out = "";
        for (int i = 0; i < 3; i++) {
            try {
                switch (k + i) {
                    case 1: out += "a"; break;
                    case 2: out += "b"; continue;
                    default: out += "c";
                }
                out += "|";
            } finally {
                out += i;
            }
        }
        return out;
    }

    static int resourceBreak() {
        int c = 0;
        outer:
        for (int i = 0; i < 3; i++) {
            try (R a = new R("a" + i, false); R b = new R("b" + i, false)) {
                for (int j = 0; j < 3; j++) {
                    c++;
                    if (j == 1) continue outer;
                    if (i == 1) break outer;
                }
            }
        }
        return c;
    }

    static int closeFailsOnReturn() {
        try (R a = new R("fails", true)) {
            return 1;
        } catch (IllegalStateException e) {
            log.add("caught " + e.getMessage() + " " + e.getSuppressed().length);
            return 2;
        }
    }

    static void manySuppressed() {
        try (R a = new R("s1", true); R b = new R("s2", true); R c = new R("s3", true)) {
            throw new IllegalArgumentException("main");
        } catch (IllegalArgumentException e) {
            log.add(e.getMessage());
            for (Throwable t : e.getSuppressed()) log.add("sup " + t.getMessage());
        }
    }

    static int plusOne(int x) { return x + 1; }

    static void lambdas() {
        Op<Integer> inc = x -> x + 1;
        Op<String> dbl = s -> s + s;
        Op<Integer> ref = JumpKit::plusOne;
        Two add = (a, b) -> a + b;
        Named sink = s -> log.add("sink " + s);
        System.out.println(inc.apply(1) + " " + inc.twice(5) + " " + dbl.twice("ab") + " " + ref.apply(9) + " " + add.add(2, 3));
        sink.take("x");
        System.out.println(sink.name());
        Op<Integer> inTry = x -> {
            try {
                return x * 2;
            } finally {
                log.add("lambda finally");
            }
        };
        try {
            System.out.println(inTry.apply(4));
        } finally {
            log.add("outer of lambda");
        }
    }

    static void show(String t) { System.out.println(t + ": " + log); log.clear(); }

    public static void main(String[] args) {
        System.out.println(twoFinallys()); show("two");
        System.out.println(throwInFinallyOnReturn()); show("thrown");
        System.out.println(conditionalOverride(true) + " " + conditionalOverride(false)); show("cond");
        System.out.println(breakInFinally());
        System.out.println(valueKept()); show("kept");
        System.out.println(switchInTry(0) + " " + switchInTry(1));
        System.out.println(resourceBreak()); show("resBreak");
        System.out.println(closeFailsOnReturn()); show("closeFails");
        manySuppressed(); show("many");
        lambdas(); show("lambdas");
    }
}
