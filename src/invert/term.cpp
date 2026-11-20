#include "invert/term.hpp"

namespace cb::invert {

Term Term::boolean(bool b) {
    Term t;
    t.kind_ = Kind::Bool;
    t.int_ = b ? 1 : 0;
    return t;
}

Term Term::integer(long long v) {
    Term t;
    t.kind_ = Kind::Int;
    t.int_ = v;
    return t;
}

Term Term::string(std::string s) {
    Term t;
    t.kind_ = Kind::Str;
    t.str_ = std::move(s);
    return t;
}

Term Term::tuple(std::initializer_list<Term> items) {
    Term t;
    t.kind_ = Kind::Tuple;
    t.items_.assign(items.begin(), items.end());
    return t;
}

Term Term::tuple(std::vector<Term> items) {
    Term t;
    t.kind_ = Kind::Tuple;
    t.items_ = std::move(items);
    return t;
}

Term Term::list(std::vector<Term> items) {
    Term t;
    t.kind_ = Kind::List;
    t.items_ = std::move(items);
    return t;
}

Term Term::dict() {
    Term t;
    t.kind_ = Kind::Dict;
    return t;
}

long long Term::number() const {
    if (!is_number()) model::fail("a node part that should be a number is not");
    return int_;
}

std::size_t Term::size() const {
    if (kind_ == Kind::Str) return str_.size();
    if (kind_ == Kind::None || is_number()) model::fail("a node part that should have items has none");
    return items_.size();
}

const Term& Term::operator[](long long j) const {
    if (!is_sequence()) model::fail("a node part that should be a tuple is not");
    const long long n = static_cast<long long>(items_.size());
    if (j < 0) j += n;
    if (j < 0 || j >= n) model::fail("a node shorter than its kind says");
    return items_[static_cast<std::size_t>(j)];
}

const Term* Term::find(const std::string& key) const {
    for (std::size_t j = 0; j < keys_.size(); ++j)
        if (keys_[j] == key) return &items_[j];
    return nullptr;
}

const Term& Term::get(const std::string& key) const {
    static const Term nothing;
    if (kind_ != Kind::Dict) model::fail("a node part that should be a dictionary is not");
    const Term* p = find(key);
    return p ? *p : nothing;
}

const Term& Term::at(const std::string& key) const {
    if (kind_ != Kind::Dict) model::fail("a node part that should be a dictionary is not");
    const Term* p = find(key);
    if (!p) model::fail("a node without its '" + key + "' part");
    return *p;
}

void Term::set(const std::string& key, const Term& value) {
    for (std::size_t j = 0; j < keys_.size(); ++j)
        if (keys_[j] == key) {
            items_[j] = value;
            return;
        }
    keys_.push_back(key);
    items_.push_back(value);
}

bool Term::truthy() const {
    switch (kind_) {
        case Kind::None: return false;
        case Kind::Bool:
        case Kind::Int: return int_ != 0;
        case Kind::Str: return !str_.empty();
        default: return !items_.empty();
    }
}

bool operator==(const Term& x, const Term& y) {
    if (x.is_number() || y.is_number()) return x.is_number() && y.is_number() && x.int_ == y.int_;
    if (x.kind_ != y.kind_) return false;
    switch (x.kind_) {
        case Term::Kind::None: return true;
        case Term::Kind::Str: return x.str_ == y.str_;
        case Term::Kind::Dict:
            if (x.keys_.size() != y.keys_.size()) return false;
            for (std::size_t j = 0; j < x.keys_.size(); ++j) {
                const Term* o = y.find(x.keys_[j]);
                if (!o || !(*o == x.items_[j])) return false;
            }
            return true;
        default: return x.items_ == y.items_;
    }
}

}  // namespace cb::invert
