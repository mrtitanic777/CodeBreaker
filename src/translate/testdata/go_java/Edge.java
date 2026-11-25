public class Edge {
    static int sideEffects = 0;
    static int next() { return ++sideEffects; }

    static String describe(char c) {
        if (Character.isDigit(c)) return "digit";
        if (Character.isLetter(c)) return Character.isUpperCase(c) ? "upper" : "lower";
        return c == ' ' ? "space" : "other";
    }

    static long gcd(long a, long b) { return b == 0 ? a : gcd(b, a % b); }

    static int[] sieve(int n) {
        boolean[] composite = new boolean[n + 1];
        int count = 0;
        for (int i = 2; i <= n; i++) {
            if (!composite[i]) {
                count++;
                for (long j = (long) i * i; j <= n; j += i) composite[(int) j] = true;
            }
        }
        int[] primes = new int[count];
        int k = 0;
        for (int i = 2; i <= n; i++) if (!composite[i]) primes[k++] = i;
        return primes;
    }

    static int nested(int a, int b) {
        int r = 0;
        loop:
        while (true) {
            switch (a) {
                case 0:
                    break loop;
                case 1:
                    r += 10;
                    break;
                default:
                    r += b;
            }
            a--;
            if (a < -3) break;
        }
        return r;
    }

    public static void main(String[] args) {
        int[] arr = new int[5];
        int i = 0;
        arr[i++] = i;
        arr[i++] = i * 10;
        arr[next()] += next();
        System.out.println(java.util.Arrays.toString(arr) + " " + i + " " + sideEffects);
        int x = 5;
        x += x++ + ++x;
        System.out.println(x);
        x = x-- - --x;
        System.out.println(x);
        System.out.println((5 & 3) + " " + (5 | 3) + " " + (5 ^ 3) + " " + (~5) + " " + (-17 >> 2) + " " + (-17 >>> 28) + " " + (1L << 63));
        System.out.println(Integer.MIN_VALUE / -1 + " " + Integer.MIN_VALUE % -1 + " " + Math.abs(Integer.MIN_VALUE) + " " + (-Integer.MIN_VALUE));
        System.out.println((int) 1e10 + " " + (int) -1e10 + " " + (int) Double.NaN + " " + (long) 3.99e18 + " " + (char) 65.7 + " " + (byte) -129 + " " + (short) 70000);
        System.out.println(0.1f + 0.2f);
        System.out.println(1e7 + " " + 1.0E-4 + " " + 123456789.0 + " " + 0.001 + " " + 100.0 + " " + (float) 1e10 + " " + 1.0 / 0 + " " + -1.0 / 0 + " " + 0.0 / 0);
        System.out.println(5 / 2.0 + " " + 5 / 2 + " " + 5 % 2.5 + " " + (double) 7 / 2 + " " + (1 + 2 + "x" + 1 + 2));
        char c = 'a';
        c += 2;
        c++;
        System.out.println(c + " " + (c + 1) + " " + (char) (c + 1) + " " + describe(c) + " " + describe('Q') + " " + describe('7') + " " + describe(' '));
        String s = "a,b,,c,,";
        String[] parts = s.split(",");
        System.out.println(parts.length + " " + String.join("|", parts) + " " + "  trim me ".trim() + "." + "x".repeat(3) + "abc".compareTo("abd") + "b".compareTo("a"));
        System.out.println("Hello".hashCode() + " " + "".hashCode() + " " + "hello world".indexOf('o') + " " + "hello world".lastIndexOf("o") + " " + "Hello".equals("hello") + " " + "Hello".equalsIgnoreCase("hello"));
        String t = null;
        System.out.println("null concat: " + t + " " + null);
        System.out.println(gcd(1071, 462) + " " + java.util.Arrays.toString(sieve(50)));
        System.out.println(nested(1, 4) + " " + nested(3, 4) + " " + nested(0, 1));
        long big = Long.MAX_VALUE;
        big++;
        System.out.println(big + " " + Long.MIN_VALUE + " " + (big == Long.MIN_VALUE));
        double d = 10;
        d /= 4;
        int n = 7;
        n *= 1.5;
        n -= 0.5;
        System.out.println(d + " " + n);
        System.out.println(String.format("%5d|%-5d|%05d|%+d|%,d|%x|%X|%o", 42, 42, 42, 42, 1234567, 255, 255, 8).replace('|', ';'));
        System.out.println(String.format("%.3f %10.2f %-10.1f| %e", Math.PI, Math.E, 2.5, 12345.678).length() > 0);
        System.out.println(String.format("%s %S %c %b %n", "str", "up", 'z', true).trim());
        System.out.println(String.format("%.0f %.1f %.2f", 0.5, 0.25, 1.005));
        StringBuilder sb = new StringBuilder("abc");
        sb.insert(0, 12).append(3.5).append(true).append('c').append(7L).reverse();
        sb.setCharAt(0, 'Z');
        sb.deleteCharAt(1);
        System.out.println(sb + " " + sb.length() + " " + sb.indexOf("c"));
        int[][] tri = new int[4][];
        for (int r = 0; r < 4; r++) {
            tri[r] = new int[r + 1];
            tri[r][0] = tri[r][r] = 1;
            for (int j = 1; j < r; j++) tri[r][j] = tri[r - 1][j - 1] + tri[r - 1][j];
        }
        System.out.println(java.util.Arrays.toString(tri[3]) + " " + tri.length + " " + tri[2].length);
        
        int total = 0;
        for (int a = 0; a < 3; a++) {
            for (int b = 0; b < 3; b++) {
                if (b == a) continue;
                if (a + b > 3) break;
                total += a * 10 + b;
            }
        }
        System.out.println(total);
        int sw = 3;
        switch (sw) {
            case 1, 2 -> System.out.println("small");
            case 3 -> System.out.println("three");
            default -> System.out.println("other");
        }
        String res = switch (sw) { case 3 -> "yes"; default -> "no"; };
        System.out.println(res);
    }
}
