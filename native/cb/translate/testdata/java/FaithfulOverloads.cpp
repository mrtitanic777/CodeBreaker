// C++ without classes: overloads (a function template's instances among them) become Java overloads; a reference
// parameter is the address of the caller's variable.
static int twice(int x) { return x * 2; }
static double twice(double x) { return x * 2.0; }
static long long twice(long long x) { return x + x; }

template <class T> static T larger(T a, T b) { return a > b ? a : b; }

static void bump(int &v) { v += 1; }

struct Vec { int x, y; };

static Vec scaled(Vec v, int k) {
    Vec r;
    r.x = v.x * k;
    r.y = v.y * k;
    return r;
}

int main() {
    int n = 4;
    bump(n);
    Vec v = {2, 3};
    Vec w = scaled(v, n);
    return twice(n) + (int)twice(1.5) + (int)twice(7LL) + larger(n, 3) + (int)larger(1.5, 0.5) + w.y;
}
