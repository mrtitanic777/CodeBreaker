// The idiomatic tier on C++: a class hierarchy (constructors with a base initialiser, virtual and pure virtual methods, a static
// member, an operator), a struct value copied, new and delete, and a destructor.
extern int printf(const char *fmt, ...);

class Shape {
public:
    Shape(int id) : id_(id) {}
    virtual int area() const = 0;
    int id() const { return id_; }
    static int made;

protected:
    int id_;
};

int Shape::made = 0;

class Rect : public Shape {
public:
    Rect(int id, int w, int h) : Shape(id), w_(w), h_(h) { Shape::made++; }
    int area() const override { return w_ * h_; }
    Rect operator+(const Rect &o) const { return Rect(id_, w_ + o.w_, h_ + o.h_); }

private:
    int w_, h_;
};

struct Pair { int a, b; };

int sum(Shape *s) { return s->area() + s->id(); }

int main() {
    Rect r(1, 2, 3);
    Rect t = r + r;
    Shape *s = new Rect(2, 4, 5);
    Pair p = {1, 2};
    Pair q = p;
    int total = sum(s) + sum(&t) + Shape::made + q.b;
    printf("%d\n", total);
    delete s;
    return total;
}
