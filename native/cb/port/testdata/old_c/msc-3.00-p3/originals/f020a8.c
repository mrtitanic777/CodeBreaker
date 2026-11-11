#include "opnd.h"

/* whether the operand p says nothing of its size (an immediate, a register-based or direct memory operand) */
f020a8(p)
struct opnd *p;
{
    switch (p->kind) {
    case 2:
    case 4:
    case 6:
    case 0xb:
    case 0xd:
        return 1;
    }
    return 0;
}
