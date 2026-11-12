// FaithfulCore.java -- translated from C by CodeBreaker (faithful tier: C memory emulated in crt.Mem).
import crt.*;

import static crt.LibC.*;

public final class FaithfulCore {

    /** union Pun: its size and its fields' offsets in memory. */
    static final class Pun {
        static final int SIZE = 4, f = 0, u = 0, b = 0;
    }

    /** struct Rec: its size and its fields' offsets in memory. */
    static final class Rec {
        static final int SIZE = 16, id = 0, name = 2, weight = 8;
    }

    /** struct Pair: its size and its fields' offsets in memory. */
    static final class Pair {
        static final int SIZE = 8, a = 0, b = 4;
    }

    interface IntIntToInt { int call(int a0, int a1); }

    static int counter = 3;
    static double ratio = 0.5;
    static int limit = 0xEE6B2800;
    static int greeting;
    static final int table = Mem.global(16, 4);  // int table[4]
    static final int origin = Mem.global(Pair.SIZE, 4);  // struct Pair origin
    static int count_n;
    static final int FN_add = Fn.register((IntIntToInt) FaithfulCore::add);
    static final int FN_mul = Fn.register((IntIntToInt) FaithfulCore::mul);

    static {
        greeting = Mem.str("hi\n\"there\"");
        Mem.setInts(table, 1, 2, 3, 4);
        Mem.setInt(origin + Pair.a, 7);
        Mem.setInt(origin + Pair.b, 9);
    }

    static int hash(int s) {
        int h = 0x811C9DC5;
        while (Mem.getByte(s) != 0) {
            h ^= Mem.getByte(s++) & 0xFF;
            h *= 16777619;
        }
        return h;
    }

    static int make(int result, short id, double w) {
        int frame = Mem.enter(16);
        try {
            int r = frame;  // struct Rec r
            Mem.setShort(r + Rec.id, id);
            Mem.setDouble(r + Rec.weight, w);
            Mem.setByte(r + Rec.name, (byte) 'a');
            Mem.setByte(r + Rec.name + 1, (byte) 0);
            return Mem.copy(result, r, Rec.SIZE);
        } finally {
            Mem.leave(frame);
        }
    }

    static int add(int a, int b) {
        return a + b;
    }

    static int mul(int a, int b) {
        return a * b;
    }

    static int apply(int f, int a, int b) {
        return ((IntIntToInt) Fn.at(f)).call(a, b);
    }

    static int classify(int x) {
        switch (x) {
            case 1:
                return 10;
            case 2: case 3:
                x += 4;
                break;
            default:
                x = -1;
        }
        return x;
    }

    static int count() {
        count_n++;
        return count_n + counter;
    }

    static int loops(int n) {
        int s = 0;
        for (int i = 0; i < n; i++) {
            if (i == 3) {
                continue;
            }
            s += i;
        }
        do {
            s--;
        } while (s > 100);
        while (true) {
            if (s > 5) {
                break;
            }
            s++;
        }
        return s;
    }

    static long wide(long a, long b) {
        return a / 7L + Long.remainderUnsigned(b, Integer.toUnsignedLong(5)) + (long) (a < 0L ? 1 : 0);
    }

    static int jump(int n) {
        int r = 0;
        again: while (true) {
            r++;
            if (r < n) {
                continue again;
            }
            break;
        }
        out: {
            if (n > 100) {
                break out;
            }
            r += 2;
        }
        return r;
    }

    static int bytes(byte c, byte d, char e) {
        c += 10;
        d -= 3;
        e <<= 2;
        return (c & 0xFF) + d + e + ((c & 0xFF) >> 1) + (d >> 1) + Integer.divideUnsigned(limit, 3) + ((Integer.compareUnsigned(limit, 5) > 0) ? 1 : 0);
    }

    static double mix(int i, int u, float f, double d) {
        return (double) (i / 2) + (double) Integer.toUnsignedLong(u) / 2.0 + (double) f * d - (double) ((byte) i & 0xFF);
    }

    static void swap(int a, int b) {
        int t = Mem.getInt(a);
        Mem.setInt(a, Mem.getInt(b));
        Mem.setInt(b, t);
    }

    static int sum(int p, int n) {
        int end = p + n * 4;
        int s = 0;
        while (p != end) {
            s += Mem.getInt((p += 4) - 4);
        }
        return s;
    }

    static int main() {
        int frame = Mem.enter(64);
        try {
            int p = frame;  // struct Pun p
            int r = frame + 8;  // struct Rec r
            Mem.copy(r, make(frame + 24, (short) 3, 2.5), Rec.SIZE);
            int q = frame + 40;  // struct Pair q
            Mem.setInt(q + Pair.a, 1);
            Mem.setInt(q + Pair.b, 2);
            int x = frame + 48;  // int x
            Mem.setInt(x, 5);
            int y = frame + 52;  // int y
            Mem.setInt(y, 6);
            Mem.setFloat(p + Pun.f, -1.5f);
            swap(x, y);
            Mem.copy(q, origin, Pair.SIZE);
            Mem.setInt(x, Mem.getInt(x) + (apply(FN_add, Mem.getInt(x), Mem.getInt(y)) + apply(FN_mul, 2, 3)));
            Mem.setInt(x, Mem.getInt(x) + (classify(2) + count() + count() + loops(10) + jump(4)));
            Mem.setInt(x, Mem.getInt(x) + ((int) wide(-100L, Integer.toUnsignedLong(99)) + bytes((byte) 250, (byte) 100, (char) 3) + (int) mix(7, 9, 1.5f, 2.0)));
            Mem.setInt(x, Mem.getInt(x) + (hash(greeting) + sum(table, 4) + (Mem.getInt(p + Pun.u) >>> 28) + Mem.getShort(r + Rec.id) + Mem.getInt(q + Pair.b)));
            Mem.setInt(x, Mem.getInt(x) + (ratio > 0.25 ? 1 : 2));
            return Mem.getInt(x) & 127;
        } finally {
            Mem.leave(frame);
        }
    }

    public static void main(String[] args) {
        exit(main());
    }
}
