#include "cb/exact/draft/prove.hpp"

#include <set>

#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"

namespace {

using namespace cb;
using namespace cb::exact::draft::prove;
using cb::exact::draft::candidates::Candidate;
using cb::exact::draft::candidates::RegisterParams;
namespace aim = cb::exact::aim;
namespace batch = cb::exact::batch;

// A world of sources the "compiler" understands: a source compiles to the object "OBJ:" + the source, unless it says ERR
// (then it stops at its second line with what the console says); a source is judged by the table of verdicts.
struct World {
    std::map<std::string, Verdict> verdicts;   // by source (absent: differs by 90 bytes)
    std::string error_message = "illegal indirection";
    int compiles = 0;
    int batches = 0;
    std::vector<std::string> compiled;

    Collaborators collaborators() {
        Collaborators c;
        c.compile = [this](std::string_view source) {
            ++compiles;
            compiled.emplace_back(source);
            batch::CompileOutcome out;
            if (source.find("ERR") != std::string_view::npos) {
                out.console = "t.c(2) : error C2100: " + error_message + "\n(the pass that stopped: C2.EXE)\n";
                return out;
            }
            out.object = to_bytes("OBJ:" + std::string(source));
            out.console = "";
            return out;
        };
        c.judge = [this](ByteView object, std::size_t) {
            const std::string source(as_text(object).substr(4));
            const auto it = verdicts.find(source);
            return it == verdicts.end() ? Verdict::differing(90) : it->second;
        };
        return c;
    }
};

}  // namespace

CB_TEST("exact.draft.prove.verdicts_read_the_way_the_ledger_wrote_them", [] {
    CB_CHECK_EQ(Verdict::same().text(), "identical");
    CB_CHECK_EQ(Verdict::differing(12).text(), "differs: 12 bytes");
    CB_CHECK_EQ(Verdict::differing(12, std::make_pair(30u, 32u)).text(), "differs: 12 bytes, length 30 not 32");
    CB_CHECK_EQ(Verdict::not_compiled("C2065: undefined").text(), "does not compile: C2065: undefined");
    CB_CHECK_EQ(Verdict::registers_left({"ax", "si"}).text(), "registers left: ax si");
    CB_CHECK_EQ(Verdict::no_function().text(), "no function compiled");
});

CB_TEST("exact.draft.prove.the_nearer_verdict_ranks_first", [] {
    const Rank right_length = rank(Verdict::differing(40));
    const Rank wrong_length = rank(Verdict::differing(2, std::make_pair(30u, 32u)));
    CB_CHECK(right_length < wrong_length);   // a length that is off is worse than any count of bytes at the right one
    CB_CHECK(rank(Verdict::differing(3)) < rank(Verdict::differing(4)));
    CB_CHECK(wrong_length < rank(Verdict::not_compiled("x")));
    CB_CHECK(rank(Verdict::not_compiled("x")) < rank(Verdict::registers_left({"ax"})));
    CB_CHECK_EQ(rank(Verdict::differing(2, std::make_pair(30u, 32u))).bytes, 4);   // (what is missing counts as differing)
});

CB_TEST("exact.draft.prove.a_reading_too_far_for_rewrites_is_hopeless", [] {
    CB_CHECK(!hopeless(Verdict::differing(80), 100));                              // bytes alone say little
    CB_CHECK(!hopeless(Verdict::differing(5, std::make_pair(110u, 100u)), 100));    // ten bytes off, 25 allowed
    CB_CHECK(hopeless(Verdict::differing(5, std::make_pair(140u, 100u)), 100));     // forty off
    CB_CHECK(!hopeless(Verdict::differing(5, std::make_pair(24u, 10u)), 10));       // a small function: sixteen bytes' room
    CB_CHECK(hopeless(Verdict::differing(5, std::make_pair(50u, 10u)), 10));
    CB_CHECK(!hopeless(Verdict::not_compiled("x"), 100));
    CB_CHECK(!hopeless(Verdict::differing(5, std::make_pair(500u, 1u)), 0));       // no length known
});

