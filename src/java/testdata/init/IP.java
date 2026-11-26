package t;
// StatList-like: its initialiser reads IQ only through the static methods it calls (make -> fill)
public class IP {
    static int[] table = make(4);
    static int[] make(int n) {
        int[] t = new int[n];
        fill(t);
        return t;
    }
    static void fill(int[] t) {
        t[0] = IQ.water.length;
    }
}
