#include <stdio.h>
#include <ctype.h>
#include "tokval.h"
extern int g_05d4, g_0e56, g_0ad4, g_0ad2;
extern char g_0e54[];
extern char *g_0e58, *g_0e6e;
extern int g_0e5a[], g_0e64[], g_0e70[], g_0e7c[], g_0e88[];
extern char g_3042[];
extern int (*g_09da)(), (*g_09dc)();
extern char *f03b0d(), *f0330d(), *f03a85();
extern f03180(), f0010e(), f03a59(), f0350b(), f03949(), f0391c(), f00167(), f01fd0(), f03784();
extern f03378(), f03248();

/* (open: of the shared "return 0x16f" ends the third pass keeps another copy than the original's) */
/*
 * the scanner: the next token (its value in *v). The one-character operators are looked up in two strings:
 * g_0e58 "^*%=!" (the token alone, g_0e5a, or with '=', g_0e64) and g_0e6e "+-&|<>" (alone g_0e70, with '='
 * g_0e7c, doubled g_0e88). Under -E (g_05d4) what is not needed is written out as it comes.
 */
f02acb(v)
union tokval *v;
{
    register int c;
    register char *p;
    int base;
    int cnt;
    int a;                      /* (a and w not used) */
    int ln;
    int ok;
    int w;

    c = (*g_09da)();
    if ((isspace(c) || !c) && c != '\n') {
        if (!g_05d4)
            return 0x16f;
        g_0e54[0] = c;
        return 0x16a;
    }
    switch (c) {
    case EOF:
        return 0x169;
    case '\n':
        if (g_05d4)
            putc('\n', stdout);
        return f03180();
    case '{':
        return 0x10a;
    case '}':
        return 0x10b;
    case '(':
        return 0x10c;
    case ')':
        return 0x10d;
    case '[':
        return 0x10e;
    case ']':
        return 0x10f;
    case '~':
        return 0x114;
    case '?':
        return 0x133;
    case ':':
        return 0x134;
    case ',':
        return 0x140;
    case ';':
        return 0x112;
    case '#':
        if (!g_05d4) {
            f0010e("preprocessor command must start as first non-whitespace");
            f03a59();
            return 0x16b;
        }
        g_0e54[0] = '#';
        return 0x16a;
    case '.':
        c = (*g_09da)();
        (*g_09dc)(c);
        if (isdigit(c))
            return f0350b(".", v);
        return 0x110;
    case '/':
        if (f03949())
            return 0x16f;
        if (f0391c('='))
            return 0x139;
        return 0x122;
    case '*':
        if (!g_05d4 && f0391c('/')) {
            f00167(1, "unmatched close comment `*/'");
            return 0x16f;
        }
    case '!':
    case '%':
    case '=':
    case '^':
        if (f0391c('='))
            return g_0e64[f03b0d(g_0e58, c) - g_0e58];
        return g_0e5a[f03b0d(g_0e58, c) - g_0e58];
    case '-':
        if (f0391c('>'))
            return 0x111;
    case '&':
    case '+':
    case '<':
    case '>':
    case '|':
        if (f0391c('='))
            return g_0e7c[f03b0d(g_0e6e, c) - g_0e6e];
        if (f0391c(c)) {
            if (f03b0d("<>", c) && f0391c('='))
                return c == '<' ? 0x13b : 0x13c;
            return g_0e88[f03b0d(g_0e6e, c) - g_0e6e];
        }
        return g_0e70[f03b0d(g_0e6e, c) - g_0e6e];
    case '"':
        if (g_05d4) {
            for (;;) {
                if (c == '\n') {
                    (*g_09dc)(c);
                    return 0x16f;
                }
                putc(c, stdout);
                if (c == '\\')
                    putc(f01fd0(), stdout);
                if ((c = f01fd0()) == '"') {
                    putc(c, stdout);
                    return 0x16f;
                }
            }
        }
        ok = 1;
        p = g_3042;
        cnt = 0;
        g_0e56 = 1;
        for (;;) {
            if ((c = f01fd0()) == '"')
                break;
            if (c == '\\' && !g_0ad4) {
                if (f0391c('\n'))
                    continue;
                c = f03784();
            } else if (c == '\n') {
                f00167(1, "newline in string constant");
                break;
            }
            if (cnt >= 512) {
                if (ok) {
                    f00167(1, "string too big, trailing chars truncated");
                    ok = 0;
                }
                p--;
                cnt--;
            }
            *p++ = c;
            cnt++;
        }
        g_0e56 = 0;
        *p = 0;
        cnt++;
        v->s = f03a85(g_3042, cnt, 3);
        v->sl.len = cnt;
        return 0x108;
    case '\'':
        if (g_05d4) {
            p = g_3042;
            for (;;) {
                if (c == '\n') {
                    (*g_09dc)(c);
                    break;
                }
                *p++ = c;
                if (c == '\\')
                    *p++ = f01fd0();
                if ((c = f01fd0()) == '\'')
                    break;
            }
            if (c == '\'')
                *p++ = c;
            *p = 0;
            ln = strlen(g_3042);
            v->s = f03a85(g_3042, ln, 3);
            v->sl.len = ln;
            return 0x103;
        }
        if (f0391c('\\')) {
            if (f0391c('\n'))
                goto plain;
            g_0e56 = 1;
            v->l = f03784();
            g_0e56 = 0;
        } else if (f0391c('\'')) {
            f00167(1, "illegal null char");
            v->l = 0;
            return 0x103;
        } else
plain:      v->l = (*g_09da)();
        if (!f0391c('\'')) {
            f0010e("too many chars in constant");
            for (;;) {
                if ((c = (*g_09da)()) == '\'')
                    break;
                if (c == '\n' || c == EOF) {
                    f0010e("no closing single quote");
                    break;
                }
            }
        }
        return 0x103;
    case '\\':
        if (g_05d4) {
            putc('\\', stdout);
            putc(f01fd0(), stdout);
            return 0x16f;
        }
        if ((c = (*g_09da)()) == '\n')
            return f02acb(v);
        (*g_09dc)(c);
        f0010e("illegal escape sequence");
        return 0x16f;
    case '0':
        if (f0391c('x') || f0391c('X'))
            base = 16;
        else if (f0391c('.'))
            return f0350b("0.", v);
        else
            base = 8;
        return f03378(base, v);
    default:
        if (isalpha(c) || c == '_') {
            v->s = f0330d(c);
            v->sl.len = strlen(v->s);
            if (!g_0ad2)
                return f03248(v);
            return 0x101;
        }
        if (isdigit(c)) {
            (*g_09dc)(c);
            return f03378(10, v);
        }
        if (g_05d4) {
            g_0e54[0] = c;
            return 0x16a;
        }
        f0010e("unknown character `0x%x'", c);
        return 0x16f;
    }
}