CB_TEST("exact.draft.prove.the_one_mending_a_cast_makes", [] {
    const std::string source = "int f()\n{\n  return p->f4 + q[1];\n}";
    const auto member = compile_fix(source, "t.c(3) : error C2100: left of '->f4' must have a struct/union type\n");
    CB_CHECK(member.has_value());
    CB_CHECK_EQ(*member, "int f()\n{\n  return *(int *)((char *)p + 4) + q[1];\n}");
    const auto subscript = compile_fix(source, "t.c(3) : error C2109: subscript on non-array\n");
    CB_CHECK_EQ(*subscript, "int f()\n{\n  return p->f4 + ((int *)q)[1];\n}");
    const auto star = compile_fix("int f()\n{\n  return *x;\n}", "t.c(3) : error C2100: illegal indirection\n");
    CB_CHECK_EQ(*star, "int f()\n{\n  return *(char *)x;\n}");
    // a warning is not an error; an error the code knows nothing of; a line past the end; no error at all
    CB_CHECK(!compile_fix(source, "t.c(3) : warning C4000: left of '->f4' must have a struct/union type\n").has_value());
    CB_CHECK(!compile_fix(source, "t.c(3) : error C2065: 'z' : undefined\n").has_value());
    CB_CHECK(!compile_fix(source, "t.c(30) : error C2109: subscript on non-array\n").has_value());
    CB_CHECK(!compile_fix(source, "compiled\n").has_value());
    // the member asked for is the one mended, not the first
    const auto second = compile_fix("a->f2 + b->f4;", "t.c(1) : error C2100: left of '->f4' must have a struct/union type\n");
    CB_CHECK_EQ(*second, "a->f2 + *(int *)((char *)b + 4);");
});

CB_TEST("exact.draft.prove.each_way_the_error_offers", [] {
    const std::string source = "int f(p)\nint p;\n{\n  return *p;\n}";
    const std::string console = "t.c(4) : error C2100: illegal indirection\n";
    const auto fixes = compile_fixes(source, console);
    bool cast = false;
    for (const std::string& f : fixes) cast |= f.find("return *(int *)p;") != std::string::npos;
    CB_CHECK(cast);   // an int stays an int and is read as a word through a cast
    CB_CHECK(fixes.back().find("*(char *)p") != std::string::npos);   // the cast of compile_fix comes last
    // a pointer the code reads a byte at a time is declared (char first), not cast as an int
    const auto bytes = compile_fixes(source, console, {"p"});
    for (const std::string& f : bytes) CB_CHECK(f.find("*(int *)p") == std::string::npos);
    // a char * local in arithmetic: an int, each local the line names
    const std::string arithmetic = "int f()\n{\n  char *a;\n  register char *b;\n  return a + b * 2;\n}";
    const auto ints = compile_fixes(arithmetic, "t.c(5) : error C2111: bad left operand\n");
    CB_CHECK_EQ(ints.size(), 2u);
    CB_CHECK(ints[0].find("  int a;") != std::string::npos);
    CB_CHECK(ints[1].find("  register int b;") != std::string::npos);
    CB_CHECK(compile_fixes(arithmetic, "t.c(5) : error C2065: nothing to do\n").empty());
    CB_CHECK(compile_fixes(arithmetic, "").empty());
});

CB_TEST("exact.draft.prove.the_compilers_view_of_a_candidate", [] {
    CB_CHECK_EQ(canon("a  =\t b;"), "a = b;");
    CB_CHECK_EQ(canon("x = ((a + b));"), "x = (a + b);");
    CB_CHECK_EQ(canon("x = (((a)));"), "x = (a);");
    CB_CHECK_EQ(canon("x = f((a), (b));"), "x = f((a), (b));");   // not a doubled pair
    CB_CHECK_EQ(canon("same"), canon("same"));
});

