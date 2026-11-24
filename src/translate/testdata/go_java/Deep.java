public class Deep {
    static int depth(int n) { return n == 0 ? 0 : 1 + depth(n - 1); }
    static long sum(long[] a, int i) { return i == a.length ? 0 : a[i] + sum(a, i + 1); }
    public static void main(String[] args) {
        System.out.println(depth(9000));
        long[] a = new long[8000];
        for (int i = 0; i < a.length; i++) a[i] = i;
        System.out.println(sum(a, 0));
    }
}
