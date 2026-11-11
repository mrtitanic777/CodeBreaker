#include "cb/exact/draft/package.hpp"

#include "cb/core/fs.hpp"
#include "cb/core/log.hpp"
#include "cb/core/testing.hpp"
#include "cb/core/text.hpp"

using namespace cb;
using namespace cb::exact;
using namespace cb::exact::draft;

// DIVERGES from the Python stage (draft/__init__.py), on purpose:
//   * the model's mutable switches (FPCASTS, UNSIGNED_CAST, SWITCH, flow16.CMPSWITCH) are arguments of each call
//     (`Reading`, `Structuring`), so no call can leak a switch into the next; what the Python reset in `finally` is
//     simply not set
//   * FRAME, FPINFO and LONGINFO, the module tables the later passes read, are fields of `ModelTexts`
//   * a decoding error of the model is any std::exception at the function boundary, its text the first line of the
//     message (cut at 100 bytes, not 100 code points)

namespace {

Bytes hex(std::string_view h) {
    Bytes out;
    for (std::size_t i = 0; i + 1 < h.size(); i += 2) out.push_back(static_cast<std::uint8_t>(std::stoi(std::string(h.substr(i, 2)), nullptr, 16)));
    return out;
}

using Strings = std::vector<std::string>;

// A model that answers by a script and records every call and what it was told.
class MockModel final : public InverseModel {
public:
    ModelCapabilities caps;
    ModelLast last_;
    Strings calls;
    bool decline_emulated = false, fail_structured = false, fail_again = false;

    ModelCapabilities capabilities() const override { return caps; }
    Recovery recover(ByteView code, const Reading& r) override {
        calls.push_back(std::format("recover len={} frame={:d} regv={:d}", code.size(), r.frame_after_bp, r.regvars_eager));
        if (!code.empty() && code[0] == 0xEE) throw FormatError("mock", 0, "the model declines this function");
        if (decline_emulated && std::ranges::find(code, std::uint8_t{0x9B}) != code.end()) throw FormatError("mock", 0, "the model declines the emulated code");
        Recovery rec;
        rec.frame_size = static_cast<std::uint32_t>(code.size() % 7 + 2);
        rec.params = {"b"};
        rec.stmts = {"ax = a;", std::format("v = {};", code.size()), "return v;"};
        return rec;
    }
    Recovery recover_structured(ByteView code, const Reading&, const Structuring& how) override {
        calls.push_back(std::format("structured switch={} cmp={:d} fpc={:d} uns={:d}", how.table_switch ? "set" : "none", how.compare_switches, how.fp_casts, how.unsigned_compares));
        if (fail_structured && !how.fp_casts && !how.unsigned_compares) throw FormatError("mock", 0, "no structured reading");
        if (fail_again && (how.fp_casts || how.unsigned_compares)) throw FormatError("mock", 0, "no second reading");
        Recovery rec;
        rec.frame_size = static_cast<std::uint32_t>(code.size() % 5 + 1);
        rec.params = {"c", "b"};
        rec.stmts = {"/* register variables: si */", "ax = a;", "if ((v >= 1)) {", "if ((v >= 2)) {", "goto L_1;", "}", "}", "L_1:", std::format("w = {};", code.size())};
        if (how.fp_casts) rec.stmts.push_back("fp = 1;");
        if (how.unsigned_compares) rec.stmts.push_back("u = 1;");
        rec.stmts.push_back("}");
        return rec;
    }
    std::string to_c(std::string_view name, const Recovery& r) override {
        std::string out = std::format("int {}({}) {{\n", name, cb::text::join(r.params, ", "));
        for (const std::string& s : r.stmts) out += "  " + s + "\n";
        return out + "}";
    }
    Strings fold_ladders(const Strings& stmts) override {
        calls.push_back(std::format("fold n={}", stmts.size()));
        Strings out;
        for (const std::string& s : stmts) out.push_back(cb::text::replace_all(s, "v >= 2", "v >= 3"));
        return out;
    }
    ModelLast last() const override { return last_; }
};

ModelCapabilities all_capabilities() {
    ModelCapabilities c;
    c.frame_after_bp = c.regvars_eager = c.structured = c.table_switch = c.fp_casts = c.unsigned_compares = c.fold_ladders = true;
    return c;
}

}  // namespace

CB_TEST("exact.draft.package.fp_native_writes_the_8087s_own_instructions", [] {
    // INT 35h (ESC D9h), INT 3Dh (fwait), INT 3Ch (ESC with a segment: es here) each as long as it was, a wait first
    CB_CHECK((fp_native(hex("558bec" "cd3546fc" "5dc3")) == hex("558bec9bd946fc5dc3")));
    CB_CHECK((fp_native(hex("cd3d")) == hex("9b90")));
    CB_CHECK((fp_native(hex("cd3c0006341290")) == hex("9b26d8" "06341290")));
    CB_CHECK((fp_native(hex("cd3cc506341290")) == hex("9b3edd" "06341290")));   // ds, ESC DDh
    const Bytes plain = hex("558bec33c05dc3");
    CB_CHECK((fp_native(plain) == plain));
});

