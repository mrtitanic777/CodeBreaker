// Tests of cb::translate::frontends::python::structfmt. The cases are the format strings the Python front end meets
// in real struct.pack/unpack/calcsize calls (the layouts of the PE/NE/ZIP headers the translated tools read).
#include "cb/translate/frontends/python/structfmt.hpp"

#include "cb/core/testing.hpp"

namespace {
namespace sf = cb::translate::frontends::python::structfmt;

sf::Item int_item(char code, std::uint64_t size, std::int64_t lo, std::uint64_t hi) {
    return {.code = code, .size = size, .kind = sf::Kind::Int, .lo = lo, .hi = hi};
}
}  // namespace

CB_TEST("translate.structfmt.integers", [] {
    const auto items = sf::parse("<HHI").value();
    CB_CHECK_EQ(items.size(), std::size_t{3});
    CB_CHECK(items[0] == int_item('H', 2, 0, 65535));
    CB_CHECK(items[1] == int_item('H', 2, 0, 65535));
    CB_CHECK(items[2] == int_item('I', 4, 0, 0xFFFFFFFFull));
    CB_CHECK_EQ(sf::size(items), std::uint64_t{8});

    const auto signed_items = sf::parse(">bhiq").value();
    CB_CHECK(signed_items[0] == int_item('b', 1, -128, 127));
    CB_CHECK(signed_items[1] == int_item('h', 2, -32768, 32767));
    CB_CHECK(signed_items[2] == int_item('i', 4, -2147483648LL, 2147483647));
    CB_CHECK(signed_items[3] == int_item('q', 8, INT64_MIN, static_cast<std::uint64_t>(INT64_MAX)));

    const auto q = sf::parse("<Q").value();
    CB_CHECK(q[0] == int_item('Q', 8, 0, UINT64_MAX));
    // l and L are four bytes (Windows x64 native, and the standard size)
    CB_CHECK_EQ(sf::parse("<l").value()[0].size, std::uint64_t{4});
    CB_CHECK_EQ(sf::parse("<L").value()[0].hi, std::uint64_t{0xFFFFFFFFull});
});

CB_TEST("translate.structfmt.repeat_counts", [] {
    const auto items = sf::parse("<12H").value();
    CB_CHECK_EQ(items.size(), std::size_t{12});
    CB_CHECK_EQ(sf::size(items), std::uint64_t{24});
    // white space may precede an item
    CB_CHECK_EQ(sf::parse("<2H  3B").value().size(), std::size_t{5});
    // 's' and 'x' are one item whose size is the count
    const auto s = sf::parse("<4s").value();
    CB_CHECK_EQ(s.size(), std::size_t{1});
    CB_CHECK(s[0].kind == sf::Kind::Bytes);
    CB_CHECK_EQ(s[0].size, std::uint64_t{4});
    CB_CHECK_EQ(s[0].lo, std::int64_t{4});
    CB_CHECK_EQ(s[0].hi, std::uint64_t{4});
    const auto x = sf::parse("<H3xH").value();
    CB_CHECK_EQ(x.size(), std::size_t{3});
    CB_CHECK_EQ(sf::size(x), std::uint64_t{7});
    CB_CHECK_EQ(sf::values(x).size(), std::size_t{2});
    // 'c' repeats as single bytes, '?' as booleans, f and d as floats
    CB_CHECK_EQ(sf::parse("<3c").value().size(), std::size_t{3});
    CB_CHECK(sf::parse("<?").value()[0].kind == sf::Kind::Bool);
    CB_CHECK_EQ(sf::parse("<fd").value()[0].size, std::uint64_t{4});
    CB_CHECK_EQ(sf::parse("<fd").value()[1].size, std::uint64_t{8});
    CB_CHECK(sf::parse("<d").value()[0].kind == sf::Kind::Float);
});

// DIVERGES from the Python: it accepted "<4" and "<H3" (a count with no code: its leftover check removed digits, so the
// count was simply dropped) and "<4 H" (white space between a count and its code, which CPython's struct refuses; read as four
// H). All three are refused now, as is a repeat count past 2**20 or a struct past 2**31-1 bytes (the Python built the list
// however long). The Python is fixed the same way (tests/translate/test_pytypes.py).
CB_TEST("translate.structfmt.refused", [] {
    CB_CHECK(!sf::parse("<P"));               // a code outside the list
    CB_CHECK(!sf::parse("<H,H"));             // a character that is not a format character
    CB_CHECK(!sf::parse("<4"));               // a count with no code
    CB_CHECK(!sf::parse("<H3"));
    CB_CHECK(!sf::parse("<4 H"));             // CPython: no white space between the count and its code
    CB_CHECK(!sf::parse("<H<H"));             // the byte-order prefix only leads
    CB_CHECK(!sf::parse("<2000000H"));        // past the repeat cap
    CB_CHECK(!sf::parse("<99999999999999999999H"));
    CB_CHECK(!sf::parse("<2147483648x"));     // larger than a struct can be
    CB_CHECK(sf::parse("<2147483647x"));
    // native alignment pads between sized numbers; the standard-size support header cannot do that
    CB_CHECK(!sf::parse("HI"));
    CB_CHECK(!sf::parse("@HH"));
    CB_CHECK(!sf::parse("@fB"));
    // but a lone number, or bytes, have no padding to model
    CB_CHECK(sf::parse("H"));
    CB_CHECK(sf::parse("@BBc4s"));
    CB_CHECK(sf::parse("=HI"));
});

CB_TEST("translate.structfmt.empty_and_prefix", [] {
    // DIVERGES: the Python raised IndexError for '' (its `fmt[:1] in '<>!=@'` is true for an empty slice); an
    // empty format is a valid, empty layout (calcsize('') is 0).
    CB_CHECK(sf::parse("").value().empty());
    CB_CHECK(sf::parse("<").value().empty());
    CB_CHECK_EQ(sf::size(sf::parse("!").value()), std::uint64_t{0});
    CB_CHECK_EQ(sf::kind_word(sf::Kind::Pad), std::string_view("pad"));
    CB_CHECK_EQ(sf::kind_word(sf::Kind::Int), std::string_view("int"));
});
