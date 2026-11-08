// codebreaker/translate/frontends/python/pytypes.cpp -- typed C++ from
// codebreaker.translate.frontends.python.pytypes.

#include "codebreaker/translate/frontends/python/pytypes.hpp"

#include <algorithm>
#include <functional>

namespace codebreaker::translate::frontends::python::pytypes {

namespace {

// -- helpers over bounds (an absent bound is None)
const Big& vmin(const std::vector<Big>& c) { return *std::min_element(c.begin(), c.end()); }
const Big& vmax(const std::vector<Big>& c) { return *std::max_element(c.begin(), c.end()); }

bool bound_eq(const Bound& a, const Bound& b) {
    if (a.has_value() != b.has_value()) {
        return false;
    }
    return !a || *a == *b;
}

py::str bound_str(const Bound& b) { return py::str(b ? b->str() : std::string("None")); }

// `None in (a.lo, a.hi, b.lo, b.hi)`
bool any_none(const T& a, const T& b) { return !a.lo || !a.hi || !b.lo || !b.hi; }

// `_bits_hi`
Big bits_hi(const Big& x) {
    Big m = x > Big(0) ? x : Big(0);  // max(x, 0)
    return Big::pow2(m.bit_length()) - Big(1);
}

// x >> n for a Python int n >= 0 (a shift count past any real width gives the sign: 0 or -1)
Big shr_big(const Big& x, const Big& n) {
    if (n > Big(100000)) {
        return Big(x.sign() < 0 ? -1 : 0);
    }
    return shr(x, static_cast<int>(n.i64()));
}

// the -ish combination `max(_bits_hi(abs(a.lo)), _bits_hi(a.hi), _bits_hi(abs(b.lo)), _bits_hi(b.hi))`
Big bits_span(const T& a, const T& b) {
    std::vector<Big> c = {bits_hi(a.lo->abs()), bits_hi(*a.hi), bits_hi(b.lo->abs()), bits_hi(*b.hi)};
    return vmax(c);
}

}  // namespace

// -------------------------------------------------------------------------------------- constants
const Big& I64_MIN() {
    static const Big v = -Big::pow2(63);
    return v;
}

const Big& I64_MAX() {
    static const Big v = Big::pow2(63) - Big(1);
    return v;
}

const Big& MEM() {
    static const Big v = Big::pow2(47);
    return v;
}

const Big& HUGE_() {
    static const Big v = Big::pow2(128);
    return v;
}

const std::vector<Big>& THRESHOLDS() {
    static const std::vector<Big> v = {Big(0),   Big(1),   Big(255),
                                       Big(static_cast<int64_t>(0xFFFF)), Big(static_cast<int64_t>(0xFFFFFFFFLL)),
                                       MEM(),    I64_MAX()};
    return v;
}

// ------------------------------------------------------------------------------------------- T
T::T(const py::str& kind, const std::vector<T>& args, const Bound& lo, const Bound& hi,
     const std::optional<py::str>& name, const std::optional<int64_t>& site, const std::optional<py::str>& why)
    : kind(kind), args(args), lo(lo), hi(hi), name(name), site(site), why(why) {}

std::tuple<py::str, std::vector<T>, Bound, Bound, std::optional<py::str>, std::optional<int64_t>> T::key() const {
    return {kind, args, lo, hi, name, site};
}

bool T::operator==(const T& o) const {
    if (!(kind == o.kind && args.size() == o.args.size() && bound_eq(lo, o.lo) && bound_eq(hi, o.hi) &&
          name == o.name && site == o.site)) {
        return false;
    }
    for (size_t i = 0; i < args.size(); ++i) {
        if (!(args[i] == o.args[i])) {
            return false;
        }
    }
    return true;
}

bool T::operator!=(const T& o) const { return !(*this == o); }

int64_t T::hash() const {
    uint64_t h = static_cast<uint64_t>(py::str_hash()(kind));
    for (const T& a : args) {
        h = h * 1000003u ^ static_cast<uint64_t>(a.hash());
    }
    h = h * 1000003u ^ (lo ? static_cast<uint64_t>(lo->hash()) : 0x9e3779b9u);
    h = h * 1000003u ^ (hi ? static_cast<uint64_t>(hi->hash()) : 0x7f4a7c15u);
    h = h * 1000003u ^ (name ? static_cast<uint64_t>(py::str_hash()(*name)) : 0x1234567u);
    h = h * 1000003u ^ (site ? static_cast<uint64_t>(*site) : 0x89abcdefu);
    return static_cast<int64_t>(h);
}

py::str T::repr() const {
    const py::str& k = kind;
    if (k == py::str("int")) {
        return py::str("int[") + bound_str(lo) + py::str("..") + bound_str(hi) + py::str("]");
    }
    if ((k == py::str("bytes") || k == py::str("bytearray")) && hi) {
        return k + py::str("(<=") + bound_str(hi) + py::str(")");
    }
    if (k == py::str("list") || k == py::str("dict") || k == py::str("set")) {
        return k + py::str("#") + (site ? py::to_str(*site) : py::str("None"));
    }
    if (k == py::str("tuple") || k == py::str("opt")) {
        py::str inner;
        for (size_t i = 0; i < args.size(); ++i) {
            if (i) {
                inner += py::str(", ");
            }
            inner += args[i].repr();
        }
        return k + py::str("[") + inner + py::str("]");
    }
    if (k == py::str("inst") || k == py::str("cls") || k == py::str("fn") || k == py::str("module") ||
        k == py::str("extern")) {
        return k + py::str(" ") + (name ? *name : py::str("None"));
    }
    if (k == py::str("top")) {
        return py::str("top(") + (why ? *why : py::str("None")) + py::str(")");
    }
    return k;
}

// -- questions
bool T::is_int() const { return kind == py::str("int"); }

bool T::fits_i64() const {
    return kind == py::str("int") && lo && hi && I64_MIN() <= *lo && *hi <= I64_MAX();
}

bool T::nonneg() const { return kind == py::str("int") && lo && *lo >= Big(0); }

bool T::positive() const { return kind == py::str("int") && lo && *lo > Big(0); }

bool T::has_zero() const {
    return !(kind == py::str("int") && ((lo && *lo > Big(0)) || (hi && *hi < Big(0))));
}

// ------------------------------------------------------------------------------- constants (types)
const T& BOTTOM() {
    static const T v(py::str("bottom"));
    return v;
}

const T& NONE() {
    static const T v(py::str("none"));
    return v;
}

const T& BOOL() {
    static const T v(py::str("bool"));
    return v;
}

const T& FLOAT() {
    static const T v(py::str("float"));
    return v;
}

const T& STR() {
    static const T v(py::str("str"));
    return v;
}

const T& BYTES() {
    static const T v(py::str("bytes"));
    return v;
}

const T& BYTEARRAY() {
    static const T v(py::str("bytearray"));
    return v;
}

T top(const py::str& why) { return T(py::str("top"), {}, std::nullopt, std::nullopt, std::nullopt, std::nullopt, why); }

T INT(const Bound& lo_, const Bound& hi_) {
    Bound lo = lo_;
    Bound hi = hi_;
    if (lo && *lo < -HUGE_()) {
        lo = std::nullopt;
    }
    if (hi && *hi > HUGE_()) {
        hi = std::nullopt;
    }
    return T(py::str("int"), {}, lo, hi);
}

T const_int(const Big& v) { return T(py::str("int"), {}, v, v); }

const T& SIZE() {
    static const T v = INT(Big(0), MEM());
    return v;
}

const T& INDEX() {
    static const T v = INT(-MEM(), MEM());
    return v;
}

const T& FIND() {
    static const T v = INT(Big(-1), MEM());
    return v;
}

T u(int64_t bits) { return bits <= 128 ? INT(Big(0), Big::pow2(static_cast<int>(bits)) - Big(1)) : INT(Big(0), std::nullopt); }

T s(int64_t bits) {
    return bits <= 128 ? INT(-Big::pow2(static_cast<int>(bits) - 1), Big::pow2(static_cast<int>(bits) - 1) - Big(1))
                       : INT();
}

T bytes_of(const Bound& maxlen, const py::str& kind) { return T(kind, {}, std::nullopt, maxlen); }

T opt(const T& t) {
    if (t.kind == py::str("opt") || t.kind == py::str("none") || t.kind == py::str("top") ||
        t.kind == py::str("bottom")) {
        return t;
    }
    return T(py::str("opt"), {t});
}

T tuple_of(const std::vector<T>& ts) { return T(py::str("tuple"), ts); }

T strip_opt(const T& t) { return t.kind == py::str("opt") ? t.args[0] : t; }

// ---------------------------------------------------------------------------------------- intervals
namespace {

// _lo / _hi / _add
Bound lo_of(const Bound& a, const Bound& b) {
    if (!a || !b) return std::nullopt;
    return std::min(*a, *b);
}

Bound hi_of(const Bound& a, const Bound& b) {
    if (!a || !b) return std::nullopt;
    return std::max(*a, *b);
}

Bound add_of(const Bound& x, const Bound& y) {
    if (!x || !y) return std::nullopt;
    return *x + *y;
}

}  // namespace

T int_join(const T& a, const T& b) { return INT(lo_of(a.lo, b.lo), hi_of(a.hi, b.hi)); }

T iadd(const T& a, const T& b) { return INT(add_of(a.lo, b.lo), add_of(a.hi, b.hi)); }

T isub(const T& a, const T& b) {
    return INT(!a.lo || !b.hi ? Bound() : Bound(*a.lo - *b.hi), !a.hi || !b.lo ? Bound() : Bound(*a.hi - *b.lo));
}

T ineg(const T& a) { return INT(!a.hi ? Bound() : Bound(-*a.hi), !a.lo ? Bound() : Bound(-*a.lo)); }

T imul(const T& a, const T& b) {
    if (any_none(a, b)) {
        if ((a.lo && a.hi && a.lo->sign() == 0 && a.hi->sign() == 0) ||
            (b.lo && b.hi && b.lo->sign() == 0 && b.hi->sign() == 0)) {
            return const_int(Big(0));
        }
        if (a.nonneg() && b.nonneg()) {
            return INT(*a.lo * *b.lo, std::nullopt);
        }
        return INT();
    }
    std::vector<Big> c = {*a.lo * *b.lo, *a.lo * *b.hi, *a.hi * *b.lo, *a.hi * *b.hi};
    return INT(vmin(c), vmax(c));
}

T ifloordiv(const T& a, const T& b) {
    if (any_none(a, b)) {
        if (a.nonneg() && b.positive()) {
            return INT(Big(0), a.hi);
        }
        return INT();
    }
    std::vector<Big> divs;
    for (const Big& d : {*b.lo, *b.hi, Big(-1), Big(1)}) {
        if (*b.lo <= d && d <= *b.hi && d.sign() != 0) {
            divs.push_back(d);
        }
    }
    if (divs.empty()) {  // always a division by zero: it raises
        return INT();
    }
    std::vector<Big> c;
    for (const Big& x : {*a.lo, *a.hi}) {
        for (const Big& d : divs) {
            c.push_back(floordiv(x, d));
        }
    }
    return INT(vmin(c), vmax(c));
}

T imod(const T& a, const T& b) {
    if (b.positive()) {
        Bound hi = !b.hi ? Bound() : Bound(*b.hi - Big(1));
        if (a.nonneg() && a.hi && (!hi || *a.hi < *hi)) {
            return INT(*b.lo > *a.hi ? Bound(*a.lo) : Bound(Big(0)), a.hi);
        }
        return INT(Big(0), hi);
    }
    if (b.hi && *b.hi < Big(0)) {
        return INT(!b.lo ? Bound() : Bound(*b.lo + Big(1)), Big(0));
    }
    return INT(!b.lo ? Bound() : Bound(std::min(Big(0), *b.lo + Big(1))),
               !b.hi ? Bound() : Bound(std::max(Big(0), *b.hi - Big(1))));
}

T ishl(const T& a, const T& b) {
    if (!b.lo || *b.lo < Big(0) || !b.hi || *b.hi > Big(128) || !a.lo || !a.hi) {
        return !a.nonneg() ? INT() : INT(Big(0), std::nullopt);
    }
    // (b.lo may exceed b.hi when the interval is empty; it is still a small shift count here)
    int bl = static_cast<int>(b.lo->i64());
    int bh = static_cast<int>(b.hi->i64());
    std::vector<Big> c = {shl(*a.lo, bl), shl(*a.lo, bh), shl(*a.hi, bl),
                          shl(*a.hi, bh)};
    return INT(vmin(c), vmax(c));
}

T ishr(const T& a, const T& b) {
    if (!b.lo || *b.lo < Big(0)) {
        return INT();
    }
    if (!a.lo || !a.hi) {
        if (a.nonneg()) {
            return INT(Big(0), std::nullopt);
        }
        return INT();
    }
    std::vector<Big> c = {shr_big(*a.lo, *b.lo), shr_big(*a.hi, *b.lo)};
    if (b.hi) {
        c.push_back(shr_big(*a.lo, *b.hi));
        c.push_back(shr_big(*a.hi, *b.hi));
    } else {
        c.push_back(Big(*a.lo >= Big(0) ? 0 : -1));
    }
    return INT(vmin(c), vmax(c));
}

T iand(const T& a, const T& b) {
    if (a.nonneg() && b.nonneg()) {
        std::vector<Big> his;
        if (a.hi) his.push_back(*a.hi);
        if (b.hi) his.push_back(*b.hi);
        return INT(Big(0), his.empty() ? Bound() : Bound(vmin(his)));
    }
    if (a.nonneg()) {
        return INT(Big(0), a.hi);
    }
    if (b.nonneg()) {
        return INT(Big(0), b.hi);
    }
    if (any_none(a, b)) {
        return INT();
    }
    Big m = bits_span(a, b);
    return INT(-m - Big(1), m);
}

T ior(const T& a, const T& b) {
    if (a.nonneg() && b.nonneg()) {
        if (!a.hi || !b.hi) {
            return INT(Big(0), std::nullopt);
        }
        return INT(std::max(*a.lo, *b.lo), bits_hi(std::max(*a.hi, *b.hi)));
    }
    if (any_none(a, b)) {
        return INT();
    }
    Big m = bits_span(a, b);
    return INT(-m - Big(1), m);
}

T ixor(const T& a, const T& b) { return ior(a, b); }

T iinvert(const T& a) {
    return INT(!a.hi ? Bound() : Bound(-*a.hi - Big(1)), !a.lo ? Bound() : Bound(-*a.lo - Big(1)));
}

T ipow(const T& a, const T& b) {
    if (any_none(a, b) || *b.lo < Big(0) || *b.hi > Big(128) || std::max(a.lo->abs(), a.hi->abs()) > Big::pow2(64)) {
        return INT();
    }
    int bl = static_cast<int>(b.lo->i64());
    int bh = static_cast<int>(b.hi->i64());
    std::vector<Big> c;
    for (const Big& x : {*a.lo, *a.hi}) {
        for (int y : {bl, bh}) {
            c.push_back(ipow(x, y));
        }
    }
    if (*a.lo < Big(0) && Big(0) < *a.hi) {
        c.push_back(Big(0));
    }
    if (bl != bh) {
        for (const Big& x : {*a.lo, *a.hi}) {
            for (int y : {bl + 1, std::max(bl, bh - 1)}) {
                c.push_back(ipow(x, y));
            }
        }
    }
    return INT(vmin(c), vmax(c));
}

T meet(const T& a, const Bound& lo, const Bound& hi) {
    Bound nlo = !lo ? a.lo : (!a.lo ? lo : Bound(std::max(*a.lo, *lo)));
    Bound nhi = !hi ? a.hi : (!a.hi ? hi : Bound(std::min(*a.hi, *hi)));
    if (nlo && nhi && *nlo > *nhi) {  // the branch cannot run: keep the guard's
        return INT(nlo, nlo);
    }
    return INT(nlo, nhi);
}

T widen_int(const T& old, const T& new_) {
    Bound lo = new_.lo;
    Bound hi = new_.hi;
    if (old.lo && (!lo || *lo < *old.lo)) {
        Bound r;  // next((-t for t in THRESHOLDS if lo is not None and -t <= lo), None)
        if (lo) {
            for (const Big& t : THRESHOLDS()) {
                if (-t <= *lo) {
                    r = -t;
                    break;
                }
            }
        }
        lo = r;
    }
    if (old.hi && (!hi || *hi > *old.hi)) {
        Bound r;  // next((t for t in THRESHOLDS if hi is not None and t >= hi), None)
        if (hi) {
            for (const Big& t : THRESHOLDS()) {
                if (t >= *hi) {
                    r = t;
                    break;
                }
            }
        }
        hi = r;
    }
    return INT(lo, hi);
}

}  // namespace codebreaker::translate::frontends::python::pytypes
