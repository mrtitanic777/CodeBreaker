// ops.cc -- Python's operators on py::var: arithmetic over unbounded ints and floats, sequences'
// + and *, sets' | & - ^, %-formatting; comparisons (int and float compared exactly, sequences
// lexicographically, TypeError across kinds that do not order); hashing as CPython hashes (so a set's
// order is CPython's); repr and str of every builtin; the reflected and in-place dunders of classes.
#include <charconv>
#include <cmath>
#include <cstring>
#include <limits>

#include "rt.hpp"

namespace py {

var NotImplemented, Ellipsis;

enum Op { ADD, SUB, MUL, TRUEDIV, FLOORDIV, MOD, POW, LSHIFT, RSHIFT, AND, OR, XOR, MATMUL };
static const char* op_sym[] = {"+", "-", "*", "/", "//", "%", "** or pow()", "<<", ">>", "&", "|", "^", "@"};
static const char* op_dunder[] = {"__add__", "__sub__", "__mul__", "__truediv__", "__floordiv__", "__mod__", "__pow__",
                                  "__lshift__", "__rshift__", "__and__", "__or__", "__xor__", "__matmul__"};
static const char* op_rdunder[] = {"__radd__", "__rsub__", "__rmul__", "__rtruediv__", "__rfloordiv__", "__rmod__", "__rpow__",
                                   "__rlshift__", "__rrshift__", "__rand__", "__ror__", "__rxor__", "__rmatmul__"};
static const char* op_idunder[] = {"__iadd__", "__isub__", "__imul__", "__itruediv__", "__ifloordiv__", "__imod__", "__ipow__",
                                   "__ilshift__", "__irshift__", "__iand__", "__ior__", "__ixor__", "__imatmul__"};

// ------------------------------------------------------------------------------------------ ints
bool is_big(const var& v) { return v.is_ref() && v.ref()->type->layout == Layout::Int; }
bool is_int(const var& x) { return x.is_small_int() || x.is_bool() || is_big(x); }
static bool is_intlike(const var& x) { return is_int(x); }
Big to_big(const var& v) {
    if (v.is_small_int() || v.is_bool()) return Big(v.small());
    if (is_big(v)) return static_cast<BigObj*>(v.ref())->v;
    raise(E_TypeError, "an integer is required, not '" + tname(v) + "'");
}
var new_int(const Big& b) {
    if (b.fits_i64()) return var(b.i64());
    auto* o = new BigObj();
    o->type = T_int;
    o->v = b;
    return var(static_cast<Obj*>(o));
}
static bool is_floatlike(const var& x) { return x.is_float() || (x.is_ref() && x.ref()->type->layout == Layout::Float); }
static double fval(const var& x) {
    if (x.is_float()) return x.flt();
    if (x.is_small_int() || x.is_bool()) return (double)x.small();
    if (is_big(x)) {
        const Big& b = static_cast<BigObj*>(x.ref())->v;
        // exact enough: via the decimal string (rounding as strtod)
        double d = std::strtod(b.str().c_str(), nullptr);
        if (std::isinf(d)) raise(E_OverflowError, "int too large to convert to float");
        return d;
    }
    if (x.is_ref() && x.ref()->type->layout == Layout::Float) return static_cast<FloatObj*>(x.ref())->v;
    raise(E_TypeError, "must be real number, not " + tname(x));
}
int64_t var::as_int64() const {
    if (tag_ == Tag::Int || tag_ == Tag::Bool) return i_;
    if (is_big(*this)) raise(E_OverflowError, "Python int too large to convert to C long");
    var r;
    if (lookup_special(*this, "__index__", r)) return call0(r).as_int64();
    raise(E_TypeError, "'" + tname(*this) + "' object cannot be interpreted as an integer");
}
double var::as_double() const { return fval(*this); }
int64_t index_(const var& v) { return v.as_int64(); }

static inline bool add_ovf(int64_t a, int64_t b, int64_t& r) { r = (int64_t)((uint64_t)a + (uint64_t)b); return ((a ^ r) & (b ^ r)) < 0; }
static inline bool sub_ovf(int64_t a, int64_t b, int64_t& r) { r = (int64_t)((uint64_t)a - (uint64_t)b); return ((a ^ b) & (a ^ r)) < 0; }
static inline bool mul_ovf(int64_t a, int64_t b, int64_t& r) {
    if (a == 0 || b == 0) { r = 0; return false; }
    int64_t hi;
    int64_t lo = _mul128(a, b, &hi);
    r = lo;
    return !((hi == 0 && lo >= 0) || (hi == -1 && lo < 0));
}

// two's complement bitwise ops on Big (via magnitude limbs)
static std::vector<uint32_t> twos(const Big& b, size_t n) {
    bool neg;
    std::vector<uint32_t> m;
    b.to_mag(neg, m);
    m.resize(n, 0);
    if (neg) {
        for (auto& x : m) x = ~x;
        for (size_t i = 0; i < n; i++) { if (++m[i] != 0) break; }
    }
    return m;
}
static Big from_twos(std::vector<uint32_t> m) {
    bool neg = !m.empty() && (m.back() & 0x80000000u);
    if (neg) {
        for (auto& x : m) x = ~x;
        for (size_t i = 0; i < m.size(); i++) { if (++m[i] != 0) break; }
    }
    while (!m.empty() && m.back() == 0) m.pop_back();
    return Big::from_mag(neg, m);
}
static Big big_bitop(const Big& a, const Big& b, int op) {
    size_t n = (size_t)std::max(a.bit_length(), b.bit_length()) / 32 + 2;
    auto x = twos(a, n), y = twos(b, n);
    for (size_t i = 0; i < n; i++) x[i] = op == AND ? (x[i] & y[i]) : op == OR ? (x[i] | y[i]) : (x[i] ^ y[i]);
    return from_twos(x);
}

static double int_truediv(const var& a, const var& b) {
    if ((a.is_small_int() || a.is_bool()) && (b.is_small_int() || b.is_bool())) {
        if (b.small() == 0) raise(E_ZeroDivisionError, "division by zero");
        return (double)a.small() / (double)b.small();
    }
    Big x = to_big(a), y = to_big(b);
    if (y.sign() == 0) raise(E_ZeroDivisionError, "division by zero");
    // correctly rounded enough: scale to 2**60 of quotient bits
    int shift = 64 - (x.bit_length() - y.bit_length());
    if (shift < 0) shift = 0;
    Big q, r;
    Big ax = x.abs(), ay = y.abs();
    Big::divmod_mag(shl(ax, shift), ay, q, r);
    double d = std::ldexp(std::strtod(q.str().c_str(), nullptr), -shift);
    return (x.sign() < 0) != (y.sign() < 0) ? -d : d;
}

static var int_pow(const var& a, const var& b) {
    Big e = to_big(b);
    if (e.sign() < 0) {
        double base = fval(a);
        if (base == 0.0) raise(E_ZeroDivisionError, "zero to a negative power");
        return std::pow(base, fval(b));
    }
    if (!e.fits_i64() || e.i64() > 100000000) {
        Big x = to_big(a);
        if (x == Big(0) || x == Big(1)) return new_int(x);
        if (x == Big(-1)) return new_int(floormod(e, Big(2)) == Big(0) ? Big(1) : Big(-1));
        raise(E_OverflowError, "exponent too large");
    }
    return new_int(ipow(to_big(a), (int)e.i64()));
}

static var int_binop(const var& a, const var& b, int op) {
    if ((a.is_small_int() || a.is_bool()) && (b.is_small_int() || b.is_bool())) {
        int64_t x = a.small(), y = b.small(), r;
        switch (op) {
        case ADD: if (!add_ovf(x, y, r)) return r; break;
        case SUB: if (!sub_ovf(x, y, r)) return r; break;
        case MUL: if (!mul_ovf(x, y, r)) return r; break;
        case FLOORDIV:
            if (y == 0) raise(E_ZeroDivisionError, "integer division or modulo by zero");
            if (!(x == INT64_MIN && y == -1)) {
                int64_t q = x / y;
                if ((x % y != 0) && ((x < 0) != (y < 0))) --q;
                return q;
            }
            break;
        case MOD:
            if (y == 0) raise(E_ZeroDivisionError, "integer modulo by zero");
            if (y == -1) return 0;
            r = x % y;
            if (r != 0 && ((r < 0) != (y < 0))) r += y;
            return r;
        case AND:
            if (a.is_bool() && b.is_bool()) return var((x & y) != 0);
            return x & y;
        case OR:
            if (a.is_bool() && b.is_bool()) return var((x | y) != 0);
            return x | y;
        case XOR:
            if (a.is_bool() && b.is_bool()) return var((x ^ y) != 0);
            return x ^ y;
        case LSHIFT:
            if (y < 0) raise(E_ValueError, "negative shift count");
            if (x == 0) return 0;
            if (y < 63) {
                int64_t s = x << y;
                if ((s >> y) == x) return s;
            }
            break;
        case RSHIFT:
            if (y < 0) raise(E_ValueError, "negative shift count");
            if (y >= 64) return x < 0 ? -1 : 0;
            return x >> y;
        case TRUEDIV: return int_truediv(a, b);
        case POW: return int_pow(a, b);
        default: return NotImplemented;
        }
    }
    if (op == TRUEDIV) return int_truediv(a, b);
    if (op == POW) return int_pow(a, b);
    Big x = to_big(a), y = to_big(b);
    switch (op) {
    case ADD: return new_int(x + y);
    case SUB: return new_int(x - y);
    case MUL: return new_int(x * y);
    case FLOORDIV:
        if (y.sign() == 0) raise(E_ZeroDivisionError, "integer division or modulo by zero");
        return new_int(floordiv(x, y));
    case MOD:
        if (y.sign() == 0) raise(E_ZeroDivisionError, "integer modulo by zero");
        return new_int(floormod(x, y));
    case AND: case OR: case XOR: return new_int(big_bitop(x, y, op));
    case LSHIFT:
        if (y.sign() < 0) raise(E_ValueError, "negative shift count");
        if (!y.fits_i64() || y.i64() > (1 << 30)) raise(E_OverflowError, "too many digits in integer");
        return new_int(shl(x, (int)y.i64()));
    case RSHIFT:
        if (y.sign() < 0) raise(E_ValueError, "negative shift count");
        if (!y.fits_i64() || y.i64() > (1 << 30)) return x.sign() < 0 ? -1 : 0;
        return new_int(shr(x, (int)y.i64()));
    default: return NotImplemented;
    }
}

static double py_fmod(double a, double b) {
    if (b == 0.0) raise(E_ZeroDivisionError, "float modulo by zero");
    double m = std::fmod(a, b);
    if (m != 0.0) {
        if ((b < 0) != (m < 0)) m += b;
    } else {
        m = std::copysign(0.0, b);
    }
    return m;
}
static double py_floordiv(double a, double b) {
    if (b == 0.0) raise(E_ZeroDivisionError, "float floor division by zero");
    double m = std::fmod(a, b);
    double div = (a - m) / b;
    if (m != 0.0 && ((b < 0) != (m < 0))) div -= 1.0;
    double f;
    if (div != 0.0) {
        f = std::floor(div);
        if (div - f > 0.5) f += 1.0;
    } else {
        f = std::copysign(0.0, a / b);
    }
    return f;
}
static var float_binop(double x, double y, int op) {
    switch (op) {
    case ADD: return x + y;
    case SUB: return x - y;
    case MUL: return x * y;
    case TRUEDIV:
        if (y == 0.0) raise(E_ZeroDivisionError, "float division by zero");
        return x / y;
    case FLOORDIV: return py_floordiv(x, y);
    case MOD: return py_fmod(x, y);
    case POW:
        if (x == 0.0 && y < 0) raise(E_ZeroDivisionError, "zero to a negative power");
        if (x < 0 && y != std::floor(y)) raise(E_ValueError, "complex results are not supported");
        return std::pow(x, y);
    default: return NotImplemented;
    }
}

// ------------------------------------------------------------------------------------------ sequences
std::string percent_format(const var& fmt, const var& args, bool bytes);
var seq_concat_(const var& a, const var& b, bool& ok);
var seq_repeat_(const var& a, int64_t n, bool& ok);
var set_binop_(const var& a, const var& b, int op, bool& ok);
var dict_or_(const var& a, const var& b, bool& ok);

static bool builtin_layout_value(const var& x) {
    // a value whose operators are the builtin's (a builtin, or a class deriving one without overriding)
    return !x.is_ref() || x.ref()->type->builtin;
}

static var builtin_binop(const var& a, const var& b, int op) {
    bool ai = is_intlike(a), bi = is_intlike(b);
    if (ai && bi) return int_binop(a, b, op);
    bool af = is_floatlike(a), bf = is_floatlike(b);
    if ((af || ai) && (bf || bi) && (af || bf)) return float_binop(fval(a), fval(b), op);
    bool ok = false;
    if (op == ADD) {
        var r = seq_concat_(a, b, ok);
        if (ok) return r;
    } else if (op == MUL) {
        if (bi && a.is_ref()) { var r = seq_repeat_(a, to_big(b).fits_i64() ? b.as_int64() : INT64_MAX, ok); if (ok) return r; }
        if (ai && b.is_ref()) { var r = seq_repeat_(b, to_big(a).fits_i64() ? a.as_int64() : INT64_MAX, ok); if (ok) return r; }
    } else if (op == MOD) {
        if (is_str(a)) return new_str(percent_format(a, b, false));
        if (a.is_ref() && (a.ref()->type->layout == Layout::Bytes || a.ref()->type->layout == Layout::ByteArray))
            return new_bytes(percent_format(a, b, true), a.ref()->type->layout == Layout::ByteArray);
    } else if (op == SUB || op == AND || op == OR || op == XOR) {
        var r = set_binop_(a, b, op, ok);
        if (ok) return r;
        if (op == OR) { r = dict_or_(a, b, ok); if (ok) return r; }
    }
    return NotImplemented;
}

static var binop(const var& a, const var& b, int op) {
    // the fast paths
    if (a.is_small_int() && b.is_small_int()) return int_binop(a, b, op);
    if (a.is_float() && b.is_float()) return float_binop(a.flt(), b.flt(), op);
    TypeObj* ta = type_of(a);
    TypeObj* tb = type_of(b);
    if (ta->builtin && tb->builtin) {
        var r = builtin_binop(a, b, op);
        if (!is(r, NotImplemented)) return r;
    } else {
        // b's reflected method first when b's type is a subclass of a's that overrides it
        const var* fa = ta->builtin ? nullptr : ta->lookup(op_dunder[op]);
        const var* rb = tb->builtin ? nullptr : tb->lookup(op_rdunder[op]);
        bool b_first = rb && ta != tb && is_subtype(tb, ta);
        if (b_first) {
            var r = call1(bind(*rb, b, tb), a);
            if (!is(r, NotImplemented)) return r;
        }
        if (fa) {
            var r = call1(bind(*fa, a, ta), b);
            if (!is(r, NotImplemented)) return r;
        } else if (ta->builtin || !ta->lookup(op_dunder[op])) {
            var r = builtin_binop(a, b, op);
            if (!is(r, NotImplemented)) return r;
        }
        if (rb && !b_first) {
            var r = call1(bind(*rb, b, tb), a);
            if (!is(r, NotImplemented)) return r;
        }
        if (!fa && !rb) {
            var r = builtin_binop(a, b, op);
            if (!is(r, NotImplemented)) return r;
        }
    }
    if (op == ADD && (is_str(a) || is_str(b)) && !(is_str(a) && is_str(b)))
        raise(E_TypeError, is_str(a) ? "can only concatenate str (not \"" + tb->name + "\") to str"
                                     : "unsupported operand type(s) for +: '" + ta->name + "' and '" + tb->name + "'");
    if (op == ADD && is_list(a) && !is_list(b)) raise(E_TypeError, "can only concatenate list (not \"" + tb->name + "\") to list");
    if (op == ADD && is_tuple(a) && !is_tuple(b)) raise(E_TypeError, "can only concatenate tuple (not \"" + tb->name + "\") to tuple");
    if (op == MUL && (a.is_ref() || b.is_ref()) && (a.is_float() || b.is_float()))
        raise(E_TypeError, "can't multiply sequence by non-int of type 'float'");
    raise(E_TypeError, std::string("unsupported operand type(s) for ") + op_sym[op] + ": '" + ta->name + "' and '" + tb->name + "'");
}

var operator+(const var& a, const var& b) { return binop(a, b, ADD); }
var operator-(const var& a, const var& b) { return binop(a, b, SUB); }
var operator*(const var& a, const var& b) { return binop(a, b, MUL); }
var operator/(const var& a, const var& b) { return binop(a, b, TRUEDIV); }
var operator%(const var& a, const var& b) { return binop(a, b, MOD); }
var operator<<(const var& a, const var& b) { return binop(a, b, LSHIFT); }
var operator>>(const var& a, const var& b) { return binop(a, b, RSHIFT); }
var operator&(const var& a, const var& b) { return binop(a, b, AND); }
var operator|(const var& a, const var& b) { return binop(a, b, OR); }
var operator^(const var& a, const var& b) { return binop(a, b, XOR); }
var floordiv(const var& a, const var& b) { return binop(a, b, FLOORDIV); }
var pow(const var& a, const var& b) { return binop(a, b, POW); }
var matmul(const var& a, const var& b) { return binop(a, b, MATMUL); }
var pow(const var& a, const var& b, const var& m) {
    if (m.is_none()) return pow(a, b);
    Big base = to_big(a), e = to_big(b), mod = to_big(m);
    if (mod.sign() == 0) raise(E_ValueError, "pow() 3rd argument cannot be 0");
    if (e.sign() < 0) {
        // pow(b, -e, m): the inverse of b modulo m (extended Euclid), to the power e
        var mv = new_int(mod.sign() < 0 ? -mod : mod);
        var r0 = a % mv, r1 = mv, s0 = var(1), s1 = var(0);
        while (r1 != var(0)) {
            var q = floordiv(r0, r1);
            var r2 = r0 - q * r1, s2 = s0 - q * s1;
            r0 = r1; r1 = r2; s0 = s1; s1 = s2;
        }
        if (r0 != var(1)) raise(E_ValueError, "base is not invertible for the given modulus");
        return pow(s0 % mv, new_int(-e), m);
    }
    Big r(1);
    base = floormod(base, mod);
    while (e.sign() > 0) {
        if (floormod(e, Big(2)) == Big(1)) r = floormod(r * base, mod);
        base = floormod(base * base, mod);
        e = shr(e, 1);
    }
    return new_int(floormod(r, mod));
}

static var inplace(var& a, const var& b, int op) {
    TypeObj* ta = type_of(a);
    if (a.is_ref() && (ta->layout == Layout::List || ta->layout == Layout::Dict || ta->layout == Layout::Set ||
                       ta->layout == Layout::ByteArray || !ta->builtin)) {
        if (const var* f = ta->lookup(op_idunder[op])) {
            var r = call1(bind(*f, a, ta), b);
            if (!is(r, NotImplemented)) { a = r; return a; }
        }
    }
    a = binop(a, b, op);
    return a;
}
var& var::operator+=(const var& o) { inplace(*this, o, ADD); return *this; }
var& var::operator-=(const var& o) { inplace(*this, o, SUB); return *this; }
var& var::operator*=(const var& o) { inplace(*this, o, MUL); return *this; }
var& var::operator/=(const var& o) { inplace(*this, o, TRUEDIV); return *this; }
var& var::operator%=(const var& o) { inplace(*this, o, MOD); return *this; }
var& var::operator|=(const var& o) { inplace(*this, o, OR); return *this; }
var& var::operator&=(const var& o) { inplace(*this, o, AND); return *this; }
var& var::operator^=(const var& o) { inplace(*this, o, XOR); return *this; }
var& var::operator<<=(const var& o) { inplace(*this, o, LSHIFT); return *this; }
var& var::operator>>=(const var& o) { inplace(*this, o, RSHIFT); return *this; }

// unary
static var unary(const var& a, const char* dunder, int which) {
    if (a.is_small_int() || a.is_bool()) {
        int64_t x = a.small();
        if (which == 0) { if (x == INT64_MIN) return new_int(-Big(x)); return -x; }
        if (which == 1) return var(x);
        return ~x;
    }
    if (a.is_float()) {
        if (which == 0) return -a.flt();
        if (which == 1) return a;
    }
    if (is_big(a)) {
        Big b = to_big(a);
        if (which == 0) return new_int(-b);
        if (which == 1) return a;
        return new_int(-b - Big(1));
    }
    var m;
    if (lookup_special(a, dunder, m)) return call0(m);
    const char* sym = which == 0 ? "-" : which == 1 ? "+" : "~";
    raise(E_TypeError, std::string("bad operand type for unary ") + sym + ": '" + tname(a) + "'");
}
var operator-(const var& a) { return unary(a, "__neg__", 0); }
var operator+(const var& a) { return unary(a, "__pos__", 1); }
var operator~(const var& a) { return unary(a, "__invert__", 2); }

// ------------------------------------------------------------------------------------------ identity
bool is(const var& a, const var& b) {
    if (a.tag() != b.tag()) return false;
    switch (a.tag()) {
    case var::Tag::None: case var::Tag::Unbound: return true;
    case var::Tag::Bool: return a.small() == b.small();
    case var::Tag::Int: return a.small() == b.small();    // (small ints are cached in CPython: -5..256; is on others is undefined)
    case var::Tag::Float: return a.flt() == b.flt();
    case var::Tag::Ref: return a.ref() == b.ref();
    }
    return false;
}
var id(const var& x) {
    if (x.is_ref()) return var((int64_t)(uintptr_t)x.ref());
    // (scalars are inline: an id stable for the value)
    return var((int64_t)(0x10000 + (uint64_t)x.small() * 16 + (int)x.tag()));
}

// ------------------------------------------------------------------------------------------ comparison
enum Cmp { LT, LE, EQ, NE, GT, GE };
static const char* cmp_dunder[] = {"__lt__", "__le__", "__eq__", "__ne__", "__gt__", "__ge__"};
static const int cmp_swap[] = {GT, GE, EQ, NE, LT, LE};
static const char* cmp_sym[] = {"<", "<=", "==", "!=", ">", ">="};

static int cmp_num(const var& a, const var& b) {   // -1 0 1, 2 unordered (nan)
    if ((a.is_small_int() || a.is_bool()) && (b.is_small_int() || b.is_bool())) return a.small() < b.small() ? -1 : a.small() > b.small();
    if (is_intlike(a) && is_intlike(b)) return cmp(to_big(a), to_big(b));
    double x, y;
    if (is_floatlike(a) && is_floatlike(b)) { x = fval(a); y = fval(b); }
    else if (is_floatlike(a)) {
        x = fval(a);
        if (std::isnan(x)) return 2;
        if (std::isinf(x)) return x < 0 ? -1 : 1;
        if (b.is_small_int() || b.is_bool()) {
            int64_t i = b.small();
            if (i > -(1LL << 53) && i < (1LL << 53)) { y = (double)i; return x < y ? -1 : x > y; }
        }
        // exact: compare floor(x) as an int, then the fraction
        double fl = std::floor(x);
        char buf[400];
        snprintf(buf, sizeof buf, "%.0f", fl);
        int c = cmp(Big::from_decimal(buf), to_big(b));
        if (c != 0) return c;
        return x > fl ? 1 : 0;
    } else {
        int c = cmp_num(b, a);
        return c == 2 ? 2 : -c;
    }
    if (std::isnan(x) || std::isnan(y)) return 2;
    return x < y ? -1 : x > y;
}

static bool cmp_result(int c, int op) {
    if (c == 2) return op == NE;
    switch (op) {
    case LT: return c < 0;
    case LE: return c <= 0;
    case EQ: return c == 0;
    case NE: return c != 0;
    case GT: return c > 0;
    default: return c >= 0;
    }
}

bool same_or_eq(const var& a, const var& b);
int seq_compare_(const var& a, const var& b, int op, bool& ok);   // containers.cc: lexicographic
bool str_eq_(const var& a, const var& b);
bool set_eq_(const var& a, const var& b, bool& ok);
int set_cmp_(const var& a, const var& b, int op, bool& ok);
bool dict_eq_(const var& a, const var& b, bool& ok);

static bool is_numeric(const var& x) { return is_intlike(x) || is_floatlike(x); }

// the builtin answer, or -1 for "not for builtins"
static int builtin_compare(const var& a, const var& b, int op) {
    if (is_numeric(a) && is_numeric(b)) return cmp_result(cmp_num(a, b), op);
    TypeObj* ta = type_of(a);
    TypeObj* tb = type_of(b);
    Layout la = ta->layout, lb = tb->layout;
    if (la == Layout::Str && lb == Layout::Str) {
        const std::string& x = static_cast<StrObj*>(a.ref())->s;
        const std::string& y = static_cast<StrObj*>(b.ref())->s;
        int c = x.compare(y);
        return cmp_result(c < 0 ? -1 : c > 0, op);
    }
    if ((la == Layout::Bytes || la == Layout::ByteArray) && (lb == Layout::Bytes || lb == Layout::ByteArray)) {
        const std::string& x = static_cast<BytesObj*>(a.ref())->s;
        const std::string& y = static_cast<BytesObj*>(b.ref())->s;
        int c = x.compare(y);
        return cmp_result(c < 0 ? -1 : c > 0, op);
    }
    bool ok = false;
    if ((la == Layout::List && lb == Layout::List) || (la == Layout::Tuple && lb == Layout::Tuple)) {
        int r = seq_compare_(a, b, op, ok);
        if (ok) return r;
    }
    if ((la == Layout::Set || la == Layout::FrozenSet) && (lb == Layout::Set || lb == Layout::FrozenSet)) {
        int r = set_cmp_(a, b, op, ok);
        if (ok) return r;
    }
    if (op == EQ || op == NE) {
        if (la == Layout::Dict && lb == Layout::Dict) {
            bool r = dict_eq_(a, b, ok);
            if (ok) return op == EQ ? r : !r;
        }
        if (la == Layout::Range && lb == Layout::Range) {
            bool r = seq_compare_(a, b, EQ, ok) == 1;
            return op == EQ ? r : !r;
        }
        if (a.is_none() || b.is_none() || la != lb || la == Layout::Object || la == Layout::Type || la == Layout::Function ||
            la == Layout::Module || la == Layout::Native || la == Layout::Cell || la == Layout::Iter || la == Layout::Generator ||
            la == Layout::Exception || la == Layout::Slice || la == Layout::NativeData || la == Layout::Property) {
            if (la == Layout::Method && lb == Layout::Method) {
                auto* m = static_cast<MethodObj*>(a.ref());
                auto* n = static_cast<MethodObj*>(b.ref());
                bool r = is(m->self, n->self) && same_or_eq(m->func, n->func);
                return op == EQ ? r : !r;
            }
            if (la == Layout::Slice && lb == Layout::Slice) {
                auto* s = static_cast<SliceObj*>(a.ref());
                auto* t = static_cast<SliceObj*>(b.ref());
                bool r = eq_(s->start, t->start) && eq_(s->stop, t->stop) && eq_(s->step, t->step);
                return op == EQ ? r : !r;
            }
            bool r = is(a, b);
            return op == EQ ? r : !r;
        }
    }
    return -1;
}

static var rich_compare(const var& a, const var& b, int op) {
    TypeObj* ta = type_of(a);
    TypeObj* tb = type_of(b);
    if (!(ta->builtin && tb->builtin)) {
        const var* fb = tb->builtin ? nullptr : tb->lookup(cmp_dunder[cmp_swap[op]]);
        bool b_first = fb && ta != tb && is_subtype(tb, ta);
        if (b_first) {
            var r = call1(bind(*fb, b, tb), a);
            if (!is(r, NotImplemented)) return r;
        }
        if (const var* fa = ta->builtin ? nullptr : ta->lookup(cmp_dunder[op])) {
            if (!(fa->is_ref() && fa->ref()->type == T_native)) {
                var r = call1(bind(*fa, a, ta), b);
                if (!is(r, NotImplemented)) return r;
            }
        }
        if (fb && !b_first && !(fb->is_ref() && fb->ref()->type == T_native)) {
            var r = call1(bind(*fb, b, tb), a);
            if (!is(r, NotImplemented)) return r;
        }
        if (op == NE) {
            // __ne__ defaults to not __eq__
            if (const var* fe = ta->builtin ? nullptr : ta->lookup("__eq__")) {
                var r = call1(bind(*fe, a, ta), b);
                if (!is(r, NotImplemented)) return var(!r.truthy());
            }
        }
    }
    int r = builtin_compare(a, b, op);
    if (r >= 0) return var(r != 0);
    if (op == EQ) return var(is(a, b));
    if (op == NE) return var(!is(a, b));
    raise(E_TypeError, std::string("'") + cmp_sym[op] + "' not supported between instances of '" + ta->name + "' and '" + tb->name + "'");
}

static bool fast_eq(const var& a, const var& b, bool& done) {
    done = true;
    if (a.is_small_int() && b.is_small_int()) return a.small() == b.small();
    if (a.is_ref() && b.is_ref()) {
        if (a.ref() == b.ref() && a.ref()->type->layout != Layout::Float) {
            if (a.ref()->type->builtin) return true;
        }
        if (a.ref()->type == T_str && b.ref()->type == T_str) {
            auto* x = static_cast<StrObj*>(a.ref());
            auto* y = static_cast<StrObj*>(b.ref());
            if (x->h != -1 && y->h != -1 && x->h != y->h) return false;
            return x->s == y->s;
        }
    }
    if (a.is_none() || b.is_none()) {
        if (a.is_none() && b.is_none()) return true;
        if (!(a.is_ref() && !a.ref()->type->builtin) && !(b.is_ref() && !b.ref()->type->builtin)) return false;
    }
    done = false;
    return false;
}
bool operator==(const var& a, const var& b) {
    bool done;
    bool r = fast_eq(a, b, done);
    if (done) return r;
    return rich_compare(a, b, EQ).truthy();
}
bool operator!=(const var& a, const var& b) {
    bool done;
    bool r = fast_eq(a, b, done);
    if (done) return !r;
    return rich_compare(a, b, NE).truthy();
}
bool operator<(const var& a, const var& b) {
    if (a.is_small_int() && b.is_small_int()) return a.small() < b.small();
    return rich_compare(a, b, LT).truthy();
}
bool operator<=(const var& a, const var& b) {
    if (a.is_small_int() && b.is_small_int()) return a.small() <= b.small();
    return rich_compare(a, b, LE).truthy();
}
bool operator>(const var& a, const var& b) {
    if (a.is_small_int() && b.is_small_int()) return a.small() > b.small();
    return rich_compare(a, b, GT).truthy();
}
bool operator>=(const var& a, const var& b) {
    if (a.is_small_int() && b.is_small_int()) return a.small() >= b.small();
    return rich_compare(a, b, GE).truthy();
}
var eq(const var& a, const var& b) { return rich_compare(a, b, EQ); }
var lt(const var& a, const var& b) { return rich_compare(a, b, LT); }
bool eq_(const var& a, const var& b) { return a == b; }
bool lt_(const var& a, const var& b) { return a < b; }
bool same_or_eq(const var& a, const var& b) { return is(a, b) || a == b; }
int cmp_(const var& a, const var& b) { return a < b ? -1 : (b < a ? 1 : 0); }

// ------------------------------------------------------------------------------------------ hashing
static const uint64_t kModulus = (1ULL << 61) - 1;
int64_t hash_int(const Big& b) {
    if (b.fits_i64()) {
        int64_t v = b.i64();
        if (v >= -(int64_t)kModulus && v <= (int64_t)kModulus) {
            // (Python: v mod (2**61 - 1) with the sign kept)
            int64_t r = v % (int64_t)kModulus;
            if (r == -1) r = -2;
            return r;
        }
    }
    bool neg;
    std::vector<uint32_t> mag;
    b.to_mag(neg, mag);
    uint64_t x = 0;
    for (size_t i = mag.size(); i-- > 0;) {
        // x = (x * 2**32 + limb) mod M, by 30-bit... (do it bitwise: 32 shifts of 1)
        for (int k = 31; k >= 0; k--) {
            x = ((x << 1) & kModulus) | (x >> 60);
            if ((mag[i] >> k) & 1) { x += 1; if (x >= kModulus) x -= kModulus; }
        }
    }
    int64_t r = (int64_t)x;
    if (neg) r = -r;
    if (r == -1) r = -2;
    return r;
}
int64_t hash_double(double v) {
    if (std::isinf(v)) return v > 0 ? 314159 : -314159;
    if (std::isnan(v)) return 0;
    int e;
    double m = std::frexp(v, &e);
    int sign = 1;
    if (m < 0) { sign = -1; m = -m; }
    uint64_t x = 0;
    while (m) {
        x = ((x << 28) & kModulus) | x >> (61 - 28);
        m *= 268435456.0;
        e -= 28;
        uint64_t y = (uint64_t)m;
        m -= (double)y;
        x += y;
        if (x >= kModulus) x -= kModulus;
    }
    e = e >= 0 ? e % 61 : 61 - 1 - ((-1 - e) % 61);
    x = ((x << e) & kModulus) | x >> (61 - e);
    int64_t r = (int64_t)x * sign;
    if (r == -1) r = -2;
    return r;
}
int64_t str_hash_(const StrObj* s);
int64_t bytes_hash_(const BytesObj* s);
int64_t tuple_hash_(const TupleObj* t);
int64_t frozenset_hash_(const SetObj* s);

int64_t hash_(const var& x) {
    switch (x.tag()) {
    case var::Tag::None: return (int64_t)0xFCA86420 >> 4;
    case var::Tag::Bool: return x.small();
    case var::Tag::Int: {
        int64_t v = x.small();
        if (v >= -(int64_t)kModulus && v <= (int64_t)kModulus) {
            int64_t r = v % (int64_t)kModulus;
            return r == -1 ? -2 : r;
        }
        return hash_int(Big(v));
    }
    case var::Tag::Float: {
        double d = x.flt();
        if (std::isnan(d)) return (int64_t)(uintptr_t)&x >> 4;
        return hash_double(d);
    }
    case var::Tag::Unbound: type_of(x);
    case var::Tag::Ref: break;
    }
    Obj* o = x.ref();
    TypeObj* t = o->type;
    if (!t->builtin) {
        if (const var* h = t->lookup("__hash__")) {
            if (h->is_none()) raise(E_TypeError, "unhashable type: '" + t->name + "'");
            if (h->is_ref() && h->ref()->type == T_function) {
                var r = call0(bind(*h, x, t));
                return to_big(r).fits_i64() ? r.as_int64() : hash_int(to_big(r));
            }
        }
    }
    switch (t->layout) {
    case Layout::Str: return str_hash_(static_cast<StrObj*>(o));
    case Layout::Int: return hash_int(static_cast<BigObj*>(o)->v);
    case Layout::Float: return hash_double(static_cast<FloatObj*>(o)->v);
    case Layout::Bytes: return bytes_hash_(static_cast<BytesObj*>(o));
    case Layout::Tuple: return tuple_hash_(static_cast<TupleObj*>(o));
    case Layout::FrozenSet: return frozenset_hash_(static_cast<SetObj*>(o));
    case Layout::List: case Layout::Dict: case Layout::Set: case Layout::ByteArray: case Layout::Slice:
        raise(E_TypeError, "unhashable type: '" + t->name + "'");
    case Layout::Method: {
        auto* m = static_cast<MethodObj*>(o);
        return hash_(m->self) ^ hash_(m->func);
    }
    case Layout::Range: {
        auto* r = static_cast<RangeObj*>(o);
        return tuple_hash_(static_cast<TupleObj*>(new_tuple({r->start, r->stop, r->step}).ref()));
    }
    default: {
        // object.__hash__: the address, rotated by 4 as CPython
        uint64_t p = (uint64_t)(uintptr_t)o;
        int64_t r = (int64_t)((p >> 4) | (p << 60));
        return r == -1 ? -2 : r;
    }
    }
}
var hash(const var& x) { return hash_(x); }

// ------------------------------------------------------------------------------------------ repr / str
std::string repr_float(double d) {
    if (std::isnan(d)) return "nan";
    if (std::isinf(d)) return d > 0 ? "inf" : "-inf";
    if (d == 0) return std::signbit(d) ? "-0.0" : "0.0";
    char buf[64];
    // the shortest digits that round-trip, then Python's layout (repr: 'r' format)
    auto r = std::to_chars(buf, buf + sizeof buf, d, std::chars_format::scientific);
    std::string s(buf, r.ptr);
    bool neg = s[0] == '-';
    if (neg) s = s.substr(1);
    size_t e = s.find('e');
    std::string mant = s.substr(0, e);
    int exp = std::atoi(s.c_str() + e + 1);
    std::string digits;
    for (char c : mant) if (c != '.') digits += c;
    while (digits.size() > 1 && digits.back() == '0') digits.pop_back();
    std::string out;
    if (exp < -4 || exp >= 16) {
        out = digits.substr(0, 1);
        if (digits.size() > 1) out += "." + digits.substr(1);
        char eb[16];
        snprintf(eb, sizeof eb, "e%c%02d", exp < 0 ? '-' : '+', exp < 0 ? -exp : exp);
        out += eb;
    } else if (exp < 0) {
        out = "0." + std::string((size_t)(-exp - 1), '0') + digits;
    } else {
        if ((int)digits.size() <= exp + 1) out = digits + std::string((size_t)(exp + 1 - digits.size()), '0') + ".0";
        else out = digits.substr(0, (size_t)exp + 1) + "." + digits.substr((size_t)exp + 1);
    }
    return neg ? "-" + out : out;
}

std::string str_repr_(const std::string& s);
std::string bytes_repr_(const std::string& s, bool bytearray);
std::string container_repr_(const var& x, bool& ok);

static std::string addr(const void* p) {
    char b[32];
    snprintf(b, sizeof b, "0x%016llx", (unsigned long long)(uintptr_t)p);
    return b;
}

std::string repr_(const var& x) {
    switch (x.tag()) {
    case var::Tag::None: return "None";
    case var::Tag::Bool: return x.small() ? "True" : "False";
    case var::Tag::Int: return std::to_string(x.small());
    case var::Tag::Float: return repr_float(x.flt());
    case var::Tag::Unbound: type_of(x);
    case var::Tag::Ref: break;
    }
    Obj* o = x.ref();
    TypeObj* t = o->type;
    if (!t->builtin || t->is_exception) {
        if (const var* r = t->lookup("__repr__")) {
            var v = call0(bind(*r, x, t));
            if (!is_str(v)) raise(E_TypeError, "__repr__ returned non-string (type " + tname(v) + ")");
            return str_utf8(v);
        }
    }
    switch (t->layout) {
    case Layout::Str: return str_repr_(static_cast<StrObj*>(o)->s);
    case Layout::Int: return static_cast<BigObj*>(o)->v.str();
    case Layout::Float: return repr_float(static_cast<FloatObj*>(o)->v);
    case Layout::Bytes: return bytes_repr_(static_cast<BytesObj*>(o)->s, false);
    case Layout::ByteArray: return bytes_repr_(static_cast<BytesObj*>(o)->s, true);
    case Layout::Type: {
        auto* ty = static_cast<TypeObj*>(o);
        if (ty->module == "builtins") return "<class '" + ty->qualname + "'>";
        return "<class '" + ty->module + "." + ty->qualname + "'>";
    }
    case Layout::Function: {
        auto* f = static_cast<FuncObj*>(o);
        return "<function " + f->qualname + " at " + addr(o) + ">";
    }
    case Layout::Native: {
        auto* n = static_cast<NativeObj*>(o);
        if (!n->self.is_unbound()) return "<built-in method " + n->name + " of " + tname(n->self) + " object at " + addr(n->self.ref()) + ">";
        return "<built-in function " + n->name + ">";
    }
    case Layout::Method: {
        auto* m = static_cast<MethodObj*>(o);
        std::string qn = m->func.is_ref() && m->func.ref()->type == T_function ? static_cast<FuncObj*>(m->func.ref())->qualname : "?";
        return "<bound method " + qn + " of " + repr_(m->self) + ">";
    }
    case Layout::Module: return "<module '" + static_cast<ModuleObj*>(o)->name + "'>";
    case Layout::Range: {
        auto* r = static_cast<RangeObj*>(o);
        std::string s = "range(" + repr_(r->start) + ", " + repr_(r->stop);
        if (!(r->step.is_small_int() && r->step.small() == 1)) s += ", " + repr_(r->step);
        return s + ")";
    }
    case Layout::Slice: {
        auto* s = static_cast<SliceObj*>(o);
        return "slice(" + repr_(s->start) + ", " + repr_(s->stop) + ", " + repr_(s->step) + ")";
    }
    case Layout::Cell: return "<cell at " + addr(o) + ">";
    case Layout::Property: return "<property object at " + addr(o) + ">";
    case Layout::Generator: return "<generator object at " + addr(o) + ">";
    default: break;
    }
    bool ok = false;
    std::string c = container_repr_(x, ok);
    if (ok) return c;
    if (t == T_notimpl) return "NotImplemented";
    if (t == T_ellipsis) return "Ellipsis";
    if (const var* r = t->lookup("__repr__")) {
        if (r->is_ref() && r->ref()->type == T_native) {
            var v = call0(bind(*r, x, t));
            return str_utf8(v);
        }
    }
    std::string mod = t->module == "builtins" ? "" : t->module + ".";
    return "<" + mod + t->qualname + " object at " + addr(o) + ">";
}

std::string str_(const var& x) {
    if (x.is_ref()) {
        Obj* o = x.ref();
        TypeObj* t = o->type;
        if (t == T_str) return static_cast<StrObj*>(o)->s;
        if (!t->builtin || t->is_exception) {
            if (const var* s = t->lookup("__str__")) {
                var v = call0(bind(*s, x, t));
                if (!is_str(v)) raise(E_TypeError, "__str__ returned non-string (type " + tname(v) + ")");
                return str_utf8(v);
            }
        }
        if (t->layout == Layout::Str) return static_cast<StrObj*>(o)->s;
    }
    return repr_(x);
}
std::string var::str() const { return str_(*this); }
std::string var::repr() const { return repr_(*this); }
var repr(const var& x) { return new_str(repr_(x)); }

void init_ops() {
    NotImplemented = var([] { auto* o = new Obj(); o->type = T_notimpl; return o; }());
    Ellipsis = var([] { auto* o = new Obj(); o->type = T_ellipsis; return o; }());
}

}  // namespace py
