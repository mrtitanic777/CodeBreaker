#include "macro.h"

f020d9(a, b)
register struct btok *a, *b;
{
    while (a || b) {
        if (!a || !b)
            return 1;
        if (a->kind != b->kind)
            return 1;
        switch (a->kind) {
        case 0x108:
            if (strcmp(a->text, b->text))
                return 1;
            break;
        case 0x17a:
            if (a->argno != a->argno)       /* (sic: b's is not looked at) */
                return 1;
            break;
        }
        a = a->next;
        b = b->next;
    }
    return 0;
}
