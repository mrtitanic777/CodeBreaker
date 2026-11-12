// FaithfulVa.java -- translated from C by CodeBreaker (faithful tier: C memory emulated in crt.Mem).
import crt.*;

import static crt.LibC.*;

public final class FaithfulVa {

    static int total(int n, Object... va) {
        int frame = Mem.enter(16);
        try {
            int vaList = Mem.varargs(va);
            int ap = frame;  // char *ap
            int s = 0;
            Mem.setPtr(ap, vaList);
            for (int i = 0; i < n; i++) {
                s += Mem.getInt(Mem.postAddPtr(ap, 8));
            }
            discard(0);
            return s;
        } finally {
            Mem.leave(frame);
        }
    }

    static int main() {
        return total(3, 1, 2, 3);
    }

    public static void main(String[] args) {
        exit(main());
    }
}
