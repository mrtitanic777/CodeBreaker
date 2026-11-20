// Classic: an abstract base, virtual dispatch, a destructor chain, a static counter.
#include <cstdio>

class Shape {
public:
    explicit Shape(const char *name) : name_(name) { ++count; }
    virtual ~Shape() { std::printf("~Shape(%s)\n", name_); --count; }
    virtual double area() const = 0;
    virtual double perimeter() const = 0;
    const char *name() const { return name_; }
    static int count;
private:
    const char *name_;
};

int Shape::count = 0;

class Rect : public Shape {
public:
    Rect(double w, double h) : Shape("rect"), w_(w), h_(h) {}
    ~Rect() override { std::printf("~Rect\n"); }
    double area() const override { return w_ * h_; }
    double perimeter() const override { return 2 * (w_ + h_); }
protected:
    double w_, h_;
};

class Square : public Rect {
public:
    explicit Square(double s) : Rect(s, s) {}
};

class Circle : public Shape {
public:
    explicit Circle(double r) : Shape("circle"), r_(r) {}
    double area() const override { return 3.14159265358979 * r_ * r_; }
    double perimeter() const override { return 2 * 3.14159265358979 * r_; }
private:
    double r_;
};

static double total_area(Shape **shapes, int n) {
    double sum = 0;
    for (int i = 0; i < n; i++) sum += shapes[i]->area();
    return sum;
}

int main() {
    Shape *shapes[3];
    shapes[0] = new Rect(3, 4);
    shapes[1] = new Square(2.5);
    shapes[2] = new Circle(1.5);
    std::printf("count = %d\n", Shape::count);
    for (int i = 0; i < 3; i++)
        std::printf("%-6s area %8.3f perimeter %8.3f\n", shapes[i]->name(), shapes[i]->area(), shapes[i]->perimeter());
    std::printf("total %.4f\n", total_area(shapes, 3));
    for (int i = 0; i < 3; i++) delete shapes[i];
    std::printf("count = %d\n", Shape::count);
    return 0;
}
