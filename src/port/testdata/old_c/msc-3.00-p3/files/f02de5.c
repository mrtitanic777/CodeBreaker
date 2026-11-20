/* an option's pattern: its letters, # (a number, or the rest), * (more), - (itself); its result */
struct opt {	/* (as struct flagdef is: one structure, two names) */
    char *pat;
    intptr_t *o2;
    char res;
    unsigned char flags;
};
extern char *g_266e;
extern f03045(), f02ea3();

/* (open: release 3.00 copies the shared "f02ea3(...); return t->res" tails into their jumps) */

/* the option s matched against the table t: what its entry's result is (its action done), 0 when none */
f02de5(t, s, fn, arg)
register struct opt *t;
char *s;
char *(*fn)();
{
    register char *p;
    char *q;

    if (!s)
        return 0;
    for (g_266e = s; t->flags; t++) {
        p = t->pat;
        q = s;
        for (;;)
            switch (*p) {
            default:
                if (*q++ == *p++)
                    continue;
                goto next;
            case '#':
                if (!*q) {
                    f02ea3(t, (*fn)(), arg);
                    return t->res;
                }
                f02ea3(t, q, arg);
                return t->res;
            case '*':
                if (!*q || !f03045(p, q, t->flags))
                    goto next;
                f02ea3(t, q, arg);
                return t->res;
            case 0:
                if (*q)
                    goto next;
                if (t->flags & 0x40) {
                    f02ea3(t, (*fn)(), arg);
                    return t->res;
                }
                f02ea3(t, 0, arg);
                return t->res;
            case '-':
                if (*q != '-')
                    goto next;
                q++;
                p++;
                break;
            }
    next:;
    }
    return 0;
}
