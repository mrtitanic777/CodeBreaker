#include "cb/exact/aim.hpp"

#include <string>
#include <vector>

#include "cb/core/crypto.hpp"
#include "cb/core/error.hpp"
#include "cb/core/testing.hpp"
#include "cb/core/text.hpp"

using namespace cb;
using namespace cb::exact;

// (CB_CHECK_EQ prints its operands, and MSVC formats a vector as a range: for buffers and lists compare with ==)
#define CB_CHECK_SAME(a, b) CB_CHECK((a) == (b))

namespace {

// A Microsoft C listing (the shape of its /Fc output) of two functions: `other`, then `f`. f is
//     int f(int a, int b) { int t = a; if (t >= b) return 1; ... }
// with the source lines 5..9.
const std::string kListing =
    "_TEXT\tSEGMENT\n"
    "\tASSUME\tCS: _TEXT\n"
    "; Line 3\n"
    "\tPUBLIC\t_other\n"
    "_other\tPROC NEAR\n"
    "*** 000000\tc3 \t\t\tret\n"
    "_other\tENDP\n"
    "; Line 5\n"
    "\tPUBLIC\t_f\n"
    "_f\tPROC NEAR\n"
    "*** 000000\t55 \t\t\tpush\tbp\n"
    "*** 000001\t8b ec \t\t\tmov\tbp,sp\n"
    "; Line 6\n"
    "*** 000003\t83 ec 02 \t\tsub\tsp,2\n"
    "*** 000006\t8b 46 04 \t\tmov\tax,WORD PTR [bp+4]\n"
    "; Line 7\n"
    "*** 000009\t89 46 fe \t\tmov\tWORD PTR [bp-2],ax\n"
    "; Line 8\n"
    "*** 00000c\t3b 46 06 \t\tcmp\tax,WORD PTR [bp+6]\n"
    "*** 00000f\t7c 03 \t\t\tjl\t$L1\n"
    "*** 000011\tb8 01 00 \t\tmov\tax,1\n"
    "$L1:\n"
    "; Line 9\n"
    "*** 000014\t8b e5 \t\t\tmov\tsp,bp\n"
    "*** 000016\t5d \t\t\tpop\tbp\n"
    "*** 000017\tc3 \t\t\tret\n"
    "_f\tENDP\n"
    "_TEXT\tENDS\n"
    "END\n";

Bytes hex(std::string_view s) { return crypto::from_hex(s); }

aim::Diagnosis diagnose_against(std::string_view program_hex) {
    const auto code = aim::listing_code(kListing, "f");
    return aim::diagnose(hex(program_hex), aim::code_of(code), code);
}

using Kinds = std::vector<std::pair<aim::Kind, std::size_t>>;

std::vector<std::string> v(std::initializer_list<const char*> items) { return {items.begin(), items.end()}; }

}  // namespace

CB_TEST("exact.aim.listing_first_function_and_named_function", [] {
    // without a name: the first function of the listing, which ends at its ENDP
    const auto first = aim::listing_code(kListing);
    CB_CHECK_EQ(first.size(), 1u);
    CB_CHECK_SAME(first[0].bytes, hex("c3"));
    CB_CHECK(first[0].line == 3u);
    // by name, the leading underscore is not compared either way
    for (const char* name : {"f", "_f"}) {
        const auto f = aim::listing_code(kListing, name);
        CB_CHECK_EQ(f.size(), 11u);
        CB_CHECK_EQ(f[0].offset, 0u);
        CB_CHECK(f[0].line == 5u);
        CB_CHECK(f[2].line == 6u);
        CB_CHECK_SAME(f[2].bytes, hex("83ec02"));
        CB_CHECK(f[6].line == 8u && f[7].line == 8u);   // two instructions under one "; Line"
        CB_CHECK(f[10].line == 9u);
    }
    CB_CHECK(aim::listing_code(kListing, "nosuch").empty());
    CB_CHECK(aim::listing_code("").empty());
});

CB_TEST("exact.aim.code_of_lays_instructions_at_their_offsets", [] {
    const auto f = aim::listing_code(kListing, "f");
    CB_CHECK_SAME(aim::code_of(f), hex("558bec83ec028b46048946fe3b46067c03b801008be55dc3"));
    CB_CHECK(aim::code_of({}).empty());
    // a listing whose first instruction is not at 0 is laid from that instruction
    std::vector<aim::ListingEntry> shifted = {{0x10, {0x90}, 1}, {0x11, {0xc3}, 1}};
    CB_CHECK_SAME(aim::code_of(shifted), hex("90c3"));
    std::vector<aim::ListingEntry> backwards = {{0x10, {0x90}, 1}, {0x08, {0xc3}, 1}};
    CB_CHECK_THROWS_AS(FormatError, (void)aim::code_of(backwards));
});

