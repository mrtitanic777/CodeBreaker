// Classic: a template stack and function templates, used at two types each (instantiation).
#include <cstdio>

template <typename T, int N>
class Stack {
public:
    Stack() : size_(0) {}
    bool push(T v) {
        if (size_ == N) return false;
        items_[size_++] = v;
        return true;
    }
    T pop() { return items_[--size_]; }
    bool empty() const { return size_ == 0; }
    int size() const { return size_; }
private:
    T items_[N];
    int size_;
};

template <typename T>
T maximum(T a, T b) { return a > b ? a : b; }

template <typename T>
T sum(const T *xs, int n) {
    T s = 0;
    for (int i = 0; i < n; i++) s += xs[i];
    return s;
}

int main() {
    Stack<int, 8> si;
    for (int i = 1; i <= 10; i++)
        if (!si.push(i * i)) std::printf("full at %d\n", i);
    std::printf("size %d\n", si.size());
    while (!si.empty()) std::printf("%d ", si.pop());
    std::printf("\n");
    Stack<double, 4> sd;
    sd.push(1.5);
    sd.push(-2.25);
    double top = sd.pop();       // (one pop per statement: C++ leaves argument order unspecified)
    double below = sd.pop();
    std::printf("%.2f %.2f\n", top, below);
    std::printf("max %d %.1f %c\n", maximum(3, 7), maximum(2.5, -1.0), maximum('a', 'z'));
    int xs[5] = {1, 2, 3, 4, 5};
    double ds[3] = {0.5, 0.25, 0.125};
    std::printf("sums %d %.3f\n", sum(xs, 5), sum(ds, 3));
    return 0;
}
