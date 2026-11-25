public class Basics {
    static final int LIMIT = 10;
    static int counter = 0;
    static long total;

    static int fib(int n) {
        if (n < 2) return n;
        return fib(n - 1) + fib(n - 2);
    }

    static long fact(int n) {
        long r = 1;
        for (int i = 2; i <= n; i++) r *= i;
        return r;
    }

    static double avg(int[] xs) {
        int sum = 0;
        for (int i = 0; i < xs.length; i++) sum += xs[i];
        return (double) sum / xs.length;
    }

    static String classify(int n) {
        switch (n % 4) {
            case 0: return "zero";
            case 1:
            case 2: return "low";
            default: return "high";
        }
    }

    static int collatz(long n) {
        int steps = 0;
        while (n != 1) {
            if (n % 2 == 0) n /= 2; else n = 3 * n + 1;
            steps++;
        }
        return steps;
    }

    public static void main(String[] args) {
        System.out.println("fib(20) = " + fib(20));
        System.out.println("fact(15) = " + fact(15));
        int[] data = {3, 1, 4, 1, 5, 9, 2, 6};
        System.out.println("avg = " + avg(data));
        for (int i = 0; i < 6; i++) System.out.println(i + " -> " + classify(i));
        System.out.println(collatz(27));
        int big = Integer.MAX_VALUE;
        big++;
        System.out.println(big);
        System.out.println(7 / 2 + " " + (-7) / 2 + " " + 7 % -3 + " " + -7.5 % 2);
        System.out.println(1 << 33);
        System.out.println(-8 >>> 28);
        System.out.println((byte) 200 + " " + (char) 66 + " " + (int) 3.99 + " " + (long) 1e19);
        double d = 0.1 + 0.2;
        System.out.println(d);
        System.out.println(100.0 / 3);
        System.out.println(1.0f / 3);
        String s = "Hello";
        s += ", World";
        System.out.println(s.length() + " " + s.toUpperCase() + " " + s.substring(7) + " " + s.indexOf("World") + s.charAt(1));
        int[][] grid = new int[3][4];
        for (int i = 0; i < 3; i++) for (int j = 0; j < 4; j++) grid[i][j] = i * j;
        int t = 0;
        for (int[] row : grid) for (int v : row) t += v;
        System.out.println("grid total " + t);
        StringBuilder sb = new StringBuilder();
        for (char c = 'a'; c < 'f'; c++) sb.append(c).append(',');
        System.out.println(sb.toString());
        outer:
        for (int i = 0; i < 5; i++) {
            for (int j = 0; j < 5; j++) {
                if (j == 3) continue outer;
                if (i == 3) break outer;
                counter += i * j;
            }
        }
        System.out.println("counter " + counter);
        long x = 1L << 40;
        total += x;
        System.out.println(total + " " + Long.MAX_VALUE + " " + Math.abs(-5) + " " + Math.max(2.5, 3) + " " + Math.sqrt(2));
        System.out.printf("%5.2f|%-6s|%04d|%x%n", 3.14159, "ab", 42, 255);
        boolean flag = counter > 5 && total != 0 || false;
        System.out.println(flag ? "yes" : "no");
        int k = 0;
        do { k += 3; if (k == 6) continue; } while (k < 10);
        System.out.println(k);
        System.out.println(10 / (k - 12));
    }
}
