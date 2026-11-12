// The idiomatic tier refuses what it cannot write: a local whose destructor must run at the end of its scope (try/finally is not
// written yet).
extern int printf(const char *fmt, ...);

class Guard {
public:
    Guard() : n_(1) {}
    ~Guard() { printf("gone %d\n", n_); }
    int n() const { return n_; }

private:
    int n_;
};

int main() {
    Guard g;
    return g.n();
}