CB_TEST("exact.draft.prove.a_candidates_kind_is_its_declarations_and_whether_it_returns", [] {
    const std::string a = "int f(x)\nint x;\n{\n  int v1;\n  register int r_si;\n  return r_si;\n}";
    const std::string b = "int f(y)\nint y;\n{\n  int v7;\n  register int r_di;\n  return r_di;\n}";
    CB_CHECK(decl_sig(a) == decl_sig(b));   // names aside
    const std::string c = "int f(x)\nint x;\n{\n  char buf[10];\n  return 1;\n}";
    CB_CHECK(!(decl_sig(a) == decl_sig(c)));
    CB_CHECK(decl_sig(a).second);
    CB_CHECK(!decl_sig("void f()\n{\n  g();\n}").second);
});

CB_TEST("exact.draft.prove.the_prologue_says_whether_the_stack_probe_is_called", [] {
    const Bytes with_probe = {0x55, 0x8B, 0xEC, 0xB8, 0x04, 0x00, 0xE8, 0x00, 0x00, 0x8B, 0xE5, 0x5D, 0xC3};   // push bp; mov bp,sp; mov ax,4; call ..
    const Bytes without = {0x55, 0x8B, 0xEC, 0x83, 0xEC, 0x04, 0x8B, 0xE5, 0x5D, 0xC3};                       // push bp; mov bp,sp; sub sp,4
    const Bytes no_frame = {0x33, 0xC0, 0xC3};
    CB_CHECK(probes(with_probe) == std::optional<bool>(true));
    CB_CHECK(probes(without) == std::optional<bool>(false));
    CB_CHECK(!probes(no_frame).has_value());
});

CB_TEST("exact.draft.prove.the_switches_a_function_needs_are_its_first_line", [] {
    Settings s;
    s.switches = "-Oat -Gs";
    s.file_switches = "MFLAGS";
    s.fp_switch = "-FPi";
    s.probe_switch = "-Gs";
    CB_CHECK_EQ(with_switches("int f;", s), "int f;");   // nothing to change: the project's switches stand
    s.fp = true;
    CB_CHECK_EQ(with_switches("int f;", s), "/* MFLAGS: -Oat -Gs -FPi */\nint f;");
    s.fp = false;
    s.probes = true;   // the prologue calls the probe: the switch that leaves it out comes off
    CB_CHECK_EQ(with_switches("int f;", s), "/* MFLAGS: -Oat */\nint f;");
    s.probes = false;  // a frame without one: it stays (the project has it)
    CB_CHECK_EQ(with_switches("int f;", s), "int f;");
    s.switches = "-Oat";
    CB_CHECK_EQ(with_switches("int f;", s), "/* MFLAGS: -Oat -Gs */\nint f;");
    s.file_switches.reset();   // a toolchain with no file switches cannot take them
    CB_CHECK_EQ(with_switches("int f;", s), "int f;");
});

CB_TEST("exact.draft.prove.the_history_of_the_beam_is_kept_and_merged", [] {
    fs::ScopedTempDir dir("priors");
    const auto file = dir.path() / "exact" / "priors.json";
    Priors a(file), b(file);
    a.learn({"order", "shape"}, "adjacent_swaps");
    a.learn({}, "for_loops");   // no kind: shape
    CB_CHECK_EQ(a.wins("order", "adjacent_swaps"), 1);
    CB_CHECK_EQ(a.wins("shape", "for_loops"), 1);
    CB_CHECK_EQ(a.wins("width", "adjacent_swaps"), 0);
    b.learn({"order"}, "adjacent_swaps");   // another process, which had read the file before a's lessons
    CB_CHECK_EQ(b.wins("order", "adjacent_swaps"), 1);
    b.learn({"order"}, "adjacent_swaps");
    CB_CHECK_EQ(b.wins("order", "adjacent_swaps"), 2);   // merged: the larger of the two counts, then one more
    CB_CHECK_EQ(b.wins("shape", "for_loops"), 1);        // what a learnt is kept
    const Priors later(file);
    CB_CHECK_EQ(later.wins("order", "adjacent_swaps"), 2);
    CB_CHECK_EQ(later.wins("shape", "adjacent_swaps"), 1);
    // no file: in memory only; a damaged file: a fresh start
    Priors memory;
    memory.learn({"frame"}, "frame_more");
    CB_CHECK_EQ(memory.wins("frame", "frame_more"), 1);
    fs::write_text(file, "{not json");
    CB_CHECK_EQ(Priors(file).wins("order", "adjacent_swaps"), 0);
});

