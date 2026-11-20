#include "tokval.h"
#include "tokens.h"
extern char g_0e54[];
extern f01555(), f00f89();

f01bc7()
{
    int tok;
    union tokval val;

    for (;;) {
        if ((tok = f01555(&val)) == 0x169)
            return;
        switch (tok) {
        case 0x101:
        case 0x103:
        case 0x104:
        case 0x105:
        case 0x106:
        case 0x107:
            printf("%s", val.s);
            break;
        case 0x108:
            printf("\"%s\"", val.s);
            break;
        case 0x16a:
            printf("%s", g_0e54);
            break;
        case 0x16b:
        case 0x16f:
            break;
        default:
            printf("%s", g_123a[tok - 0x101].name);
            break;
        }
        f00f89(3);
    }
}
