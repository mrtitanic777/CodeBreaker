import java.util.function.*;

public class Names {
    static int sizeof(int auto) { return auto * 2; }

    public static void main(String[] args) {
        int register = 7;
        int self = 3;
        int a$b = 5;
        Supplier<Integer> sr = () -> register + self + a$b;
        IntUnaryOperator twice = x -> x + register;
        String s = "é😀x\t1";
        System.out.println(sr.get() + " " + twice.applyAsInt(1) + " " + sizeof(4) + " " + s.length() + " " + (int) s.charAt(1));
    }
}
