/* The faithful tier on programs that call no library: records and unions in emulated memory, unsigned arithmetic,
   function pointers, switch, loops, goto that nests, static locals, initialisers. */
union Pun { float f; unsigned u; unsigned char b[4]; };
struct Rec { short id; char name[6]; double weight; };
struct Pair { int a; int b; };

static int counter = 3;
static double ratio = 0.5;
static unsigned limit = 4000000000u;
static const char *greeting = "hi\n\"there\"";
static int table[4] = {1, 2, 3, 4};
static struct Pair origin = {7, 9};

typedef int (*binop)(int, int);

static unsigned hash(const unsigned char *s) {
    unsigned h = 2166136261u;
    while (*s) { h ^= *s++; h *= 16777619u; }
    return h;
}
static struct Rec make(short id, double w) {
    struct Rec r;
    r.id = id; r.weight = w; r.name[0] = 'a'; r.name[1] = 0;
    return r;
}
static int add(int a, int b) { return a + b; }
static int mul(int a, int b) { return a * b; }
static int apply(binop f, int a, int b) { return f(a, b); }
static int classify(int x) {
    switch (x) {
    case 1: return 10;
    case 2:
    case 3: x += 4; break;
    default: x = -1;
    }
    return x;
}
static int count(void) { static int n; n++; return n + counter; }
static int loops(int n) {
    int s = 0;
    for (int i = 0; i < n; i++) { if (i == 3) continue; s += i; }
    do { s--; } while (s > 100);
    while (1) { if (s > 5) break; s++; }
    return s;
}
static long long wide(long long a, unsigned long long b) {
    return a / 7 + (long long)(b % 5u) + (a < 0);
}
static int jump(int n) {
    int r = 0;
again:
    r++;
    if (r < n) goto again;
    if (n > 100) goto out;
    r += 2;
out:
    return r;
}
static int bytes(unsigned char c, signed char d, unsigned short e) {
    c += 10; d -= 3; e <<= 2;
    return c + d + e + (c >> 1) + (d >> 1) + (int)(limit / 3u) + (limit > 5u);
}
static double mix(int i, unsigned u, float f, double d) {
    return i / 2 + u / 2.0 + f * d - (double)(unsigned char)i;
}
static void swap(int *a, int *b) { int t = *a; *a = *b; *b = t; }
static int sum(const int *p, int n) {
    const int *end = p + n;
    int s = 0;
    while (p != end) s += *p++;
    return s;
}
int main(void) {
    union Pun p;
    struct Rec r = make(3, 2.5);
    struct Pair q = {1, 2};
    int x = 5, y = 6;
    p.f = -1.5f;
    swap(&x, &y);
    q = origin;
    x += apply(add, x, y) + apply(mul, 2, 3);
    x += classify(2) + count() + count() + loops(10) + jump(4);
    x += (int)wide(-100, 99u) + bytes(250, 100, 3) + (int)mix(7, 9u, 1.5f, 2.0);
    x += (int)hash((const unsigned char *)greeting) + sum(table, 4) + (int)(p.u >> 28) + r.id + q.b;
    x += ratio > 0.25 ? 1 : 2;
    return x & 127;
}
