// GapGoto.java -- translated from C by CodeBreaker (faithful tier: C memory emulated in crt.Mem).
import crt.*;

import static crt.LibC.*;

public final class GapGoto {

    static int tangle(int n) {
        // goto mid both forward and backward in tangle
        throw new UnsupportedOperationException("CodeBreaker gap: tangle: goto mid both forward and backward in tangle");
    }

    static int plain(int a) {
        return a + 1;
    }

    static int main() {
        return plain(2) + (tangle(0) > 100 ? 1 : 0);
    }

    public static void main(String[] args) {
        exit(main());
    }
}
