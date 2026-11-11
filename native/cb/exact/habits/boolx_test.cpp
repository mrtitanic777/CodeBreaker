#include "cb/exact/habits/boolx.hpp"

#include <string>
#include <vector>

#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "cb/core/text.hpp"

using namespace cb;
using namespace cb::exact::habits;

namespace {

using Code = std::vector<boolx::Instruction>;

boolx::Verdict compare(const Code& original, std::uint32_t original_hi, const Code& compiled, std::uint32_t compiled_hi,
                       boolx::Bases original_bases = {}, boolx::Bases compiled_bases = {}) {
    return boolx::same({original, original.empty() ? 0 : original.front().offset, original_hi, original_bases},
                       {compiled, compiled.empty() ? 0 : compiled.front().offset, compiled_hi, compiled_bases});
}

// if a == 3 and b == 7 (the original leaves at the first term that fails, to 0x40)
Code early_exit() {
    return {{0, "cmp word ptr [bp - 2], 3"}, {4, "jne 0x40"}, {6, "cmp word ptr [bp - 4], 7"}, {10, "jne 0x40"}};
}

// the same worked out in full: ax = (a == 3), cx = (b == 7), ax &= cx, and one test of the result
Code worked_out(int b_compared_with = 7) {
    return {{0, "mov ax, 0"},   {3, "cmp word ptr [bp - 2], 3"}, {8, "jne 0xb"},
            {10, "inc ax"},     {11, "mov cx, 0"},               {14, "cmp word ptr [bp - 4], " + std::to_string(b_compared_with)},
            {19, "jne 0x16"},   {21, "inc cx"},                  {22, "and al, cl"},
            {24, "je 0x40"}};
}

}  // namespace

CB_TEST("exact.habits.boolx.a_condition_worked_out_in_full_is_the_one_that_leaves_early", [] {
    const auto v = compare(early_exit(), 12, worked_out(), 26);
    CB_CHECK(v.same);
    CB_CHECK(v.else_targets.has_value());
    if (v.else_targets) CB_CHECK(*v.else_targets == (boolx::ElseTargets{0x40, 0x40}));
    // compared with another constant it is another condition: with b below 7 the original leaves, the compiled one too,
    // but with b == 8 the original goes on and the compiled one leaves
    const auto other = compare(early_exit(), 12, worked_out(8), 26);
    CB_CHECK(!other.same);
    CB_CHECK(other.why.starts_with("with "));
});

CB_TEST("exact.habits.boolx.a_boolean_byte_tested_directly_or_loaded_and_masked", [] {
    const Code direct = {{0, "test byte ptr [bp - 2], 1"}, {4, "je 0x20"}};
    const Code masked = {{0, "mov al, byte ptr [bp - 2]"}, {3, "and al, 1"}, {5, "je 0x20"}};
    const auto v = compare(direct, 6, masked, 7);
    CB_CHECK(v.same);
    if (v.else_targets) CB_CHECK(*v.else_targets == (boolx::ElseTargets{0x20, 0x20}));
    // the other byte is another unknown
    const Code other = {{0, "mov al, byte ptr [bp - 4]"}, {3, "and al, 1"}, {5, "je 0x20"}};
    CB_CHECK(!compare(direct, 6, other, 7).same);
    // jne instead of je is the opposite condition
    const Code opposite = {{0, "mov al, byte ptr [bp - 2]"}, {3, "and al, 1"}, {5, "jne 0x20"}};
    CB_CHECK(!compare(direct, 6, opposite, 7).same);
});

CB_TEST("exact.habits.boolx.the_low_bit_through_a_shift", [] {
    const Code shifted = {{0, "mov ax, word ptr [bp - 2]"}, {3, "shr ax, 1"}, {5, "jb 0x30"}};
    const Code tested = {{0, "test byte ptr [bp - 2], 1"}, {4, "jne 0x30"}};
    const auto v = compare(tested, 6, shifted, 7);
    CB_CHECK(v.same);
    if (v.else_targets) CB_CHECK(*v.else_targets == (boolx::ElseTargets{0x30, 0x30}));
    // after a shift the zero flag is not known: a jump on it is refused rather than guessed
    const Code on_zero = {{0, "mov ax, word ptr [bp - 2]"}, {3, "shr ax, 1"}, {5, "je 0x30"}};
    CB_CHECK(compare(tested, 6, on_zero, 7).why.starts_with("zero flag not known"));
});