CB_TEST("exact.draft.package.inline_goto_arms", [] {
    const Strings lines = {"if ((a)) {", "    goto L_1;", "}", "x = 0;", "L_1:", "y = 2;", "z = 3;"};
    // the arm that is only a goto takes the statements from the label to the end; the label goes when nothing wants it
    CB_CHECK((inline_goto_arms(lines) == Strings{"if ((a)) {", "    y = 2;", "    z = 3;", "}", "x = 0;", "y = 2;", "z = 3;"}));
    // a label another goto still wants stays
    Strings wanted = lines;
    wanted.insert(wanted.begin() + 3, "goto L_1;");
    const Strings kept = inline_goto_arms(wanted);
    CB_CHECK(std::ranges::find(kept, "L_1:") != kept.end());
    // nothing to do
    const Strings nothing = {"x = 1;", "goto L_1;", "L_1:", "return;"};
    CB_CHECK((inline_goto_arms(nothing) == nothing));
});

CB_TEST("exact.draft.package.table_hook", [] {
    // cmp ax, 3; jmp cs:[bx + 0x10]; nops; a table of 4 places at 0x10
    Bytes code = hex("83f803" "2effa71000");
    code.insert(code.end(), 8, 0x90);
    for (int w : {8, 9, 10, 11}) {
        code.push_back(static_cast<std::uint8_t>(w));
        code.push_back(0);
    }
    const TableHook hook(code, 0);
    const auto places = hook(3);
    CB_CHECK(places.has_value());
    CB_CHECK((*places == std::vector<std::int64_t>{8, 9, 10, 11}));
    CB_CHECK(!hook(1).has_value());   // not an instruction's address
    CB_CHECK(!hook(0).has_value());   // not a jump through a table
    // the function at image offset 0x100: its code says cs:[bx + 0x110] and the table's words are the program's
    Bytes at_100 = hex("83f803" "2effa71001");
    at_100.insert(at_100.end(), 8, 0x90);
    for (int w : {0x108, 0x109, 0x10A, 0x10B}) {
        at_100.push_back(static_cast<std::uint8_t>(w & 0xFF));
        at_100.push_back(static_cast<std::uint8_t>(w >> 8));
    }
    const auto shifted = TableHook(at_100, 0x100)(3);
    CB_CHECK(shifted.has_value());
    CB_CHECK((*shifted == std::vector<std::int64_t>{8, 9, 10, 11}));
    // a table that runs past the code, and no compare before the jump
    CB_CHECK(!TableHook(hex("83f8ff" "2effa71000"), 0)(3).has_value());
    CB_CHECK(!TableHook(hex("90" "2effa71000"), 0)(1).has_value());
});

CB_TEST("exact.draft.package.model_texts_a_linear_walk", [] {
    MockModel m;
    const Bytes code = hex("558bec33c05dc3");
    const ModelTexts t = model_texts(m, code, "f00020");
    CB_CHECK_EQ(t.texts.size(), 1u);
    CB_CHECK_EQ(t.frame_size, 2u);
    CB_CHECK_EQ(t.texts[0].text, "int f00020(b) {\n  ax = a;\n  v = 7;\n  return v;\n}");   // (no register parameters known)
    CB_CHECK_EQ(m.calls.size(), 1u);
    CB_CHECK(!t.fp.fp);
    CB_CHECK(t.longs.ret.has_value() && !*t.longs.ret);   // the code ends without dx given
});

CB_TEST("exact.draft.package.model_texts_each_structurer_and_what_it_was_told", [] {
    MockModel m;
    m.caps = all_capabilities();
    m.last_.regparam.insert_or_assign("ax", 0);
    m.last_.unsigned_longs = {"a"};
    const Bytes code = hex("558bec33c05dc3");
    const ModelTexts t = model_texts(m, code, "f00020", 0x20u);
    // the linear text, the structured one, and the one with the ladder folded (the compare ladder is in the text)
    CB_CHECK_EQ(t.texts.size(), 3u);
    CB_CHECK_EQ(m.calls.size(), 4u);
    CB_CHECK_EQ(m.calls[0], "recover len=7 frame=1 regv=1");
    CB_CHECK_EQ(m.calls[1], "structured switch=set cmp=1 fpc=0 uns=0");   // the table switch only the first structured read has
    CB_CHECK_EQ(m.calls[2], "structured switch=none cmp=1 fpc=0 uns=1");
    CB_CHECK(m.calls[3].starts_with("fold n="));
    // the register parameter `ax = a;` is not a statement: gone from the first three statements and from the linear text
    CB_CHECK(t.texts[0].text.find("ax = a;") == std::string::npos);
    CB_CHECK(t.texts[1].text.find("ax = a;") == std::string::npos);
    CB_CHECK(t.texts[1].text.find("/* register variables") == std::string::npos);
    CB_CHECK_EQ(t.texts[1].text.substr(0, 17), "int f00020(a, b, ");   // the parameter it names is declared, the list sorted
    CB_CHECK(t.texts[2].text.find("v >= 3") != std::string::npos);    // folded
    CB_CHECK(t.texts[2].text.find("u = 1;") != std::string::npos);    // read with the unsigned compares marked
    CB_CHECK_EQ(t.texts[0].regparam.at("ax"), 0);
    CB_CHECK_EQ(t.frame_size, 2u);                                     // of the linear walk
    CB_CHECK(t.longs.unsigned_names.contains("a"));
});

