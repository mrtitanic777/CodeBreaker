// The idiomatic tier on C++, where it writes a gap and where it writes more than a class: a method that returns a reference to an
// int (refused: the object model has no place to put it), a call of a library function the runtime does not have (a gap too), a
// class template's instance (named by its arguments), a bool, and a function-pointer call through an explicit `*`.
extern int printf(const char *fmt, ...);
extern int mystery(int x);

template <class T, int N>
struct Stack {
    T items[N];
    int top;
    void push(T v) { items[top++] = v; }
    T pop() { return items[--top]; }
};

struct Box {
    int v;
    int &ref() { return v; }
    int get() { return v; }
};

static int inc(int x) { return x + 1; }

static bool positive(int x) { return x > 0; }

int use_mystery(int v) { return mystery(v); }

int main() {
    Stack<int, 4> s;
    s.top = 0;
    s.push(3);
    s.push(4);
    Box b;
    b.v = s.pop();
    int (*f)(int) = inc;
    bool ok = positive(b.get());
    printf("%d\n", (*f)(b.v));
    return ok ? s.pop() : 0;
}