CB_TEST("exact.habits.boolx.a_word_masked_with_a_constant_is_zero_or_not", [] {
    const Code want = {{0, "mov ax, word ptr [bp - 2]"}, {3, "and ax, 0x4"}, {6, "je 0x20"}};
    CB_CHECK(compare(want, 8, want, 8).same);
    const Code other_mask = {{0, "mov ax, word ptr [bp - 2]"}, {3, "and ax, 0x8"}, {6, "je 0x20"}};
    CB_CHECK(!compare(want, 8, other_mask, 8).same);
    // anything but a memory word and a lower-case constant is not known
    const Code upper = {{0, "mov ax, word ptr [bp - 2]"}, {3, "and ax, 0X4"}, {6, "je 0x20"}};
    CB_CHECK_EQ(compare(upper, 8, upper, 8).why, "instruction and ax, 0X4");
});

CB_TEST("exact.habits.boolx.si_and_di_are_what_they_were_loaded_from", [] {
    const Code loads_it = {{0, "mov si, word ptr [bp - 6]"}, {3, "cmp word ptr [si + 2], 1"}, {8, "jne 0x40"}};
    const Code given = {{0, "cmp word ptr [si + 2], 1"}, {5, "jne 0x40"}};
    boolx::Bases bases;
    bases.si = "bp - 6";
    const auto v = compare(loads_it, 10, given, 7, {}, bases);
    CB_CHECK(v.same);
    // ... and without being told, the other stretch's si is some other value
    CB_CHECK(!compare(loads_it, 10, given, 7).same);
    // an address spelled with si inside a longer word is not si
    const Code similar = {{0, "cmp word ptr [bp - 6 + 2], 1"}, {5, "jne 0x40"}};
    CB_CHECK(!compare(loads_it, 10, similar, 7).same);
});

CB_TEST("exact.habits.boolx.every_comparison_of_the_same_operands_is_one_unknown", [] {
    // a < b is decided once: the second test of the same comparison cannot disagree with the first
    const Code twice = {{0, "cmp word ptr [bp - 2], 5"}, {5, "jb 0x40"}, {7, "cmp word ptr [bp - 2], 5"}, {12, "jb 0x40"}};
    const Code once = {{0, "cmp word ptr [bp - 2], 5"}, {5, "jb 0x40"}};
    CB_CHECK(compare(twice, 14, once, 7).same);
    // constants compare as constants
    const Code constants = {{0, "mov ax, 3"}, {3, "cmp ax, 5"}, {6, "jb 0x40"}};
    CB_CHECK_EQ(compare(constants, 8, constants, 8).why, "not a condition: it always goes the same way");
});

