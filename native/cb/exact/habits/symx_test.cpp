#include "cb/exact/habits/symx.hpp"

#include <string>
#include <vector>

#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "cb/core/text.hpp"

using namespace cb;
using namespace cb::exact::habits;

namespace {

symx::Instruction ins(std::string text, Bytes bytes = {}) { return {std::move(text), std::move(bytes)}; }

using Stretch = std::vector<symx::Instruction>;

symx::Verdict compare(const Stretch& want, const Stretch& got, const symx::LinkerWords& words = {}, symx::Options options = {}) {
    return symx::same(want, got, words, options);
}

}  // namespace

CB_TEST("exact.habits.symx.sums_are_compared_without_regard_to_the_order_of_terms", [] {
    const Stretch want = {ins("mov ax, bx"), ins("add ax, cx"), ins("push ax")};
    const Stretch got = {ins("mov ax, cx"), ins("add ax, bx"), ins("push ax")};
    CB_CHECK(compare(want, got).same);
    // another sum is not the same
    const Stretch other = {ins("mov ax, cx"), ins("add ax, si"), ins("push ax")};
    const auto v = compare(want, other);
    CB_CHECK(!v.same);
    CB_CHECK(v.why.starts_with("other values pushed: "));
});

CB_TEST("exact.habits.symx.a_shift_is_a_product", [] {
    const Stretch by_count = {ins("mov cx, 3"), ins("shl ax, cl"), ins("push ax")};
    const Stretch one_at_a_time = {ins("mov cx, 3"), ins("shl ax, 1"), ins("shl ax, 1"), ins("shl ax, 1"), ins("push ax")};
    CB_CHECK(compare(by_count, one_at_a_time).same);
    // x >> 1 >> 2 is x >> 3
    const Stretch right_by_count = {ins("mov cx, 3"), ins("shr ax, cl"), ins("push ax")};
    const Stretch right_in_steps = {ins("shr ax, 1"), ins("mov cx, 2"), ins("shr ax, cl"), ins("push ax")};
    CB_CHECK(compare(right_by_count, right_in_steps).same);
    // a shift by a count that is not known is refused
    const auto unknown = compare({ins("shl ax, cl")}, {ins("shl ax, cl")});
    CB_CHECK(!unknown.same);
    CB_CHECK(unknown.why.starts_with("not known: "));
});

CB_TEST("exact.habits.symx.constants_fold_and_a_linker_word_matches_any_constant", [] {
    const Stretch want = {ins("mov ax, 0x1234"), ins("add ax, 2"), ins("push ax")};
    CB_CHECK(compare(want, {ins("mov ax, 0x1236"), ins("push ax")}).same);
    CB_CHECK(!compare(want, {ins("mov ax, 0x1237"), ins("push ax")}).same);
    // the linker fills in the first number of the compiled code: whatever it is, it stands for the original's
    CB_CHECK(compare(want, {ins("mov ax, 0x9999"), ins("add ax, 2"), ins("push ax")}, {{0, {0x9999}}}).same);
    CB_CHECK(!compare(want, {ins("mov ax, 0x9999"), ins("add ax, 2"), ins("push ax")}).same);
    // 16-bit wrap
    CB_CHECK(compare({ins("mov ax, 0xffff"), ins("inc ax"), ins("push ax")}, {ins("xor ax, ax"), ins("push ax")}).same);
});

CB_TEST("exact.habits.symx.frame_temporaries_may_be_other_temporaries", [] {
    const Stretch want = {ins("mov word ptr [bp - 2], ax"), ins("mov word ptr [bp - 4], bx")};
    CB_CHECK(compare(want, {ins("mov word ptr [bp - 6], bx"), ins("mov word ptr [bp - 8], ax")}).same);
    const auto v = compare(want, {ins("mov word ptr [bp - 6], bx"), ins("mov word ptr [bp - 8], cx")});
    CB_CHECK(!v.same);
    CB_CHECK_EQ(v.why, "other values left in the frame");
    // but a store through another register has to be at the same place, in the same order
    CB_CHECK(compare({ins("mov word ptr [bx + 2], ax"), ins("mov word ptr [bx + 4], cx")},
                     {ins("mov word ptr [bx + 2], ax"), ins("mov word ptr [bx + 4], cx")}).same);
    CB_CHECK_EQ(compare({ins("mov word ptr [bx + 2], ax"), ins("mov word ptr [bx + 4], cx")},
                        {ins("mov word ptr [bx + 4], cx"), ins("mov word ptr [bx + 2], ax")}).why,
                "other values stored");
    // a temporary read back as the other size is refused
    CB_CHECK(compare({ins("mov word ptr [bp - 2], ax"), ins("mov al, byte ptr [bp - 2]")}, {}).why.starts_with("not known"));
});

