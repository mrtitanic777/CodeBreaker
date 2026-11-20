/* a fixup waiting for its record (the cells of f03fec) */
struct fix {
    struct fix *next;
    char seg;                   /* (the M bit: segment-relative) */
    char loc5;
    char loc;                   /* (the location's kind) */
    char f5;
    int off;                    /* (+6: its low byte the offset, bits 8-9 the high bits) */
    char fthread;               /* (+8: the frame is a thread) */
    char frame;
    char tthread;               /* (+0ah: the target is a thread) */
    char target;
    char p;
    char fd;
    int fe;
    int idx;                    /* (+10h: the target's index) */
};
