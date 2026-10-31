// Classic: a fixed-size matrix class -- copy constructor semantics, operator*, operator() access, a static factory.
#include <cstdio>

class Matrix {
public:
    Matrix() { for (int i = 0; i < 9; i++) m_[i] = 0; }
    static Matrix identity() {
        Matrix r;
        for (int i = 0; i < 3; i++) r.m_[i * 3 + i] = 1;
        return r;
    }
    double &at(int r, int c) { return m_[r * 3 + c]; }
    double get(int r, int c) const { return m_[r * 3 + c]; }
    void set(int r, int c, double v) { m_[r * 3 + c] = v; }
    Matrix operator*(const Matrix &o) const {
        Matrix r;
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++) {
                double s = 0;
                for (int k = 0; k < 3; k++) s += get(i, k) * o.get(k, j);
                r.set(i, j, s);
            }
        return r;
    }
    double det() const {
        return get(0, 0) * (get(1, 1) * get(2, 2) - get(1, 2) * get(2, 1)) -
               get(0, 1) * (get(1, 0) * get(2, 2) - get(1, 2) * get(2, 0)) +
               get(0, 2) * (get(1, 0) * get(2, 1) - get(1, 1) * get(2, 0));
    }
    void print(const char *label) const {
        std::printf("%s:\n", label);
        for (int i = 0; i < 3; i++) std::printf("  %7.2f %7.2f %7.2f\n", get(i, 0), get(i, 1), get(i, 2));
    }
private:
    double m_[9];
};

int main() {
    Matrix a = Matrix::identity();
    a.set(0, 1, 2);
    a.set(1, 2, -1.5);
    a.set(2, 0, 0.5);
    Matrix b = a;            // a copy: changing b leaves a alone
    b.set(2, 2, 4);
    Matrix c = a * b;
    a.print("a");
    b.print("b");
    c.print("a*b");
    std::printf("det a %.4f, det b %.4f, det a*b %.4f\n", a.det(), b.det(), c.det());
    Matrix p = Matrix::identity();
    for (int i = 0; i < 5; i++) p = p * a;
    std::printf("det a^5 %.4f\n", p.det());
    return 0;
}