CB_TEST("exact.habits.symx.reading_back_a_temporary_gives_what_was_stored", [] {
    const Stretch want = {ins("mov ax, bx"), ins("mov word ptr [bp - 2], ax"), ins("mov cx, word ptr [bp - 2]"), ins("push cx")};
    const Stretch got = {ins("push bx")};
    CB_CHECK(!compare(want, got).same);   // (the temporary is left in the frame; the other has none)
    const Stretch kept = {ins("push bx"), ins("mov word ptr [bp - 6], bx")};
    CB_CHECK(compare(want, kept).same);
});

CB_TEST("exact.habits.symx.a_boolean_through_a_conditional_jump", [] {
    // cmp ax, bx / mov cx, 0 / jb over / inc cx: the compiler's way to the value of a < b
    const Stretch boolean = {ins("cmp ax, bx"), ins("mov cx, 0", {0xB9, 0, 0}), ins("jb 0x9", {0x72, 0x01}), ins("inc cx", {0x41}), ins("push cx")};
    CB_CHECK(compare(boolean, boolean, {}, {.flags = true}).same);
    // the same Boolean from the other comparison is not the same value
    Stretch other = boolean;
    other[0] = ins("cmp ax, cx");
    CB_CHECK(!compare(boolean, other).same);
    // without the jump's bytes it is an instruction this does not know
    Stretch bare = boolean;
    bare[2].bytes.clear();
    CB_CHECK(compare(bare, bare).why.starts_with("not known"));
    // what a jump that is anything else does
    Stretch far = boolean;
    far[2].bytes = {0x72, 0x03};
    CB_CHECK(compare(far, far).why.starts_with("not known"));
});

CB_TEST("exact.habits.symx.registers_and_flags_when_asked", [] {
    const Stretch want = {ins("mov ax, bx")};
    const Stretch got = {ins("mov ax, cx")};
    CB_CHECK(compare(want, got).same);   // nothing is stored, pushed or left in the frame
    const auto registers = compare(want, got, {}, {.registers = true});
    CB_CHECK(!registers.same);
    CB_CHECK_EQ(registers.why, "other values left in the registers");
    CB_CHECK(compare({ins("cmp ax, bx")}, {ins("cmp ax, bx")}, {}, {.flags = true}).same);
    CB_CHECK_EQ(compare({ins("cmp ax, bx")}, {ins("cmp ax, cx")}, {}, {.flags = true}).why, "another condition");
    // after an add the flags are those of its result: a different order of the same terms is the same condition
    CB_CHECK(compare({ins("add ax, bx")}, {ins("add bx, ax"), ins("mov ax, bx")}, {}, {.flags = true}).same);
});

CB_TEST("exact.habits.symx.the_state_after_a_stretch", [] {
    const Stretch stretch = {ins("mov ax, 5"), ins("add ax, 3"), ins("mov word ptr [bp - 2], ax"), ins("push word ptr [bx + 1]"), ins("mul cx")};
    const auto state = symx::try_run(stretch, {});
    CB_CHECK(state.ok());
    if (!state.ok()) return;
    const symx::State& s = state.value();
    // mul: ax = ax * cx = 8 * cx, dx = high word of it
    CB_CHECK_EQ(symx::show(s.reg("ax")), "(* cx 8)");
    CB_CHECK_EQ(symx::show(s.reg("dx")), "(high (* cx 8))");
    CB_CHECK_EQ(s.slots.size(), std::size_t{1});
    CB_CHECK_EQ(s.pushes.size(), std::size_t{1});
    CB_CHECK_EQ(symx::show(s.pushes[0]), "(m (+ bx 1) word after 0 stores)");
    CB_CHECK(s.flags.has_value());
    CB_CHECK_EQ(symx::show(s.reg("si")), "si");
    // div uses dx:ax
    const auto division = symx::try_run(Stretch{ins("div bx")}, {});
    CB_CHECK(division.ok());
    CB_CHECK_EQ(symx::show(division.value().reg("ax")), "(div dx ax bx)");
    CB_CHECK(!division.value().flags.has_value());
});

