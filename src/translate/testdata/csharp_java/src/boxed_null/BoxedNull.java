public class BoxedNull {
    // a boxed value that is null: Java keeps it a reference, the translation a value: declined
    static Integer absent() { Integer x = null; return x; }

    static int present() { Integer x = 5; return x + 1; }
}
