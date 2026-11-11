// Tests of the Python front end's types. The curated results below were taken from the original module and reviewed;
// the soundness test checks every interval operation against concrete integers, which is what the inference relies on.
#include "cb/translate/frontends/python/pytypes.hpp"

#include <functional>

#include "cb/core/testing.hpp"

namespace {
namespace pt = cb::translate::frontends::python::pytypes;
using cb::translate::frontends::python::Wide;
using pt::Bound;
using pt::Kind;
using pt::Type;

Bound B(std::int64_t v) { return Wide(v); }
const Bound N = std::nullopt;   // unbounded

Type I(const Bound& lo, const Bound& hi) { return pt::int_range(lo, hi); }
std::string show(const Type& t) { return pt::describe(t); }
Wide dec(const char* text) { return Wide::from_decimal(text).value(); }
}  // namespace

CB_TEST("translate.pytypes.describe", [] {
    CB_CHECK_EQ(show(I(B(0), B(255))), std::string("int[0..255]"));
    CB_CHECK_EQ(show(pt::int_range()), std::string("int[None..None]"));
    CB_CHECK_EQ(show(pt::tuple_of({I(B(0), B(1)), pt::str()})), std::string("tuple[int[0..1], str]"));
    CB_CHECK_EQ(show(pt::optional_of(pt::int_range())), std::string("opt[int[None..None]]"));
    CB_CHECK_EQ(show(pt::bytes_of(B(4))), std::string("bytes(<=4)"));
    CB_CHECK_EQ(show(pt::bytes_of(N, Kind::Bytearray)), std::string("bytearray"));
    CB_CHECK_EQ(show({.kind = Kind::List, .site = 3}), std::string("list#3"));
    CB_CHECK_EQ(show({.kind = Kind::Set}), std::string("set#None"));
    CB_CHECK_EQ(show(pt::top("why")), std::string("top(why)"));
    CB_CHECK_EQ(show({.kind = Kind::Inst, .name = "a.B"}), std::string("inst a.B"));
    CB_CHECK_EQ(show({.kind = Kind::Fn}), std::string("fn None"));
    CB_CHECK_EQ(show(pt::bottom()), std::string("bottom"));
    CB_CHECK_EQ(show(pt::boolean()), std::string("bool"));
    CB_CHECK_EQ(show(pt::const_int(Wide(-7))), std::string("int[-7..-7]"));
    CB_CHECK_EQ(show(pt::unsigned_int(128)), std::string("int[0..340282366920938463463374607431768211455]"));
});

CB_TEST("translate.pytypes.constructors", [] {
    // a bound past 2**128 in magnitude is dropped; 2**128 itself stays
    CB_CHECK_EQ(show(I(B(5), Wide::pow2(129))), std::string("int[5..None]"));
    CB_CHECK_EQ(show(I(-Wide::pow2(129), B(5))), std::string("int[None..5]"));
    CB_CHECK_EQ(show(I(Wide::pow2(129), B(5))), std::string("int[680564733841876926926749214863536422912..5]"));   // only the end that is past its limit goes
    CB_CHECK_EQ(show(I(-Wide::pow2(129), Wide::pow2(129))), std::string("int[None..None]"));
    CB_CHECK_EQ(show(I(-Wide::pow2(128), Wide::pow2(128))),
                std::string("int[-340282366920938463463374607431768211456..340282366920938463463374607431768211456]"));
    CB_CHECK_EQ(show(pt::unsigned_int(129)), std::string("int[0..None]"));
    CB_CHECK_EQ(show(pt::unsigned_int(8)), std::string("int[0..255]"));
    CB_CHECK_EQ(show(pt::signed_int(8)), std::string("int[-128..127]"));
    CB_CHECK_EQ(show(pt::signed_int(128)),
                std::string("int[-170141183460469231731687303715884105728..170141183460469231731687303715884105727]"));
    CB_CHECK_EQ(show(pt::signed_int(200)), std::string("int[None..None]"));
    CB_CHECK_EQ(show(pt::size_type()), std::string("int[0..140737488355328]"));
    CB_CHECK_EQ(show(pt::find_type()), std::string("int[-1..140737488355328]"));
    // optional_of leaves optional, None, top and bottom as they are
    CB_CHECK(pt::optional_of(pt::none()) == pt::none());
    CB_CHECK(pt::optional_of(pt::bottom()) == pt::bottom());
    CB_CHECK(pt::optional_of(pt::optional_of(pt::str())) == pt::optional_of(pt::str()));
    CB_CHECK(pt::strip_optional(pt::optional_of(pt::str())) == pt::str());
    CB_CHECK(pt::strip_optional(pt::str()) == pt::str());
    // const_int does not clamp: it is exactly what the literal said
    CB_CHECK(pt::const_int(Wide::pow2(200)).lo == Bound(Wide::pow2(200)));
});

