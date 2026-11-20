/* an item of the third pass's code: an instruction (kind 0), a label (1), a line (2), an entry (3), a word of a
   label's place (7), a comment (8) */
struct ins {
    struct ins *next;
    struct ins *prev;
    int kind;
    int op;                     /* an instruction's operator; else the label, the line, ... */
    char size;                  /* its width (a word of a label's place: the label, a word) */
    char i9;
    struct opnd *a, *b;         /* its operands */
};
