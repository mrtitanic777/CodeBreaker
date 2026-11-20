// analyze/origin_test.cpp -- was a 16-bit function written in assembly, or made by a C compiler? Hand-assembled functions (each one a
// thing C compilers of the period never emit, or the plain C frame), the summary and the report, and the verdicts on a real module.
//
// Notes:
//  - module16() is not cached: it is worked out every time.
//  - the verdict is a typed record (Language, vectors of evidence); the Result is ordered by address.
//  - a REP prefix is found among the instruction's leading prefix bytes.
//  - `main` is split: the report is `report()`, the command line is the command layer's.
#include "analyze/origin.hpp"

#include <algorithm>
#include <format>

#include "core/error.hpp"
#include "core/fs.hpp"
#include "core/testing.hpp"
#include "formats/ne.hpp"

using namespace cb;
using namespace cb::analyze::origin;
namespace dsc = cb::decompile::describe;
using OriginResult = cb::analyze::origin::Result;

namespace {

// One function at offset 0 of its own segment, judged.
Origin origin_judged(const Bytes& bytes, bool g3 = false) {
    const dsc::Code16 code(bytes);
    Origin o;
    o.name = "f";
    judge(o, code, 0, static_cast<std::int64_t>(bytes.size()), {0}, g3, {}, 0);
    return o;
}

bool origin_has(const Origin& o, std::string_view kind) {
    return std::any_of(o.evidence.begin(), o.evidence.end(), [&](const std::string& e) { return e.starts_with(kind); });
}

bool origin_has_text(const std::string& text, std::string_view part) { return text.find(part) != std::string::npos; }

}  // namespace

CB_TEST("analyze.origin.a_plain_c_function_is_c", [] {
    // push bp ; mov bp,sp ; mov ax,[bp+4] ; pop bp ; ret
    const Origin o = origin_judged({0x55, 0x8B, 0xEC, 0x8B, 0x46, 0x04, 0x5D, 0xC3});
    CB_CHECK(o.language == Language::C);
    CB_CHECK(o.evidence.empty());
    CB_CHECK_EQ(o.describe(), "<f c: nothing either way>");
});

CB_TEST("analyze.origin.nothing_but_a_return_is_assembly", [] {
    const Origin o = origin_judged({0xCB});   // retf
    CB_CHECK(o.language == Language::Assembly);
    CB_CHECK_EQ(o.evidence.size(), std::size_t{1});
    CB_CHECK_EQ(o.evidence[0], "does nothing but return (0000 retf)");
    CB_CHECK_EQ(o.describe(), "<f assembly: does nothing but return (0000 retf)>");
});

CB_TEST("analyze.origin.what_c_never_emits", [] {
    struct Case {
        Bytes code;
        const char* evidence;   // the start of the first piece of evidence
    };
    const std::vector<Case> cases = {
        {{0x55, 0x8B, 0xEC, 0xE2, 0xFE, 0x5D, 0xC3}, "LOOP ("},                       // loop $
        {{0x55, 0x8B, 0xEC, 0xF9, 0x5D, 0xC3}, "STC: a result in the carry flag ("},   // stc
        {{0x55, 0x8B, 0xEC, 0xCD, 0x21, 0x5D, 0xC3}, "INT 0x21 ("},                   // int 21h
        {{0x55, 0x8B, 0xEC, 0xFA, 0x5D, 0xC3}, "CLI ("},                               // cli
        {{0x55, 0x8B, 0xEC, 0xAC, 0x5D, 0xC3}, "LODSB without REP ("},                 // lodsb alone
        {{0x55, 0x8B, 0xEC, 0x05, 0x01, 0x00, 0x5D, 0xC3}, "argument in AX ("},        // add ax,1 : AX read before it is written
        {{0x55, 0x8B, 0xEC, 0x66, 0xB8, 0x01, 0x00, 0x00, 0x00, 0x5D, 0xC3}, "32-bit register ("},   // mov eax,1
    };
    for (const Case& c : cases) {
        const Origin o = origin_judged(c.code);
        CB_CHECK(o.language == Language::Assembly);
        CB_CHECK(!o.evidence.empty());
        if (!o.evidence.empty()) CB_CHECK(o.evidence[0].starts_with(c.evidence));
    }
    // a REP string move is what C makes of memcpy: SI, DI and CX set up and saved, the flags set before the string instruction reads them
    //   push bp ; mov bp,sp ; push si ; push di ; mov si,[bp+4] ; mov di,[bp+6] ; mov cx,4 ; shr cx,1 ; rep movsw ; pop di ; pop si ; pop bp ; ret
    const Origin rep = origin_judged({0x55, 0x8B, 0xEC, 0x56, 0x57, 0x8B, 0x76, 0x04, 0x8B, 0x7E, 0x06, 0xB9, 0x04, 0x00, 0xD1, 0xE9, 0xF3, 0xA5, 0x5F, 0x5E, 0x5D, 0xC3});
    CB_CHECK_EQ(rep.describe(), "<f c: nothing either way>");
    // the same without the REP is the lone string instruction of hand-written code
    const Origin lone = origin_judged({0x55, 0x8B, 0xEC, 0x56, 0x57, 0x8B, 0x76, 0x04, 0x8B, 0x7E, 0x06, 0xB9, 0x04, 0x00, 0xD1, 0xE9, 0xAD, 0x5F, 0x5E, 0x5D, 0xC3});
    CB_CHECK_EQ(lone.language == Language::Assembly, true);
    CB_CHECK_EQ(lone.evidence.at(0), "LODSW without REP (0010)");
    // the 32-bit register is no evidence in a module their linker made (C 7/8 under /G3), where it only addresses memory
    const Origin g3 = origin_judged({0x55, 0x8B, 0xEC, 0x66, 0xB8, 0x01, 0x00, 0x00, 0x00, 0x5D, 0xC3}, true);
    CB_CHECK(!origin_has(g3, "32-bit register"));
});

