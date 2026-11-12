// cb-unity: alone
#include "cb/verify/authored/repair.hpp"

#include <algorithm>
#include <mutex>

#include "cb/core/testing.hpp"
#include "cb/core/text.hpp"

namespace {
namespace rp = cb::verify::authored::repair;
using cb::Bytes;
using cb::verify::ByteMatch;

std::string hex_of(const std::string& bytes) {
    std::string out;
    for (const char c : bytes) out += std::format("{:02x}", static_cast<unsigned>(static_cast<unsigned char>(c)));
    return out;
}

bool has(std::string_view hay, std::string_view needle) { return hay.find(needle) != std::string_view::npos; }

// A function as the decompiler writes it: K&R parameters, locals in a block, the result in one variable.
const char* const kDraft =
    "int4 __cdecl count(a, b)\n"
    "int4 a;\n"
    "int4 b;\n"
    "{\n"
    "    int4 i;\n"
    "    int4 total;\n"
    "    register int4 j;\n"
    "\n"
    "    total = 0;\n"
    "    for (i = 0; i < a; i++) {\n"
    "        total = total + b;\n"
    "    }\n"
    "    return total;\n"
    "}";

// A compiler that is not one: the bytes of a function are its text without spaces, newlines or a trailing "!" a switch
// set may put on; two texts that differ only in layout give the same bytes, and any other rewrite gives other ones.
rp::Code fake_code(std::string_view text, std::string_view flags = "/O2") {
    rp::Code c;
    for (const char ch : text)
        if (!cb::text::is_space(ch)) c.bytes.push_back(static_cast<std::uint8_t>(ch));
    if (flags == "/O1") c.bytes.push_back('!');
    return c;
}

rp::ByteVerdict fake_distance(const rp::Code& built, const rp::Code& original) {
    rp::ByteVerdict v;
    v.total = std::max(built.bytes.size(), original.bytes.size());
    if (built.bytes.size() != original.bytes.size()) {
        v.match = ByteMatch::Different;
        v.differing = v.total;   // the whole tail counts as differing
        return v;
    }
    for (std::size_t i = 0; i < built.bytes.size(); ++i) v.differing += built.bytes[i] != original.bytes[i];
    v.match = v.differing ? ByteMatch::SameSize : ByteMatch::Identical;
    return v;
}

std::vector<std::string> names_of(const std::vector<rp::Knob>& ks) {
    std::vector<std::string> out;
    for (const rp::Knob& k : ks) out.push_back(k.name);
    return out;
}

bool has_name(const std::vector<rp::Knob>& ks, std::string_view name) {
    return std::any_of(ks.begin(), ks.end(), [&](const rp::Knob& k) { return k.name == name; });
}

const rp::Knob& knob_named(const std::vector<rp::Knob>& ks, std::string_view name) {
    return *std::find_if(ks.begin(), ks.end(), [&](const rp::Knob& k) { return k.name == name; });
}
}  // namespace

CB_TEST("verify.repair.split", [] {
    const rp::Split s = rp::split(kDraft);
    CB_CHECK(s.head.ends_with("{"));
    CB_CHECK(s.head.starts_with("int4 __cdecl count(a, b)"));
    // the locals, not the K&R parameters (they are before the brace); a blank line does not end the run
    CB_CHECK_EQ(s.decls.size(), std::size_t{3});
    CB_CHECK_EQ(s.decls[2], std::string("    register int4 j;"));
    CB_CHECK(has(s.body, "total = 0;"));
    CB_CHECK(!has(s.body, "int4 i;"));
    // a source with no brace is all heading
    const rp::Split none = rp::split("int x;");
    CB_CHECK_EQ(none.head, std::string("int x;"));
    CB_CHECK(none.decls.empty());
    CB_CHECK(none.body.empty());
    // arrays and pointers are declarations; the run ends at the first statement
    const rp::Split arr = rp::split("f()\n{\n    char buf[16];\n    int4 **p;\n    x = 1;\n    int4 z;\n}");
    CB_CHECK_EQ(arr.decls.size(), std::size_t{2});
    CB_CHECK(arr.body.starts_with("    x = 1;"));
});

CB_TEST("verify.repair.toggle_register", [] {
    CB_CHECK_EQ(rp::toggle_register("    int4 i;").value(), std::string("    register int4 i;"));
    CB_CHECK_EQ(rp::toggle_register("    register int4 j;").value(), std::string("    int4 j;"));
    CB_CHECK(!rp::toggle_register("    char buf[16];"));          // an array is not kept in a register
    CB_CHECK(!rp::toggle_register("    static int4 n;"));
    CB_CHECK(!rp::toggle_register("    extern int4 n;"));
    CB_CHECK(!rp::toggle_register("    total = 0;"));             // not a declaration
    CB_CHECK(!rp::toggle_register("    register int;"));          // taking `register` off leaves nothing to declare
});

