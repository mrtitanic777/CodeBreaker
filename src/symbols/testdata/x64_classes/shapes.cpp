// shapes.cpp -- a small x64 C++ program for CodeBreaker's class recovery tests: single and multiple inheritance, pure and
// overridden virtual functions, data members of several widths. Built with: cl /nologo /O2 /GR /EHsc /MD shapes.cpp
#include <cstdio>
#include <cstdlib>

class Shape {
public:
    __declspec(noinline) explicit Shape(int id) : id_(id), scale_(1.0) {}
    virtual ~Shape() {}
    virtual double area() const = 0;
    virtual int sides() const { return 0; }
    virtual void grow(int by) { id_ += by; scale_ *= 1.5; }
    int id_;
    double scale_;
};

class Rect : public Shape {
public:
    __declspec(noinline) Rect(int id, int w, int h) : Shape(id), w_(w), h_(h) {}
    double area() const override { return w_ * h_ * scale_; }
    int sides() const override { return 4; }
    virtual int perimeter() const { return 2 * (w_ + h_); }
    int w_, h_;
};

class Square : public Rect {
public:
    __declspec(noinline) Square(int id, int s) : Rect(id, s, s), flags_(0) {}
    int perimeter() const override { return 4 * w_; }
    virtual void set(int s) { w_ = h_ = s; flags_ |= 1; }
    unsigned short flags_;
};

class Circle : public Shape {
public:
    __declspec(noinline) Circle(int id, int r) : Shape(id), r_(r) {}
    double area() const override { return 3.14159 * r_ * r_ * scale_; }
    void grow(int by) override { r_ += by; }
    long long r_;
};

class Named {
public:
    virtual const char* name() const { return "named"; }
    virtual int rank() const { return rank_; }
    int rank_ = 7;
};

class Label : public Rect, public Named {
public:
    __declspec(noinline) Label(int id) : Rect(id, 10, 2) {}
    const char* name() const override { return "label"; }
    int sides() const override { return 5; }
};

__declspec(noinline) Shape* make(int kind, int id) {
    switch (kind % 4) {
    case 0: return new Rect(id, 3, 4);
    case 1: return new Square(id, 5);
    case 2: return new Circle(id, 2);
    default: return new Label(id);
    }
}

__declspec(noinline) double total(Shape** shapes, int n) {
    double sum = 0;
    for (int i = 0; i < n; ++i) {
        shapes[i]->grow(i);
        sum += shapes[i]->area() + shapes[i]->sides();
    }
    return sum;
}

int main(int argc, char** argv) {
    Shape* shapes[8];
    for (int i = 0; i < 8; ++i) shapes[i] = make(i + argc, i);
    if (Square* s = dynamic_cast<Square*>(shapes[1])) s->set(argc + 6);
    if (Named* n = dynamic_cast<Named*>(shapes[3])) std::printf("%s %d\n", n->name(), n->rank());
    std::printf("%.2f\n", total(shapes, 8));
    for (Shape* s : shapes) delete s;
    return 0;
}
