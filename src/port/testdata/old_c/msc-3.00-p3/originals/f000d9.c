/* (an entry of 10 bytes: its count, its kind, a flag) */
struct ent {
    int e0, e2, e4;
    int kind;
    char e8, flag;
};

/* (e marked: kind 40h made 3fh, with the flag, when its count is 0) */
f000d9(e)
register struct ent *e;
{
    if (!e->e4)
        switch (e->kind) {
        case 0x40:
            e->kind = 0x3f;
            e->flag = 1;
            break;
        default:
            e->flag = 0;
            break;
        }
}
