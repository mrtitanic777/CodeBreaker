// Tests of cb/invert/known_differences.hpp: each seeded pattern on the divergence lines it was written for and on
// near misses, the table's order, and recording a new pattern. The inputs follow the old harness's cases (checked
// against the Python table), reviewed one by one.
//
// DIVERGES: the Python's `$` also matched before a trailing newline, so "len+1\n" was a trailing-pad hit; cb::Regex's
// `$` is the end of the text only. A divergence line is one line, so a trailing newline does not occur in practice.
#include "cb/invert/known_differences.hpp"

#include <string>

#include "cb/core/testing.hpp"

using namespace cb;
using namespace cb::invert;

namespace {

std::string ids(const KnownDifferences& kb, std::string_view text) {
    std::string out;
    for (const DifferencePattern* p : kb.diagnose(text)) out += (out.empty() ? "" : ",") + p->id;
    return out;
}

}  // namespace

CB_TEST("invert.known_differences.seeded_table", [] {
    const KnownDifferences kb;
    CB_CHECK_EQ(kb.patterns().size(), std::size_t{7});
    const char* order[] = {"seg-indexed-in-cond", "far-param-as-int", "trailing-pad",    "cdecl-arg-order",
                           "byte-store-mask",     "recipe-Os",        "addr-of-global-imm"};
    for (std::size_t i = 0; i < 7; ++i) CB_CHECK_EQ(kb.patterns()[i].id, order[i]);
    for (const auto& p : kb.patterns()) CB_CHECK(p.automatic);
    CB_CHECK_EQ(kb.patterns()[2].construct, "verify_inverted padding-strip");
    CB_CHECK_EQ(kb.patterns()[2].example, "LZEXPAND Cleanup");
});

CB_TEST("invert.known_differences.diagnose", [] {
    const KnownDifferences kb;
    CB_CHECK_EQ(ids(kb, "nothing the table knows"), "");
    CB_CHECK_EQ(ids(kb, ""), "");
    // seg-indexed-in-cond: a raw segment override leaked into the C (case does not matter)
    CB_CHECK_EQ(ids(kb, "ES:[bx]"), "seg-indexed-in-cond");
    CB_CHECK_EQ(ids(kb, "es:[bx+2]"), "seg-indexed-in-cond");
    CB_CHECK_EQ(ids(kb, "ds:"), "");   // no bracket: not the pattern
    // trailing-pad: the length differs by one, anchored at the end, or a named trailing byte
    CB_CHECK_EQ(ids(kb, "len+1"), "trailing-pad");
    CB_CHECK_EQ(ids(kb, "LEN+1"), "trailing-pad");
    CB_CHECK_EQ(ids(kb, "len+1\nmore"), "");
    CB_CHECK_EQ(ids(kb, "xlen+1"), "trailing-pad");
    CB_CHECK_EQ(ids(kb, "TRAILING 00"), "trailing-pad");
    CB_CHECK_EQ(ids(kb, "trailing 91"), "");
    // cdecl-arg-order
    CB_CHECK_EQ(ids(kb, "push[bp + N] foo vs bar push[bp + N]"), "cdecl-arg-order");
    CB_CHECK_EQ(ids(kb, "arg order"), "cdecl-arg-order");
    CB_CHECK_EQ(ids(kb, "ARG-ORDER"), "cdecl-arg-order");
    CB_CHECK_EQ(ids(kb, "arg  order"), "");   // `.?` is at most one character
    CB_CHECK_EQ(ids(kb, "arg\norder"), "");   // and `.` is not a newline
    // byte-store-mask
    CB_CHECK_EQ(ids(kb, "C606 4"), "byte-store-mask");
    CB_CHECK_EQ(ids(kb, "BYTE STORE and offset"), "byte-store-mask");
    CB_CHECK_EQ(ids(kb, "byte store\noffset"), "");
    // recipe-Os
    CB_CHECK_EQ(ids(kb, "len-3 xx frame"), "recipe-Os");
    CB_CHECK_EQ(ids(kb, "len+frame"), "");
    CB_CHECK_EQ(ids(kb, "SUB SP vs"), "recipe-Os");
    CB_CHECK_EQ(ids(kb, "sub sp"), "");
    CB_CHECK_EQ(ids(kb, "83EC02 ret"), "recipe-Os");
    // addr-of-global-imm
    CB_CHECK_EQ(ids(kb, "push imm global"), "addr-of-global-imm");
    CB_CHECK_EQ(ids(kb, "push N a global"), "addr-of-global-imm");
    CB_CHECK_EQ(ids(kb, "68 N"), "addr-of-global-imm");
});

CB_TEST("invert.known_differences.several_hits_in_table_order", [] {
    const KnownDifferences kb;
    CB_CHECK_EQ(ids(kb, "ship[les [bp+4]] vs ours[mov ax,[bp+4]] len+1 trailing 90 83ec02"),
                "far-param-as-int,trailing-pad,recipe-Os");
    const auto hits = kb.diagnose("c606 and push imm global");
    CB_CHECK_EQ(hits.size(), std::size_t{2});
    CB_CHECK_EQ(hits[0]->id, "byte-store-mask");
    CB_CHECK_EQ(hits[1]->id, "addr-of-global-imm");
});

CB_TEST("invert.known_differences.record", [] {
    KnownDifferences kb;
    CB_CHECK_EQ(ids(kb, "mov ax, 5"), "");
    const DifferencePattern& added = kb.record({.id = "mov-imm",
                                                .match = R"(mov ax, \d+)",
                                                .cause = "an immediate loaded the other way round",
                                                .fix = "swap the operands",
                                                .construct = "none yet",
                                                .automatic = false,
                                                .example = "test"});
    CB_CHECK_EQ(added.id, "mov-imm");
    CB_CHECK_EQ(kb.patterns().size(), std::size_t{8});
    CB_CHECK_EQ(ids(kb, "MOV AX, 5"), "mov-imm");
    // a pattern that is not a regex is refused and leaves the table as it was
    CB_CHECK_THROWS_AS(ParseError, kb.record({.id = "bad", .match = "(unclosed"}));
    CB_CHECK_EQ(kb.patterns().size(), std::size_t{8});
    CB_CHECK_EQ(ids(kb, "MOV AX, 5"), "mov-imm");
});

CB_TEST("invert.known_differences.listing", [] {
    KnownDifferences kb;
    const std::string text = kb.listing();
    CB_CHECK(text.starts_with("divergence KB: 7 known patterns\n\n  [auto] seg-indexed-in-cond\n"));
    CB_CHECK(text.find("      fix:   les [bp+off] marks off a far param (4 bytes); bound the count by retf N  (far-param "
                       "inference (les + retf N))\n") != std::string::npos);
    kb.record({.id = "manual-one", .match = "x", .automatic = false});
    CB_CHECK(kb.listing().find("  [manual] manual-one\n") != std::string::npos);
});

CB_TEST("invert.known_differences.own_table", [] {
    const KnownDifferences own(std::vector<DifferencePattern>{{.id = "a", .match = "x+y"}, {.id = "b", .match = "^z"}});
    CB_CHECK_EQ(own.patterns().size(), std::size_t{2});
    CB_CHECK_EQ(ids(own, "XXY"), "a");
    CB_CHECK_EQ(ids(own, "zed xy"), "a,b");
    CB_CHECK_EQ(ids(own, "es:[bx]"), "");   // the seeded patterns are not there
    CB_CHECK_THROWS_AS(ParseError, (void)KnownDifferences(std::vector<DifferencePattern>{{.id = "bad", .match = "("}}));
});
