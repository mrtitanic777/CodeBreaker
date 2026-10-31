// Classic: N queens by backtracking in a class, member arrays, a const method, recursion.
#include <cstdio>

class Board {
public:
    explicit Board(int n) : n_(n), solutions_(0) {
        for (int i = 0; i < 16; i++) col_[i] = -1;
    }
    int solve() { place(0); return solutions_; }
    void print_first() const {
        for (int r = 0; r < n_; r++) {
            for (int c = 0; c < n_; c++) std::putchar(first_[r] == c ? 'Q' : '.');
            std::putchar('\n');
        }
    }
private:
    bool safe(int row, int c) const {
        for (int r = 0; r < row; r++) {
            int d = col_[r] - c;
            if (d == 0 || d == row - r || -d == row - r) return false;
        }
        return true;
    }
    void place(int row) {
        if (row == n_) {
            if (solutions_++ == 0)
                for (int r = 0; r < n_; r++) first_[r] = col_[r];
            return;
        }
        for (int c = 0; c < n_; c++) {
            if (safe(row, c)) {
                col_[row] = c;
                place(row + 1);
                col_[row] = -1;
            }
        }
    }
    int n_;
    int solutions_;
    int col_[16];
    int first_[16];
};

int main() {
    for (int n = 4; n <= 9; n++) {
        Board b(n);
        std::printf("%d queens: %d solutions\n", n, b.solve());
    }
    Board eight(8);
    eight.solve();
    eight.print_first();
    return 0;
}
