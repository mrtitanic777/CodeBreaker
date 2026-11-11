struct flagdef {
    char *pat;
    int *var;
    char val;
    unsigned char flags;
};
struct slots {
    int top;
    char *v[20];
};
struct sub {                    /* one letter of a switch of letters (kind C) */
    int c;
    int type;
    int *var;
};
extern char *g_0888, *g_266e;
extern char *strdup();
extern f02c04(), f017cb();

f02ea3(t, val, isdup)
struct flagdef *t;
char *val;
int isdup;
{
    char *arg;
    register struct sub *sp;
    struct slots *fs;

    arg = val;
    switch (t->flags) {
    case 1:
        *t->var = 1;
        break;
    case 5:
        *t->var = 0;
        break;
    case 'G':
        if (*t->var) {
            f02c04("bad %s flag, would overwrite %s with %s\n", t->pat, *t->var, val);
            break;
        }
    case 'B':
        *t->var = isdup ? strdup(val) : val;
        break;
    case 'D':
        *t->var = atol(val);         /* (atol: the program has one routine for atoi and atol, and tzset calls it _atol) */
        break;
    case 'F':
        fs = (struct slots *)t->var;
        if (fs->top > 0)
            fs->v[--fs->top] = isdup ? strdup(val) : val;
        else
            f02c04("too many %s flags, %s\n", t->pat, val);
        break;
    case 'C':
        for (; *val; val++) {
            for (sp = (struct sub *)t->var; sp->c; sp++)
                if (*val == sp->c) {
                    switch (sp->type) {
                    case 1:
                        *sp->var = 1;
                        break;
                    case 5:
                        *sp->var = 0;
                        break;
                    default:
                        f017cb(g_0888, 178);
                    }
                    break;
                }
            if (!sp->c)
                f02c04("Unknown option (%c) in %s\n", *val, g_266e);
        }
        break;
    default:
        f017cb(g_0888, 188);
    }
}
