// ObjStructs.java -- translated from C by CodeBreaker (idiomatic tier: structs as classes, pointers as references).
import crt.*;

import static crt.LibC.*;

public final class ObjStructs {

    /** struct Vec */
    static class Vec {
        int x;
        int y;

        Vec() {
        }

        Vec(int x, int y) {
            this.x = x;
            this.y = y;
        }

        /** C's copy of a struct value. */
        Vec(Vec o) {
            this.x = o.x;
            this.y = o.y;
        }

        /** C's assignment of a struct value: copies o into this one. */
        Vec set(Vec o) {
            this.x = o.x;
            this.y = o.y;
            return this;
        }
    }

    /** struct Line */
    static class Line {
        Vec a = new Vec();
        Vec b = new Vec();
        int[] tags = new int[3];

        Line() {
        }

        Line(Vec a, Vec b, int[] tags) {
            this.a = a;
            this.b = b;
            this.tags = tags;
        }

        /** C's copy of a struct value. */
        Line(Line o) {
            this.a = new Vec(o.a);
            this.b = new Vec(o.b);
            this.tags = o.tags.clone();
        }
    }

    interface IntIntToInt {
        int call(int a0, int a1);
    }

    static final int RED = 0;
    static final int GREEN = 5;
    static final int BLUE = 6;
    static int counter = 0;
    static final int[] table = new int[]{1, 2, 3, 4};
    static final Vec origin = new Vec(0, 0);
    static int main_calls = 0;

    static Vec add(Vec a, Vec b) {
        Vec r = new Vec(a.x + b.x, a.y + b.y);
        return r;
    }

    static void bump(int[] n) {
        n[0] += 1;
    }

    static int total(Line l) {
        return l.a.x + l.b.y + l.tags[1];
    }

    static int cmp(int a, int b) {
        return a - b;
    }

    static int apply(IntIntToInt f, int a, int b) {
        return f.call(a, b);
    }

    static int main() {
        Vec v = new Vec(1, 2);
        Vec w = add(new Vec(v), new Vec(v));
        Line line = new Line(new Vec(1, 2), new Vec(3, 4), new int[]{5, 6, 7});
        Line copy = new Line(line);
        copy.a.set(w);
        int[] n = {5};
        bump(n);
        Vec p = new Vec();
        p.x = 9;
        int[] nums = new int[4];
        nums[2] = 8;
        main_calls++;
        counter += n[0];
        printf("%d %d %d %d\n", total(copy), p.x + nums[2], apply(ObjStructs::cmp, 3, 1), table[2] + main_calls);
        // free(p): the collector frees it
        // free(nums): the collector frees it
        int c = GREEN;
        return counter + c + origin.x;
    }

    public static void main(String[] args) {
        exit(main());
    }
}
