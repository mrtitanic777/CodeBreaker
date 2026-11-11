// The shims at codebreaker/compare/{mask,image}.hpp answer what the old layer's callers expect, through the new code.
#include "codebreaker/compare/image.hpp"
#include "codebreaker/compare/mask.hpp"
#include "cb/core/testing.hpp"

namespace old = codebreaker::compare;

namespace {
py::bytes B(std::initializer_list<unsigned char> v) { return py::bytes(std::string(v.begin(), v.end())); }
}  // namespace

CB_TEST("compat.compare_shim.mask", [] {
    py::bytes a = B({0x55, 0x8B, 0xEC, 0xE8, 0x10, 0x00, 0x00, 0x00, 0x5D, 0xC3});
    py::bytes b = B({0x55, 0x8B, 0xEC, 0xE8, 0x7C, 0x33, 0x00, 0x00, 0x5D, 0xC3});
    py::list<int64_t> offs{4, 5, 6, 7, 99};
    py::bytearray m = old::mask::of(offs, 10);
    CB_CHECK_EQ(m.size(), 10);
    CB_CHECK_EQ(static_cast<int64_t>(m[4]), 1);
    CB_CHECK_EQ(static_cast<int64_t>(m[3]), 0);
    py::set<int64_t> sset;
    sset.add(4);
    CB_CHECK_EQ(static_cast<int64_t>(old::mask::of(sset, 10)[4]), 1);

    py::list<old::mask::MaybeMask> none;
    CB_CHECK(!old::mask::same(a, b, none));
    py::list<old::mask::MaybeMask> masks;
    masks.append(old::mask::MaybeMask(old::mask::Mask(py::bytes(m.raw()))));
    masks.append(old::mask::MaybeMask(std::nullopt));
    CB_CHECK(old::mask::same(a, b, masks));
    CB_CHECK_EQ(old::mask::differing(a, b, none).size(), 2);

    py::bytes blank = old::mask::blank(a, offs);
    CB_CHECK_EQ(static_cast<unsigned char>(blank.raw()[4]), 0);
    CB_CHECK_EQ(static_cast<unsigned char>(blank.raw()[3]), 0xE8);

    auto [verdict, diff, compared, first] = old::mask::distance(a, b);
    CB_CHECK(verdict == py::str("same size"));
    CB_CHECK_EQ(diff, 2);
    CB_CHECK_EQ(compared, 10);
    CB_CHECK_EQ(first, 4);
    auto masked = old::mask::distance(a, b, old::mask::MaybeMask(old::mask::Mask(py::bytes(m.raw()))));
    CB_CHECK(std::get<0>(masked) == py::str("identical"));
    CB_CHECK_EQ(std::get<3>(masked), -1);
});

CB_TEST("compat.compare_shim.words", [] {
    py::list<int64_t> a{0x48000001, 0x38600000}, b{0x4BFFFF01, 0x38600000}, ignore{0x03FFFFFC, 0};
    CB_CHECK(old::mask::same_words(a, b, ignore));
    py::bytes x = B({0x48, 0, 0, 1, 0x38, 0x60, 0, 5}), y = B({0x4B, 0xFF, 0xFF, 1, 0x38, 0x60, 0, 5});
    old::mask::MaybeMask mw{old::mask::Mask(py::list<int64_t>{0x03FFFFFC})};
    auto r = old::mask::distance(x, y, mw, std::nullopt, 4, py::str("big"));
    CB_CHECK(std::get<0>(r) == py::str("identical"));
    CB_CHECK_EQ(std::get<2>(r), 2);
    CB_CHECK_THROWS_AS(py::ValueError, old::mask::distance(x, y, std::nullopt, std::nullopt, 4, py::str("middle")));
    CB_CHECK_THROWS_AS(py::ValueError, old::mask::distance(x, y, std::nullopt, std::nullopt, 9));
});

CB_TEST("compat.compare_shim.image", [] {
    CB_CHECK(!old::image::first_difference(B({1, 2}), B({1, 2})).has_value());
    CB_CHECK_EQ(old::image::first_difference(B({1, 2}), B({1, 3})).value_or(-1), 1);
    CB_CHECK_EQ(old::image::first_difference(B({1, 2}), B({1, 2, 3})).value_or(-1), 2);
});
