// ObjNames.java -- translated from C by CodeBreaker (idiomatic tier: structs as classes, pointers as references).
import crt.*;

import static crt.LibC.*;

public final class ObjNames {

    /** struct counter */
    static class counter_ {
        int v;
        int new_;

        counter_() {
        }

        counter_(int v, int new_) {
            this.v = v;
            this.new_ = new_;
        }
    }

    /** struct (anonymous) 4_1 */
    static class Anon4_1 {
        int a;
    }

    static int counter = 3;
    static final Anon4_1 anon_var = new Anon4_1();

    static int main() {
        counter_ c = new counter_(counter, 2);
        anon_var.a = c.v + c.new_;
        return anon_var.a;
    }

    public static void main(String[] args) {
        exit(main());
    }
}