CB_TEST("analyze.origin.a_call_passed_on_and_a_jump_stub_are_assembly", [] {
    const Origin pass = origin_judged({0xE8, 0x10, 0x00, 0xC3});   // call near ; ret
    CB_CHECK(pass.language == Language::Assembly);
    CB_CHECK(origin_has(pass, "a call passed on (0000 call"));
    const Origin stub = origin_judged({0xE9, 0x10, 0x00});   // jmp near
    CB_CHECK(stub.language == Language::Assembly);
    CB_CHECK(origin_has(stub, "a jump stub (0000 jmp"));
    const Origin pop = origin_judged({0x59, 0x58, 0x50, 0x51, 0xCB});   // pop cx ; pop ax ; push ax ; push cx ; retf: takes its own return address
    CB_CHECK(pop.language == Language::Assembly);
    CB_CHECK(origin_has(pop, "pops its own return address (0000 pop cx)"));
});

CB_TEST("analyze.origin.the_summary_counts_languages_and_the_first_reason_of_each", [] {
    OriginResult r;
    auto add = [&](std::int64_t va, const char* name, Language l, std::vector<std::string> ev) {
        Origin o;
        o.va = va;
        o.name = name;
        o.language = l;
        o.evidence = std::move(ev);
        r.functions.emplace(va, std::move(o));
    };
    add(1, "a", Language::C, {});
    add(2, "b", Language::Assembly, {"LOOP (0003)", "CLI (0004)"});
    add(3, "c", Language::Assembly, {"CLI (0001)"});
    add(4, "d", Language::Assembly, {"STC: a result in the carry flag (0002)"});
    add(5, "e", Language::Assembly, {"CLI (0009)"});
    add(6, "f", Language::Assembly, {"LOOP (0009)"});
    const Summary s = r.summary();
    CB_CHECK_EQ(s.c, std::size_t{1});
    CB_CHECK_EQ(s.assembly, std::size_t{5});
    // CLI twice, LOOP twice (b and f: the first piece of each), STC once; ties in the order first met (LOOP before CLI)
    CB_CHECK_EQ(s.reasons.size(), std::size_t{3});
    CB_CHECK_EQ(s.reasons[0].first, "LOOP");
    CB_CHECK_EQ(s.reasons[0].second, std::size_t{2});
    CB_CHECK_EQ(s.reasons[1].first, "CLI");
    CB_CHECK_EQ(s.reasons[1].second, std::size_t{2});
    CB_CHECK_EQ(s.reasons[2].first, "STC: a result in the carry flag");
    // the report: a line of totals, the reasons, and with `list` a line for each function
    const std::string text = report("m.dll", r, false);
    CB_CHECK(text.starts_with("m.dll: 6 functions -- 1 C, 5 assembly\n"));
    CB_CHECK(origin_has_text(text, "     2  LOOP\n"));
    CB_CHECK(!origin_has_text(text, "  a  "));
    const std::string listed = report("m.dll", r, true);
    CB_CHECK(origin_has_text(listed, "  b                            assembly  LOOP (0003); CLI (0004)\n"));
    CB_CHECK(origin_has_text(listed, "  a                            c         \n"));
    // a C function with a compiler's fingerprint lists it
    OriginResult c;
    Origin o;
    o.va = 9;
    o.name = "h";
    o.compiler = {"calls __aFldiv", "_fastcall (arguments in AX)"};
    c.functions.emplace(9, o);
    CB_CHECK(origin_has_text(report("x", c, true), "  h                            c         calls __aFldiv; _fastcall (arguments in AX)\n"));
    CB_CHECK_EQ(o.describe(), "<h c: calls __aFldiv; _fastcall (arguments in AX)>");
});

