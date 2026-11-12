/* The idiomatic tier on C, more: a linked list (pointers are references, 0 is null), a struct holding arrays of arrays and of
   structs, a function-pointer typedef, string literals as byte arrays, a global array of structs, unsigned and wide integers. */
extern int printf(const char *fmt, ...);
extern unsigned long long strlen(const char *s);
extern void *malloc(unsigned long long n);

struct Node { int v; struct Node *next; };
struct Grid { int cells[2][3]; struct Node nodes[2]; };
typedef int (*Op)(int);

struct Node pool[2];
static int mask = 0xFF;

static int twice(int x) { return x * 2; }

static struct Node *push(struct Node *head, int v) {
    struct Node *n = (struct Node *)malloc(sizeof(struct Node));
    n->v = v;
    n->next = head;
    return n;
}

static int sum(struct Node *h) {
    int t = 0;
    while (h != 0) {
        t += h->v;
        h = h->next;
    }
    return t;
}

int main(void) {
    char msg[8] = "hi";
    const char *text = "static";
    struct Grid g = {{{1, 2, 3}, {4, 5, 6}}, {{7, 0}, {8, 0}}};
    g.cells[1][2] = 9;
    Op op = twice;
    struct Node *list = push(push(0, 1), 2);
    unsigned char b = 200;
    long long big = 1LL << 40;
    double d = 1.5;
    pool[1].v = 3;
    printf("%s %d\n", text, mask);
    return sum(list) + op(g.cells[1][2]) + (int)strlen(text) + (b > 100) + (int)(big >> 38) + (int)d + msg[0] + g.nodes[1].v + pool[1].v;
}