CB_TEST("exact.aim.normalize_masks_what_a_link_changes", [] {
    const auto norm = [](const char* mnemonic, const char* ops) {
        return aim::normalize(x86::Insn{0, {}, mnemonic, ops});
    };
    CB_CHECK_EQ(norm("jl", "0x14"), "jl T");                       // a jump's target
    CB_CHECK_EQ(norm("call", "0x1234"), "call T");
    CB_CHECK_EQ(norm("loop", "0x10"), "loop T");
    CB_CHECK_EQ(norm("mov", "ax, word ptr [bp - 6]"), "mov ax, word ptr [bp-6]");   // a frame's place is kept
    CB_CHECK_EQ(norm("mov", "ax, word ptr [bp + 4]"), "mov ax, word ptr [bp+4]");
    CB_CHECK_EQ(norm("mov", "ax, word ptr [0x1234]"), "mov ax, word ptr [M]");       // a data place is masked
    CB_CHECK_EQ(norm("mov", "ax, word ptr [bx + 0x1234]"), "mov ax, word ptr [bx]");
    CB_CHECK_EQ(norm("mov", "ax, word ptr [bx + si + 6]"), "mov ax, word ptr [bx+si+6]");   // a small field offset is kept
    CB_CHECK_EQ(norm("mov", "ax, word ptr [bx + 0x80]"), "mov ax, word ptr [bx]");
    CB_CHECK_EQ(norm("mov", "ax, 1"), "mov ax, 1");                 // a small constant is kept
    CB_CHECK_EQ(norm("mov", "ax, 0x7f"), "mov ax, 0x7f");
    CB_CHECK_EQ(norm("mov", "ax, 0x80"), "mov ax, N");
    CB_CHECK_EQ(norm("mov", "ax, 0"), "mov ax, N");                 // 0 is the unlinked object's: masked
    CB_CHECK_EQ(norm("mov", "ax, 0x12345678"), "mov ax, N");
    CB_CHECK_EQ(norm("mov", "ax, 0xffffffffffffffffff"), "mov ax, N");   // past 64 bits: still just a big number
    CB_CHECK_EQ(norm("ret", ""), "ret");
    CB_CHECK_EQ(norm("cbw", ""), "cbw");
    CB_CHECK_EQ(norm("mov", "ax, word ptr [bp"), "mov ax, word ptr [bp");   // an unclosed bracket is left as it is
});

CB_TEST("exact.aim.kinds_of_difference", [] {
    using aim::Kind;
    CB_CHECK(aim::classify(v({"mov ax, [bp-2]"}), v({"mov ax, [bp-4]"})) == Kind::Frame);
    CB_CHECK(aim::classify(v({"jl T", "cmp ax, bx"}), v({"jb T", "cmp ax, bx"})) == Kind::Sign);
    CB_CHECK(aim::classify(v({"sar ax, 1"}), v({"shr ax, 1"})) == Kind::Sign);
    CB_CHECK(aim::classify(v({"cbw", "mov ax, bx"}), v({"xor ah, ah", "mov ax, bx"})) == Kind::Width);   // (a widening)
    CB_CHECK(aim::classify(v({"mov al, byte ptr [bx]"}), v({"mov ax, word ptr [bx]"})) == Kind::Width);
    CB_CHECK(aim::classify(v({"mov ax, [bx]"}), v({"mov al, [bx]"})) == Kind::Shape);   // only the first's byte register counts
    CB_CHECK(aim::classify(v({"mov al, [bx]"}), v({"mov ax, [bx]"})) == Kind::Width);
    CB_CHECK(aim::classify(v({"a", "b"}), v({"b", "a"})) == Kind::Order);
    CB_CHECK(aim::classify(v({"a"}), v({"b"})) == Kind::Shape);
    // a widening that changed the instruction count by up to two, markers in different places
    CB_CHECK(aim::classify(v({"mov al, byte ptr [bx]", "cbw", "push ax"}), v({"mov ax, [bx]", "push ax"})) == Kind::Width);
    CB_CHECK(aim::classify(v({"cbw", "a", "b", "c"}), v({"cbw", "a", "b", "d"})) == Kind::Shape);   // the markers are the same
    // a register named only as part of a longer word is not a byte register
    CB_CHECK(aim::classify(v({"mov alpha, 1"}), v({"mov beta, 1"})) == Kind::Shape);
});

