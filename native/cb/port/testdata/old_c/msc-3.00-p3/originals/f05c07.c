#include "init.h"
#include "targ.h"
#include "seg.h"
#include "rec.h"
/* (an address: its symbol, its offset, its size) */
struct iref {
    struct init *next;
    int kind;
    struct targ *sym;
    long off;
    char ba;
    char size;
};
extern struct seg *g_11b4;      /* (the segment the data goes in) */
extern char *g_123c;
extern f048bd(), f0484a(), f040fb(), f04127(), f0414b(), f04176(), f0418c(), f04224(), f05069(), f017cb();

/* (NOROOM 9 takes it from 407 differing bytes to 270: the compiler of 3.00 was short of room here too) */
/* (open: 652 bytes for 651 - the cases' shared tails are kept differently) */

/* the initializers from p put in LEDATA records, up to the end of a repetition: the item there */
struct init *
f05c07(p)
register struct init *p;
{
    register n;
    struct init *r;
    long val;
    struct targ *sym;

    if (p->kind == 6)
        return p;
    f048bd(0xa0, g_11b4);
    for (; p; p = p->next) {
        if (g_3368 > 1000)
            f0484a(g_360e[0], g_11b4);
        switch (p->kind) {
        default:
            f017cb(g_123c, 1797);
            break;
        case 1:
            switch ((char)p->w4) {
            case 0x11:
            case 0x21:
                f040fb((unsigned char)p->v.c);
                break;
            case 0x14:
            case 0x24:
                f0414b(p->v.w, p->v.h.hi);
                break;
            case 0x12:
            case 0x22:
            case 0x42:
                f04127(p->v.w);
                break;
            case 0x44:
                f04176(p->v.w, p->v.h.hi);
                break;
            case 0x34:
                f0418c(&p->v, 4);
                break;
            case 0x38:
                f0418c(&p->v, 8);
                break;
            default:
                f017cb(g_123c, 1710);
                break;
            }
            g_11b4->size += (char)p->w4 & 0xf;
            break;
        case 2:
            sym = ((struct iref *)p)->sym;
            if (sym->kind != 0 && sym->kind != 2)
                f017cb(g_123c, 1721);
            val = sym->kind == 2 ? 0L : *(long *)&sym->off + ((struct iref *)p)->off;
            if (((struct iref *)p)->size == 4) {
                f04176((int)val, (int)(val >> 16));
                f05069(sym, 3);
                g_11b4->size += 4;
            } else {
                f04127((int)val);
                f05069(sym, 1);
                g_11b4->size += 2;
            }
            break;
        case 3:
            f0418c(p->v.w, p->w4);
            g_11b4->size += p->w4;
            break;
        case 4:
            if ((((unsigned char)g_360e[5] << 8) + (unsigned char)g_360e[4] + g_3368 - 6) % (char)p->w4 == 0)
                break;
            if ((char)p->w4 - ((unsigned char)g_360e[5] << 8) - (unsigned char)g_360e[4] - g_3368 + 6 != 1)
                f017cb(g_123c, 1745);
            f040fb(0);
            g_11b4->size++;
            break;
        case 8:
            if (*(long *)&p->w4 < 20) {
                f04224(p->w4);
                g_11b4->size += p->w4;
            } else {
                g_11b4->size += p->w4 - 1;
                f0484a(g_360e[0], g_11b4);
                f040fb(0);
                g_11b4->size++;
            }
            break;
        case 5:
            n = p->w4;
            while (n--)
                r = f05c07(p->next);
            if (r)
                f017cb(g_123c, 1792);
            p = r;
            break;
        }
    }
    return p;
}
