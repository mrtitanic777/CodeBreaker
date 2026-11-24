public class Misc2 {
    static int sum(int... xs) {
        int s = 0;
        for (int x : xs) s += x;
        return s;
    }
    static String join(String sep, String... parts) {
        StringBuilder sb = new StringBuilder();
        for (int i = 0; i < parts.length; i++) {
            if (i > 0) sb.append(sep);
            sb.append(parts[i]);
        }
        return sb.toString();
    }
    static String level(int n) {
        StringBuilder sb = new StringBuilder();
        switch (n) {
            case 3: sb.append("three ");
            case 2: sb.append("two ");
            case 1: sb.append("one "); break;
            case 0: sb.append("zero ");
            default: sb.append("many ");
        }
        return sb.toString();
    }
    public static void main(String[] args) {
        System.out.println(sum() + " " + sum(1) + " " + sum(1, 2, 3) + " " + sum(new int[]{4, 5}));
        System.out.println(join("-") + "|" + join("-", "a") + "|" + join("-", "a", "b", "c"));
        for (int i = 0; i <= 4; i++) System.out.println(i + ": " + level(i));
        Integer a = 127, b = 127, c = 128, d = 128;
        System.out.println((a == b) + " " + (c == d) + " " + c.equals(d) + " " + (c.intValue() == d));
    }
}
