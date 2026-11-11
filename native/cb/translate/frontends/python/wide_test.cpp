#include "cb/translate/frontends/python/wide.hpp"

#include "cb/core/testing.hpp"

namespace {
using cb::translate::frontends::python::Wide;
using cb::translate::frontends::python::floor_div;
using cb::translate::frontends::python::raised_to;
using cb::translate::frontends::python::shifted_left;
using cb::translate::frontends::python::shifted_right;

Wide dec(const char* text) { return Wide::from_decimal(text).value(); }
}  // namespace

CB_TEST("translate.wide.text_and_values", [] {
    CB_CHECK_EQ(Wide(0).to_string(), std::string("0"));
    CB_CHECK_EQ(Wide(-5).to_string(), std::string("-5"));
    CB_CHECK_EQ(Wide(INT64_MIN).to_string(), std::string("-9223372036854775808"));
    CB_CHECK_EQ(Wide::from_u64(UINT64_MAX).to_string(), std::string("18446744073709551615"));
    CB_CHECK_EQ(Wide::pow2(128).to_string(), std::string("340282366920938463463374607431768211456"));
    CB_CHECK_EQ(Wide::pow2(255).to_string(),
                std::string("57896044618658097711785492504343953926634992332820282019728792003956564819968"));
    // decimal text round trips, a leading zero or sign included
    CB_CHECK_EQ(dec("-123456789012345678901234567890").to_string(), std::string("-123456789012345678901234567890"));
    CB_CHECK_EQ(dec("000").to_string(), std::string("0"));
    CB_CHECK_EQ(dec("-0").to_string(), std::string("0"));
    CB_CHECK(!Wide::from_decimal(""));
    CB_CHECK(!Wide::from_decimal("-"));
    CB_CHECK(!Wide::from_decimal("12a"));
    CB_CHECK(!Wide::from_decimal("0x10"));
    // to_i64 / to_u64 at their edges
    CB_CHECK_EQ(Wide(INT64_MAX).to_i64().value(), INT64_MAX);
    CB_CHECK_EQ(Wide(INT64_MIN).to_i64().value(), INT64_MIN);
    CB_CHECK(!(Wide(INT64_MAX) + Wide(1)).to_i64());
    CB_CHECK(!(Wide(INT64_MIN) - Wide(1)).to_i64());
    CB_CHECK_EQ((Wide(INT64_MAX) + Wide(1)).to_u64().value(), std::uint64_t{1} << 63);
    CB_CHECK(!Wide(-1).to_u64());
    CB_CHECK_EQ(Wide(0).bit_length(), 0u);
    CB_CHECK_EQ(Wide(255).bit_length(), 8u);
    CB_CHECK_EQ(Wide(-256).bit_length(), 9u);
    CB_CHECK_EQ(Wide::pow2(200).bit_length(), 201u);
});

CB_TEST("translate.wide.arithmetic_is_exact", [] {
    CB_CHECK(Wide(7) + Wide(-10) == Wide(-3));
    CB_CHECK(Wide(-7) - Wide(-10) == Wide(3));
    CB_CHECK(Wide(5) - Wide(5) == Wide(0));
    CB_CHECK_EQ((Wide(5) - Wide(5)).sign(), 0);
    // carries across limbs
    CB_CHECK_EQ((Wide::from_u64(UINT64_MAX) + Wide(1)).to_string(), std::string("18446744073709551616"));
    CB_CHECK_EQ((Wide::pow2(128) - Wide(1)).to_string(), std::string("340282366920938463463374607431768211455"));
    // products past 64 and 128 bits
    CB_CHECK_EQ((Wide::from_u64(UINT64_MAX) * Wide::from_u64(UINT64_MAX)).to_string(), std::string("340282366920938463426481119284349108225"));
    CB_CHECK(Wide::pow2(128) * Wide::pow2(100) == Wide::pow2(228));
    CB_CHECK(Wide(-3) * Wide::pow2(70) == -(Wide::pow2(70) * Wide(3)));
    CB_CHECK_EQ((dec("123456789012345678901234567890") * dec("987654321098765432109876543210")).to_string(),
                std::string("121932631137021795226185032733622923332237463801111263526900"));
    // a * 0 and the sign of a zero
    CB_CHECK((Wide(-4) * Wide(0)).sign() == 0);
    CB_CHECK(Wide(-4) * Wide(0) == Wide(0));
});

