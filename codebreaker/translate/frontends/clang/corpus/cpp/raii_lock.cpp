// RAII: a guard object whose destructor runs at the end of its scope. Java has no destructors, so this
// needs the try/finally lowering of the design; until it is written the translator must refuse it.
#include <cstdio>

static int depth = 0;

class Guard {
public:
    explicit Guard(const char *n) : name(n) { std::printf("%*senter %s\n", depth * 2, "", name); ++depth; }
    ~Guard() { --depth; std::printf("%*sleave %s\n", depth * 2, "", name); }
private:
    const char *name;
};

static int work(int n) {
    Guard g("work");
    if (n == 0) return 0;
    Guard inner("inner");
    return n + work(n - 1);
}

int main() {
    std::printf("result %d\n", work(3));
    return 0;
}
