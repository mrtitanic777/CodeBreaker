#include "cb/translate/frontends/python/pytypes.hpp"

#include <algorithm>
#include <array>
#include <format>

#include "cb/core/text.hpp"

namespace cb::translate::frontends::python::pytypes {

namespace {

// a shift count too large to matter is one that clears or fills every bit of a Wide
constexpr unsigned kHugeShift = 1000;
constexpr std::int64_t kMaxShiftLeft = 128;
constexpr unsigned kMaxExponent = 128;

Wide wide_pow2(unsigned bits) { return Wide::pow2(bits); }

bool is_bounded(const Type& a) { return a.lo && a.hi; }

Bound lower_of(const Bound& a, const Bound& b) { return a && b ? std::optional<Wide>(std::min(*a, *b)) : std::nullopt; }
Bound upper_of(const Bound& a, const Bound& b) { return a && b ? std::optional<Wide>(std::max(*a, *b)) : std::nullopt; }
Bound sum_of(const Bound& x, const Bound& y) { return x && y ? std::optional<Wide>(*x + *y) : std::nullopt; }

Type interval_of(std::initializer_list<Wide> candidates) {
    return int_range(*std::ranges::min_element(candidates), *std::ranges::max_element(candidates));
}

// (1 << bit_length(max(x, 0))) - 1: the all-ones mask over the bits x can use
Wide mask_over(const Wide& x) { return wide_pow2((x.sign() > 0 ? x : Wide(0)).bit_length()) - Wide(1); }

// the mask over every bound of two bounded intervals
Wide mask_over_all(const Type& a, const Type& b) {
    return std::max({mask_over(a.lo->abs()), mask_over(*a.hi), mask_over(b.lo->abs()), mask_over(*b.hi)});
}

unsigned shift_count(const Wide& n) {
    const auto v = n.to_i64();
    return v && *v < static_cast<std::int64_t>(kHugeShift) ? static_cast<unsigned>(*v) : kHugeShift;
}

}  // namespace

std::string_view kind_word(Kind kind) {
    switch (kind) {
        case Kind::Bottom: return "bottom";
        case Kind::Top: return "top";
        case Kind::None: return "none";
        case Kind::Bool: return "bool";
        case Kind::Float: return "float";
        case Kind::Str: return "str";
        case Kind::Int: return "int";
        case Kind::Bytes: return "bytes";
        case Kind::Bytearray: return "bytearray";
        case Kind::Tuple: return "tuple";
        case Kind::Opt: return "opt";
        case Kind::List: return "list";
        case Kind::Dict: return "dict";
        case Kind::Set: return "set";
        case Kind::Inst: return "inst";
        case Kind::Cls: return "cls";
        case Kind::Fn: return "fn";
        case Kind::Module: return "module";
        case Kind::Extern: return "extern";
    }
    return "";
}

const Wide& i64_min() { static const Wide v(INT64_MIN); return v; }
const Wide& i64_max() { static const Wide v(INT64_MAX); return v; }
const Wide& mem() { static const Wide v = Wide::pow2(47); return v; }
const Wide& huge() { static const Wide v = Wide::pow2(128); return v; }

// ---- Type -----------------------------------------------------------------------------------------------
bool Type::operator==(const Type& other) const {
    return kind == other.kind && lo == other.lo && hi == other.hi && name == other.name && site == other.site &&
           args == other.args;
}

std::size_t Type::hash() const {
    std::size_t h = static_cast<std::size_t>(kind) * 0x9E3779B97F4A7C15ull;
    const auto mix = [&h](std::size_t v) { h = (h ^ v) * 0x100000001B3ull + 0x7F4A7C15u; };
    mix(lo ? lo->hash() : 1);
    mix(hi ? hi->hash() : 2);
    mix(name ? std::hash<std::string>()(*name) : 3);
    mix(site ? *site : 4);
    for (const Type& a : args) mix(a.hash());
    return h;
}

bool Type::fits_i64() const { return kind == Kind::Int && lo && hi && i64_min() <= *lo && *hi <= i64_max(); }
bool Type::nonneg() const { return kind == Kind::Int && lo && lo->sign() >= 0; }
bool Type::positive() const { return kind == Kind::Int && lo && lo->sign() > 0; }
bool Type::has_zero() const {
    return !(kind == Kind::Int && ((lo && lo->sign() > 0) || (hi && hi->sign() < 0)));
}

std::string describe(const Type& t) {
    const auto bound = [](const Bound& b) { return b ? b->to_string() : std::string("None"); };
    switch (t.kind) {
        case Kind::Int: return std::format("int[{}..{}]", bound(t.lo), bound(t.hi));
        case Kind::Bytes:
        case Kind::Bytearray:
            if (t.hi) return std::format("{}(<={})", kind_word(t.kind), t.hi->to_string());
            break;
        case Kind::List:
        case Kind::Dict:
        case Kind::Set:
            return std::format("{}#{}", kind_word(t.kind), t.site ? std::to_string(*t.site) : "None");
        case Kind::Tuple:
        case Kind::Opt: {
            std::vector<std::string> parts;
            for (const Type& a : t.args) parts.push_back(describe(a));
            return std::format("{}[{}]", kind_word(t.kind), text::join(parts, ", "));
        }
        case Kind::Inst:
        case Kind::Cls:
        case Kind::Fn:
        case Kind::Module:
        case Kind::Extern: return std::format("{} {}", kind_word(t.kind), t.name ? *t.name : "None");
        case Kind::Top: return std::format("top({})", t.why ? *t.why : "None");
        default: break;
    }
    return std::string(kind_word(t.kind));
}

// ---- constructors -------------------------------------------------------------------------------------
const Type& bottom() { static const Type t{.kind = Kind::Bottom}; return t; }
const Type& none() { static const Type t{.kind = Kind::None}; return t; }
const Type& boolean() { static const Type t{.kind = Kind::Bool}; return t; }
const Type& floating() { static const Type t{.kind = Kind::Float}; return t; }
const Type& str() { static const Type t{.kind = Kind::Str}; return t; }
const Type& bytes() { static const Type t{.kind = Kind::Bytes}; return t; }
const Type& bytearray() { static const Type t{.kind = Kind::Bytearray}; return t; }
Type top(std::string why) { return {.kind = Kind::Top, .why = std::move(why)}; }

Type int_range(const Bound& lo, const Bound& hi) {
    Type t{.kind = Kind::Int, .lo = lo, .hi = hi};
    if (t.lo && *t.lo < -huge()) t.lo.reset();
    if (t.hi && *t.hi > huge()) t.hi.reset();
    return t;
}

Type const_int(const Wide& v) { return {.kind = Kind::Int, .lo = v, .hi = v}; }

const Type& size_type() { static const Type t = int_range(Wide(0), mem()); return t; }
const Type& index_type() { static const Type t = int_range(-mem(), mem()); return t; }
const Type& find_type() { static const Type t = int_range(Wide(-1), mem()); return t; }

Type unsigned_int(unsigned bits) {
    return bits <= 128 ? int_range(Wide(0), wide_pow2(bits) - Wide(1)) : int_range(Wide(0), std::nullopt);
}

Type signed_int(unsigned bits) {
    return bits <= 128 ? int_range(-wide_pow2(bits - 1), wide_pow2(bits - 1) - Wide(1)) : int_range();
}

Type bytes_of(const Bound& max_len, Kind kind) { return {.kind = kind, .hi = max_len}; }

Type optional_of(const Type& t) {
    if (t.kind == Kind::Opt || t.kind == Kind::None || t.kind == Kind::Top || t.kind == Kind::Bottom) return t;
    return {.kind = Kind::Opt, .args = {t}};
}

Type tuple_of(std::vector<Type> elements) { return {.kind = Kind::Tuple, .args = std::move(elements)}; }

Type strip_optional(const Type& t) { return t.kind == Kind::Opt ? t.args.front() : t; }

// ---- intervals ----------------------------------------------------------------------------------------
Type join_int(const Type& a, const Type& b) { return int_range(lower_of(a.lo, b.lo), upper_of(a.hi, b.hi)); }

Type add(const Type& a, const Type& b) { return int_range(sum_of(a.lo, b.lo), sum_of(a.hi, b.hi)); }

Type subtract(const Type& a, const Type& b) {
    return int_range(a.lo && b.hi ? std::optional<Wide>(*a.lo - *b.hi) : std::nullopt,
                     a.hi && b.lo ? std::optional<Wide>(*a.hi - *b.lo) : std::nullopt);
}

Type negate(const Type& a) {
    return int_range(a.hi ? std::optional<Wide>(-*a.hi) : std::nullopt, a.lo ? std::optional<Wide>(-*a.lo) : std::nullopt);
}

Type multiply(const Type& a, const Type& b) {
    if (!is_bounded(a) || !is_bounded(b)) {
        const auto zero = [](const Type& t) { return t.lo && t.hi && t.lo->sign() == 0 && t.hi->sign() == 0; };
        if (zero(a) || zero(b)) return const_int(Wide(0));
        if (a.nonneg() && b.nonneg()) return int_range(*a.lo * *b.lo, std::nullopt);
        return int_range();
    }
    return interval_of({*a.lo * *b.lo, *a.lo * *b.hi, *a.hi * *b.lo, *a.hi * *b.hi});
}

Type floor_divide(const Type& a, const Type& b) {
    if (!is_bounded(a) || !is_bounded(b)) {
        if (a.nonneg() && b.positive()) return int_range(Wide(0), a.hi);
        return int_range();
    }
    std::vector<Wide> divisors;   // the divisors the quotient is extreme at: the ends, -1 and 1, each if b can be it
    for (const Wide& d : {*b.lo, *b.hi, Wide(-1), Wide(1)}) {
        if (*b.lo <= d && d <= *b.hi && d.sign() != 0) divisors.push_back(d);
    }
    if (divisors.empty()) return int_range();   // always a division by zero: it raises
    std::vector<Wide> quotients;
    for (const Wide& x : {*a.lo, *a.hi}) {
        for (const Wide& d : divisors) quotients.push_back(floor_div(x, d));
    }
    return int_range(*std::ranges::min_element(quotients), *std::ranges::max_element(quotients));
}

Type modulo(const Type& a, const Type& b) {
    if (b.positive()) {
        const Bound top_value = b.hi ? std::optional<Wide>(*b.hi - Wide(1)) : std::nullopt;
        if (a.nonneg() && a.hi && (!top_value || *a.hi < *top_value))
            return int_range(*b.lo > *a.hi ? *a.lo : Wide(0), *a.hi);
        return int_range(Wide(0), top_value);
    }
    if (b.hi && b.hi->sign() < 0) return int_range(b.lo ? std::optional<Wide>(*b.lo + Wide(1)) : std::nullopt, Wide(0));
    return int_range(b.lo ? std::optional<Wide>(std::min(Wide(0), *b.lo + Wide(1))) : std::nullopt,
                     b.hi ? std::optional<Wide>(std::max(Wide(0), *b.hi - Wide(1))) : std::nullopt);
}

Type shift_left(const Type& a, const Type& b) {
    if (!b.lo || b.lo->sign() < 0 || !b.hi || *b.hi > Wide(kMaxShiftLeft) || !is_bounded(a))
        return a.nonneg() ? int_range(Wide(0), std::nullopt) : int_range();
    const auto lo = static_cast<unsigned>(*b.lo->to_i64()), hi = static_cast<unsigned>(*b.hi->to_i64());
    return interval_of({python::shifted_left(*a.lo, lo), python::shifted_left(*a.lo, hi), python::shifted_left(*a.hi, lo),
                        python::shifted_left(*a.hi, hi)});
}

Type shift_right(const Type& a, const Type& b) {
    if (!b.lo || b.lo->sign() < 0) return int_range();
    if (!is_bounded(a)) return a.nonneg() ? int_range(Wide(0), std::nullopt) : int_range();
    const unsigned lo = shift_count(*b.lo);
    std::vector<Wide> c{python::shifted_right(*a.lo, lo), python::shifted_right(*a.hi, lo)};
    if (b.hi) {
        const unsigned hi = shift_count(*b.hi);
        c.push_back(python::shifted_right(*a.lo, hi));
        c.push_back(python::shifted_right(*a.hi, hi));
    } else {
        c.push_back(a.lo->sign() >= 0 ? Wide(0) : Wide(-1));   // shifted by ever more: all zeros, or all ones
    }
    return int_range(*std::ranges::min_element(c), *std::ranges::max_element(c));
}

Type bit_and(const Type& a, const Type& b) {
    if (a.nonneg() && b.nonneg()) {
        // the result cannot exceed either operand
        if (a.hi && b.hi) return int_range(Wide(0), std::min(*a.hi, *b.hi));
        return int_range(Wide(0), a.hi ? a.hi : b.hi);
    }
    if (a.nonneg()) return int_range(Wide(0), a.hi);
    if (b.nonneg()) return int_range(Wide(0), b.hi);
    if (!is_bounded(a) || !is_bounded(b)) return int_range();
    const Wide m = mask_over_all(a, b);
    return int_range(-m - Wide(1), m);
}

Type bit_or(const Type& a, const Type& b) {
    if (a.nonneg() && b.nonneg()) {
        if (!a.hi || !b.hi) return int_range(Wide(0), std::nullopt);
        return int_range(std::max(*a.lo, *b.lo), mask_over(std::max(*a.hi, *b.hi)));
    }
    if (!is_bounded(a) || !is_bounded(b)) return int_range();
    const Wide m = mask_over_all(a, b);
    return int_range(-m - Wide(1), m);
}

// DIVERGES from the Python, which assigned bit_or's function to xor too: for two non-negative operands the lower
// bound max(a.lo, b.lo) it gave is wrong (5 ^ 5 is 0), and a wrong lower bound lets a division by the result pass
// as safe. The upper bound, and the signed case, are the same as for `or`.
Type bit_xor(const Type& a, const Type& b) {
    if (a.nonneg() && b.nonneg()) {
        if (!a.hi || !b.hi) return int_range(Wide(0), std::nullopt);
        return int_range(Wide(0), mask_over(std::max(*a.hi, *b.hi)));
    }
    return bit_or(a, b);
}

Type bit_invert(const Type& a) {
    return int_range(a.hi ? std::optional<Wide>(-*a.hi - Wide(1)) : std::nullopt,
                     a.lo ? std::optional<Wide>(-*a.lo - Wide(1)) : std::nullopt);
}

Type power(const Type& a, const Type& b) {
    if (!is_bounded(a) || !is_bounded(b) || b.lo->sign() < 0 || *b.hi > Wide(kMaxExponent) ||
        std::max(a.lo->abs(), a.hi->abs()) > Wide::pow2(64))
        return int_range();
    const auto exponent = [](const Wide& w) { return static_cast<unsigned>(*w.to_i64()); };
    const unsigned elo = exponent(*b.lo), ehi = exponent(*b.hi);
    std::vector<Wide> c;
    for (const Wide& x : {*a.lo, *a.hi}) {
        for (const unsigned y : {elo, ehi}) c.push_back(python::raised_to(x, y));
    }
    if (a.lo->sign() < 0 && a.hi->sign() > 0) c.push_back(Wide(0));
    if (elo != ehi) {   // an exponent strictly inside the range
        for (const Wide& x : {*a.lo, *a.hi}) {
            for (const unsigned y : {elo + 1, std::max(elo, ehi - 1)}) c.push_back(python::raised_to(x, y));
        }
    }
    return int_range(*std::ranges::min_element(c), *std::ranges::max_element(c));
}

Type meet(const Type& a, const Bound& lo, const Bound& hi) {
    const Bound new_lo = !lo ? a.lo : (!a.lo ? lo : std::optional<Wide>(std::max(*a.lo, *lo)));
    const Bound new_hi = !hi ? a.hi : (!a.hi ? hi : std::optional<Wide>(std::min(*a.hi, *hi)));
    if (new_lo && new_hi && *new_lo > *new_hi) return int_range(new_lo, new_lo);   // the branch cannot run: keep the guard's
    return int_range(new_lo, new_hi);
}

Type widen_int(const Type& old_type, const Type& new_type) {
    static const std::array<Wide, 7> thresholds = {Wide(0), Wide(1), Wide(255), Wide(0xFFFF), Wide(0xFFFFFFFFll), mem(), i64_max()};
    Bound lo = new_type.lo, hi = new_type.hi;
    if (old_type.lo && (!lo || *lo < *old_type.lo)) {   // the lower bound fell: out to the next threshold below it
        const Bound grown = lo;
        lo.reset();
        for (const Wide& t : thresholds) {
            if (grown && -t <= *grown) { lo = -t; break; }
        }
    }
    if (old_type.hi && (!hi || *hi > *old_type.hi)) {   // the upper bound rose
        const Bound grown = hi;
        hi.reset();
        for (const Wide& t : thresholds) {
            if (grown && t >= *grown) { hi = t; break; }
        }
    }
    return int_range(lo, hi);
}

}  // namespace cb::translate::frontends::python::pytypes
