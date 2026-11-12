// ObjDtor.java -- translated from C++ by CodeBreaker (idiomatic tier: structs as classes, pointers as references).
import crt.*;

import static crt.LibC.*;

public final class ObjDtor {

    /** class Base */
    static class Base {
        int n_;

        Base() {
            n_ = 1;
        }

        /** ~Base() */
        void destroy() {
            printf("~Base %d\n", n_);
        }

        int n() {
            return n_;
        }
    }

    /** class Derived */
    static class Derived extends Base {

        Derived() {
            super();
            this.n_ = 2;
        }

        /** ~Derived() */
        void destroy() {
            printf("~Derived\n");
            super.destroy();
        }
    }

    static int main() {
        Base b = new Derived();
        int n = b.n();
        b.destroy();
        return n;
    }

    public static void main(String[] args) {
        exit(main());
    }
}
