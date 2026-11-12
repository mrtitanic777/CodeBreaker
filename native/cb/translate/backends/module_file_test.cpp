// Tests of cb/translate/backends/module_file: the files a module's reconstruction is written as -- the C file (one, or in
// parts) with the status at its head, and a driver's assembly file -- from the L2 tree and the facts of the binary.
//
// The expected texts are the output of the Python back end (translate/backends/module_file.py) for the same module and
// facts, read through and kept because they say what the file is meant to say.
//
// DIVERGES from the Python: the facts are a record (the status classes are an ordered list of title and count, not a key
// order plus two dicts); a unit that is not one of the kinds the file writes is declined with the assembly back end's
// error, not a Python exception; a module with no source for the assembly file takes the same one-line stand-in.
#include "cb/translate/backends/module_file.hpp"

#include "cb/core/testing.hpp"

namespace {

namespace ir = cb::translate::ir;
namespace mf = cb::translate::backends::module_file;
namespace as = cb::translate::backends::assembly;

mf::Facts mf_facts() {
    mf::Facts f;
    f.name = "lz32.dll";
    f.stem = "lz32";
    f.date = "2026-11-11";
    f.digest = "ab12";
    f.statuses = {{"VERIFIED", 2}, {"WEAK", 1}, {"UNVERIFIED", 0}};
    f.total = 3;
    f.real = 4;
    f.header = true;
    f.how = "the judge said fine";
    f.rows = {{"00401000", "sub_401000", "WEAK", "one input"}};
    return f;
}

ir::Node* mf_function(cb::Arena& arena, std::string status, std::string text) {
    ir::MethodDecl m;
    m.kind = "function";
    m.comments = {std::move(status)};
    m.text = std::move(text);
    return ir::make(arena, std::move(m));
}

ir::Node* mf_comment(cb::Arena& arena, std::string text) { return ir::make(arena, ir::Comment{.text = std::move(text)}); }

ir::Node* mf_module(cb::Arena& arena, std::vector<ir::Node*> units) { return ir::make(arena, ir::Module{.units = std::move(units)}); }

// int f(void) {...}, a function with no reconstruction, int g(void) {...}
ir::Node* mf_c_module(cb::Arena& arena) {
    return mf_module(arena, {mf_function(arena, "VERIFIED", "int f(void) { return 1; }"), mf_comment(arena, "WEAK  sub_2"),
                             mf_function(arena, "WEAK", "int g(void) { return 2; }")});
}

constexpr std::string_view kStatusLines = R"( * 3 functions:
 *   VERIFIED                         2   66.7%
 *   WEAK                             1   33.3%
 *   UNVERIFIED                       0    0.0%
)";

constexpr std::string_view kBodies = R"(/* VERIFIED */
int f(void) { return 1; }

/* WEAK  sub_2 */

/* WEAK */
int g(void) { return 2; }
)";

}  // namespace

CB_TEST("translate.module_file.c_not_built", [] {
    cb::Arena arena;
    auto parts = mf::parts(mf_c_module(arena));
    CB_CHECK(parts.ok());
    CB_CHECK_EQ(parts.value().size(), std::size_t{3});
    CB_CHECK(parts.value()[1].status == "WEAK  sub_2" && !parts.value()[1].text);
    const auto files = mf::c(parts.value(), mf_facts());
    CB_CHECK_EQ(files.size(), std::size_t{1});
    CB_CHECK_EQ(files[0].name, std::string("lz32.c"));
    const std::string expected = std::string(R"(/* lz32.dll -- reconstructed source.
 *
 * STATUS, generated 2026-11-11 by CodeBreaker (verifier settings ab12).
)") + std::string(kStatusLines) + R"( *
 * Builds as a whole module: not built here (run `codebreaker build lz32.dll`).
 * Functions are compiled and checked one at a time; the module's data (globals, tables,
 * resources) is referred to by its address in the original unless the whole module is built.
 * Runs in place of the original: NOT TESTED.
 *
 * VERIFIED = rebuilt from this text with OpenWatcom and run beside the original in an x86 emulator:
 * same return value, same calls out of the module with the same arguments, same memory changed,
 * same stack and saved registers, on every input tried. It is evidence, not proof. Names of
 * internal functions (sub_...) and locals are placeholders; no original names are claimed.
 * Written in after judging, changing no code: the SDK's names for constants passed to
 * Windows (with their values in lz32.h), and names for locals, parameters and globals
 * taken from the Windows functions they are passed to or hold the result of.
 * IDENTIFIED = the function's bytes are those of a function in a library that came with the
 * compiler; it is named after that function. It is reconstructed and judged all the same.
 * Imports by number are written with their real names (4 of them; lz32.h lists the
 * module and number of each). The text that was built and judged calls them MODULE_number.
 */

#include "lz32.h"

)" + std::string(kBodies);
    CB_CHECK_EQ(files[0].text, expected);
});

