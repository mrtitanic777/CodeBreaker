// FaithfulOverloads.java -- translated from C by CodeBreaker (faithful tier: C memory emulated in crt.Mem).
import crt.*;

import static crt.LibC.*;

public final class FaithfulOverloads {

    /** struct Vec: its size and its fields' offsets in memory. */
    static final class Vec {
        static final int SIZE = 8, x = 0, y = 4;
    }

    static int twice(int x) {
        return x * 2;
    }

    static double twice(double x) {
        return x * 2.0;
    }

    static long twice(long x) {
        return x + x;
    }

    static int larger(int a, int b) {
        return a > b ? a : b;
    }

    static double larger(double a, double b) {
        return a > b ? a : b;
    }

    static void bump(int v) {
        Mem.setInt(v, Mem.getInt(v) + 1);
    }

    static int scaled(int result, int v, int k) {
        int frame = Mem.enter(16);
        try {
            int r = frame;  // struct Vec r
            Mem.fill(r, 0, Vec.SIZE);
            Mem.setInt(r + Vec.x, Mem.getInt(v + Vec.x) * k);
            Mem.setInt(r + Vec.y, Mem.getInt(v + Vec.y) * k);
            return Mem.copy(result, r, Vec.SIZE);
        } finally {
            Mem.leave(frame);
        }
    }

    static int main() {
        int frame = Mem.enter(48);
        try {
            int n = frame;  // int n
            Mem.setInt(n, 4);
            bump(n);
            int v = frame + 4;  // struct Vec v
            Mem.setInt(v + Vec.x, 2);
            Mem.setInt(v + Vec.y, 3);
            int w = frame + 12;  // struct Vec w
            Mem.copy(w, scaled(frame + 20, Mem.copy(frame + 28, v, Vec.SIZE), Mem.getInt(n)), Vec.SIZE);
            return twice(Mem.getInt(n)) + (int) twice(1.5) + (int) twice(7L) + larger(Mem.getInt(n), 3) + (int) larger(1.5, 0.5) + Mem.getInt(w + Vec.y);
        } finally {
            Mem.leave(frame);
        }
    }

    public static void main(String[] args) {
        exit(main());
    }
}
