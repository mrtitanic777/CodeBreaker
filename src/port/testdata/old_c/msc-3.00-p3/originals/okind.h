/* MD.c's table of the operands' kinds: by the kind, the letters of what follows such an operand in the code
   buffer (m, b or r, g, d, o or v, s, i), and its length */
struct okind {
    char *fmt;
    int x;
};
extern struct okind g_3b18[];