CB_TEST("exact.draft.prove.a_source_is_proved_where_it_compiles", [] {
    World w;
    w.verdicts["good"] = Verdict::same();
    Proof proof({}, w.collaborators(), nullptr, 10);
    const Outcome o = proof.prove("good");
    CB_CHECK(o.verdict.identical());
    CB_CHECK_EQ(o.source, "good");
    CB_CHECK_EQ(proof.spent(), 1);
    // one that differs: how
    w.verdicts["near"] = Verdict::differing(4);
    const Outcome n = proof.prove("near");
    CB_CHECK_EQ(n.verdict.text(), "differs: 4 bytes");
});

CB_TEST("exact.draft.prove.a_source_the_compiler_stops_at_is_mended_and_tried_again", [] {
    // line 2 holds an indirection through a plain int: the compiler stops; the mending (a cast in place) compiles and is the program's
    World w;
    w.verdicts["ok\n  return *(int *)p;\n"] = Verdict::same();
    auto c = w.collaborators();
    c.compile = [&w](std::string_view source) {
        ++w.compiles;
        batch::CompileOutcome out;
        if (source.find("*(int *)p") == std::string_view::npos) {
            out.console = "t.c(2) : error C2100: illegal indirection\n";
            return out;
        }
        out.object = to_bytes("OBJ:" + std::string(source));
        return out;
    };
    Proof proof({}, c, nullptr, 10);
    const Outcome o = proof.prove("ok\n  return *p;\n");
    CB_CHECK(o.verdict.identical());
    CB_CHECK_EQ(o.source, "ok\n  return *(int *)p;\n");   // the source the verdict is of: as the errors mended it
    CB_CHECK(w.compiles >= 2);
});

CB_TEST("exact.draft.prove.a_source_that_never_compiles_says_why", [] {
    World w;
    Proof proof({}, w.collaborators(), nullptr, 10);
    const Outcome o = proof.prove("ERR\nsecond line");
    CB_CHECK_EQ(o.verdict.kind, Verdict::Kind::DoesNotCompile);
    CB_CHECK(o.verdict.text().starts_with("does not compile: t.c(2) : error C2100: illegal indirection"));
    CB_CHECK(o.verdict.text().find("(the pass") == std::string::npos);   // not the note of which pass stopped
    CB_CHECK_EQ(o.source, "ERR\nsecond line");
    // the budget bounds the compiles
    World w2;
    w2.error_message = "illegal indirection";
    Proof limited({}, w2.collaborators(), nullptr, 10);
    (void)limited.prove("ERR\n *x;", 4, 2);
    CB_CHECK(w2.compiles <= 2);
});

CB_TEST("exact.draft.prove.the_nearest_that_compiles_is_the_answer", [] {
    World w;
    w.verdicts["a"] = Verdict::differing(20);
    w.verdicts["b"] = Verdict::differing(3);
    w.verdicts["c"] = Verdict::differing(1, std::make_pair(9u, 10u));
    Proof proof({}, w.collaborators(), nullptr, 10);
    auto c = w.collaborators();
    // three candidates through prove_best (no refiners: the nearest stands)
    c.candidates = [](const std::string&, const RegisterParams&) {
        return std::vector<Candidate>{{"a", {}}, {"b", {}}, {"c", {}}};
    };
    Settings s;
    s.batch = 0;
    Proof p(s, c, nullptr, 10);
    const Outcome o = p.prove_best("model text", {});
    CB_CHECK_EQ(o.source, "b");   // 3 bytes at the right length beats 1 byte at a wrong one
    CB_CHECK_EQ(o.verdict.text(), "differs: 3 bytes");
    (void)proof;
});

