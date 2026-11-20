// Classic: a value class with operator overloading, references and copies; a Mandelbrot row count.
#include <cstdio>

namespace num {

struct Complex {
    double re, im;
    Complex() : re(0), im(0) {}
    Complex(double r, double i) : re(r), im(i) {}
    Complex operator+(const Complex &o) const { return Complex(re + o.re, im + o.im); }
    Complex operator-(const Complex &o) const { return Complex(re - o.re, im - o.im); }
    Complex operator*(const Complex &o) const { return Complex(re * o.re - im * o.im, re * o.im + im * o.re); }
    Complex &operator+=(const Complex &o) { re += o.re; im += o.im; return *this; }
    bool operator==(const Complex &o) const { return re == o.re && im == o.im; }
    double norm2() const { return re * re + im * im; }
};

}  // namespace num

using num::Complex;

static int escape(Complex c, int limit) {
    Complex z;
    for (int i = 0; i < limit; i++) {
        z = z * z + c;
        if (z.norm2() > 4.0) return i;
    }
    return limit;
}

static void swap(Complex &a, Complex &b) {
    Complex t = a;
    a = b;
    b = t;
}

int main() {
    Complex a(1.5, -2), b(0.25, 4);
    Complex c = a * b - Complex(1, 1);
    std::printf("a*b-1-i = (%.3f, %.3f)\n", c.re, c.im);
    Complex acc;
    for (int i = 0; i < 5; i++) acc += Complex(i, -i);
    std::printf("acc = (%g, %g)\n", acc.re, acc.im);
    swap(a, b);
    std::printf("swapped: a = (%g, %g), b = (%g, %g), equal %d\n", a.re, a.im, b.re, b.im, a == b);
    Complex copy = a;
    copy.re = 99;
    std::printf("copy is separate: %g vs %g\n", copy.re, a.re);
    for (int y = -6; y <= 6; y += 3) {
        int row = 0;
        for (int x = -20; x <= 10; x++) row += escape(Complex(x / 10.0, y / 10.0), 50);
        std::printf("row %3d: %d\n", y, row);
    }
    return 0;
}
