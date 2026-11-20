import java.util.*;

public class Ops {
    public static int total;
    public static long big = -9223372036854775808L;
    public static int small = -2147483648;
    public static float f1 = 1.5F;
    public static double d1 = 1e10;
    public static double d2 = 0.1;
    public static double d3 = 1.0E-5;
    public static float f2 = 3.0e-7f;
    public static double d4 = 100.0;
    public static char c1 = 'a';
    public static char c2 = '\n';
    public static char c3 = '\'';
    public static char c4 = '\\';
    public static char c5 = 'é';
    public static char c6 = '￿';
    public static char c7 = (char) 200;
    public static String s1 = "he said \"hi\"\n\t\\ § é 日 \r end";
    public static byte b1 = (byte) 200;
    public static short sh = 12345;
    public static boolean flag = true;
    public static int cnt;
    public int[] cells = new int[8];
    public int[][] grid = new int[3][4];
    public int[][] jag = new int[3][];
    public String[] names = new String[]{"a", "b"};
    public long[] longs;
    public double[][][] cube = new double[2][2][2];
    public Object o;
    private Integer boxed = 5;
    public Ops next;

    public int shifts(int a, int b) {
        int x = a >>> b;
        x >>>= 2;
        long y = (long) a << 33 >>> 3;
        total >>>= 1;
        this.cells[0] >>>= 1;
        return x + (int) y + (a >> 2 & 0xff | b ^ 3);
    }

    public int shiftField(int a) {
        total >>>= a;
        this.cnt >>>= 1;
        long q = 5L;
        q >>>= 2;
        return (int) q;
    }

    public float fmods(float a, double b) {
        float c = a % 2.5F;
        double d = b % 3.0;
        return c + (float) d;
    }

    public float fmodAssign(float a) {
        a %= 2.0F;
        return a;
    }

    public int casts(double d, long l, float f, char c, byte b) {
        int i = (int) d + (int) l + (int) f + c + b;
        long j = (long) d + (long) f;
        byte bb = (byte) i;
        char cc = (char) (i + 1);
        short ss = (short) l;
        float ff = (float) d;
        double dd = (double) l;
        return i + (int) j + bb + cc + ss + (int) ff + (int) dd;
    }

    public String ternaries(int a, String s, Object ob) {
        String r = a > 0 ? s : null;
        Object q = a > 1 ? ob : s;
        double m = a > 2 ? 1 : 2.5;
        Object z = a > 3 ? null : null;
        return r == null ? "none" : r + (a > 4 ? 'x' : 'y');
    }

    public int arrays(int n) {
        int[] a = new int[n];
        int[] b = {1, 2, 3};
        int[][] c = {{1, 2}, {3}};
        int[][] d = new int[n][n];
        String[] e = new String[n];
        Ops[] f = new Ops[]{new Ops(), null};
        long[][][] g = new long[2][3][4];
        int[][] h = new int[2][];
        a[0] = b[1] + c[1][0] + d[0][0] + e.length + f.length + (int) g[0][0][0] + h.length;
        a[n - 1]++;
        --a[0];
        return a[0] + a.length + cells.length + this.grid[1][2] + jag.length;
    }

    public int loops(int n) {
        int s = 0;
        for (int i = 0, j = n; i < j; i++, j--) s += i * j;
        int k;
        for (k = 0; k < 3; k++) { s += k; }
        for (;;) { if (s > 10) break; s++; }
        while (s < 100) { s += 7; if (s % 2 == 0) continue; s++; }
        do { s--; } while (s > 50);
        outer:
        for (int a = 0; a < 5; a++) {
            for (int b = 0; b < 5; b++) {
                if (a * b == n) { s += 1; break outer; }
                if (b > a) continue outer;
            }
        }
        lbl: { s++; if (s > 0) break lbl; s--; }
        int w = 0;
        wl: while (w < 3) { w++; if (w == 2) continue wl; }
        dl: do { w--; if (w == 1) break dl; } while (w > 0);
        return s;
    }

    public int switches(int v, char ch) {
        int r = 0;
        switch (v & 3) { case 0: r = 1; case 1: r += 2; break; case 2: { r = 3; break; } default: r = -1; }
        switch (ch) { case 'a': r++; break; case 'b': case 'c': r += 2; break; }
        switch (v) { default: break; }
        return r;
    }

    public String strSwitch(String s) {
        switch (s) { case "a": return "A"; case "b": case "c": return "BC"; default: return "?"; }
    }

    public int strSwitch2(String s) {
        int r = 0;
        switch (s) { case "x": r = 1; break; case "y": r = 2; break; default: r = 3; }
        return r;
    }

    public int strSwitch3(String s) {
        int r = 0;
        switch (s) { case "x": r = 1; case "y": r = 2; break; default: r = 3; }
        return r;
    }

    public int order(int i) {
        int[] p = new int[10];
        p[i] = i++;
        int a = f(i++, i++);
        int b = f(g(), g());
        int c = g() + g() * i;
        total += f(total++, 1);
        cells[i] = i = 3;
        String s = "a" + g() + g();
        return a + b + c + p[0];
    }

    static int g() { return cnt++; }
    static int f(int a, int b) { return a - b; }

    public String strs(int i, long l, char c, double d, boolean b, Object o, String s) {
        String r = "a" + i + l + c + d + b + o + s + null;
        r += i;
        r += "x";
        s = s + 1;
        return r + s.length() + s.charAt(0) + s.substring(1) + s.equals("q") + s.indexOf('x');
    }

