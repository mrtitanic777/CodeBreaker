// The idiomatic tier on C++: a class with a (virtual) destructor, made and deleted on the heap -- `delete` runs it.
extern int printf(const char *fmt, ...);

class Base {
public:
    Base() : n_(1) {}
    virtual ~Base() { printf("~Base %d\n", n_); }
    int n() const { return n_; }

protected:
    int n_;
};

class Derived : public Base {
public:
    Derived() { n_ = 2; }
    ~Derived() { printf("~Derived\n"); }
};

int main() {
    Base *b = new Derived();
    int n = b->n();
    delete b;
    return n;
}
