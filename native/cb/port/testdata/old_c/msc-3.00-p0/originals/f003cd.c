#include "getflags.h"
extern char *g_1394, *g_1f78;
extern char *strdup();
extern f0010e(), f00081();

f003cd(t, val, isdup)
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
            f0010e("bad %s flag, would overwrite %s with %s\n", t->pat, *t->var, val);
            break;
        }
    case 'B':
        *t->var = isdup ? strdup(val) : val;
        break;
    case 'D':
        *t->var = atoi(val);
        break;
    case 'F':
        fs = (struct slots *)t->var;
        if (fs->top > 0)
            fs->v[--fs->top] = isdup ? strdup(val) : val;
        else
            f0010e("too many %s flags, %s\n", t->pat, val);
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
                        f00081(g_1394, 178);
                    }
                    break;
                }
            if (!sp->c)
                f0010e("Unknown option (%c) in %s\n", *val, g_1f78);
        }
        break;
    default:
        f00081(g_1394, 188);
    }
}