CB_TEST("translate.pytypes.equality_and_hash", [] {
    const Type a = I(B(0), B(5));
    CB_CHECK(a == I(B(0), B(5)));
    CB_CHECK(a != I(B(0), B(6)));
    CB_CHECK(a.hash() == I(B(0), B(5)).hash());
    // `why` is not part of what a type is
    Type t1 = pt::top("one"), t2 = pt::top("two");
    CB_CHECK(t1 == t2);
    CB_CHECK(t1.hash() == t2.hash());
    // site and name are
    CB_CHECK((Type{.kind = Kind::List, .site = 1}) != (Type{.kind = Kind::List, .site = 2}));
    CB_CHECK((Type{.kind = Kind::Inst, .name = "a"}) != (Type{.kind = Kind::Inst, .name = "b"}));
    CB_CHECK(pt::tuple_of({pt::str()}) != pt::tuple_of({pt::str(), pt::str()}));
    CB_CHECK(pt::tuple_of({pt::str()}) == pt::tuple_of({pt::str()}));
    CB_CHECK(pt::bytes_of(B(4)) != pt::bytes());
    CB_CHECK(std::hash<Type>()(a) == a.hash());
});

CB_TEST("translate.pytypes.questions", [] {
    CB_CHECK(I(B(0), B(255)).fits_i64());
    CB_CHECK(pt::signed_int(64).fits_i64());
    CB_CHECK(!pt::unsigned_int(64).fits_i64());   // 2**64-1 leaves int64
    CB_CHECK(!pt::int_range(B(0), N).fits_i64());
    CB_CHECK(!pt::str().fits_i64());
    CB_CHECK(I(B(0), N).nonneg());
    CB_CHECK(!I(B(-1), B(5)).nonneg());
    CB_CHECK(!I(N, B(5)).nonneg());
    CB_CHECK(I(B(1), N).positive());
    CB_CHECK(!I(B(0), B(5)).positive());
    CB_CHECK(!I(B(0), B(5)).has_zero() == false);
    CB_CHECK(!I(B(1), B(5)).has_zero());
    CB_CHECK(!I(B(-5), B(-1)).has_zero());
    CB_CHECK(I(B(-5), B(5)).has_zero());
    CB_CHECK(pt::int_range().has_zero());
    CB_CHECK(pt::str().has_zero());   // not an int: no claim
});

