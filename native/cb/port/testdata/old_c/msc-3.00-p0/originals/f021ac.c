#include "tokval.h"
extern int g_09d8, g_05d4, g_0ad4, g_0c28, g_08e6;
extern long g_091a;
extern char *g_09d4, *g_08e8;
extern int g_2e02[];
extern int (*g_09dc)();
extern char *f01135();
extern f03a28(), f00c74(), f025a4(), f01b16(), f019b9(), f024c8(), f01555(), f024e0(), f00f49(), f02855();
extern f0010e(), f03a59(), f01d6e(), f00143(), f01f2e(), f028ca(), f02acb(), f024f2(), f015b0();
extern struct mac *f0208c();
extern f00167(), f0213a();

/* (open: the third pass keeps the earlier copy of the shared error calls; the original kept the later) */
/* a line beginning with #: the directive's name is at p (its first character is made '#' to look it up) */
f021ac(p)
char *p;
{
    register int tok, c;
    union tokval val;
    char ch;
    int kw, state;
    int what;
    char *file;

    ch = *p;
    *p = '#';
    tok = 0x16f;
    c = f03a28();
    (*g_09dc)(c);
    switch (kw = f00c74(p)) {
    case 0x174:                                         /* #define */
        return f025a4();
    case 0x16e:                                         /* #line */
        f01b16();
        if (f019b9(0x103)) {
            g_09d8 = (int)g_091a - 1;
            c = f03a28();
            (*g_09dc)(c);
            if (c == '\n')
                return 0x16f;
            f024c8();
            tok = f01555(&val);
            f024e0();
            if (tok == 0x108) {
                g_09d4 = f01135(val.s, 0);
                f00f49();
            }
            if (g_05d4)
                printf("\n#line %d %s", g_09d8 + 1, g_09d4);
        } else
            f0010e("#line expected a line number");
        break;
    case 0x173:                                         /* #include */
        if (c == '\n')
            return 0x16f;
        f024c8();
        g_0ad4 = 1;
        tok = f01555(&val);
        g_0ad4 = 0;
        f024e0();
        if (tok == 0x129)
            return f02855();
        if (tok != 0x108) {
            f0010e("#include\texpected a file name");
            break;
        }
        file = val.s;
        f03a59();
        if (*file == '/') {
            if (f01d6e(file, 0))
                break;
            f00143("cannot find `%s'", file);
            break;
        }
        if (f01f2e(file))
            return 0x16f;
        return f028ca(file);
    case 0x175:                                         /* #ifdef */
    case 0x176:                                         /* #ifndef */
        if (c == '\n')
            tok = 0x16f;
        else {
            f024c8();
            tok = f02acb(&val);
            f024e0();
        }
        if (tok != 0x101) {
            f00143("#if[n]def expected an identifier");
            break;
        }
        state = f0208c(val.s) != 0;
        if (kw == 0x176)
            state = !state;
        if (state == 1)
            goto taken;
        if (f024f2(5) != 3)
            break;
        goto taken;
    case 0x158:                                         /* #if */
        g_08e6 = 0;
        f01b16();
        state = f015b0();
        if (g_08e6)
            f00143(g_08e8);
        if (state)
            goto taken;
        if ((what = f024f2(5)) == 3) {
taken:
            g_2e02[++g_0c28] = 1;
            break;
        }
        if (what != 2)
            break;
        return f021ac("if");
    case 0x177:                                         /* #elif */
        if (g_0c28-- < 0)
            f00143("unexpected `#elif'");
        while (f024f2(5) != 4)
            ;
        break;
    case 0x153:                                         /* #else */
        if (g_2e02[g_0c28] != 1)
            f00143("unexpected `#else'");
        f024f2(6);
        break;
    case 0x178:                                         /* #endif */
        if (g_0c28-- < 0)
            f00143("unexpected `#endif'");
        break;
    case 0x179:                                         /* #undef */
        if (c == '\n')
            return 0x16f;
        if ((tok = f02acb(&val)) != 0x101)
            f00167(1, "#undef expected an identifier");
        f0213a(val.s);
        break;
    default:
        *p = ch;
        f00143("bad preprocessor command `%s'", p);
        break;
    }
    f03a59();
    return 0x16f;
}
