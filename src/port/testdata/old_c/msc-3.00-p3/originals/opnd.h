/* an instruction's operand as the third pass reads it */
struct opnd {
    char kind;                  /* (4 a register, 0ah ..., 0bh ...) */
    char o1, o2;
    char size;
    long val;                   /* its value (a constant's, a displacement) */
    int sym;                    /* (its symbol) */
    char o10, o11;
    char o12;                   /* (+0ch: the index register of a based operand) */
    char o13;
};