CB_TEST("translate.pytypes.interval_results", [] {
    // each result below is what the original module gave
    CB_CHECK_EQ(show(pt::add(I(B(0), B(255)), I(B(1), B(1)))), std::string("int[1..256]"));
    CB_CHECK_EQ(show(pt::subtract(I(B(0), B(255)), I(B(0), B(255)))), std::string("int[-255..255]"));
    CB_CHECK_EQ(show(pt::multiply(pt::unsigned_int(64), pt::unsigned_int(64))), std::string("int[0..340282366920938463426481119284349108225]"));
    CB_CHECK_EQ(show(pt::multiply(I(B(-3), B(4)), I(B(-5), B(2)))), std::string("int[-20..15]"));
    CB_CHECK_EQ(show(pt::multiply(I(B(0), N), I(B(0), B(0)))), std::string("int[0..0]"));
    CB_CHECK_EQ(show(pt::multiply(I(B(1), N), I(B(2), N))), std::string("int[2..None]"));
    CB_CHECK_EQ(show(pt::floor_divide(I(B(-7), B(7)), I(B(2), B(3)))), std::string("int[-4..3]"));
    CB_CHECK_EQ(show(pt::floor_divide(I(B(-7), B(7)), I(B(-2), B(3)))), std::string("int[-7..7]"));
    CB_CHECK_EQ(show(pt::floor_divide(I(B(0), N), I(B(1), N))), std::string("int[0..None]"));
    CB_CHECK_EQ(show(pt::floor_divide(I(B(0), B(5)), I(B(0), B(0)))), std::string("int[None..None]"));
    CB_CHECK_EQ(show(pt::modulo(I(B(0), B(100)), I(B(1), B(10)))), std::string("int[0..9]"));
    CB_CHECK_EQ(show(pt::modulo(I(B(3), B(5)), I(B(10), B(20)))), std::string("int[3..5]"));
    CB_CHECK_EQ(show(pt::modulo(I(B(-5), B(5)), I(B(-4), B(-2)))), std::string("int[-3..0]"));
    CB_CHECK_EQ(show(pt::modulo(I(B(-5), B(5)), I(B(-4), B(4)))), std::string("int[-3..3]"));
    CB_CHECK_EQ(show(pt::shift_left(I(B(1), B(1)), I(B(0), B(63)))), std::string("int[1..9223372036854775808]"));
    CB_CHECK_EQ(show(pt::shift_left(I(B(-2), B(3)), I(B(1), B(4)))), std::string("int[-32..48]"));
    CB_CHECK_EQ(show(pt::shift_left(I(B(0), B(255)), I(B(0), B(200)))), std::string("int[0..None]"));
    CB_CHECK_EQ(show(pt::shift_right(I(B(-100), B(100)), I(B(1), B(3)))), std::string("int[-50..50]"));
    CB_CHECK_EQ(show(pt::shift_right(I(B(0), N), I(B(4), N))), std::string("int[0..None]"));
    CB_CHECK_EQ(show(pt::bit_and(I(B(0), B(255)), I(B(-5), B(5)))), std::string("int[0..255]"));
    CB_CHECK_EQ(show(pt::bit_and(I(B(-8), B(-1)), I(B(-4), B(-1)))), std::string("int[-16..15]"));
    CB_CHECK_EQ(show(pt::bit_or(I(B(5), B(7)), I(B(8), B(9)))), std::string("int[8..15]"));
    CB_CHECK_EQ(show(pt::bit_or(I(B(-4), B(3)), I(B(0), B(8)))), std::string("int[-16..15]"));
    CB_CHECK_EQ(show(pt::bit_invert(I(B(0), B(255)))), std::string("int[-256..-1]"));
    CB_CHECK_EQ(show(pt::negate(I(B(-3), N))), std::string("int[None..3]"));
    CB_CHECK_EQ(show(pt::power(I(B(2), B(3)), I(B(2), B(4)))), std::string("int[4..81]"));
    CB_CHECK_EQ(show(pt::power(I(B(-3), B(2)), I(B(2), B(2)))), std::string("int[0..9]"));
    CB_CHECK_EQ(show(pt::power(I(B(2), B(2)), I(B(0), B(64)))), std::string("int[1..18446744073709551616]"));
    CB_CHECK_EQ(show(pt::join_int(I(B(0), B(5)), I(B(3), B(9)))), std::string("int[0..9]"));
    CB_CHECK_EQ(show(pt::join_int(I(B(0), B(5)), pt::int_range())), std::string("int[None..None]"));
    CB_CHECK_EQ(show(pt::meet(I(N, N), B(0), B(255))), std::string("int[0..255]"));
    CB_CHECK_EQ(show(pt::meet(I(B(0), B(100)), B(10), B(3))), std::string("int[10..10]"));   // the guard cannot hold: its lower bound
    CB_CHECK_EQ(show(pt::meet(I(B(-10), N), B(0), N)), std::string("int[0..None]"));
});

