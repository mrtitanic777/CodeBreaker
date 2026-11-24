// String.format given one Object[] spreads it into the arguments, as Java's varargs do.
public class FormatKit {
    static String pass(String f, Object... rest) { return String.format(f, rest); }

    public static void main(String[] args) {
        Object[] held = {"E", 3};
        System.out.println(String.format("press %s for %d", held));
        System.out.println(pass("open %s now", "inventory"));
        System.out.println(String.format("%s-%s", "a", "b"));
    }
}
