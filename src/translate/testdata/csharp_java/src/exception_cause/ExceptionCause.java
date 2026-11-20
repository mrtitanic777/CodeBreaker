public class ExceptionCause {
    static void wrap() { throw new IllegalArgumentException("bad", new RuntimeException("cause")); }

    static void plain() { throw new IllegalArgumentException("bad"); }
}
