import java.util.ArrayList;
import java.util.List;

// What the C translation must do as Java does when a try is left: every way out runs each finally and closes each resource, in order.
public class TryKit {
    static final List<String> log = new ArrayList<>();

    static class Res implements AutoCloseable {
        final String name;
        final boolean failClose;
        Res(String name) { this(name, false); }
        Res(String name, boolean failClose) { this.name = name; this.failClose = failClose; log.add("open " + name); }
        @Override public void close() {
            log.add("close " + name);
            if (failClose) throw new IllegalStateException("close " + name);
        }
    }

    static class Sub extends Res {
        Sub(String n) { super(n); }
        @Override public void close() { log.add("sub"); super.close(); }
    }

    interface Handle extends AutoCloseable { @Override void close(); }

    static void flush(String title) {
        System.out.println(title + ": " + log);
        log.clear();
    }

    static int returned(int x) {
        try {
            return x + 1;
        } finally {
            x = 100;
            log.add("finally " + x);
        }
    }

    static int overridden() {
        try {
            throw new RuntimeException("lost");
        } finally {
            return 7;
        }
    }

    static int nested(int n) {
        try {
            try {
                if (n == 0) return 10;
                if (n == 1) throw new IllegalArgumentException("one");
                log.add("inner end");
            } finally {
                log.add("inner");
            }
            log.add("between");
            return 20;
        } catch (IllegalArgumentException e) {
            log.add("caught " + e.getMessage());
            return 30;
        } finally {
            log.add("outer");
        }
    }

    static int loops() {
        int total = 0;
        for (int i = 0; i < 6; i++) {
            try {
                if (i == 1) continue;
                if (i == 4) break;
                total += i;
            } finally {
                total += 100;
                log.add("f" + i);
            }
        }
        return total;
    }

    static String labelled() {
        StringBuilder b = new StringBuilder();
        try {
            outer:
            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 3; j++) {
                    try {
                        if (j == 1) continue outer;
                        if (i == 2) break outer;
                        b.append(i).append(j).append(' ');
                    } finally {
                        b.append('f');
                    }
                }
            }
            b.append(" after");
            throw new RuntimeException("still caught");
        } catch (RuntimeException e) {
            b.append(' ').append(e.getMessage());
        }
        return b.toString();
    }

    static int switched(int n) {
        int r = 0;
        for (int i = 0; i < 2; i++) {
            switch (n) {
                case 1:
                    try {
                        r += 1;
                        break;
                    } finally {
                        r += 10;
                    }
                default:
                    r += 100;
            }
        }
        return r;
    }

    static int resourceReturn() {
        try (Res a = new Res("a"); Res b = new Res("b")) {
            log.add("body");
            return 5;
        } finally {
            log.add("after");
        }
    }

    static void resourceThrow() {
        try (Res a = new Res("a", true); Res b = new Res("b", true)) {
            throw new RuntimeException("body");
        } catch (RuntimeException e) {
            log.add(e.getMessage() + " suppressed " + e.getSuppressed().length);
            for (Throwable s : e.getSuppressed()) log.add(s.getMessage());
        }
    }

    static void closeThrows() {
        try (Res a = new Res("a", true)) {
            log.add("fine");
        } catch (IllegalStateException e) {
            log.add("from close: " + e.getMessage() + " " + e.getSuppressed().length);
        }
    }

    static void nullResource() {
        try (Res none = null) {
            log.add("null is " + (none == null));
        }
    }

    static void existing() {
        Res r = new Res("held");
        try (r) {
            log.add("using " + r.name);
        }
        log.add("still " + r.name);
    }

    static void kinds() {
        try (Sub s = new Sub("s")) {
            log.add(s.name);
        }
        Handle h = () -> log.add("handle closed");
        try (h) {
            log.add("handle");
        }
    }

    static void inLoop() {
        for (int i = 0; i < 4; i++) {
            try (Res r = new Res("r" + i)) {
                if (i == 1) continue;
                if (i == 2) break;
                log.add("kept " + i);
            }
        }
    }

    static void failsToOpen() {
        try (Res a = new Res("a"); Res b = make()) {
            log.add("never");
        } catch (RuntimeException e) {
            log.add(e.getMessage());
        }
    }

    static Res make() { throw new RuntimeException("no second"); }

    static int lambdaInFinally() {
        int base = 3;
        try {
            return base;
        } finally {
            Runnable r = () -> log.add("lambda " + base);
            r.run();
        }
    }

    public static void main(String[] args) {
        System.out.println(returned(1));
        flush("returned");
        System.out.println(overridden());
        for (int i = 0; i < 3; i++) {
            System.out.println(nested(i));
            flush("nested" + i);
        }
        System.out.println(loops());
        flush("loops");
        System.out.println(labelled());
        System.out.println(switched(1) + " " + switched(2));
        System.out.println(resourceReturn());
        flush("resourceReturn");
        resourceThrow();
        flush("resourceThrow");
        closeThrows();
        flush("closeThrows");
        nullResource();
        flush("nullResource");
        existing();
        flush("existing");
        kinds();
        flush("kinds");
        inLoop();
        flush("inLoop");
        failsToOpen();
        flush("failsToOpen");
        System.out.println(lambdaInFinally());
        flush("lambda");
        int[][][] cube = new int[2][3][4];
        cube[1][2][3] = 9;
        long[][][] partial = new long[2][2][];
        System.out.println(cube.length + " " + cube[0].length + " " + cube[1][2].length + " " + cube[1][2][3] + " " + (partial[1][1] == null));
        try {
            int[][][] bad = new int[2][-1][3];
            System.out.println(bad.length);
        } catch (NegativeArraySizeException e) {
            System.out.println("negative " + e.getMessage());
        }
        System.out.printf("%s %d %5.1f|%n", "printf", 42, 2.25);
    }
}