CB_TEST("exact.draft.prove.the_first_reading_that_is_identical_ends_the_search", [] {
    World w;
    w.verdicts["a"] = Verdict::differing(2);
    w.verdicts["b"] = Verdict::same();
    auto c = w.collaborators();
    c.candidates = [](const std::string&, const RegisterParams&) {
        return std::vector<Candidate>{{"a", {}}, {"b", {}}, {"c", {}}};
    };
    Settings s;
    s.batch = 0;
    Proof p(s, c, nullptr, 10);
    const Outcome o = p.prove_best("t", {});
    CB_CHECK(o.verdict.identical());
    CB_CHECK_EQ(o.source, "b");
    CB_CHECK(std::find(w.compiled.begin(), w.compiled.end(), "c") == w.compiled.end());   // "c" was never compiled
});

CB_TEST("exact.draft.prove.candidates_that_are_the_same_to_the_compiler_are_compiled_once", [] {
    World w;
    w.verdicts["x = (a);"] = Verdict::differing(5);
    auto c = w.collaborators();
    c.candidates = [](const std::string&, const RegisterParams&) {
        return std::vector<Candidate>{{"x = (a);", {}}, {"x  =  ((a));", {}}, {"x = (a);", {}}};
    };
    Settings s;
    s.batch = 0;
    Proof p(s, c, nullptr, 10);
    (void)p.prove_best("t", {});
    CB_CHECK_EQ(w.compiles, 1);
});

CB_TEST("exact.draft.prove.a_candidate_with_registers_left_is_never_compiled", [] {
    World w;
    w.verdicts["b"] = Verdict::differing(9);
    auto c = w.collaborators();
    c.candidates = [](const std::string&, const RegisterParams&) {
        return std::vector<Candidate>{{"a", {"ax", "si"}}, {"b", {}}};
    };
    Settings s;
    s.batch = 0;
    Proof p(s, c, nullptr, 10);
    const Outcome o = p.prove_best("t", {});
    CB_CHECK_EQ(o.source, "b");   // a compiled reading is nearer than one with registers left
    CB_CHECK(std::find(w.compiled.begin(), w.compiled.end(), "a") == w.compiled.end());
    // only candidates with registers left: the first is the answer, with what is left
    auto only = w.collaborators();
    only.candidates = [](const std::string&, const RegisterParams&) { return std::vector<Candidate>{{"a", {"ax"}}}; };
    Proof q(s, only, nullptr, 10);
    const Outcome r = q.prove_best("t", {});
    CB_CHECK_EQ(r.verdict.text(), "registers left: ax");
    // nothing at all: no function
    auto none = w.collaborators();
    none.candidates = [](const std::string&, const RegisterParams&) { return std::vector<Candidate>{}; };
    CB_CHECK_EQ(Proof(s, none, nullptr, 10).prove_best("t", {}).verdict.text(), "no function compiled");
});

CB_TEST("exact.draft.prove.the_nearest_reading_is_refined_by_the_rewrites", [] {
    World w;
    w.verdicts["x = a + b;"] = Verdict::differing(12);
    w.verdicts["x = b + a;"] = Verdict::differing(6);      // one rewrite brings it nearer ...
    w.verdicts["x = b + a; y = 1;"] = Verdict::same();     // ... and another finishes it
    auto c = w.collaborators();
    c.candidates = [](const std::string&, const RegisterParams&) { return std::vector<Candidate>{{"x = a + b;", {}}}; };
    c.refiners = {
        {"adjacent_swaps", [](const std::string& s) { return s == "x = a + b;" ? std::vector<std::string>{"x = b + a;"} : std::vector<std::string>{}; }},
        {"assign_in_args", [](const std::string& s) { return s == "x = b + a;" ? std::vector<std::string>{"x = b + a; y = 1;"} : std::vector<std::string>{}; }},
        {"nothing", [](const std::string&) { return std::vector<std::string>{}; }},
    };
    Settings s;
    s.batch = 0;
    s.aim = false;
    Priors priors;
    Proof p(s, c, &priors, 100);
    const Outcome o = p.prove_best("t", {});
    CB_CHECK(o.verdict.identical());
    CB_CHECK_EQ(o.source, "x = b + a; y = 1;");
});

