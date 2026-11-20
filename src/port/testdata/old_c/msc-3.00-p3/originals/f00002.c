extern char *g_00ae;
extern f017cb();

/* (an operand p made ready for the instruction op: 0eh, 0ah, 2) */
f00002(op, p)
char op;
register char *p;
{
    if (op == 0xe) {
        if (*p != 4)
            f017cb(g_00ae, 42);
        p[3] = 4;
    } else if (op == 0xa) {
        if (*p != 0xa)
            f017cb(g_00ae, 46);
        p[0xa] = 2;
    } else if (op == 2)
        *(int *)(p + 6) = 0;
}