CB_TEST("verify.repair.test_swaps", [] {
    const std::string body = " if (a < b) { x = 1; }\n while (n != 0) { n = n - 1; }\n if (a < b && c) { }\n if (f(a) == 1) { }\n if (a == a) { }";
    const auto swaps = rp::test_swaps(body, body);
    // the compound test, the call and the test of a value with itself are left alone
    CB_CHECK_EQ(swaps.size(), std::size_t{2});
    CB_CHECK_EQ(swaps[0].text, std::string("b > a"));
    CB_CHECK_EQ(body.substr(swaps[0].begin, swaps[0].end - swaps[0].begin), std::string("a < b"));
    CB_CHECK_EQ(swaps[1].text, std::string("0 != n"));
});

CB_TEST("verify.repair.knobs", [] {
    const auto ks = rp::knobs(kDraft, 0, std::nullopt);
    // three locals: every pair changes places (order0..order2); the two that may take or lose `register`
    CB_CHECK(has_name(ks, "order0") && has_name(ks, "order1") && has_name(ks, "order2"));
    CB_CHECK(has_name(ks, "register0") && has_name(ks, "register1") && has_name(ks, "register2"));
    CB_CHECK(has_name(ks, "regparam1"));   // `int4 a;` of the K&R heading
    // `i < a` and `total + b` are pairs of operands that may change places (swap0, swap1); a for is no if or while
    CB_CHECK(has_name(ks, "swap0") && has_name(ks, "swap1") && !has_name(ks, "swap2"));
    CB_CHECK(!has_name(ks, "test0"));
    CB_CHECK(has_name(ks, "step0.1") && has_name(ks, "step0.2") && !has_name(ks, "step0.0"));
    CB_CHECK(has(knob_named(ks, "swap0").source, "a > i") && has(knob_named(ks, "swap1").source, "total = b + total;"));
    CB_CHECK(has(knob_named(ks, "step0.1").source, "++i)"));
    CB_CHECK(has(knob_named(ks, "step0.2").source, "i += 1)"));
    CB_CHECK(knob_named(ks, "order0").source.find("int4 total;") < knob_named(ks, "order0").source.find("int4 i;"));
    // a pad appears only when the original's frame is larger than the draft's
    CB_CHECK(!has_name(ks, "pad16"));
    const auto padded = rp::knobs(kDraft, 0x30, 0x20);
    CB_CHECK(has_name(padded, "pad16"));
    CB_CHECK(has(knob_named(padded, "pad16").source, "char codebreaker_unused[16];"));
    CB_CHECK(!has_name(rp::knobs(kDraft, 0x20, 0x30), "pad16"));
    // each of them keeps what the function does: only the order, spelling and storage of things move
    for (const rp::Knob& k : ks) CB_CHECK(has(k.source, "return total;"));
});

CB_TEST("verify.repair.knobs_frame", [] {
    const std::string src = "int4 f()\n{\n    struct { int4 a; uint1 _rest[24]; } _f;\n    return 0;\n}";
    // the draft's frame is 8 bytes larger than the original's: 8 bytes of room are taken out of the spare
    const auto ks = rp::knobs(src, 0x10, 0x18);
    CB_CHECK(has_name(ks, "frame16"));
    CB_CHECK(has(knob_named(ks, "frame16").source, "uint1 _rest[16]; } _f;"));
    // all the room taken: the member goes
    const auto gone = rp::knobs(src, 0x10, 0x28);
    CB_CHECK(has_name(gone, "frame0"));
    CB_CHECK(has(knob_named(gone, "frame0").source, " } _f;") && !has(knob_named(gone, "frame0").source, "_rest"));
    // more room taken than there is: nothing
    CB_CHECK(!has_name(rp::knobs(src, 0x10, 0x40), "frame0"));
});

CB_TEST("verify.repair.knobs_switch_return", [] {
    const std::string src =
        "int4 f(x)\nint4 x;\n{\n    int4 r;\n\n    if (x == 1) {\n        r = 10;\n    }\n    else if (x == 2) {\n        r = 20;\n    }\n"
        "    else {\n        r = 30;\n    }\n    return r;\n}";
    const auto ks = rp::knobs(src, 0, std::nullopt);
    CB_CHECK(has_name(ks, "switch0"));
    CB_CHECK(has(knob_named(ks, "switch0").source, "switch (x)"));
    CB_CHECK(has_name(ks, "return"));
    CB_CHECK(has(knob_named(ks, "return").source, "return 10;"));
});

