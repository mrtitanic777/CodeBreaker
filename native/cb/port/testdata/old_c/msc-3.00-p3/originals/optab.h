/* opMD.c's table of the instructions (g_17b2, 10 bytes each): by the operator its name, its flags (1: an 8087
   instruction), the number of its operands, its opcode's size, the reg field of its ModR/M byte, its opcodes */
struct optab {
    char *name;
    unsigned flags;
    char nops;
    char size;
    char ext;
    char o7;
    char *bytes;
};
extern struct optab g_17b2[];