CB_TEST("translate.pytypes.widening", [] {
    CB_CHECK_EQ(show(pt::widen_int(I(B(0), B(10)), I(B(0), B(11)))), std::string("int[0..255]"));
    CB_CHECK_EQ(show(pt::widen_int(I(B(0), B(10)), I(B(-1), B(10)))), std::string("int[-1..10]"));
    CB_CHECK_EQ(show(pt::widen_int(I(B(0), B(10)), I(B(-2), B(10)))), std::string("int[-255..10]"));
    CB_CHECK_EQ(show(pt::widen_int(I(B(0), B(255)), I(B(0), B(256)))), std::string("int[0..65535]"));
    CB_CHECK_EQ(show(pt::widen_int(I(B(0), Wide::pow2(47)), I(B(0), Wide::pow2(47) + Wide(1)))), std::string("int[0..9223372036854775807]"));
    CB_CHECK_EQ(show(pt::widen_int(I(B(0), B(INT64_MAX)), I(B(0), Wide(INT64_MAX) + Wide(1)))), std::string("int[0..None]"));
    // a bound that did not grow stays
    CB_CHECK_EQ(show(pt::widen_int(I(B(0), B(10)), I(B(0), B(10)))), std::string("int[0..10]"));
    CB_CHECK_EQ(show(pt::widen_int(I(B(0), B(10)), I(B(3), B(7)))), std::string("int[3..7]"));
    // an old end that was unbounded stays unbounded however the new one moves
    CB_CHECK_EQ(show(pt::widen_int(I(B(0), N), I(B(0), B(1000000)))), std::string("int[0..1000000]"));
    // a chain reaches the top: grow by one each time
    Type cur = I(B(0), B(0));
    for (const std::int64_t v : {1, 2, 3, 100, 255, 256, 70000}) cur = pt::widen_int(cur, I(B(0), B(v)));
    CB_CHECK_EQ(show(cur), std::string("int[0..4294967295]"));
});

// ---- soundness against concrete integers --------------------------------------------------------------
namespace {
bool within(const Type& t, std::int64_t v) {
    return (!t.lo || *t.lo <= Wide(v)) && (!t.hi || Wide(v) <= *t.hi);
}

std::int64_t floor_div64(std::int64_t a, std::int64_t b) {
    std::int64_t q = a / b;
    if ((a % b != 0) && ((a < 0) != (b < 0))) --q;
    return q;
}

struct Op {
    const char* name;
    std::function<Type(const Type&, const Type&)> interval;
    // the concrete result for two values, or nullopt where the operation raises (or is outside what is checked)
    std::function<std::optional<std::int64_t>(std::int64_t, std::int64_t)> concrete;
};

std::vector<Op> operations() {
    return {
        {"add", pt::add, [](auto a, auto b) { return std::optional<std::int64_t>(a + b); }},
        {"subtract", pt::subtract, [](auto a, auto b) { return std::optional<std::int64_t>(a - b); }},
        {"multiply", pt::multiply, [](auto a, auto b) { return std::optional<std::int64_t>(a * b); }},
        {"floor_divide", pt::floor_divide, [](auto a, auto b) { return b == 0 ? std::nullopt : std::optional<std::int64_t>(floor_div64(a, b)); }},
        {"modulo", pt::modulo,
         [](auto a, auto b) { return b == 0 ? std::nullopt : std::optional<std::int64_t>(a - floor_div64(a, b) * b); }},
        {"shift_left", pt::shift_left, [](auto a, auto b) { return b < 0 || b > 20 ? std::nullopt : std::optional<std::int64_t>(a * (std::int64_t{1} << b)); }},
        {"shift_right", pt::shift_right, [](auto a, auto b) { return b < 0 || b > 62 ? std::nullopt : std::optional<std::int64_t>(a >> b); }},
        {"bit_and", pt::bit_and, [](auto a, auto b) { return std::optional<std::int64_t>(a & b); }},
        {"bit_or", pt::bit_or, [](auto a, auto b) { return std::optional<std::int64_t>(a | b); }},
        {"bit_xor", pt::bit_xor, [](auto a, auto b) { return std::optional<std::int64_t>(a ^ b); }},
        {"power", pt::power,
         [](auto a, auto b) -> std::optional<std::int64_t> {
             if (b < 0 || b > 8) return std::nullopt;
             std::int64_t r = 1;
             for (std::int64_t i = 0; i < b; ++i) r *= a;
             return r;
         }},
    };
}
}  // namespace