CB_TEST("verify.repair.frame_x86", [] {
    // push ebp; mov ebp,esp; sub esp,0x24
    rp::Searcher x86(rp::Isa::X86_32, fake_distance);
    const Bytes prologue = {0x55, 0x8B, 0xEC, 0x83, 0xEC, 0x24, 0x53, 0xC3};
    CB_CHECK_EQ(x86.frame(prologue), std::size_t{0x24});
    const Bytes no_frame = {0x55, 0x8B, 0xEC, 0xC3};
    CB_CHECK_EQ(x86.frame(no_frame), std::size_t{0});
    const Bytes nothing;
    CB_CHECK_EQ(x86.frame(nothing), std::size_t{0});
});

CB_TEST("verify.repair.instructions_x86", [] {
    rp::Searcher x86(rp::Isa::X86_32, fake_distance);
    // each instruction is one entry; the bytes the linker fills in are blanked (the call's 4-byte distance)
    rp::Code c;
    c.bytes = {0x55, 0xE8, 0x11, 0x22, 0x33, 0x44, 0xC3};
    c.mask = {0, 0, 1, 1, 1, 1, 0};
    const auto insns = x86.instructions(c);
    CB_CHECK_EQ(insns.size(), std::size_t{3});
    CB_CHECK_EQ(hex_of(insns[1]), std::string("e800000000"));
    // two functions that differ in the filled-in places alone are alike
    rp::Code d = c;
    d.bytes[3] = 0x99;
    CB_CHECK(x86.instructions(d) == insns);
});

CB_TEST("verify.repair.frame_and_instructions_powerpc", [] {
    // stwu r1,-0x40(r1) is 0x9421FFC0
    rp::Searcher ppc(rp::Isa::PowerPC, fake_distance);
    const Bytes words = {0x7C, 0x08, 0x02, 0xA6, 0x94, 0x21, 0xFF, 0xC0};
    CB_CHECK_EQ(ppc.frame(words), std::size_t{0x40});
    const Bytes no_stwu = {0x7C, 0x08, 0x02, 0xA6};
    CB_CHECK_EQ(ppc.frame(no_stwu), std::size_t{0});
    // the words, with the masked bits cleared
    rp::Code w;
    w.bytes = {0x48, 0x00, 0x00, 0x05, 0x4E, 0x80, 0x00, 0x20};
    w.word_mask = {0x03FFFFFC, 0};
    const auto pw = ppc.instructions(w);
    CB_CHECK_EQ(pw.size(), std::size_t{2});
    CB_CHECK_EQ(hex_of(pw[0]), std::string("48000001"));
});

CB_TEST("verify.repair.score_lines_up_by_instruction", [] {
    rp::Searcher s(rp::Isa::X86_32, fake_distance);
    // the original: push ebx; xor eax,eax; pop ebx; ret.  The draft has one more instruction at the top: by bytes every
    // later byte moves, by instruction it is one away.
    rp::Code original{{0x53, 0x33, 0xC0, 0x5B, 0xC3}, {}, {}};
    rp::Code draft{{0x90, 0x53, 0x33, 0xC0, 0x5B, 0xC3}, {}, {}};
    const auto sc = s.score(draft, original);
    CB_CHECK_EQ(sc.rank.first, std::size_t{2});   // (longer list 5) - (4 matching) + 1
    CB_CHECK_EQ(sc.rank.second, std::size_t{6});  // the fake comparison says: different size, all bytes
    CB_CHECK(sc.verdict.match == ByteMatch::Different);
    // identical: rank (0, 0)
    const auto same = s.score(original, original);
    CB_CHECK((same.rank == std::pair<std::size_t, std::size_t>{0, 0}));
});

