// cb/compare/mask_test.cpp -- tests of compare::mask and compare::image. Expected values were worked out by hand
// from what the functions are for (a relocated address is left out, an opcode is not).
#include "cb/compare/image.hpp"
#include "cb/compare/mask.hpp"
#include "cb/core/error.hpp"
#include "cb/core/testing.hpp"

using namespace cb;
using namespace cb::compare;

namespace {
// A call: E8 rel32, then a ret. The two builds differ only in the displacement.
const Bytes kBuildA = {0x55, 0x8B, 0xEC, 0xE8, 0x10, 0x00, 0x00, 0x00, 0x5D, 0xC3};
const Bytes kBuildB = {0x55, 0x8B, 0xEC, 0xE8, 0x7C, 0x33, 0x00, 0x00, 0x5D, 0xC3};
}  // namespace

CB_TEST("compare.mask.of_marks_only_offsets_inside", [] {
    const Bytes m = mask::of(std::vector<int>{4, 5, 6, 7, -1, 10, 99}, 10);
    CB_CHECK_EQ(m.size(), 10u);
    CB_CHECK(m == (Bytes{0, 0, 0, 0, 1, 1, 1, 1, 0, 0}));     // -1, 10 and 99 are outside and ignored
    CB_CHECK(mask::of(std::vector<std::uint32_t>{}, 3) == (Bytes{0, 0, 0}));
    CB_CHECK(mask::of(std::vector<int>{0}, 0).empty());
});

CB_TEST("compare.mask.blank_zeroes_the_offsets", [] {
    const Bytes out = mask::blank(kBuildA, std::vector<std::size_t>{4, 5, 6, 7, 400});
    CB_CHECK(out == (Bytes{0x55, 0x8B, 0xEC, 0xE8, 0, 0, 0, 0, 0x5D, 0xC3}));
    CB_CHECK(kBuildA[4] == 0x10);                              // the input is not touched
    CB_CHECK(mask::blank(kBuildA, std::vector<int>{-3}) == kBuildA);
});

CB_TEST("compare.mask.same_leaves_out_the_masked_bytes", [] {
    CB_CHECK(!mask::same(kBuildA, kBuildB));
    const Bytes call_target = mask::of(std::vector<int>{4, 5, 6, 7}, kBuildA.size());
    const mask::ByteMask masks[] = {call_target};
    CB_CHECK(mask::same(kBuildA, kBuildB, masks));
    // a difference in an opcode is still a difference
    Bytes c = kBuildB;
    c[3] = 0xE9;
    CB_CHECK(!mask::same(kBuildA, c, masks));
    // a different length is never the same, whatever the masks leave out
    Bytes d = kBuildB;
    d.push_back(0x90);
    CB_CHECK(!mask::same(kBuildA, d, masks));
    CB_CHECK(mask::same({}, {}));
});

CB_TEST("compare.mask.same_takes_the_union_of_several_masks", [] {
    const Bytes m1 = mask::of(std::vector<int>{4}, 10);
    const Bytes m2 = mask::of(std::vector<int>{5}, 10);
    const mask::ByteMask one[] = {m1};
    const mask::ByteMask two[] = {m1, m2};
    CB_CHECK(!mask::same(kBuildA, kBuildB, one));              // byte 4 is left out, byte 5 still differs
    CB_CHECK(mask::same(kBuildA, kBuildB, two));
    const mask::ByteMask with_empty[] = {m1, mask::ByteMask{}, m2};        // an empty mask leaves nothing out
    CB_CHECK(mask::same(kBuildA, kBuildB, with_empty));
});

// DIVERGES: the Python raised IndexError when a mask was shorter than the code it was applied to; here a short mask
// covers its prefix and the bytes past its end are compared (the stricter answer).
CB_TEST("compare.mask.a_short_mask_covers_its_prefix", [] {
    const Bytes short_mask = {0, 0, 0, 0, 1};
    const mask::ByteMask masks[] = {short_mask};
    CB_CHECK(!mask::same(kBuildA, kBuildB, masks));            // bytes 5..7 differ and lie past the mask
    CB_CHECK_EQ(mask::differing(kBuildA, kBuildB, masks).size(), 1u);   // only byte 5 (6 and 7 are 0 in both)
});