CB_TEST("exact.habits.symx.what_it_does_not_know_it_refuses", [] {
    const char* refused[] = {"jmp 0x10", "mov ax, bp", "mov ah, 1", "mov ax", "xor ax", "mov", "shl ax", "mov ax, word ptr [si*2]", "mov ax, word ptr [sp]",
                             "mov ax, 99999999999999999999", "mov ax, [bx", "rep movsb", "mov ax, ", "pop ax", "mov ax,bx"};
    for (const char* text : refused) {
        const auto r = symx::try_run(Stretch{ins(text)}, {});
        CB_CHECK(!r.ok());
        if (!r.ok()) CB_CHECK_EQ(r.error().kind(), "UnsupportedError");
    }
    CB_CHECK(symx::try_run(Stretch{ins("mov ax, 0")}, {}).ok());
    CB_CHECK(symx::try_run(Stretch{}, {}).ok());
    CB_CHECK(compare({}, {}).same);
});

CB_TEST("exact.habits.symx.byte_registers", [] {
    // al is the low byte of ax: writing it keeps the high byte; xor ah, ah then zero-extends it
    const Stretch want = {ins("mov al, bl"), ins("xor ah, ah"), ins("push ax")};
    const Stretch got = {ins("mov al, bl"), ins("xor ah, ah"), ins("push ax")};
    CB_CHECK(compare(want, got).same);
    CB_CHECK(!compare(want, {ins("mov al, cl"), ins("xor ah, ah"), ins("push ax")}).same);
    // and ax, 0xff does the same as the pair
    CB_CHECK(compare({ins("mov ax, bx"), ins("and ax, 0xff"), ins("push ax")}, {ins("mov al, bl"), ins("xor ah, ah"), ins("push ax")}).same);
    CB_CHECK(compare({ins("mov ax, bx"), ins("and ax, 0xff"), ins("push ax")}, {ins("mov ax, bx"), ins("and ax, 0xff"), ins("push ax")}).same);
});

// The older Python's verdicts on 694 pairs of stretches (a third of them "the same", the rest one of each kind of refusal),
// checked in as testdata/symx.cases; the pairs are those of the harness that generated them. Only the kind of reason is
// compared (what is up to the colon), not its wording.
CB_TEST("exact.habits.symx.cases_of_the_python_it_replaces", [] {
    const std::string data = fs::read_text(testing::data_path("exact/habits/testdata/symx.cases"));
    const auto parse_stretch = [](std::string_view s) {
        Stretch out;
        if (s.empty()) return out;
        for (const std::string_view one : text::split(s, " | ")) {
            const auto at = one.rfind('@');
            symx::Instruction i;
            i.text = std::string(one.substr(0, at));
            const std::string_view hex = one.substr(at + 1);
            for (std::size_t k = 0; k + 1 < hex.size(); k += 2) i.bytes.push_back(*text::parse_int<std::uint8_t>(hex.substr(k, 2), 16));
            out.push_back(std::move(i));
        }
        return out;
    };
    int cases = 0;
    int same = 0;
    for (const std::string_view line : text::lines(data)) {
        if (line.empty() || line.front() == '#') continue;
        const auto f = text::split(line, '\t');
        CB_CHECK_EQ(f.size(), std::size_t{5});
        if (f.size() != 5) continue;
        symx::LinkerWords words;
        if (f[1] != "-") {
            for (const std::string_view entry : text::split(f[1], ';')) {
                const auto colon = entry.find(':');
                auto& set = words[*text::parse_int<std::size_t>(entry.substr(0, colon))];
                for (const std::string_view v : text::split(entry.substr(colon + 1), ',')) set.insert(*text::parse_int<std::int64_t>(v));
            }
        }
        const symx::Verdict v = symx::same(parse_stretch(f[2]), parse_stretch(f[3]), words, {.registers = f[0][0] == '1', .flags = f[0][1] == '1'});
        ++cases;
        if (f[4] == "T") {
            ++same;
            CB_CHECK(v.same);
        } else {
            CB_CHECK(!v.same);
            CB_CHECK_EQ(v.why.substr(0, v.why.find(':')), std::string(f[4].substr(2)));
        }
    }
    CB_CHECK_EQ(cases, 694);
    CB_CHECK_EQ(same, 260);
});
