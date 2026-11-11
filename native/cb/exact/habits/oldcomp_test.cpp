#include "cb/exact/habits/oldcomp.hpp"

#include <string>
#include <vector>

#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "cb/core/text.hpp"

using namespace cb;
using namespace cb::exact::habits;

namespace {

Bytes bytes_of(std::initializer_list<int> values) {
    Bytes out;
    for (int v : values) out.push_back(static_cast<std::uint8_t>(v));
    return out;
}

Bytes from_hex(std::string_view hex) {
    Bytes out;
    if (hex == "-") return out;
    for (std::size_t i = 0; i + 1 < hex.size(); i += 2) out.push_back(*text::parse_int<std::uint8_t>(hex.substr(i, 2), 16));
    return out;
}

oldcomp::Verdict judge(const Bytes& want, const Bytes& got, const std::set<std::size_t>& fixups = {}) {
    return oldcomp::equivalent(want, got, fixups);
}

}  // namespace

CB_TEST("exact.habits.oldcomp.for_entry_test", [] {
    // older: mov dx,[10]  cmp dx,[20]  ja +3  xchg dx,ax   kit: mov dx,[20]  cmp dx,[10]  jb +5  mov ax,[10]; the jump of
    // the kit's code is two bytes longer, so the two end the same place only because of the extra instruction
    const Bytes want = bytes_of({0x8b, 0x56, 0x10, 0x3b, 0x56, 0x20, 0x77, 0x03, 0x92, 0x90, 0x90});
    const Bytes got = bytes_of({0x8b, 0x56, 0x20, 0x3b, 0x56, 0x10, 0x72, 0x05, 0x8b, 0x46, 0x10, 0x90, 0x90});
    const auto v = judge(want, got);
    CB_CHECK(v.equivalent);
    CB_CHECK_EQ(text::join(v.found, "|"), "FOR entry test at +0000");
    // the jump of the kit's code lands one byte short of where the original's does
    Bytes wrong = got;
    wrong[7] = 0x04;
    const auto w = judge(want, wrong);
    CB_CHECK(!w.equivalent);
    CB_CHECK_EQ(w.stopped, "a jump to +000b goes to +000c in the compiled code, not +000d");
});

CB_TEST("exact.habits.oldcomp.string_parameter_upper_bound", [] {
    CB_CHECK(judge(bytes_of({0x8d, 0x7e, 0x04, 0xff, 0x75, 0x02}), bytes_of({0xff, 0x76, 0x06})).equivalent);
    // the displacement of the pair must be two more
    const auto v = judge(bytes_of({0x8d, 0x7e, 0x04, 0xff, 0x75, 0x02}), bytes_of({0xff, 0x76, 0x07}));
    CB_CHECK(!v.equivalent);
    CB_CHECK_EQ(v.stopped, "no pattern at +0000 (original 8d 7e 04 ff 75 02, compiled ff 76 07)");
});

CB_TEST("exact.habits.oldcomp.return_in_place", [] {
    // the older compiler writes lea sp,[bp-16]; pop bp; ret in place, the kit's jumps to the exit at the end
    const Bytes exit = bytes_of({0x8d, 0x66, 0xf0, 0x5d, 0xc3});
    Bytes want = exit;
    want.insert(want.end(), exit.begin(), exit.end());
    Bytes got = bytes_of({0xeb, 0x00});
    got.insert(got.end(), exit.begin(), exit.end());
    const auto v = judge(want, got);
    CB_CHECK(v.equivalent);
    CB_CHECK_EQ(text::join(v.found, "|"), "RETURN at +0000");
    // a jump to something that is not the exit is not this habit
    got[1] = 0x01;
    CB_CHECK(!judge(want, got).equivalent);
    // the near form
    Bytes near_got = bytes_of({0xe9, 0x00, 0x00});
    near_got.insert(near_got.end(), exit.begin(), exit.end());
    CB_CHECK(judge(want, near_got).equivalent);
});

CB_TEST("exact.habits.oldcomp.jump_made_long", [] {
    const Bytes want = bytes_of({0x72, 0x05, 0x90, 0x90, 0x90, 0x90, 0x90});
    const Bytes got = bytes_of({0x73, 0x03, 0xe9, 0x05, 0x00, 0x90, 0x90, 0x90, 0x90, 0x90});
    const auto v = judge(want, got);
    CB_CHECK(v.equivalent);
    CB_CHECK_EQ(text::join(v.found, "|"), "long jump at +0000");
    // and the other way round
    // and the other way round: the original has the long form, the compiled code the short one
    const auto back = judge(got, want);
    CB_CHECK(back.equivalent);
    CB_CHECK_EQ(text::join(back.found, "|"), "long jump at +0000");
    // the long jump going to the wrong place
    Bytes wrong = got;
    wrong[3] = 0x04;
    CB_CHECK(!judge(want, wrong).equivalent);
});

CB_TEST("exact.habits.oldcomp.value_kept_in_dx", [] {
    const Bytes prefix = bytes_of({0x8b, 0x56, 0x08, 0x8b, 0x3e, 0x34, 0x12, 0x89, 0x15});
    Bytes want = prefix;
    for (int b : {0x03, 0x56, 0x0a, 0x92}) want.push_back(static_cast<std::uint8_t>(b));
    Bytes got = prefix;
    for (int b : {0x8b, 0x46, 0x08, 0x03, 0x46, 0x0a}) got.push_back(static_cast<std::uint8_t>(b));
    const auto v = judge(want, got);
    CB_CHECK(v.equivalent);
    CB_CHECK_EQ(text::join(v.found, "|"), "value kept in DX at +0009");
    got[11] = 0x09;   // the other operand is not the one DX still holds
    CB_CHECK(!judge(want, got).equivalent);
});