CB_TEST("compare.mask.differing_lists_offsets_up_to_the_shorter", [] {
    CB_CHECK(mask::differing(kBuildA, kBuildB) == (std::vector<std::size_t>{4, 5}));
    Bytes longer = kBuildB;
    longer.push_back(0xFF);
    CB_CHECK(mask::differing(kBuildA, longer) == (std::vector<std::size_t>{4, 5}));   // the tail is not listed
    const Bytes m = mask::of(std::vector<int>{4}, 10);
    const mask::ByteMask masks[] = {m};
    CB_CHECK(mask::differing(kBuildA, kBuildB, masks) == (std::vector<std::size_t>{5}));
});

CB_TEST("compare.mask.same_words_ignores_the_masked_bits", [] {
    // PowerPC `bl` instructions: the 24-bit target field is filled in by the linker, opcode and LK are not
    const std::vector<std::uint64_t> a = {0x48000001, 0x38600000};
    const std::vector<std::uint64_t> b = {0x4BFFFF01, 0x38600000};
    const std::vector<std::uint64_t> ignore = {0x03FFFFFC, 0};
    CB_CHECK(mask::same_words(a, b, ignore));
    const std::vector<std::uint64_t> c = {0x48000001, 0x38600001};
    CB_CHECK(!mask::same_words(a, c, ignore));
    const std::vector<std::uint64_t> none = {0, 0};
    CB_CHECK(!mask::same_words(a, b, none));
    // the shortest run decides how many words are looked at
    const std::vector<std::uint64_t> one = {0x03FFFFFC};
    CB_CHECK(mask::same_words(a, c, one));
});

CB_TEST("compare.mask.distance_bytes", [] {
    const Bytes ma = mask::of(std::vector<int>{4, 5, 6, 7}, 10);
    auto r = mask::distance(kBuildA, kBuildB);
    CB_CHECK(r.verdict == mask::Match::SameSize);
    CB_CHECK_EQ(r.differing, 2u);
    CB_CHECK_EQ(r.compared, 10u);
    CB_CHECK_EQ(r.first.value_or(99), 4u);
    r = mask::distance(kBuildA, kBuildB, ma);
    CB_CHECK(r.verdict == mask::Match::Identical);
    CB_CHECK_EQ(r.differing, 0u);
    CB_CHECK(!r.first.has_value());
    CB_CHECK(mask::distance(kBuildA, kBuildA).verdict == mask::Match::Identical);
});

CB_TEST("compare.mask.distance_counts_the_tail_of_a_longer_run", [] {
    Bytes longer = kBuildA;
    longer.insert(longer.end(), {1, 2, 3});
    auto r = mask::distance(kBuildA, longer);
    CB_CHECK(r.verdict == mask::Match::Different);
    CB_CHECK_EQ(r.differing, 3u);
    CB_CHECK_EQ(r.compared, 13u);
    CB_CHECK_EQ(r.first.value_or(99), 10u);                    // the first unit past the shorter one
    // a difference inside the shared part comes first
    longer[2] ^= 1;
    r = mask::distance(kBuildA, longer);
    CB_CHECK_EQ(r.differing, 4u);
    CB_CHECK_EQ(r.first.value_or(99), 2u);
    r = mask::distance({}, {});
    CB_CHECK(r.verdict == mask::Match::Identical);
    CB_CHECK_EQ(r.compared, 0u);
});

