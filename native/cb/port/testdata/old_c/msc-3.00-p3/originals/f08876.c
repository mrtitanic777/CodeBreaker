extern char *g_1f7e;
extern f017cb();

/* the conditional jump op taken when what it compares is a and b */
f08876(op, a, b)
char op;
long a, b;
{
    switch (op) {
    case 4:
        return a != b;
    case 5:
        return a == b;
    case 12:
        return a >= b;
    case 14:
        return a > b;
    case 15:
        return a <= b;
    case 13:
        return a < b;
    case 7:
        return (unsigned long)a <= (unsigned long)b;
    case 6:
        return (unsigned long)a > (unsigned long)b;
    case 3:
        return (unsigned long)a < (unsigned long)b;
    case 2:
        return (unsigned long)a >= (unsigned long)b;
    default:
        f017cb(g_1f7e, 1471);
    }
}