CB_TEST("exact.draft.package.model_texts_calls", [] {
    MockModel m;
    m.caps = all_capabilities();
    m.last_.fptypes["p->x"] = "float";
    m.last_.fpret = "double";
    // a long returned: dx given before the way out; and 8087 code
    const Bytes code = hex("b80100" "99" "c3");
    ModelTexts t = model_texts(m, code, "f00030", 0x30u);
    const Strings expected = {"recover len=5 frame=1 regv=1", "structured switch=set cmp=1 fpc=0 uns=0", "structured switch=none cmp=1 fpc=1 uns=0"};
    // (the third structured call, folding, only when a ladder or an unsigned compare is in the text: the mock's text has one)
    CB_CHECK(m.calls.size() >= 3u);
    for (std::size_t i = 0; i < expected.size(); ++i) CB_CHECK_EQ(m.calls[i], expected[i]);
    CB_CHECK(t.longs.ret.has_value() && *t.longs.ret);
    CB_CHECK_EQ(t.fp.ret.value_or(""), "double");
    CB_CHECK_EQ(t.fp.types.at("p->x"), "float");
    CB_CHECK(!t.fp.fp);
    // the 8087's calls in the code: the model reads them rewritten, and `fp` says the code has them
    MockModel e;
    e.caps = all_capabilities();
    const ModelTexts emu = model_texts(e, hex("558bec" "cd35460" "4" "5dc3"), "f00040");
    CB_CHECK(emu.fp.fp);
    CB_CHECK_EQ(e.calls[0], "recover len=9 frame=1 regv=1");
    // declined for the rewritten code: tried again as it was
    MockModel d;
    d.decline_emulated = true;
    const ModelTexts retried = model_texts(d, hex("558bec" "cd35460" "4" "5dc3"), "f00040");
    CB_CHECK_EQ(d.calls.size(), 2u);
    CB_CHECK(retried.fp.fp);
    // declined as it is: the model's error goes on
    MockModel x;
    x.decline_emulated = true;
    CB_CHECK_THROWS_AS(FormatError, (void)model_texts(x, hex("9bd946fc"), "f00050"));
});

CB_TEST("exact.draft.package.a_failing_structurer_leaves_the_others", [] {
    MockModel m;
    m.caps = all_capabilities();
    m.fail_structured = true;
    const ModelTexts only_linear = model_texts(m, hex("558bec33c05dc3"), "f1", 0x20u);
    CB_CHECK_EQ(only_linear.texts.size(), 1u);
    CB_CHECK(only_linear.longs.ret.has_value());   // (what was read is still noted)
    MockModel n;
    n.caps = all_capabilities();
    n.fail_again = true;
    n.last_.fptypes["p->x"] = "float";
    const ModelTexts without_the_rest = model_texts(n, hex("558bec33c05dc3"), "f2", 0x20u);
    CB_CHECK_EQ(without_the_rest.texts.size(), 2u);   // the linear and the structured texts stand
});