CB_TEST("translate.pytypes.every_operation_contains_its_results", [] {
    constexpr int kRange = 6;
    const auto ops = operations();
    int failures = 0;
    for (int alo = -kRange; alo <= kRange; ++alo) {
        for (int ahi = alo; ahi <= kRange; ++ahi) {
            for (int blo = -kRange; blo <= kRange; ++blo) {
                for (int bhi = blo; bhi <= kRange; ++bhi) {
                    const Type a = I(B(alo), B(ahi)), b = I(B(blo), B(bhi));
                    for (const Op& op : ops) {
                        const Type r = op.interval(a, b);
                        for (int x = alo; x <= ahi; ++x) {
                            for (int y = blo; y <= bhi; ++y) {
                                const auto v = op.concrete(x, y);
                                if (v && !within(r, *v) && failures++ < 5)
                                    cb::testing::record_failure(__FILE__, __LINE__,
                                                                std::format("{}({}, {}) = {} is not in {}", op.name, show(a), show(b), *v, show(r)));
                            }
                        }
                    }
                }
            }
        }
    }
    CB_CHECK_EQ(failures, 0);
});

CB_TEST("translate.pytypes.unary_operations_contain_their_results", [] {
    int failures = 0;
    for (int lo = -9; lo <= 9; ++lo) {
        for (int hi = lo; hi <= 9; ++hi) {
            const Type a = I(B(lo), B(hi));
            for (int x = lo; x <= hi; ++x) {
                failures += !within(pt::negate(a), -x);
                failures += !within(pt::bit_invert(a), ~x);
            }
        }
    }
    CB_CHECK_EQ(failures, 0);
    // and a bound that leaves one end open still holds the other
    CB_CHECK(within(pt::add(I(B(0), N), I(B(5), B(5))), 1000000));
    CB_CHECK(!within(pt::add(I(B(0), N), I(B(5), B(5))), 4));
});

CB_TEST("translate.pytypes.xor_lower_bound", [] {
    // DIVERGES: the Python gave xor the bounds of or, whose lower bound max(a.lo, b.lo) is wrong for xor
    // (5 ^ 5 is 0), and a wrong lower bound lets a division by the result pass as safe.
    const Type same = pt::bit_xor(I(B(5), B(5)), I(B(5), B(5)));
    CB_CHECK(within(same, 0));
    CB_CHECK(same.has_zero());
    CB_CHECK_EQ(show(pt::bit_xor(I(B(5), B(7)), I(B(5), B(7)))), std::string("int[0..7]"));
    CB_CHECK_EQ(show(pt::bit_xor(I(B(0), B(255)), I(B(0), B(15)))), std::string("int[0..255]"));
    CB_CHECK_EQ(show(pt::bit_xor(I(B(0), N), I(B(0), B(3)))), std::string("int[0..None]"));
    // for a signed operand it is `or`'s
    CB_CHECK_EQ(show(pt::bit_xor(I(B(-4), B(3)), I(B(0), B(8)))), show(pt::bit_or(I(B(-4), B(3)), I(B(0), B(8)))));
});

CB_TEST("translate.pytypes.wide_bounds", [] {
    // an interval of u64 values multiplied, then shifted down: exact where the products pass 2**64 and 2**128
    const Type product = pt::multiply(pt::unsigned_int(64), pt::unsigned_int(64));
    CB_CHECK_EQ(show(pt::shift_right(product, I(B(64), B(64)))), std::string("int[0..18446744073709551614]"));
    CB_CHECK(pt::shift_right(product, I(B(64), B(64))).fits_i64() == false);
    CB_CHECK(pt::shift_right(product, I(B(65), B(65))).fits_i64());   // (2**64-1)**2 >> 65 < 2**63
    // products and powers of anything past 2**128 are dropped as unbounded
    CB_CHECK_EQ(show(pt::multiply(pt::unsigned_int(128), pt::unsigned_int(128))), std::string("int[0..None]"));
    CB_CHECK_EQ(show(pt::power(I(B(10), B(10)), I(B(40), B(40)))), std::string("int[10000000000000000000000000000000000000000..None]"));
    CB_CHECK_EQ(show(pt::shift_left(I(B(1), B(1)), I(B(0), B(128)))), std::string("int[1..340282366920938463463374607431768211456]"));
    // DIVERGES: a bound is exact only up to 2**256 in magnitude (the Python's integers were unbounded). A literal
    // beyond that, shifted down again, is unbounded here and was exact there; every such program is refused either way
    // (the value leaves int64 at the literal).
    CB_CHECK(!pt::const_int(dec("1000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000")).fits_i64());
});
