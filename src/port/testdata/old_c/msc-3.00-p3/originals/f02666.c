#include "init.h"
/* (an address: its symbol, its offset, its size) */
struct iref {
    struct init *next;
    int kind;
    int sym;
    long off;
    char ba;
    char size;
};
extern char *f0475f();
extern f023f8(), f0242e(), f02472(), printf();

/* (NOROOM 8 takes it from 169 differing bytes to 137: the compiler of 3.00 was short of room here too) */
/* (open: of the printf calls' shared tails the original keeps the last copies, release 3.00 the first) */

/* the initializers from p in the listing, up to the end of a repetition: the item there */
struct init *
f02666(p)
register struct init *p;
{
    int x1;                     /* (not used) */
    int x2;                     /* (not used) */
    char *s;
    struct init *rp;
    register cnt;

    for (; p; p = p->next)
        switch (p->kind) {
        case 1:
            switch ((char)p->w4) {
            case 0x11:
            case 0x21:
                printf("\tDB\t0%01xH\n", (unsigned char)p->v.c);
                break;
            case 0x12:
            case 0x22:
                printf("\tDW\t0%xH\n", p->v.w);
                break;
            case 0x14:
            case 0x24:
                printf("\tDD\t0%lxH\n", p->v.l);
                break;
            case 0x42:
                printf("\tDW\t0%xH\n", p->v.w);
                break;
            case 0x44:
                printf("\tDD\t%lxH\n", p->v.l);
                break;
            case 0x34:
                f023f8(&p->v.f);
                break;
            case 0x38:
                f0242e(&p->v.d);
                break;
            }
            break;
        case 2:
            if (((struct iref *)p)->size == 4)
                printf("\tDD\t%s", f0475f(((struct iref *)p)->sym));
            else
                printf("\tDW\t%s", f0475f(((struct iref *)p)->sym));
            if (((struct iref *)p)->off) {
                if (((struct iref *)p)->off > 0)
                    printf("+");
                printf("%1ld", ((struct iref *)p)->off);
            }
            printf("\n");
            break;
        case 3:
            cnt = p->w4;
            s = (char *)p->v.w;
            f02472(s, cnt);
            break;
        case 4:
            printf("\tEVEN\n");
            break;
        case 8:
            printf("\tORG\t$+%1ld\n", *(long *)&p->w4);
            break;
        case 5:
            cnt = p->w4;
            while (cnt--)
                rp = f02666(p->next);
            p = rp;
            break;
        case 6:
            return p;
        case 7:
            break;
        }
    return p;
}