namespace {

class MockProject final : public Project {
public:
    explicit MockProject(std::filesystem::path dir) : dir_(std::move(dir)) {
        image_ = Bytes(0x50, 0x90);
        image_[0x40] = 0xEE;   // the model declines the function at 0x40
        symbols_.insert_or_assign("_main", SymPlace{SymKind::Code, 0x20});
        symbols_.insert_or_assign("plain", SymPlace{SymKind::Code, 0x20});
        symbols_.insert_or_assign("_data", SymPlace{SymKind::Data, 0x40});
        symbols_.insert_or_assign("_far:x", SymPlace{SymKind::Code, 0x60});
        symbols_.insert_or_assign("__under", SymPlace{SymKind::Code, 0x80});
        symbols_.insert_or_assign("_x", SymPlace{SymKind::Code, 0xA0});
    }
    std::optional<std::string> model;
    std::set<std::uint32_t> have;
    std::shared_ptr<MockModel> loaded;
    std::string toolchain_name() const override { return "mscx"; }
    std::optional<std::string> inverse_model_name() const override { return model; }
    std::shared_ptr<InverseModel> load_inverse_model(std::string_view) override {
        loaded = std::make_shared<MockModel>();
        loaded->caps = all_capabilities();
        return loaded;
    }
    std::vector<LibraryPlace> library_places() const override {
        LibraryModule m1, m2, m3;
        m1.publics.insert_or_assign("_f1", std::make_pair(1u, 0x10u));
        m1.publics.insert_or_assign("g1", std::make_pair(2u, 0x11u));
        m1.publics.insert_or_assign("__f2", std::make_pair(1u, 0x12u));
        m1.first_code_segment = 1;
        m2.publics.insert_or_assign("_h", std::make_pair(3u, 0u));
        m2.publics.insert_or_assign("_f1", std::make_pair(3u, 4u));
        m2.first_code_segment = 3;
        m3.publics.insert_or_assign("lone", std::make_pair(5u, 2u));
        m3.first_code_segment = 5;
        LibraryPlace coded{0x100, m1, {m2}};
        LibraryPlace lone{0x200, m3, {}};
        return {coded, lone};
    }
    const SymbolTable& symbols() const override { return symbols_; }
    ByteView image() const override { return image_; }
    std::set<std::uint32_t> source_addresses() const override { return have; }
    std::filesystem::path path(std::string_view name) const override { return dir_ / std::filesystem::path(name); }
    std::vector<std::pair<std::uint32_t, std::uint32_t>> functions() const override { return {{0x10, 8}, {0x20, 8}, {0x30, 8}, {0x40, 8}}; }
    ProveOutcome prove_texts(InverseModel&, const ModelTexts& texts, std::uint32_t address, std::uint32_t, const std::map<std::uint32_t, std::string>& names) override {
        seen_names = names.size();
        seen_texts = texts.texts.size();
        if (address == 0x30) throw ToolError("compiler", 1, "the compiler fell over", "failed");
        return {std::format("src {:x}\n", address), address == 0x10 ? "identical" : "differs: size"};
    }
    std::size_t seen_names = 0, seen_texts = 0;

private:
    std::filesystem::path dir_;
    Bytes image_;
    SymbolTable symbols_;
};

}  // namespace

CB_TEST("exact.draft.package.names_of_the_code_the_program_has", [] {
    fs::ScopedTempDir dir("draft");
    const MockProject p(dir.path());
    const auto names = code_names(p);
    CB_CHECK((names == std::map<std::uint32_t, std::string>{{0x20, "main"}, {0x80, "under"}, {0xA0, "x"}, {0x100, "h"}, {0x104, "f1"},
                                                            {0x110, "f1"}, {0x112, "f2"}, {0x202, "lone"}}));
});

CB_TEST("exact.draft.package.the_stage", [] {
    fs::ScopedTempDir dir("draft");
    MockProject p(dir.path());
    p.model = "m1";
    p.have = {0x20};
    {
        testing::CapturedStd out;
        const Verdicts v = run(p, Options{.every = false, .limit = std::nullopt, .verbose = true});
        // 0x20 has a source already; 0x40 the model declines; 0x30 the prover fails on; 0x10 is identical
        CB_CHECK_EQ(v.size(), 3u);
        CB_CHECK_EQ(*v.find(0x10), "identical");
        CB_CHECK(v.find(0x30)->starts_with("failed: ToolError: "));
        CB_CHECK(v.find(0x40)->starts_with("declined: "));
        CB_CHECK_EQ(out.out(), "00010     8  identical\n");   // -v: a line for each function proved
        CB_CHECK_EQ(p.seen_names, 8u);
        CB_CHECK_EQ(p.seen_texts, 3u);   // linear, structured, folded
    }
    CB_CHECK_EQ(fs::read_text(dir.path() / "f00010.c"), "/* drafted by codebreaker exact draft (m1), identical */\nsrc 10\n");
    CB_CHECK(!std::filesystem::exists(dir.path() / "f00040.c"));
    // --all: the written ones too, and --limit
    {
        testing::CapturedStd out;
        const Verdicts all = run(p, Options{.every = true, .limit = 2});
        CB_CHECK_EQ(all.size(), 1u);   // the first two functions: 0x10 and 0x20 (which has a source: skipped)
    }
});

CB_TEST("exact.draft.package.the_stage_needs_a_model", [] {
    fs::ScopedTempDir dir("draft");
    MockProject p(dir.path());
    CB_CHECK_THROWS_AS(UsageError, (void)run(p, {}));
    p.model = "m1";
    testing::CapturedStd out;
    CB_CHECK_EQ(run_command(p, {}), 0);
    CB_CHECK_EQ(out.out(), "4 functions drafted: 1 identical, 1 differs, 1 failed, 1 declined\n");
});