CB_TEST("translate.module_file.c_whole_build_in_parts", [] {
    cb::Arena arena;
    auto parts = mf::parts(mf_c_module(arena)).value();
    mf::Facts facts = mf_facts();
    facts.lane = mf::Lane::PowerPc;
    facts.whole = mf::Whole{.linked = true, .exports_same = 3, .exports_total = 5, .rejected = 1, .absent = 0, .built = "x.dll"};
    const auto files = mf::c(parts, facts, /*split=*/2);
    CB_CHECK_EQ(files.size(), std::size_t{2});
    CB_CHECK_EQ(files[0].name, std::string("lz32_1.c"));
    CB_CHECK_EQ(files[1].name, std::string("lz32_2.c"));
    const std::string first = std::string(R"(/* lz32.dll -- reconstructed source.
 *
 * STATUS, generated 2026-11-11 by CodeBreaker (verifier settings ab12).
)") + std::string(kStatusLines) + R"( *
 * Builds as a whole module: YES (3 of 5 exports match). Every original export is present; a function the
 * compiler rejected, or a call to code with no reconstruction, is a stand-in that returns 0
 * (1 rejected, 0 absent). The built image is x.dll.
 * Runs in place of the original: NOT TESTED as a whole.
 *
 * Imports by number are written with their real names (4 of them; lz32.h lists the
 * module and number of each). The text that was built and judged calls them MODULE_number.
 */

#include "lz32.h"

/* VERIFIED */
int f(void) { return 1; }

/* WEAK  sub_2 */
)";
    CB_CHECK_EQ(files[0].text, first);
    // a later part says which part it is, and includes what the first includes
    CB_CHECK_EQ(files[1].text, std::string(R"(/* lz32.dll -- reconstructed source, part 2 of 2 (the status is in lz32_1.c). */

#include "lz32.h"

/* WEAK */
int g(void) { return 2; }
)"));
    // all exports matching is said in words; a split larger than the module is one file
    facts.whole->exports_same = 5;
    CB_CHECK(mf::banner(facts)[8].find("YES (all the original's exports match)") != std::string::npos);
    CB_CHECK_EQ(mf::c(parts, facts, 3).size(), std::size_t{1});
    CB_CHECK_EQ(mf::c(parts, facts, 1).size(), std::size_t{3});
});

CB_TEST("translate.module_file.c_whole_build_failed", [] {
    cb::Arena arena;
    const auto parts = mf::parts(mf_c_module(arena)).value();
    mf::Facts facts = mf_facts();
    facts.whole = mf::Whole{.error = "link error"};
    facts.real = 0;
    facts.header = false;
    const auto files = mf::c(parts, facts);
    CB_CHECK(files[0].text.find(" * Builds as a whole module: NOT YET (link error).\n") != std::string::npos);
    CB_CHECK(files[0].text.find("Imports by number") == std::string::npos);
    // no header: the head ends with the comment, a blank line, and the (empty) include slot, then the functions
    CB_CHECK(files[0].text.find(" */\n\n\n/* VERIFIED */\n") != std::string::npos);
    // an empty module is the head alone
    CB_CHECK(mf::c({}, facts)[0].text.ends_with(" */\n\n\n"));
    // PowerPC names its compiler and interpreter
    facts.lane = mf::Lane::PowerPc;
    CB_CHECK(mf::c(parts, facts)[0].text.find("a PowerPC compiler") != std::string::npos);
    CB_CHECK(mf::c(parts, facts)[0].text.find("a PowerPC interpreter") != std::string::npos);
});

CB_TEST("translate.module_file.parts_declines_other_documents", [] {
    cb::Arena arena;
    // not a Module
    CB_CHECK_THROWS_AS(as::Declined, (void)mf::parts(mf_comment(arena, "x")).value());
    CB_CHECK_THROWS_AS(as::Declined, (void)mf::parts(nullptr).value());
    // a unit that is neither a function nor a comment
    const auto r = mf::parts(mf_module(arena, {mf_function(arena, "WEAK", "int f(void);"), ir::make(arena, ir::Empty{})}));
    CB_CHECK(!r.ok());
    CB_CHECK(std::string(r.error().what()).find("Empty") != std::string::npos);
    // a function with no status line, and none with no text
    ir::MethodDecl bare;
    const auto one = mf::parts(mf_module(arena, {ir::make(arena, std::move(bare))})).value();
    CB_CHECK(one.size() == 1 && one[0].status.empty() && one[0].text == std::string());
    CB_CHECK_EQ(mf::part_text(one[0]), std::string("/*  */\n\n"));
    // DIVERGES: a status that holds "*/" no longer ends its own comment (the Python wrote it as it was, leaving C text behind)
    CB_CHECK_EQ(mf::part_text({"a */ b", std::nullopt}), std::string("/* a * / b */\n"));
});

CB_TEST("translate.module_file.assembly", [] {
    cb::Arena arena;
    ir::MachineFragment source;
    source.name = "a";
    source.assembly = "mov ax, 1\nret";
    const auto units = mf::source_units(mf_module(arena, {ir::make(arena, std::move(source))}));
    CB_CHECK(units.ok());
    const auto text = mf::assembly_file(units.value(), mf_facts());
    CB_CHECK(text.ok());
    CB_CHECK_EQ(text.value(), std::string(R"(; lz32.dll -- reconstructed source.
;
; STATUS, generated 2026-11-11 by CodeBreaker (verifier settings ab12).
; 3 entry points or functions:
;   VERIFIED                         2   66.7%
;   WEAK                             1   33.3%
;   UNVERIFIED                       0    0.0%
;
; the judge said fine
;   00401000   sub_401000                   WEAK one input
;
mov ax, 1
ret)"));
    // a module with no source, and no judge's word, and no rows
    mf::Facts facts = mf_facts();
    facts.how.clear();
    facts.rows.clear();
    const auto none = mf::assembly_file({}, facts);
    CB_CHECK(none.value().ends_with(";\n; (no source could be written)\n"));
    CB_CHECK(none.value().find("the judge") == std::string::npos);
    // the judge's word is cut at 160 characters
    facts.how = std::string(200, 'w');
    CB_CHECK(mf::asm_banner(facts)[mf::asm_banner(facts).size() - 2] == std::string(160, 'w'));
});

CB_TEST("translate.module_file.assembly_units_and_declines", [] {
    cb::Arena arena;
    ir::MachineFragment outer;
    outer.name = "outer";
    outer.assembly = "a";
    ir::MachineFragment inside;
    inside.name = "inside";
    inside.within = "outer";   // its text is in the fragment that holds it
    const auto units = mf::source_units(mf_module(arena, {ir::make(arena, std::move(outer)), ir::make(arena, std::move(inside)),
                                                          mf_comment(arena, "c")}))
                           .value();
    CB_CHECK_EQ(units.size(), std::size_t{3});
    const auto text = mf::assembly_file(units, mf_facts()).value();
    CB_CHECK(text.ends_with("\na\n; c"));
    // not a Module; and a unit that is not assembly makes the assembly file decline
    CB_CHECK_THROWS_AS(as::Declined, (void)mf::source_units(mf_comment(arena, "x")).value());
    const auto other = mf::source_units(mf_module(arena, {ir::make(arena, ir::Empty{})})).value();
    CB_CHECK_THROWS_AS(as::Declined, (void)mf::assembly_file(other, mf_facts()).value());
    // a fragment with no text and nothing that holds it is a fault in the lane that built it
    ir::MachineFragment lost;
    lost.name = "lost";
    const auto bad = mf::source_units(mf_module(arena, {ir::make(arena, std::move(lost))})).value();
    CB_CHECK(!mf::assembly_file(bad, mf_facts()).ok());
});
