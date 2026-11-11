/* (what a jump's label is in: its operand, or for a word of a label's place the item itself) */
struct jt {
    int j0, j2, j4, j6;
    struct lab *lab;            /* (+8) */
};
#define JT(p)   ((struct jt *)((p)->kind == 0 ? (char *)(p)->a : (char *)(p)))
