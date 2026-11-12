/* The idiomatic tier on C: structs as classes (copies are copies), arrays, a scalar whose address is taken (boxed), malloc and
   free, a function pointer, an enum, globals and a static local. */
extern int printf(const char *fmt, ...);
extern void *malloc(unsigned long long n);
extern void free(void *p);

struct Vec { int x; int y; };
struct Line { struct Vec a; struct Vec b; int tags[3]; };
enum Color { RED, GREEN = 5, BLUE };

static int counter;
int table[4] = {1, 2, 3, 4};
struct Vec origin = {0, 0};

static struct Vec add(struct Vec a, struct Vec b) {
    struct Vec r = {a.x + b.x, a.y + b.y};
    return r;
}
static void bump(int *n) { *n += 1; }
static int total(struct Line *l) { return l->a.x + l->b.y + l->tags[1]; }
static int cmp(int a, int b) { return a - b; }
static int apply(int (*f)(int, int), int a, int b) { return f(a, b); }

int main(void) {
    struct Vec v = {1, 2};
    struct Vec w = add(v, v);
    struct Line line = {{1, 2}, {3, 4}, {5, 6, 7}};
    struct Line copy = line;
    copy.a = w;
    int n = 5;
    bump(&n);
    struct Vec *p = (struct Vec *)malloc(sizeof(struct Vec));
    p->x = 9;
    int *nums = (int *)malloc(4 * sizeof(int));
    nums[2] = 8;
    static int calls = 0;
    calls++;
    counter += n;
    printf("%d %d %d %d\n", total(&copy), p->x + nums[2], apply(cmp, 3, 1), table[2] + calls);
    free(p);
    free(nums);
    enum Color c = GREEN;
    return counter + (int)c + origin.x;
}
