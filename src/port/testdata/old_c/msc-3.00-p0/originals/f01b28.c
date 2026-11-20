extern int (*g_09dc)();
extern char *f0330d();
extern f03a28(), f0010e(), f00167(), f0208c();

f01b28(name)
char *name;
{
    register int c;
    int r;

    if (strcmp(name, "defined"))
        return 0;
    if ((c = f03a28()) != '(') {
        f0010e("missing open paren after keyword `defined'");
        (*g_09dc)(c);
        return 0;
    }
    name = f0330d(f03a28());
    if (!*name) {
        f0010e("expected `defined(id)'");
        r = 0;
    } else
        r = f0208c(name) != 0;
    if ((c = f03a28()) != ')') {
        f00167(1, "missing close parenthesis after `defined'");
        (*g_09dc)(c);
    }
    return r;
}
