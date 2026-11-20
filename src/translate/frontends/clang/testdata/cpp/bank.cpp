// Classic: a small bank -- objects on the heap linked in a list, an enum class, references as out-parameters.
#include <cstdio>
#include <cstring>

enum class Kind { Checking, Savings };

class Account {
public:
    Account(int id, Kind kind, long long cents) : id_(id), kind_(kind), cents_(cents), next(nullptr) {}
    int id() const { return id_; }
    Kind kind() const { return kind_; }
    long long balance() const { return cents_; }
    bool withdraw(long long amount) {
        if (amount > cents_) return false;
        cents_ -= amount;
        return true;
    }
    void deposit(long long amount) { cents_ += amount; }
    void add_interest(int basis_points) { cents_ += cents_ * basis_points / 10000; }
    Account *next;
private:
    int id_;
    Kind kind_;
    long long cents_;
};

class Bank {
public:
    Bank() : head_(nullptr), count_(0) {}
    Account *open(Kind kind, long long cents) {
        Account *a = new Account(++count_, kind, cents);
        a->next = head_;
        head_ = a;
        return a;
    }
    Account *find(int id) {
        for (Account *a = head_; a != nullptr; a = a->next)
            if (a->id() == id) return a;
        return nullptr;
    }
    bool transfer(int from, int to, long long amount, int &error) {
        Account *a = find(from), *b = find(to);
        if (!a || !b) { error = 1; return false; }
        if (!a->withdraw(amount)) { error = 2; return false; }
        b->deposit(amount);
        error = 0;
        return true;
    }
    void month_end() {
        for (Account *a = head_; a; a = a->next)
            if (a->kind() == Kind::Savings) a->add_interest(125);
    }
    long long total() const {
        long long t = 0;
        for (Account *a = head_; a; a = a->next) t += a->balance();
        return t;
    }
    void report() const {
        for (Account *a = head_; a; a = a->next)
            std::printf("  #%d %-8s %10lld.%02lld\n", a->id(), a->kind() == Kind::Savings ? "savings" : "checking",
                        a->balance() / 100, a->balance() % 100);
    }
private:
    Account *head_;
    int count_;
};

int main() {
    Bank bank;
    bank.open(Kind::Checking, 150000);
    bank.open(Kind::Savings, 1000000);
    bank.open(Kind::Checking, 2500);
    int error = -1;
    bool ok = bank.transfer(1, 3, 40000, error);
    std::printf("transfer 1->3: %d (error %d)\n", ok, error);
    ok = bank.transfer(3, 2, 1000000, error);
    std::printf("transfer 3->2: %d (error %d)\n", ok, error);
    ok = bank.transfer(9, 2, 1, error);
    std::printf("transfer 9->2: %d (error %d)\n", ok, error);
    for (int m = 0; m < 3; m++) bank.month_end();
    bank.report();
    std::printf("total %lld\n", bank.total());
    return 0;
}