CB_TEST("exact.draft.prove.a_reading_too_far_is_not_refined", [] {
    World w;
    w.verdicts["far"] = Verdict::differing(5, std::make_pair(10u, 100u));   // ninety bytes short of a hundred
    auto c = w.collaborators();
    c.candidates = [](const std::string&, const RegisterParams&) { return std::vector<Candidate>{{"far", {}}}; };
    int asked = 0;
    c.refiners = {{"adjacent_swaps", [&](const std::string&) {
                       ++asked;
                       return std::vector<std::string>{"never"};
                   }}};
    Settings s;
    s.batch = 0;
    Proof p(s, c, nullptr, 100);
    const Outcome o = p.prove_best("t", {});
    CB_CHECK_EQ(o.source, "far");
    CB_CHECK_EQ(asked, 0);
});

CB_TEST("exact.draft.prove.the_differences_order_the_rewrites_and_the_winner_is_remembered", [] {
    World w;
    w.verdicts["start"] = Verdict::differing(9);
    w.verdicts["by order"] = Verdict::same();
    auto c = w.collaborators();
    c.candidates = [](const std::string&, const RegisterParams&) { return std::vector<Candidate>{{"start", {}}}; };
    std::vector<std::string> asked;
    const auto make = [&](const std::string& name, const std::string& result) {
        return Refiner{name, [&asked, name, result](const std::string& s) {
                           asked.push_back(name);
                           return s == "start" ? std::vector<std::string>{result} : std::vector<std::string>{};
                       }};
    };
    // a refiner nothing asks for is listed first, the order's second; the diagnosis says "order": the one it asks for goes first
    c.refiners = {make("unrelated", "x"), make("adjacent_swaps", "by order")};
    c.diagnose = [](std::string_view) {
        aim::Diagnosis d;
        d.kinds = {{aim::Kind::Order, 3}};
        return std::optional<aim::Diagnosis>(d);
    };
    Priors priors;
    Settings s;
    s.batch = 0;
    Proof p(s, c, &priors, 100);
    const Outcome o = p.prove_best("t", {});
    CB_CHECK(o.verdict.identical());
    CB_CHECK_EQ(asked.front(), "adjacent_swaps");   // the refiners the difference asks for come before the rest
    CB_CHECK_EQ(priors.wins("order", "adjacent_swaps"), 1);   // and its win is learnt for the kind diagnosed
    // the next time, the wins order the refiners of one kind: swap what is listed first
    auto again = c;
    again.refiners = {make("empty_cases_first", "none"), make("adjacent_swaps", "by order")};
    asked.clear();
    Proof q(s, again, &priors, 100);
    (void)q.prove_best("t", {});
    CB_CHECK_EQ(asked.front(), "adjacent_swaps");   // (both asked for by `order`; the one that won before goes first)
});

CB_TEST("exact.draft.prove.candidates_go_through_the_compiler_together_when_it_can", [] {
    World w;
    w.verdicts["a"] = Verdict::differing(8);
    w.verdicts["b"] = Verdict::same();
    auto c = w.collaborators();
    c.candidates = [](const std::string&, const RegisterParams&) { return std::vector<Candidate>{{"a", {}}, {"b", {}}}; };
    int runs = 0;
    c.compile_many = [&](const std::vector<std::string>& sources) {
        ++runs;
        std::map<std::size_t, std::pair<Bytes, std::size_t>> out;
        for (std::size_t i = 0; i < sources.size(); ++i) out[i] = {to_bytes("OBJ:" + sources[i]), 0};
        return out;
    };
    Settings s;
    s.batch = 4;
    Proof p(s, c, nullptr, 10);
    const Outcome o = p.prove_best("t", {});
    CB_CHECK(o.verdict.identical());
    CB_CHECK_EQ(runs, 1);
    CB_CHECK_EQ(w.compiles, 0);   // one run of the compiler for both; neither alone
    CB_CHECK_EQ(p.spent(), 2);    // counted as two compiles
});

