// pytypes.cc -- the inference's types and integer intervals (pytypes.py, rule for rule).
#include "pytypes.hpp"

#include <algorithm>

namespace pyengine {

const Big I64_MIN = -Big::pow2(63);
const Big I64_MAX = Big::pow2(63) - Big(1);
const Big MEM = Big::pow2(47);
const Big HUGE_BOUND = Big::pow2(128);

static const TX& empty_tx() {
    static TX e;
    return e;
}

const char* kind_name(TK k) {
    switch (k) {
        case TK::Bottom: return "bottom";
        case TK::Top: return "top";
        case TK::None: return "none";
        case TK::Bool: return "bool";
        case TK::Float: return "float";
        case TK::Str: return "str";
        case TK::Int: return "int";
        case TK::Bytes: return "bytes";
        case TK::Bytearray: return "bytearray";
        case TK::Tuple: return "tuple";
        case TK::Opt: return "opt";
        case TK::List: return "list";
        case TK::Dict: return "dict";
        case TK::Set: return "set";
        case TK::Inst: return "inst";
        case TK::Cls: return "cls";
        case TK::Fn: return "fn";
        case TK::Module: return "module";
        case TK::Extern: return "extern";
        case TK::Iter: return "iter";
        case TK::Variant: return "variant";
        case TK::Closure: return "closure";
    }
    return "?";
}

T::T(TK k, std::vector<T> args, OBig lo, OBig hi, std::string name, int site_, std::string why) : kind(k), site(site_) {
    if (args.empty() && !lo && !hi && name.empty() && why.empty()) return;
    auto p = std::make_shared<TX>();
    p->args = std::move(args);
    p->lo = std::move(lo);
    p->hi = std::move(hi);
    p->name = std::move(name);
    p->why = std::move(why);
    x = std::move(p);
}

const std::vector<T>& T::args() const { return x ? x->args : empty_tx().args; }
const OBig& T::lo() const { return x ? x->lo : empty_tx().lo; }
const OBig& T::hi() const { return x ? x->hi : empty_tx().hi; }
const std::string& T::name() const { return x ? x->name : empty_tx().name; }
const std::string& T::why() const { return x ? x->why : empty_tx().why; }

bool T::fits_i64() const {
    return kind == TK::Int && lo() && hi() && I64_MIN <= *lo() && *hi() <= I64_MAX;
}
bool T::nonneg() const { return kind == TK::Int && lo() && lo()->sign() >= 0; }
bool T::positive() const { return kind == TK::Int && lo() && lo()->sign() > 0; }

std::string bstr(const OBig& b) { return b ? b->str() : "None"; }

std::string T::repr() const {
    switch (kind) {
        case TK::Int: return "int[" + bstr(lo()) + ".." + bstr(hi()) + "]";
        case TK::Bytes:
        case TK::Bytearray:
            if (hi()) return std::string(kind_name(kind)) + "(<=" + hi()->str() + ")";
            return kind_name(kind);
        case TK::List:
        case TK::Dict:
        case TK::Set: return std::string(kind_name(kind)) + "#" + std::to_string(site);
        case TK::Tuple:
        case TK::Opt:
        case TK::Variant: {
            std::string s = std::string(kind_name(kind)) + "[";
            for (size_t i = 0; i < args().size(); i++) s += (i ? ", " : "") + args()[i].repr();
            return s + "]";
        }
        case TK::Inst:
        case TK::Cls:
        case TK::Fn:
        case TK::Module:
        case TK::Extern:
        case TK::Closure: return std::string(kind_name(kind)) + " " + name();
        case TK::Top: return "top(" + why() + ")";
        default: return kind_name(kind);
    }
}

static bool obig_eq(const OBig& a, const OBig& b) {
    if (a.has_value() != b.has_value()) return false;
    return !a || *a == *b;
}

bool operator==(const T& a, const T& b) {
    if (a.kind != b.kind || a.site != b.site) return false;
    if (a.x == b.x) return true;
    const auto& aa = a.args();
    const auto& ba = b.args();
    if (aa.size() != ba.size()) return false;
    for (size_t i = 0; i < aa.size(); i++)
        if (!(aa[i] == ba[i])) return false;
    return obig_eq(a.lo(), b.lo()) && obig_eq(a.hi(), b.hi()) && a.name() == b.name();
}

std::string key_of(const T& t) {
    std::string s = std::to_string((int)t.kind);
    if (t.site) s += "#" + std::to_string(t.site);
    if (t.lo() || t.hi()) s += "[" + bstr(t.lo()) + "," + bstr(t.hi()) + "]";
    if (!t.name().empty()) s += "'" + t.name() + "'";
    if (!t.args().empty()) {
        s += "(";
        for (auto& a : t.args()) s += key_of(a) + ";";
        s += ")";
    }
    return s;
}

T BOTTOM() { return T(TK::Bottom); }
T NONE() { return T(TK::None); }
T BOOL() { return T(TK::Bool); }
T FLOAT() { return T(TK::Float); }
T STR() { return T(TK::Str); }
T BYTES() { return T(TK::Bytes); }
T BYTEARRAY() { return T(TK::Bytearray); }
T top(const std::string& why) { return T(TK::Top, {}, {}, {}, {}, 0, why); }

T INT(OBig lo, OBig hi) {
    if (lo && *lo < -HUGE_BOUND) lo.reset();
    if (hi && *hi > HUGE_BOUND) hi.reset();
    return T(TK::Int, {}, std::move(lo), std::move(hi));
}
T const_int(const Big& v) { return T(TK::Int, {}, v, v); }
T SIZE() { return INT(Big(0), MEM); }
T INDEX() { return INT(-MEM, MEM); }
T FIND() { return INT(Big(-1), MEM); }
T u(int bits) { return bits <= 128 ? INT(Big(0), Big::pow2(bits) - Big(1)) : INT(Big(0), {}); }
T s(int bits) { return bits <= 128 ? INT(-Big::pow2(bits - 1), Big::pow2(bits - 1) - Big(1)) : INT(); }
T bytes_of(OBig maxlen, TK kind) { return T(kind, {}, {}, std::move(maxlen)); }
T opt(const T& t) {
    if (t.kind == TK::Opt || t.kind == TK::None || t.kind == TK::Top || t.kind == TK::Bottom) return t;
    return T(TK::Opt, {t});
}
T tuple_of(std::vector<T> ts) { return T(TK::Tuple, std::move(ts)); }
T strip_opt(const T& t) { return t.kind == TK::Opt ? t.args()[0] : t; }
T named(TK k, const std::string& name) { return T(k, {}, {}, {}, name); }
T container(TK k, int site) { return T(k, {}, {}, {}, {}, site); }
T iter_of(const T& elem, OBig count) { return T(TK::Iter, {elem}, std::move(count)); }

// ------------------------------------------------------------------------------------ intervals
static OBig lo_min(const OBig& a, const OBig& b) {
    if (!a || !b) return {};
    return std::min(*a, *b);
}
static OBig hi_max(const OBig& a, const OBig& b) {
    if (!a || !b) return {};
    return std::max(*a, *b);
}
static OBig add(const OBig& a, const OBig& b) {
    if (!a || !b) return {};
    return *a + *b;
}

T int_join(const T& a, const T& b) { return INT(lo_min(a.lo(), b.lo()), hi_max(a.hi(), b.hi())); }
T iadd(const T& a, const T& b) { return INT(add(a.lo(), b.lo()), add(a.hi(), b.hi())); }
T isub(const T& a, const T& b) {
    OBig lo, hi;
    if (a.lo() && b.hi()) lo = *a.lo() - *b.hi();
    if (a.hi() && b.lo()) hi = *a.hi() - *b.lo();
    return INT(lo, hi);
}
T ineg(const T& a) {
    OBig lo, hi;
    if (a.hi()) lo = -*a.hi();
    if (a.lo()) hi = -*a.lo();
    return INT(lo, hi);
}
static bool any_none(const T& a, const T& b) { return !a.lo() || !a.hi() || !b.lo() || !b.hi(); }
static bool is_zero(const T& a) { return a.lo() && a.hi() && a.lo()->sign() == 0 && a.hi()->sign() == 0; }

T imul(const T& a, const T& b) {
    if (any_none(a, b)) {
        if (is_zero(a) || is_zero(b)) return const_int(Big(0));
        if (a.nonneg() && b.nonneg()) return INT(*a.lo() * *b.lo(), {});
        return INT();
    }
    Big c[4] = {*a.lo() * *b.lo(), *a.lo() * *b.hi(), *a.hi() * *b.lo(), *a.hi() * *b.hi()};
    return INT(*std::min_element(c, c + 4), *std::max_element(c, c + 4));
}

T ifloordiv(const T& a, const T& b) {
    if (any_none(a, b)) {
        if (a.nonneg() && b.positive()) return INT(Big(0), a.hi());
        return INT();
    }
    std::vector<Big> divs;
    for (const Big& d : {*b.lo(), *b.hi(), Big(-1), Big(1)})
        if (*b.lo() <= d && d <= *b.hi() && d.sign() != 0) divs.push_back(d);
    if (divs.empty()) return INT();
    std::vector<Big> c;
    for (const Big& x : {*a.lo(), *a.hi()})
        for (const Big& d : divs) c.push_back(floordiv(x, d));
    return INT(*std::min_element(c.begin(), c.end()), *std::max_element(c.begin(), c.end()));
}

T imod(const T& a, const T& b) {
    if (b.positive()) {
        OBig hi;
        if (b.hi()) hi = *b.hi() - Big(1);
        if (a.nonneg() && a.hi() && (!hi || *a.hi() < *hi))
            return INT(*b.lo() > *a.hi() ? *a.lo() : Big(0), a.hi());
        return INT(Big(0), hi);
    }
    if (b.hi() && b.hi()->sign() < 0) return INT(b.lo() ? OBig(*b.lo() + Big(1)) : OBig(), Big(0));
    OBig lo, hi;
    if (b.lo()) lo = std::min(Big(0), *b.lo() + Big(1));
    if (b.hi()) hi = std::max(Big(0), *b.hi() - Big(1));
    return INT(lo, hi);
}

T ishl(const T& a, const T& b) {
    if (!b.lo() || b.lo()->sign() < 0 || !b.hi() || *b.hi() > Big(128) || !a.lo() || !a.hi())
        return !a.nonneg() ? INT() : INT(Big(0), {});
    int bl = (int)b.lo()->i64(), bh = (int)b.hi()->i64();
    // (a negative a << n is a * 2**n: shl floors nothing)
    auto sh = [](const Big& x, int n) { return x.sign() < 0 ? -shl(-x, n) : shl(x, n); };
    Big c[4] = {sh(*a.lo(), bl), sh(*a.lo(), bh), sh(*a.hi(), bl), sh(*a.hi(), bh)};
    return INT(*std::min_element(c, c + 4), *std::max_element(c, c + 4));
}

static Big shr_big(const Big& a, const Big& n) {
    if (n > Big(100000)) return Big(a.sign() < 0 ? -1 : 0);
    return shr(a, (int)n.i64());
}

T ishr(const T& a, const T& b) {
    if (!b.lo() || b.lo()->sign() < 0) return INT();
    if (!a.lo() || !a.hi()) {
        if (a.nonneg()) return INT(Big(0), {});
        return INT();
    }
    std::vector<Big> c = {shr_big(*a.lo(), *b.lo()), shr_big(*a.hi(), *b.lo())};
    if (b.hi()) {
        c.push_back(shr_big(*a.lo(), *b.hi()));
        c.push_back(shr_big(*a.hi(), *b.hi()));
    } else {
        c.push_back(Big(a.lo()->sign() >= 0 ? 0 : -1));
    }
    return INT(*std::min_element(c.begin(), c.end()), *std::max_element(c.begin(), c.end()));
}

static Big bits_hi(const Big& x) {
    Big m = x.sign() > 0 ? x : Big(0);
    return Big::pow2(m.bit_length()) - Big(1);
}

T iand(const T& a, const T& b) {
    if (a.nonneg() && b.nonneg()) {
        OBig h;
        if (a.hi() && b.hi()) h = std::min(*a.hi(), *b.hi());
        else if (a.hi()) h = a.hi();
        else if (b.hi()) h = b.hi();
        return INT(Big(0), h);
    }
    if (a.nonneg()) return INT(Big(0), a.hi());
    if (b.nonneg()) return INT(Big(0), b.hi());
    if (any_none(a, b)) return INT();
    Big m = std::max({bits_hi(a.lo()->abs()), bits_hi(*a.hi()), bits_hi(b.lo()->abs()), bits_hi(*b.hi())});
    return INT(-m - Big(1), m);
}

T ior(const T& a, const T& b) {
    if (a.nonneg() && b.nonneg()) {
        if (!a.hi() || !b.hi()) return INT(Big(0), {});
        return INT(std::max(*a.lo(), *b.lo()), bits_hi(std::max(*a.hi(), *b.hi())));
    }
    if (any_none(a, b)) return INT();
    Big m = std::max({bits_hi(a.lo()->abs()), bits_hi(*a.hi()), bits_hi(b.lo()->abs()), bits_hi(*b.hi())});
    return INT(-m - Big(1), m);
}

T iinvert(const T& a) {
    OBig lo, hi;
    if (a.hi()) lo = -*a.hi() - Big(1);
    if (a.lo()) hi = -*a.lo() - Big(1);
    return INT(lo, hi);
}

T ipow_t(const T& a, const T& b) {
    if (any_none(a, b) || b.lo()->sign() < 0 || *b.hi() > Big(128) ||
        std::max(a.lo()->abs(), a.hi()->abs()) > Big::pow2(64))
        return INT();
    int bl = (int)b.lo()->i64(), bh = (int)b.hi()->i64();
    std::vector<Big> c;
    for (const Big& x : {*a.lo(), *a.hi()})
        for (int y : {bl, bh}) c.push_back(ipow(x, y));
    if (a.lo()->sign() < 0 && a.hi()->sign() > 0) c.push_back(Big(0));
    if (bl != bh)
        for (const Big& x : {*a.lo(), *a.hi()})
            for (int y : {bl + 1, std::max(bl, bh - 1)}) c.push_back(ipow(x, y));
    return INT(*std::min_element(c.begin(), c.end()), *std::max_element(c.begin(), c.end()));
}

T meet(const T& a, OBig lo, OBig hi) {
    OBig nlo = !lo ? a.lo() : (!a.lo() ? lo : OBig(std::max(*a.lo(), *lo)));
    OBig nhi = !hi ? a.hi() : (!a.hi() ? hi : OBig(std::min(*a.hi(), *hi)));
    if (nlo && nhi && *nlo > *nhi) return INT(nlo, nlo);
    return INT(nlo, nhi);
}

T widen_int(const T& old, const T& nw) {
    static const Big TH[] = {Big(0), Big(1), Big(255), Big(0xFFFF), Big((int64_t)0xFFFFFFFFLL), MEM, I64_MAX};
    OBig lo = nw.lo(), hi = nw.hi();
    if (old.lo() && (!lo || *lo < *old.lo())) {
        OBig r;
        if (lo)
            for (const Big& t : TH)
                if (-t <= *lo) {
                    r = -t;
                    break;
                }
        lo = r;
    }
    if (old.hi() && (!hi || *hi > *old.hi())) {
        OBig r;
        if (hi)
            for (const Big& t : TH)
                if (t >= *hi) {
                    r = t;
                    break;
                }
        hi = r;
    }
    return INT(lo, hi);
}

}  // namespace pyengine
