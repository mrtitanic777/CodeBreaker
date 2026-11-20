import java.util.*;

public class Mod {
    enum Color { RED, GREEN, BLUE }
    int field;

    static int sw(Color c) { return switch (c) { case RED -> 1; case GREEN -> 2; default -> 3; }; }
    static String sw2(int v) { String s = switch (v) { case 1, 2 -> "low"; case 3 -> { String t = "m"; yield t + t; } default -> "hi"; }; return s; }
    static String sw3(String s) { return switch (s) { case "a", "b" -> "ab"; default -> "other"; }; }
    static int sw4(int v) { int r; r = switch (v) { case 0: yield 5; default: yield 6; }; return r; }
    void sw5(int v) { this.field = switch (v) { case 0 -> 1; default -> 2; }; }
    static void sw6(List<Integer> l, int v) { l.add(switch (v) { case 0 -> 1; default -> 2; }); }
    static int sw7(int v) { return 1 + switch (v) { case 0 -> 1; default -> 2; }; }
    static int sw8(Color c) { switch (c) { case RED: return 1; case GREEN: return 2; default: return 3; } }
    static int sw9(int v) { return switch (v) { case 0 -> throw new IllegalStateException(); default -> v; }; }
    static String sw10(String s) { String r = switch (s) { case "a" -> "x"; case "b" -> { yield "y"; } default -> "z"; }; return r; }
    static int sw11(int v) { int r = switch (v) { case 0 -> 1; case 1 -> { if (v > 3) { yield 4; } yield 5; } default -> 2; }; return r; }
    static int pat(Object o) { if (o instanceof String s && s.length() > 2) return s.length(); if (o instanceof Integer i) return i; return 0; }
    static int pat2(Object o) { if (!(o instanceof Mod m)) return -1; return m.field; }
    static int pat3(Object o) { int r = 0; if (o instanceof Mod m) r = m.field; if (o instanceof Mod m2) r += m2.field; else r--; while (o instanceof String s) { r += s.length(); o = null; } return r; }
    static int pat4(Object o) { return o instanceof Mod m ? m.field : 0; }
    static int each(int[] xs, List<String> ys, Set<Integer> zs) { int r = 0; for (int x : xs) r += x; for (String y : ys) r += y.length(); for (Integer z : zs) r += z; lbl: for (int x : xs) { if (x > 2) continue lbl; r++; } return r; }
    static int each2(Map<String, Integer> m) { int r = 0; for (Map.Entry<String, Integer> e : m.entrySet()) r += e.getValue(); for (var k : m.keySet()) r += k.length(); return r; }
    static int each3(List<? extends Number> l) { int r = 0; for (Number n : l) r += n.intValue(); return r; }
    static int each4(int[][] g) { int r = 0; for (int[] row : g) for (int v : row) r += v; return r; }
    static int each5(Iterable<String> it) { int r = 0; for (String s : it) { r += s.length(); } return r; }
    static int each6(List raw) { int r = 0; for (Object o : raw) r++; return r; }
    static <T extends Comparable<T>> int each7(List<T> l) { int r = 0; for (T t : l) r++; return r; }
}