CB_TEST("exact.habits.boolx.what_is_refused", [] {
    const auto why = [](const Code& c, std::uint32_t hi) { return compare(c, hi, c, hi).why; };
    CB_CHECK_EQ(why({}, 0), "not a condition: it always goes the same way");
    CB_CHECK_EQ(why({{0, "je 0x40"}}, 2), "jump without flags");
    CB_CHECK_EQ(why({{0, "call 0x10"}}, 3), "instruction call 0x10");
    CB_CHECK_EQ(why({{0, "mov ax, 0"}, {3, "jmp 0x4"}, {5, "nop"}}, 6), "jump into an instruction");
    CB_CHECK_EQ(why({{0, "jmp 0x0"}}, 2), "loop");
    CB_CHECK_EQ(why({{0, "mov ax, bx"}, {2, "inc ax"}}, 3), "inc of an unknown");
    CB_CHECK_EQ(why({{0, "and al, 1"}}, 2), "not a Boolean: reg ax");
    CB_CHECK_EQ(why({{0, "mov ax, 0"}, {3, "and al, 0"}, {5, "ja 0x40"}}, 8), "ja after a logical operation");
    CB_CHECK_EQ(why({{0, "mov dx, 4"}}, 3), "not a condition: it always goes the same way");   // (the move itself is fine)
    CB_CHECK_EQ(why({{0, "mov word ptr [bx], 4"}}, 4), "store mov word ptr [bx], 4");
    CB_CHECK_EQ(why({{0, "mov ax, cx"}, {3, "mov"}}, 5), "mov with 0 operands");
    CB_CHECK_EQ(why({{0, "je zzz"}}, 2), "a jump this cannot read: je zzz");
    CB_CHECK_EQ(why({{0, "shr ax, 1"}}, 2), "shr of an unknown");
    CB_CHECK_EQ(why({{0, "cmp ax"}}, 2), "operands missing in cmp ax");
});

CB_TEST("exact.habits.boolx.more_than_one_way_out_is_not_a_single_condition", [] {
    const Code two_exits = {{0, "cmp word ptr [bp - 2], 3"}, {4, "jne 0x40"}, {6, "cmp word ptr [bp - 4], 7"}, {10, "jne 0x50"}};
    const auto v = compare(two_exits, 12, two_exits, 12);
    CB_CHECK(!v.same);
    CB_CHECK_EQ(v.why, "more than one way out");
});

// What the Python this replaces said about 279 pairs of stretches (every pair it called the same, a few of each kind of
// refusal), checked in as testdata/boolx.cases; the stretches are those of the harness that generated them. A reason is
// compared whole, except those that name a value (up to the first words).
CB_TEST("exact.habits.boolx.cases_of_the_python_it_replaces", [] {
    const std::string data = fs::read_text(testing::data_path("exact/habits/testdata/boolx.cases"));
    const auto parse_code = [](std::string_view s) {
        Code out;
        for (const std::string_view item : text::split(s, " ; ")) {
            const auto colon = item.find(':');
            out.push_back({*text::parse_int<std::uint32_t>(item.substr(0, colon)), std::string(item.substr(colon + 1))});
        }
        return out;
    };
    const auto category = [](std::string_view why) {
        for (const std::string_view prefix : {"with ", "not a Boolean", "and of"}) {
            if (why.starts_with(prefix)) return std::string(prefix == std::string_view("with ") ? "with" : prefix);
        }
        return std::string(why);
    };
    int cases = 0;
    int same = 0;
    for (const std::string_view line : text::lines(data)) {
        if (line.empty() || line.front() == '#') continue;
        const auto f = text::split(line, '\t');
        CB_CHECK_EQ(f.size(), std::size_t{11});
        if (f.size() != 11) continue;
        const Code original = parse_code(f[8]);
        const Code compiled = parse_code(f[9]);
        const auto number = [&](std::size_t i) { return *text::parse_int<std::uint32_t>(f[i]); };
        const boolx::Verdict v = boolx::same({original, number(0), number(1), {std::string(f[4]), std::string(f[5])}},
                                             {compiled, number(2), number(3), {std::string(f[6]), std::string(f[7])}});
        ++cases;
        if (f[10].starts_with("T")) {
            ++same;
            CB_CHECK(v.same);
            if (f[10] == "T:-") {
                CB_CHECK(!v.else_targets.has_value());
            } else if (v.else_targets) {
                const auto pair = text::split(f[10].substr(2), ',');
                CB_CHECK_EQ(std::to_string(v.else_targets->original), std::string(pair[0]));
                CB_CHECK_EQ(std::to_string(v.else_targets->compiled), std::string(pair[1]));
            } else {
                CB_CHECK(false);
            }
        } else {
            CB_CHECK(!v.same);
            CB_CHECK_EQ(category(v.why), std::string(f[10].substr(2)));
        }
    }
    CB_CHECK_EQ(cases, 279);
    CB_CHECK_EQ(same, 83);
});