CB_TEST("analyze.origin.verdicts_on_a_real_module", [] {
    const std::filesystem::path path = testing::data_path("formats/testdata/lzexpand.dll");
    fs::ScopedTempDir dir("cb-origin");
    dsc::NoKnowledge knowledge;
    const formats::ne::Module module = formats::ne::Module::open_file(path);
    const OriginResult r = module16(module, dir.path(), knowledge);
    CB_CHECK(!r.functions.empty());
    const Summary s = r.summary();
    CB_CHECK_EQ(s.c + s.assembly, r.functions.size());
    std::size_t reasons = 0;
    for (const auto& [kind, n] : s.reasons) reasons += n;
    CB_CHECK_EQ(reasons, s.assembly);   // every assembly function says why (the first piece of its evidence)
    for (const auto& [va, o] : r.functions) {
        CB_CHECK_EQ(o.va, va);
        CB_CHECK(!o.name.empty());
        // assembly says why; C says nothing against itself
        CB_CHECK_EQ(o.language == Language::Assembly, !o.evidence.empty());
    }
    // reviewed by hand against the disassembly: the DOS-facing routines of segment 11 are assembly (INT 21h, JCXZ, jumps into shared tails,
    // their own code segment read), the rest of LZEXPAND is C
    CB_CHECK_EQ(r.functions.size(), std::size_t{46});
    CB_CHECK_EQ(s.c, std::size_t{33});
    CB_CHECK_EQ(s.assembly, std::size_t{13});
    CB_CHECK_EQ(s.reasons.front().first, "reads its code segment");
    CB_CHECK_EQ(s.reasons.front().second, std::size_t{4});
    auto verdict = [&](std::string_view name) -> const Origin& {
        for (const auto& [va, o] : r.functions) {
            if (o.name == name) return o;
        }
        throw InternalError("no such function");
    };
    CB_CHECK(verdict("LZINIT").language == Language::C);
    CB_CHECK(verdict("LZCOPY").language == Language::C);
    CB_CHECK(verdict("sub_11_00D8").language == Language::Assembly);
    CB_CHECK_EQ(verdict("sub_11_00D8").evidence.at(1), "INT 0x21 (00f9)");
    CB_CHECK_EQ(verdict("sub_11_02E2").evidence.at(0), "LODSW without REP (02f6)");
    CB_CHECK_EQ(verdict("sub_5_0052").describe(), "<sub_5_0052 assembly: argument in SI (00ac)>");
    // asked again, the same
    const OriginResult again = module16(module, dir.path(), knowledge);
    CB_CHECK_EQ(report("lzexpand.dll", r, true), report("lzexpand.dll", again, true));
    // functions found besides the module's own tables are judged too
    std::map<std::int64_t, std::string> more;
    const std::int64_t first = r.functions.begin()->first;
    more[first] = "renamed";   // already there: not replaced
    CB_CHECK_EQ(module16(module, dir.path(), knowledge, &more).functions.at(first).name, r.functions.at(first).name);
    CB_CHECK(report("lzexpand.dll", r, false).starts_with(std::format("lzexpand.dll: {} functions -- {} C, {} assembly\n", r.functions.size(), s.c, s.assembly)));
});

// Calls to compiler helpers: near and far calls to the C compiler's own helpers (named)
// are noted in the function's compiler notes; a call to any other function is not.
CB_TEST("analyze.origin.a_call_to_a_compiler_helper_is_noted_near_or_far", [] {
    const std::int64_t base = dsc::address(1, 0);
    Bytes code = {0x55, 0x8B, 0xEC};                                                   // push bp ; mov bp,sp
    auto le16 = [&](std::int64_t v) {
        code.push_back(static_cast<std::uint8_t>(v & 0xFF));
        code.push_back(static_cast<std::uint8_t>((v >> 8) & 0xFF));
    };
    code.push_back(0xE8);                                                              // call near 0x20
    le16(0x20 - static_cast<std::int64_t>(code.size() + 2));
    for (const std::int64_t target : {0x30, 0x40}) {                                   // call far selector:0x30 / :0x40 (not a helper)
        code.push_back(0x9A);
        le16(target);
        le16(base >> 16);
    }
    code.push_back(0x5D);
    code.push_back(0xCB);                                                              // pop bp ; retf
    code.resize(0x20, 0);
    code.push_back(0xCB);
    code.resize(code.size() + 0x0F, 0);
    code.push_back(0xCB);
    code.resize(code.size() + 0x20, 0);
    const std::map<std::int64_t, std::string> names = {{base, "caller"}, {base + 0x20, "__aNlmul"}, {base + 0x30, "__aFuldiv"}, {base + 0x40, "other"}};
    const dsc::Code16 segment(code);
    Origin o;
    o.name = "caller";
    judge(o, segment, 0, 0x20, {0, 0x20, 0x30, 0x40}, false, names, base);
    CB_CHECK((o.compiler == std::vector<std::string>{"calls __aNlmul", "calls __aFuldiv"}));
});