CB_TEST("verify.repair.repair_finds_the_authored_order", [] {
    // The authored source declares total before i and compares the other way round; the draft is as the decompiler wrote it.
    const std::string authored =
        "int4 __cdecl count(a, b)\nint4 a;\nint4 b;\n{\n    int4 total;\n    int4 i;\n    register int4 j;\n\n    total = 0;\n"
        "    for (i = 0; i < a; ++i) {\n        total = b + total;\n    }\n    return total;\n}";
    const rp::Code original = fake_code(authored);
    const rp::Compile compile = [](std::string_view text) { return std::optional<rp::Code>(fake_code(text)); };
    rp::Searcher searcher(rp::Isa::Unknown, fake_distance);
    const auto r = searcher.repair(compile, kDraft, original);
    CB_CHECK(r.has_value());
    CB_CHECK_EQ(r->verdict.differing, std::size_t{0});
    CB_CHECK(r->verdict.match == ByteMatch::Identical);
    CB_CHECK(r->source.find("int4 total;") < r->source.find("int4 i;"));
    CB_CHECK(has(r->source, "total = b + total;") && has(r->source, "++i)"));
    // three knobs, in the order they were found (the search goes on from each step)
    CB_CHECK_EQ(cb::text::split(r->knobs, ',').size(), std::size_t{3});
    CB_CHECK(r->tried > 3 && r->tried <= rp::kMaxWork);
    // a draft that already is the original is returned as it is, untried
    const auto same = searcher.repair(compile, authored, original);
    CB_CHECK(same && same->knobs.empty() && same->source == authored && same->tried == 1);
    // a draft that does not compile gives nothing
    CB_CHECK(!searcher.repair([](std::string_view) { return std::optional<rp::Code>(); }, kDraft, original));
});

CB_TEST("verify.repair.repair_stops_at_the_work_ceiling", [] {
    // an original no rewrite can reach: the search ends, on its patience or its ceiling, with the best it had
    const rp::Code original = fake_code("completely different");
    const rp::Compile compile = [](std::string_view text) { return std::optional<rp::Code>(fake_code(text)); };
    rp::Searcher searcher(rp::Isa::Unknown, fake_distance);
    const auto r = searcher.repair(compile, kDraft, original);
    CB_CHECK(r.has_value());
    CB_CHECK(r->verdict.differing != 0);
    CB_CHECK(r->tried <= rp::kMaxWork);
});

namespace {
// A compiler for run(): "/O2" gives the text's bytes, "/O1" gives them with a mark; PowerPC-shaped (words of 4 bytes).
class FakeCompiler : public rp::Compiler {
public:
    std::optional<rp::Code> compile(std::string_view, std::string_view source, std::string_view, std::string_view flags,
                                    const std::filesystem::path&) const override {
        std::lock_guard lock(mutex);
        ++compiles;
        if (has(source, "does-not-compile")) return std::nullopt;
        rp::Code c = fake_code(source, flags);
        while (c.bytes.size() % 4) c.bytes.push_back(0);
        return c;
    }
    rp::ByteVerdict distance(const rp::Code& built, const rp::Code& original) const override { return fake_distance(built, original); }
    std::vector<std::string> flag_sets() const override { return {"/O1", "/O2"}; }
    mutable std::mutex mutex;
    mutable int compiles = 0;
};

rp::Target target_of(std::string name, const std::string& authored, std::string text) {
    rp::Code original = fake_code(authored);
    while (original.bytes.size() % 4) original.bytes.push_back(0);
    return {name, "key:" + name, std::move(text), std::move(original)};
}
}  // namespace

