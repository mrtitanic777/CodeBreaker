#include "macro.h"
extern char *f01135();
extern char *f010b8();

struct btok *
f02817(kind, argno, text, t, pool)
int kind;
char argno;
char *text;
register struct btok *t;
int pool;
{
    t = (struct btok *)f010b8(8, t, pool);
    t->text = f01135(text, pool);
    t->kind = kind;
    t->argno = argno;
    return t;
}
