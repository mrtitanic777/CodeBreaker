// ObjClasses.java -- translated from C++ by CodeBreaker (idiomatic tier: structs as classes, pointers as references).
import crt.*;

import static crt.LibC.*;

public final class ObjClasses {

    /** class Shape */
    static abstract class Shape {
        static int made = 0;
        int id_;

        Shape(int id) {
            id_ = id;
        }

        abstract int area();

        int id() {
            return id_;
        }
    }

    /** class Rect */
    static class Rect extends Shape {
        int w_;
        int h_;

        Rect(int id, int w, int h) {
            super(id);
            w_ = w;
            h_ = h;
            Shape.made++;
        }

        int area() {
            return w_ * h_;
        }

        Rect plus(Rect o) {
            return new Rect(this.id_, w_ + o.w_, h_ + o.h_);
        }
    }

    /** struct Pair */
    static class Pair {
        int a;
        int b;

        Pair() {
        }

        Pair(int a, int b) {
            this.a = a;
            this.b = b;
        }

        /** C's copy of a struct value. */
        Pair(Pair o) {
            this.a = o.a;
            this.b = o.b;
        }
    }

    static int sum(Shape s) {
        return s.area() + s.id();
    }

    static int main() {
        Rect r = new Rect(1, 2, 3);
        Rect t = r.plus(r);
        Shape s = new Rect(2, 4, 5);
        Pair p = new Pair(1, 2);
        Pair q = new Pair(p);
        int total = sum(s) + sum(t) + Shape.made + q.b;
        printf("%d\n", total);
        // delete s: the collector frees it
        return total;
    }

    public static void main(String[] args) {
        exit(main());
    }
}