CB_TEST("exact.aim.diagnose_against_the_listing", [] {
    using aim::Kind;
    const auto none = diagnose_against("558bec83ec028b46048946fe3b46067c03b801008be55dc3");
    CB_CHECK(none.kinds.empty() && none.lines.empty() && !none.first());
    // jl where the program has jb: Sign, from the source line of the compare (8)
    const auto sign = diagnose_against("558bec83ec028b46048946fe3b46067203b801008be55dc3");
    CB_CHECK(sign.kinds == (Kinds{{Kind::Sign, 1}}));
    CB_CHECK(sign.lines == std::vector<std::uint32_t>{8});
    CB_CHECK(sign.first() == 8u);
    // [bp+6] where the candidate has [bp+4]
    const auto frame = diagnose_against("558bec83ec028b46068946fe3b46067c03b801008be55dc3");
    CB_CHECK(frame.kinds == (Kinds{{Kind::Frame, 1}}));
    CB_CHECK(frame.first() == 6u);
    // mov al, [bp+4] where the candidate has mov ax, [bp+4]
    const auto width = diagnose_against("558bec83ec028a46048946fe3b46067c03b801008be55dc3");
    CB_CHECK(width.kinds == (Kinds{{Kind::Width, 1}}));
    CB_CHECK(width.first() == 6u);
    // another constant, and an instruction fewer: Shape
    const auto constant = diagnose_against("558bec83ec028b46048946fe3b46067c03b802008be55dc3");
    CB_CHECK(constant.kinds == (Kinds{{Kind::Shape, 1}}));
    CB_CHECK(constant.first() == 8u);
    const auto fewer = diagnose_against("558bec83ec028b46048946fe3b46067c038be55dc3");
    CB_CHECK(fewer.kinds == (Kinds{{Kind::Shape, 1}}));
    // the same instructions with `sub sp, 2` moved after the first mov: Order (two instructions out of place)
    const auto moved = diagnose_against("558bec8b460483ec028946fe3b46067c03b801008be55dc3");
    CB_CHECK(moved.kinds == (Kinds{{Kind::Order, 2}}));
    CB_CHECK(moved.lines == (std::vector<std::uint32_t>{6, 7}));
});

CB_TEST("exact.aim.diagnose_is_deterministic_on_garbage", [] {
    // bytes that decode to nothing are `db` lines; no crash, and the same answer twice
    const Bytes junk = {0xff, 0xff, 0xff, 0x0f, 0x0b, 0x90};
    const auto code = aim::listing_code(kListing, "f");
    const auto a = aim::diagnose(junk, aim::code_of(code), code);
    const auto b = aim::diagnose(junk, aim::code_of(code), code);
    CB_CHECK(a.kinds == b.kinds && a.lines == b.lines);
    CB_CHECK(!a.kinds.empty());
    CB_CHECK(aim::diagnose({}, {}, {}).kinds.empty());
});

CB_TEST("exact.aim.opcodes_are_difflibs", [] {
    using T = aim::Opcode::Tag;
    const auto split = [](std::string_view s) {
        std::vector<std::string> out;
        for (const auto w : text::split_ws(s)) out.emplace_back(w);
        return out;
    };
    // (computed with difflib.SequenceMatcher(None, a, b, autojunk=False).get_opcodes())
    const auto one = aim::opcodes(split("a b c d e f g"), split("a x c d f g h"));
    const std::vector<aim::Opcode> want1 = {{T::Equal, 0, 1, 0, 1},   {T::Replace, 1, 2, 1, 2}, {T::Equal, 2, 4, 2, 4},
                                            {T::Delete, 4, 5, 4, 4},  {T::Equal, 5, 7, 4, 6},   {T::Insert, 7, 7, 6, 7}};
    CB_CHECK(one == want1);
    // repeats: the earliest longest match, so the leading y is inserted
    const auto two = aim::opcodes(split("x y x y x"), split("y x y x y"));
    const std::vector<aim::Opcode> want2 = {{T::Insert, 0, 0, 0, 1}, {T::Equal, 0, 4, 1, 5}, {T::Delete, 4, 5, 5, 5}};
    CB_CHECK(two == want2);
    CB_CHECK(aim::opcodes({}, {}).empty());
    CB_CHECK(aim::opcodes(split("a b"), split("a b")) == (std::vector<aim::Opcode>{{T::Equal, 0, 2, 0, 2}}));
    CB_CHECK(aim::opcodes(split("a"), {}) == (std::vector<aim::Opcode>{{T::Delete, 0, 1, 0, 0}}));
    CB_CHECK(aim::opcodes({}, split("a")) == (std::vector<aim::Opcode>{{T::Insert, 0, 0, 0, 1}}));
});
