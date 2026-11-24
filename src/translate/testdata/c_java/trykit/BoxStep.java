// x++ and x-- of a box wherever a box can be held: each kind of number, the value it gives, a place worked out once, wrapping, and a null.
public class BoxStep {
    static Integer counter = 5;
    Long own = 10L;
    static int calls = 0;

    static Integer[] held = {1, 2, 3};

    static int index() {
        calls++;
        return 1;
    }

    BoxStep self() {
        calls++;
        return this;
    }

    public static void main(String[] args) {
        Integer i = 1;
        int a = i++;
        int b = ++i;
        Integer c = i--;
        Integer d = --i;
        System.out.println(a + " " + b + " " + c + " " + d + " " + i);
        counter++;
        System.out.println(counter-- + " " + counter);
        BoxStep o = new BoxStep();
        o.self().own++;
        System.out.println(o.own + " " + calls);
        held[index()]++;
        System.out.println(held[1] + " " + calls);
        Long l = Long.MAX_VALUE;
        l++;
        Integer m = Integer.MAX_VALUE;
        m++;
        Short s = Short.MAX_VALUE;
        s++;
        Byte y = Byte.MIN_VALUE;
        y--;
        Character ch = 'a';
        ch++;
        Double v = 1.5;
        v--;
        Float f = 2.5f;
        f++;
        System.out.println(l + " " + m + " " + s + " " + y + " " + ch + " " + v + " " + f);
        for (Integer n = 0; n < 3; n++) System.out.print(n + " ");
        System.out.println();
        Integer total = 7;
        total += 3;
        total *= 2;
        total /= 3;
        total %= 4;
        total <<= 3;
        total >>= 1;
        total >>>= 1;
        total ^= 5;
        total |= 8;
        total &= 12;
        Long big = 1L;
        big += total;
        big <<= 40;
        Double half = 1.0;
        half /= 4;
        half %= 0.2;
        Short small = 30000;
        small = (short) (small + 1);
        Character next = 'x';
        next = (char) (next + 1);
        held[index()] += 10;
        o.self().own -= 3;
        counter *= 3;
        System.out.println(total + " " + big + " " + half + " " + small + " " + next + " " + held[1] + " " + o.own + " " + counter + " " + calls);
        Integer shown = 2;
        System.out.println((shown += 5) + " " + shown);
        Integer zero = 4;
        try {
            zero /= zero - 4;
        } catch (ArithmeticException e) {
            System.out.println("div " + zero);
        }
        Integer absent = null;
        try {
            absent += index();
        } catch (NullPointerException e) {
            System.out.println("npe before the right side " + calls);
        }
        Integer none = null;
        try {
            none++;
        } catch (NullPointerException e) {
            System.out.println("npe " + (none == null));
        }
    }
}
