/* an initializer of the data: a constant (1), an address (2), a string (3), EVEN (4), a repetition (5) of the
   items after it, its end (6), ... (7), room (8) */
struct init {
    struct init *next;
    int kind;
    int w4;                     /* (a constant's type: 11h a byte ... 34h a float, 38h a double, 42h ...) */
    union {
        char c;
        int w;
        long l;
        float f;
        double d;
        struct init_h {
            int lo;
            int hi;
        } h;
    } v;
};