CB_TEST("verify.repair.run_chooses_switches_and_records_variants", [] {
    const std::string authored_a = "int4 f()\n{\n    int4 a;\n    int4 b;\n    return a + b;\n}";
    const std::string draft_a = "int4 f()\n{\n    int4 b;\n    int4 a;\n    return a + b;\n}";   // locals the other way round
    const std::string authored_b = "int4 g()\n{\n    return 1;\n}";
    rp::Module m;
    m.isa = rp::Isa::PowerPC;
    m.header = "typedef int int4;";
    m.compiler_name = "fake";
    // g is first: the switches are chosen on a sample of the module (here its first function, already the authored text)
    m.targets = {target_of("g", authored_b, authored_b), target_of("f", authored_a, draft_a),
                 target_of("h", "int4 h()\n{\n    return 2;\n}", "does-not-compile")};
    FakeCompiler cc;
    struct Kept {
        std::string name, setting, source, knobs;
    };
    std::vector<Kept> kept;
    std::vector<std::string> lines;
    rp::RunOptions options;
    options.work = "unused";   // the fake compiler writes nothing
    options.record = [&](const rp::Target& t, const rp::Repaired& r, const std::string& setting) {
        kept.push_back({t.name, setting, r.source, r.knobs});
    };
    options.log = [&](std::string_view line) { lines.emplace_back(line); };
    const rp::Counts counts = rp::run(m, cc, options);
    // h does not compile: it is not counted, not recorded
    CB_CHECK((counts == rp::Counts{2, 2, 0, 0}));
    CB_CHECK_EQ(kept.size(), std::size_t{2});
    // the switches that fit best were found and said, and are the ones recorded
    CB_CHECK(!lines.empty() && lines[0] == "  compiler switches that fit best: /O2");
    CB_CHECK_EQ(kept[0].setting, std::string("fake /O2"));
    CB_CHECK_EQ(kept[0].name, std::string("g"));
    CB_CHECK_EQ(kept[0].knobs, std::string());
    CB_CHECK_EQ(kept[1].name, std::string("f"));
    CB_CHECK(kept[1].source.find("int4 a;") < kept[1].source.find("int4 b;"));
    CB_CHECK(kept[1].knobs.starts_with("order"));
    // the one function that needed knobs is said, with its distance
    CB_CHECK(lines.size() == 2 && has(lines[1], "identical") && has(lines[1], "via order0"));
    // `limit` stops after the first N; given switches are not searched for
    kept.clear();
    lines.clear();
    options.limit = 1;
    options.flags = "/O1";
    const rp::Counts one = rp::run(m, cc, options);
    CB_CHECK_EQ(one.functions, std::size_t{1});
    CB_CHECK_EQ(kept[0].setting, std::string("fake /O1"));
    // under /O1 every compile is one byte longer than the original's: none can be identical, and the search ends at its limits
    CB_CHECK_EQ(one.identical, std::size_t{0});
    CB_CHECK(lines.empty() || !has(lines[0], "fit best"));
});

CB_TEST("verify.repair.truth", [] {
    // units: authored sources; the search must bring back a source knocked away by a knob
    const std::string a = "int4 f()\n{\n    int4 a;\n    int4 b;\n    b = 1;\n    a = b + 2;\n    return a;\n}";
    const std::string b = "int4 g()\n{\n    return 1;\n}";   // nothing to knock: no counts
    rp::Profile profile;
    profile.isa = rp::Isa::Unknown;
    profile.distance = fake_distance;
    profile.units = [&] { return std::vector<rp::Unit>{{"f", a, fake_code(a)}, {"g", b, fake_code(b)}}; };
    profile.compile = [](std::string_view text, const std::filesystem::path&) { return std::optional<rp::Code>(fake_code(text)); };
    std::vector<std::string> lines;
    const rp::TruthCounts t = rp::truth(profile, "unused", 0, [&](std::string_view l) { lines.emplace_back(l); });
    CB_CHECK_EQ(t.units, std::size_t{2});
    CB_CHECK_EQ(t.identical, std::size_t{2});
    CB_CHECK(t.knocked > 0);
    // a knob with an inverse (order, register, swap, step) is brought back; `return a;` written `return b + 2;` has none
    CB_CHECK(t.brought_back >= 3 && t.brought_back <= t.knocked);
    CB_CHECK_EQ(lines.size(), t.knocked - t.brought_back);   // each that was not is said, with its distance
    // limit
    CB_CHECK_EQ(rp::truth(profile, "unused", 1).units, std::size_t{1});
});

CB_TEST("verify.repair.arguments", [] {
    const auto a = rp::parse_arguments({"setup.dll", "--flags", "/O2 /Gz", "--first", "5"});
    CB_CHECK(a.has_value());
    CB_CHECK_EQ(a->module, std::string("setup.dll"));
    CB_CHECK_EQ(a->flags.value(), std::string("/O2 /Gz"));
    CB_CHECK_EQ(a->first, std::size_t{5});
    CB_CHECK(!a->truth);
    const auto t = rp::parse_arguments({"--truth", "msc6", "ne"});
    CB_CHECK(t && t->truth && t->truth_args == std::vector<std::string>({"msc6", "ne"}));
    CB_CHECK(!rp::parse_arguments({}));                         // no module
    CB_CHECK(!rp::parse_arguments({"--truth"}));                // no compiler
    CB_CHECK(!rp::parse_arguments({"x.dll", "--first"}));       // an option with no value
    CB_CHECK(!rp::parse_arguments({"x.dll", "--first", "many"}));
    CB_CHECK_EQ(rp::parse_arguments({"--json", "out.json", "x.dll"})->module, std::string("x.dll"));
    CB_CHECK_EQ(rp::summary_line("x.dll", rp::Counts{3, 1, 1, 1}),
                std::string("x.dll: 3 functions repaired toward the original's bytes: 1 identical, 1 same size, 1 different"));
});