CB_TEST("exact.habits.oldcomp.displacements_alone_are_not_a_habit", [] {
    // a call whose displacement differs: allowed, but nothing was explained by a habit
    const auto call = judge(bytes_of({0xe8, 0x10, 0x00}), bytes_of({0xe8, 0x12, 0x00}));
    CB_CHECK(!call.equivalent);
    CB_CHECK_EQ(call.stopped, "differs only in jump displacements?");
    // the same call next to a habit
    Bytes want = bytes_of({0xe8, 0x10, 0x00, 0x72, 0x05, 0x90, 0x90, 0x90, 0x90, 0x90});
    Bytes got = bytes_of({0xe8, 0x12, 0x00, 0x73, 0x03, 0xe9, 0x05, 0x00, 0x90, 0x90, 0x90, 0x90, 0x90});
    // the first of the three bytes after E8 differs: the displacement; the long-jump habit follows
    const auto both = judge(want, got);
    CB_CHECK(both.equivalent);
    CB_CHECK_EQ(text::join(both.found, "|"), "long jump at +0003");
});

CB_TEST("exact.habits.oldcomp.fixups_stand_for_equal_bytes", [] {
    // a byte the linker fills in is taken as equal; with nothing else to explain it the verdict is still "no habit"
    const auto v = judge(bytes_of({0x90, 0x90}), bytes_of({0x90, 0x91}), {1});
    CB_CHECK(!v.equivalent);
    CB_CHECK_EQ(v.stopped, "differs only in jump displacements?");
    CB_CHECK_EQ(judge(bytes_of({0x90, 0x90}), bytes_of({0x90, 0x91})).stopped, "no pattern at +0001 (original 90, compiled 91)");
});

CB_TEST("exact.habits.oldcomp.truncated_input_fails_without_error", [] {
    // a pattern needing bytes past the end does not match; nothing throws
    const auto cut = judge(bytes_of({0x8d, 0x7e}), bytes_of({0xff}));
    CB_CHECK(!cut.equivalent);
    CB_CHECK_EQ(cut.stopped, "no pattern at +0000 (original 8d 7e, compiled ff)");
    CB_CHECK_EQ(judge(bytes_of({0x01}), bytes_of({0x02})).stopped, "no pattern at +0000 (original 01, compiled 02)");
    CB_CHECK_EQ(judge({}, bytes_of({0x90})).stopped, "lengths do not close: 0 of 0, 0 of 1");
    CB_CHECK_EQ(judge(bytes_of({0x90}), {}).stopped, "lengths do not close: 0 of 1, 0 of 0");
    CB_CHECK_EQ(judge({}, {}).stopped, "differs only in jump displacements?");
    // a conditional jump cut off before its long form's displacement
    CB_CHECK(!judge(bytes_of({0x72, 0x05}), bytes_of({0x73, 0x03, 0xe9, 0x05})).equivalent);
    // a call cut off before its displacement is complete
    CB_CHECK(!judge(bytes_of({0xe8, 0x10}), bytes_of({0xe8, 0x12})).equivalent);
});

// The older Python's verdicts on 384 procedure pairs (every pair it accepted, a share of each kind of refusal, and the pairs
// it died on), checked in as testdata/oldcomp.cases. The pairs are those of the harness that generated them: a procedure
// built from the habits above, damaged in a few ways.
CB_TEST("exact.habits.oldcomp.cases_of_the_python_it_replaces", [] {
    const std::string data = fs::read_text(testing::data_path("exact/habits/testdata/oldcomp.cases"));
    int cases = 0;
    int accepted = 0;
    for (const std::string_view line : text::lines(data)) {
        if (line.empty() || line.front() == '#') continue;
        const auto f = text::split(line, '\t');
        CB_CHECK_EQ(f.size(), std::size_t{4});
        if (f.size() != 4) continue;
        std::set<std::size_t> fixups;
        if (f[2] != "-") {
            for (const auto v : text::split(f[2], ',')) fixups.insert(*text::parse_int<std::size_t>(v));
        }
        const auto verdict = oldcomp::equivalent(from_hex(f[0]), from_hex(f[1]), fixups);
        ++cases;
        const auto expected = text::split(f[3], '|');
        if (expected[0] == "ERR") continue;   // the Python raised IndexError / struct.error: here the pair just fails or not
        if (expected[0] == "T") {
            ++accepted;
            CB_CHECK(verdict.equivalent);
            CB_CHECK_EQ(verdict.found.size(), expected.size() - 1);
            for (std::size_t k = 0; k + 1 < expected.size() && k < verdict.found.size(); ++k) CB_CHECK_EQ(verdict.found[k], std::string(expected[k + 1]));
        } else {
            CB_CHECK(!verdict.equivalent);
            CB_CHECK_EQ(verdict.stopped, std::string(expected[1]));
        }
    }
    CB_CHECK_EQ(cases, 384);
    CB_CHECK_EQ(accepted, 256);
});
