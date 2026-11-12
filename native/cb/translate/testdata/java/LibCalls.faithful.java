// LibCalls.java -- translated from C by CodeBreaker (faithful tier: C memory emulated in crt.Mem).
import crt.*;

import static crt.LibC.*;

public final class LibCalls {

    interface IntIntToInt { int call(int a0, int a1); }
    static final int FN_cmp_int = Fn.register((IntIntToInt) LibCalls::cmp_int);

    static int cmp_int(int a, int b) {
        return Mem.getInt(a) - Mem.getInt(b);
    }

    static int use_mystery(int v) {
        // library function mystery: not in the C runtime for Java (api_maps/libc.json)
        throw new UnsupportedOperationException("CodeBreaker gap: use_mystery: library function mystery: not in the C runtime for Java (api_maps/libc.json)");
    }

    static int main() {
        int frame = Mem.enter(16);
        try {
            int v = frame;  // int v[4]
            Mem.setInts(v, 3, 1, 2, 0);
            int f = FN_cmp_int;
            qsort(v, 4L, 4L, LibCalls::cmp_int);
            qsort(v, 4L, 4L, (a, b) -> ((IntIntToInt) Fn.at(f)).call(a, b));
            printf("%d %s %f\n", Mem.getInt(v), "txt", Math.sqrt(2.0));
            return (int) strlen("hello");
        } finally {
            Mem.leave(frame);
        }
    }

    public static void main(String[] args) {
        exit(main());
    }
}
