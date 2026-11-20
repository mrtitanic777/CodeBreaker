// A C++ class: its tier is the idiomatic one (methods, constructors, virtual calls); the faithful tier has no form for it.
class Point {
public:
    Point(int x, int y) : x_(x), y_(y) {}
    int sum() const { return x_ + y_; }

private:
    int x_, y_;
};

int main() {
    Point p(1, 2);
    return p.sum();
}