CB_TEST("exact.draft.prove.a_batch_the_compiler_refuses_leaves_each_to_be_proved_alone", [] {
    World w;
    w.verdicts["a"] = Verdict::differing(8);
    w.verdicts["b"] = Verdict::same();
    auto c = w.collaborators();
    c.candidates = [](const std::string&, const RegisterParams&) { return std::vector<Candidate>{{"a", {}}, {"b", {}}}; };
    c.compile_many = [](const std::vector<std::string>&) -> std::map<std::size_t, std::pair<Bytes, std::size_t>> {
        throw ToolError("cl", 1, "", "crashed");
    };
    Settings s;
    s.batch = 4;
    Proof p(s, c, nullptr, 10);
    CB_CHECK(p.prove_best("t", {}).verdict.identical());
    CB_CHECK_EQ(w.compiles, 2);
});

CB_TEST("exact.draft.prove.a_register_given_a_new_value_is_tried_as_a_local_too", [] {
    World w;
    std::vector<std::pair<std::string, std::vector<std::string>>> seen;   // (text, register names kept)
    auto c = w.collaborators();
    c.candidates = [&](const std::string& text, const RegisterParams& regparam) {
        std::vector<std::string> names;
        for (const auto& [reg, i] : regparam) names.push_back(reg);
        seen.emplace_back(text, names);
        return std::vector<Candidate>{{"c" + std::to_string(seen.size()), {}}};
    };
    Settings s;
    s.batch = 0;
    Proof p(s, c, nullptr, 10);
    RegisterParams regparam;
    regparam.insert_or_assign("si", 0);
    regparam.insert_or_assign("di", 1);
    (void)p.prove_texts({{"  si = f();\n  di = 2;\n  return si + di;\n", regparam}});
    // the text as it is, without si, without di, and without both
    CB_CHECK_EQ(seen.size(), 4u);
    CB_CHECK((seen[0].second == std::vector<std::string>{"si", "di"}));
    CB_CHECK((seen[1].second == std::vector<std::string>{"si"}));   // (the registers in name order: di given a value, then si)
    CB_CHECK((seen[2].second == std::vector<std::string>{"di"}));
    CB_CHECK(seen[3].second.empty());
    // a parameter register that is not reassigned: one text
    seen.clear();
    Proof q(s, c, nullptr, 10);
    (void)q.prove_texts({{"  return si + di;\n", regparam}});
    CB_CHECK_EQ(seen.size(), 1u);
});

CB_TEST("exact.draft.prove.of_the_texts_the_first_identical_or_else_the_nearest", [] {
    World w;
    w.verdicts["one"] = Verdict::differing(30);
    w.verdicts["two"] = Verdict::differing(4);
    w.verdicts["three"] = Verdict::same();
    auto c = w.collaborators();
    c.candidates = [](const std::string& text, const RegisterParams&) { return std::vector<Candidate>{{text, {}}}; };
    Settings s;
    s.batch = 0;
    Proof p(s, c, nullptr, 10);
    CB_CHECK_EQ(p.prove_texts({{"one", {}}, {"two", {}}}).source, "two");
    Proof q(s, c, nullptr, 10);
    const Outcome o = q.prove_texts({{"one", {}}, {"three", {}}, {"two", {}}});
    CB_CHECK(o.verdict.identical());
    CB_CHECK_EQ(o.source, "three");
    CB_CHECK(std::find(w.compiled.begin(), w.compiled.end(), "two") != w.compiled.end());   // compiled in the first call; not again in the second
});