CB_TEST("translate.wide.saturation_never_wraps", [] {
    // 2**255 * 2 and 2**128 * 2**128 are 2**256: beyond the capacity, so the largest magnitude -- still enormous, with its sign
    const Wide big = Wide::pow2(128) * Wide::pow2(128);
    CB_CHECK(big > Wide::pow2(255));
    CB_CHECK(-big < -Wide::pow2(255));
    CB_CHECK(big * Wide(-1) == -big);
    CB_CHECK((big + big) == big);                      // stays saturated
    CB_CHECK(raised_to(Wide(2), 300) > Wide::pow2(255));
    CB_CHECK(raised_to(Wide(-3), 301) < -Wide::pow2(255));   // the sign of an odd power survives
    CB_CHECK(raised_to(Wide(-3), 300) > Wide::pow2(255));
    CB_CHECK(shifted_left(Wide(1), 400) > Wide::pow2(255));
    CB_CHECK(shifted_left(Wide(-1), 400) < -Wide::pow2(255));
    CB_CHECK(shifted_left(Wide(0), 400) == Wide(0));
    CB_CHECK(Wide::pow2(300) > Wide::pow2(255));
    CB_CHECK(dec("9999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999") >
             Wide::pow2(255));
});

CB_TEST("translate.wide.division_and_shifts_floor", [] {
    CB_CHECK(floor_div(Wide(7), Wide(2)) == Wide(3));
    CB_CHECK(floor_div(Wide(-7), Wide(2)) == Wide(-4));
    CB_CHECK(floor_div(Wide(7), Wide(-2)) == Wide(-4));
    CB_CHECK(floor_div(Wide(-7), Wide(-2)) == Wide(3));
    CB_CHECK(floor_div(Wide(-8), Wide(2)) == Wide(-4));
    CB_CHECK(floor_div(Wide(0), Wide(-3)) == Wide(0));
    CB_CHECK(floor_div(Wide(3), Wide(7)) == Wide(0));
    CB_CHECK(floor_div(Wide(-3), Wide(7)) == Wide(-1));
    // past 64 bits
    CB_CHECK(floor_div(Wide::pow2(200), Wide::pow2(100)) == Wide::pow2(100));
    CB_CHECK(floor_div(Wide::pow2(200) + Wide(5), Wide::pow2(100)) == Wide::pow2(100));
    CB_CHECK(floor_div(-Wide::pow2(200) - Wide(5), Wide::pow2(100)) == -Wide::pow2(100) - Wide(1));
    CB_CHECK_EQ(floor_div(dec("340282366920938463463374607431768211456"), dec("18446744073709551616")).to_string(),
                std::string("18446744073709551616"));
    CB_CHECK(floor_div(dec("1000000000000000000000000000000"), dec("7")) == dec("142857142857142857142857142857"));
    // shifts: right shifts floor
    CB_CHECK(shifted_left(Wide(3), 4) == Wide(48));
    CB_CHECK(shifted_left(Wide(-3), 70) == -(Wide(3) * Wide::pow2(70)));
    CB_CHECK(shifted_right(Wide(100), 3) == Wide(12));
    CB_CHECK(shifted_right(Wide(-100), 3) == Wide(-13));
    CB_CHECK(shifted_right(Wide(-8), 3) == Wide(-1));
    CB_CHECK(shifted_right(Wide(-1), 1000) == Wide(-1));
    CB_CHECK(shifted_right(Wide(1), 1000) == Wide(0));
    CB_CHECK(shifted_right(Wide::pow2(130), 66) == Wide::pow2(64));
    CB_CHECK(shifted_right(-Wide::pow2(130), 66) == -Wide::pow2(64));
    CB_CHECK(shifted_right(-Wide::pow2(130) - Wide(1), 66) == -Wide::pow2(64) - Wide(1));
    // powers
    CB_CHECK(raised_to(Wide(7), 0) == Wide(1));
    CB_CHECK(raised_to(Wide(-2), 3) == Wide(-8));
    CB_CHECK(raised_to(Wide(2), 100) == Wide::pow2(100));
    CB_CHECK(raised_to(Wide(0), 5) == Wide(0));
});

CB_TEST("translate.wide.ordering_and_hash", [] {
    CB_CHECK(Wide(-1) < Wide(0));
    CB_CHECK(Wide(-5) < Wide(-3));
    CB_CHECK(Wide(3) < Wide(5));
    CB_CHECK(-Wide::pow2(100) < -Wide::pow2(99));
    CB_CHECK(Wide::pow2(100) > Wide::pow2(99));
    CB_CHECK(Wide(0) == -Wide(0));
    CB_CHECK(Wide(0).hash() == (-Wide(0)).hash());
    CB_CHECK(Wide(5).hash() == dec("5").hash());
    CB_CHECK(Wide(5).hash() != Wide(-5).hash());
    CB_CHECK(Wide(5) <= Wide(5));
});
