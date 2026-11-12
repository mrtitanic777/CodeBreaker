#include "tokval.h"
extern int g_05d4, g_06ea;
extern f00940(), f00b3b(), f009ed(), f00b1d(), f00b5a();

f00ea0(tok, vp)
register int tok;
register union tokval *vp;
{
    int w;

    if (g_05d4)
        return;
    if (tok == 0x16e) {
        g_06ea = vp->i;
        return;
    }
    if (g_06ea != -1) {
        w = 0x16e;
        f00940(&w);
        f00940(&g_06ea);
        g_06ea = -1;
    }
    w = tok;
    f00940(&w);
    switch (tok) {
    case 0x101:
    case 0x16c:
    case 0x16d:
        f00b3b(vp->s);
        break;
    case 0x108:
        f00b5a(vp->str.n, vp->str.s);
        break;
    case 0x103:
    case 0x104:
        f009ed(&vp->l);
        break;
    case 0x107:
        f00b1d(&vp->d);
        break;
    }
}
