/* an operand that a register is known to hold the value of: the next, the operand */
struct regc {
    struct regc *next;
    struct opnd *o;
};
extern struct regc *g_1fa8[];   /* (for each register, what it holds) */
extern struct regc *g_1fe4;     /* (the free ones) */