CB_TEST("compare.mask.distance_words_both_byte_orders", [] {
    // two big-endian words, the second differing in its low half only
    const Bytes a = {0x48, 0, 0, 1, 0x38, 0x60, 0x00, 0x05};
    const Bytes b = {0x4B, 0xFF, 0xFF, 0x01, 0x38, 0x60, 0x00, 0x05};
    const std::vector<std::uint64_t> ma = {0x03FFFFFC};
    auto r = mask::distance_words(a, b, 4, mask::ByteOrder::Big, ma);
    CB_CHECK(r.verdict == mask::Match::Identical);
    CB_CHECK_EQ(r.compared, 2u);
    r = mask::distance_words(a, b, 4, mask::ByteOrder::Big);
    CB_CHECK_EQ(r.differing, 1u);
    CB_CHECK_EQ(r.first.value_or(99), 0u);
    // read little-endian the same bytes are other words (0x01000048 against 0x01FFFF4B): the xor 0x00FFFF03 has its
    // low two bits outside the mask
    r = mask::distance_words(a, b, 4, mask::ByteOrder::Little, ma);
    CB_CHECK_EQ(r.differing, 1u);
    // a word of 2 bytes, and a ragged tail (the last byte is not a whole unit and is not looked at)
    const Bytes c = {1, 0, 2, 0, 9};
    const Bytes d = {1, 0, 3, 0, 8};
    r = mask::distance_words(c, d, 2, mask::ByteOrder::Little);
    CB_CHECK_EQ(r.differing, 1u);
    CB_CHECK_EQ(r.first.value_or(99), 1u);
    CB_CHECK_EQ(r.compared, 2u);
    // the second word's masks come from either side
    const std::vector<std::uint64_t> mb = {0, 0x0100};
    const std::vector<std::uint64_t> m0 = {};
    r = mask::distance_words(c, d, 2, mask::ByteOrder::Little, m0, mb);
    CB_CHECK_EQ(r.differing, 1u);                              // 0x0002 vs 0x0003: the low bit is not masked
    const std::vector<std::uint64_t> mlow = {0, 0x0001};
    r = mask::distance_words(c, d, 2, mask::ByteOrder::Little, m0, mlow);
    CB_CHECK_EQ(r.differing, 0u);
});

CB_TEST("compare.mask.distance_words_rejects_a_bad_unit", [] {
    const Bytes a = {1, 2, 3};
    CB_CHECK_THROWS_AS(UsageError, mask::distance_words(a, a, 0, mask::ByteOrder::Little));
    CB_CHECK_THROWS_AS(UsageError, mask::distance_words(a, a, 9, mask::ByteOrder::Little));
    // eight bytes is the widest: the whole 64-bit word is compared
    const Bytes x(8, 0xFF);
    Bytes y = x;
    y[0] = 0;
    CB_CHECK_EQ(mask::distance_words(x, y, 8, mask::ByteOrder::Little).differing, 1u);
});

CB_TEST("compare.mask.match_words", [] {
    CB_CHECK_EQ(std::string(mask::to_text(mask::Match::Identical)), "identical");
    CB_CHECK_EQ(std::string(mask::to_text(mask::Match::SameSize)), "same size");
    CB_CHECK_EQ(std::string(mask::to_text(mask::Match::Different)), "different");
});

CB_TEST("compare.image.first_difference", [] {
    CB_CHECK(!image::first_difference(kBuildA, kBuildA).has_value());
    CB_CHECK_EQ(image::first_difference(kBuildA, kBuildB).value_or(99), 4u);
    Bytes longer = kBuildA;
    longer.push_back(0);
    CB_CHECK_EQ(image::first_difference(kBuildA, longer).value_or(99), kBuildA.size());   // one ends first: its length
    CB_CHECK_EQ(image::first_difference(longer, kBuildA).value_or(99), kBuildA.size());
    CB_CHECK(!image::first_difference({}, {}).has_value());
    CB_CHECK_EQ(image::first_difference({}, kBuildA).value_or(99), 0u);
});

CB_TEST("compare.image.first_difference_in_a_large_image", [] {
    Bytes a(1u << 20, 0x5A);
    Bytes b = a;
    CB_CHECK(!image::first_difference(a, b).has_value());
    for (std::size_t at : {std::size_t{0}, std::size_t{7}, std::size_t{8}, std::size_t{9}, std::size_t{(1u << 20) - 1}}) {
        Bytes c = a;
        c[at] ^= 0x80;
        CB_CHECK_EQ(image::first_difference(a, c).value_or(1u << 21), at);
    }
});