    public int boxes(Integer a, Long b, Double d, Boolean z, Character ch) {
        int x = a.intValue() + b.intValue() + (int) d.doubleValue();
        boolean y = a.equals(3) || z.booleanValue();
        Integer n = new Integer(5);
        Object ob = a;
        if (ob instanceof Integer) x++;
        if (ob instanceof String) x--;
        if (ob instanceof Character) x--;
        if (ob instanceof Boolean) x--;
        Object[] arr = new Object[]{"a"};
        if (arr instanceof String[]) x++;
        if (ob instanceof Ops) x++;
        return x + (y ? 1 : 0) + n;
    }

    public String objs(Object a, Ops b) {
        String s = a.toString() + a.hashCode() + a.getClass() + b.getClass();
        boolean e = a.equals(b) || a == b || b.equals(a);
        Class k = Ops.class;
        Class k2 = String.class;
        Class k3 = Integer.class;
        Class k4 = Object.class;
        Object nul = null;
        if (nul == null) return "n";
        if (a != null && a == nul) return "m";
        return s + e + k + k2 + k3 + k4;
    }

    public double maths(double x, int n) {
        return Math.sin(x) + Math.abs(n) + Math.floor(x) + Math.pow(x, 2) + Math.sqrt(x) + Math.PI + Math.max(n, 3)
            + Math.min(x, 1.0) + Math.round(x) + Math.atan2(x, x) + Math.random() + Math.signum(x) + Math.ceil(x);
    }

    public void lib(int n) {
        System.out.println("hi");
        System.err.println(n);
        int m = Integer.MAX_VALUE + Integer.parseInt("3");
        long t = System.currentTimeMillis() + System.nanoTime();
        StringBuilder sb = new StringBuilder();
        sb.append(n).append("x").append('c');
        List<String> list = new ArrayList<String>();
        list.add("a");
        Map<String, Integer> map = new HashMap<String, Integer>();
        map.put("a", 1);
        Iterator<String> it = list.iterator();
        while (it.hasNext()) { String s = it.next(); }
        String f = String.format("%d %s", n, "x");
        List<Integer> asl = Arrays.asList(1, 2, 3);
        java.io.File file = new java.io.File("a/" + n);
        file.delete();
        Random rnd = new Random(5L);
        rnd.nextInt(10);
        Thread th = new Thread();
        char ch = Character.toUpperCase('a');
        String vs = String.valueOf(n);
        double pi = Math.PI;
        int[] copy = new int[3];
        System.arraycopy(cells, 0, copy, 0, 3);
        System.exit(0);
    }

    public Object refl(String n) throws Exception {
        Object x = Class.forName(n).newInstance();
        return x;
    }

    public void sync() {
        synchronized (this) { total++; }
    }

    public Runnable anon() {
        return new Runnable() { public void run() { total++; } };
    }

    public Runnable lam() {
        return () -> total++;
    }

    public Object local() {
        class Loc {}
        return new Loc();
    }

    public int tries(int a) {
        int r = 0;
        try { r = 10 / a; } catch (ArithmeticException e) { r = -1; }
        try { r += 1; } finally { total++; }
        try { r += 2; } catch (RuntimeException e) { r--; } finally { total--; }
        try { Object o = null; o.hashCode(); } catch (Exception e) { r++; }
        try { r++; } catch (IllegalStateException | IllegalArgumentException e) { r--; }
        try { return r; } finally { total += 5; }
    }

    public int tryLoop(int n) {
        int r = 0;
        for (int i = 0; i < n; i++) {
            try { if (i == 2) continue; if (i == 4) break; r += i; } finally { r++; }
        }
        return r;
    }

    public int tryInner(int n) {
        try { try { return n; } finally { total++; } } finally { total--; }
    }

    public void tryRes() throws Exception {
        try (java.io.Reader rd = new java.io.StringReader("x")) { rd.read(); }
    }

    public void thrower(int n) {
        if (n < 0) throw new IllegalArgumentException("neg");
    }

    public int tryNpe(Ops o) {
        try { return o.cells.length + o.next.cells[0] + o.next.next.grid.length; } catch (NullPointerException e) { return -1; }
    }

    public int tryLabelled(int n) {
        int r = 0;
        lab: for (int i = 0; i < n; i++) {
            try { if (i > 1) break lab; r++; } finally { r += 2; }
        }
        return r;
    }

    public void tryVoid() {
        try { total++; if (total > 3) return; } finally { total--; }
    }

    public int tryThrow(int a) {
        try { if (a > 1) throw new RuntimeException("x"); return 1; } catch (RuntimeException e) { return 2; } finally { total++; }
    }

    public int nested(Ops o) {
        return o.next.cells[2] + this.next.grid[1][1] + next.next.next.cells.length + o.names.length;
    }

    public Ops chain() { return this; }
    public int callChain() { return this.chain().chain().cells[0] + chain().next.cells[1]; }
    public int unaries(int a, long b, boolean c) { int r = -a + +a + ~a; r = (int) -b; return c ? r : !c ? 1 : 0; }
    public boolean cmp(int a, long b, double c, String s, Object o) { return a == b || a != c && s == null || o != null && s != "q" && a < b && a >= c; }
    public void compound(int a) { a += 1; a -= 2; a *= 3; a /= 4; a %= 5; a <<= 1; a >>= 1; a &= 7; a |= 8; a ^= 9; total += a; this.cells[a & 3] += a; this.cnt -= 1; }
}
