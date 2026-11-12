// Tests of the Java back end: the text helpers (names, literals, types), the pointer analysis, tier choice, and whole programs.
//
// The programs are C and C++ sources the clang front end lowered (testdata/java/*.c, *.cpp; the L2 documents beside them are its
// output, without columns). Each golden .java file is what codebreaker/translate/backends/java.py wrote for that document; it was
// read through, and the real judge (translate/judge_native_jvm: clang build of the original beside javac and the JVM) ran each
// program and found its exit code equal to the C build's: FaithfulCore, FaithfulArith (every integer width and sign, floats,
// conversions, compound assignment on locals and in memory), FaithfulVa, FaithfulOverloads. GapGoto's body reaches its gap on
// purpose. make_java_golden.py makes the files again.
//
// DIVERGES from java.py:
//   * a C name's non-ASCII bytes become `_` (the Python kept letters outside ASCII: Java would have taken them, C sources
//     here have none);
//   * a literal's digits are written by std::to_chars's shortest form, the same digits Python's repr gave;
//   * the library mapping (java_libc) and the idiomatic tier (java_objects) are the caller's: a program that calls the C library
//     with no mapping is refused for the call, and the idiomatic tier is a TierFactory (a fake one stands in below);
//   * fixed in this module and in java.py: a char constant of 127 (DEL) was written as repr's `'\x7f'`, which Java does not
//     read, and a character literal whose C spelling Java lacks ('\x41', '\a') was copied as it stood; `a ? f() : g();` as a
//     statement lowered its first branch twice (the discarded lines still took frame space and temporary names); a dereference
//     as an lvalue wrote its pointer expression twice (the second only for its precedence).
#include "cb/translate/backends/java.hpp"

#include <algorithm>
#include <chrono>
#include <cmath>
#include <iostream>
#include <limits>

#include <nlohmann/json.hpp>

#include "cb/core/fs.hpp"
#include "cb/core/json.hpp"
#include "cb/core/testing.hpp"
#include "cb/translate/l2.hpp"

namespace {

namespace java = cb::translate::backends::java;
namespace ir = cb::translate::ir;
namespace l2 = cb::translate::l2;
using cb::Arena;
using ir::Node;

// ---- the programs ---------------------------------------------------------------------------------------------------------

struct JvProgram {
    Arena arena;
    Node* unit = nullptr;
};

std::unique_ptr<JvProgram> jv_load(const std::string& name) {
    auto p = std::make_unique<JvProgram>();
    p->unit = l2::from_json(p->arena, cb::json::load(cb::testing::data_path("translate/testdata/java/" + name + ".l2.json")));
    return p;
}

std::string jv_golden(const std::string& file) { return cb::fs::read_text(cb::testing::data_path("translate/testdata/java/" + file)); }

std::vector<std::string> jv_lines(const std::string& s) {
    std::vector<std::string> out;
    std::size_t start = 0;
    while (start <= s.size()) {
        const std::size_t end = s.find('\n', start);
        out.push_back(s.substr(start, end == std::string::npos ? std::string::npos : end - start));
        if (end == std::string::npos) break;
        start = end + 1;
    }
    return out;
}

// Both texts equal; where they are not, the first line that differs, with its neighbours.
void jv_check_same_text(const std::string& actual, const std::string& expected, const std::string& what) {
    cb::testing::count_check();
    if (actual == expected) return;
    const auto a = jv_lines(actual);
    const auto e = jv_lines(expected);
    std::size_t i = 0;
    while (i < a.size() && i < e.size() && a[i] == e[i]) ++i;
    std::string msg = what + ": text differs at line " + std::to_string(i + 1) + " (actual " + std::to_string(a.size()) + " lines, expected " + std::to_string(e.size()) + ")\n";
    for (std::size_t k = i > 2 ? i - 2 : 0; k < std::min(i + 3, std::max(a.size(), e.size())); ++k) {
        msg += "  actual   " + (k < a.size() ? a[k] : std::string("<none>")) + "\n  expected " + (k < e.size() ? e[k] : std::string("<none>")) + "\n";
    }
    cb::testing::record_failure(__FILE__, __LINE__, msg);
}

java::Program jv_write(JvProgram& p, const std::string& name, java::Options options = {}) {
    Node* units[] = {p.unit};
    options.tier = java::Tier::Faithful;
    return java::program(units, name, options);
}

// The program's notes as the golden file lists them, one per line.
std::string jv_notes_text(const java::Program& prog) {
    std::string out;
    for (const std::string& n : prog.notes) out += n + "\n";
    return out;
}

// What the Python made of each program: its Java and its notes.
void jv_check_golden(const std::string& name) {
    auto p = jv_load(name);
    const java::Program prog = jv_write(*p, name);
    CB_CHECK(prog.tier == java::Tier::Faithful);
    jv_check_same_text(prog.source, jv_golden(name + ".faithful.java"), name + ".java");
    jv_check_same_text(jv_notes_text(prog), jv_golden(name + ".notes.txt"), name + " notes");
}

CB_TEST("translate.java.golden.faithful_core", [] { jv_check_golden("FaithfulCore"); });
CB_TEST("translate.java.golden.faithful_arith", [] { jv_check_golden("FaithfulArith"); });
CB_TEST("translate.java.golden.faithful_va", [] { jv_check_golden("FaithfulVa"); });
CB_TEST("translate.java.golden.faithful_overloads", [] { jv_check_golden("FaithfulOverloads"); });
CB_TEST("translate.java.golden.gap_goto", [] {
    jv_check_golden("GapGoto");
    // the gap is the function's own, listed; the rest of the program is written
    auto p = jv_load("GapGoto");
    const java::Program prog = jv_write(*p, "GapGoto");
    CB_CHECK(prog.source.find("throw new UnsupportedOperationException(\"CodeBreaker gap: tangle:") != std::string::npos);
    CB_CHECK(prog.source.find("static int plain(int a)") != std::string::npos);
    CB_CHECK_EQ(prog.notes.front(), "gap: tangle (goto mid both forward and backward in tangle)");
});

// ---- what the tiers do ------------------------------------------------------------------------------------------------------

CB_TEST("translate.java.analysis_reasons", [] {
    auto p = jv_load("FaithfulCore");
    Node* units[] = {p->unit};
    java::Analysis a(units);
    a.run();
    CB_CHECK(!a.cpp_classes);
    const auto has = [&](const std::string& why) { return std::find(a.why.begin(), a.why.end(), why) != a.why.end(); };
    CB_CHECK(has("union Pun"));
    CB_CHECK(has("pointer arithmetic"));
    CB_CHECK(has("a cast between pointer types"));
    // the scalars whose address is taken: `&x` and `&y` for swap, and the global `&counter`-style are named
    CB_CHECK(a.addressed.contains("x"));
    CB_CHECK(a.addressed.contains("y"));

    auto cls = jv_load("ClassPoint");
    Node* cunits[] = {cls->unit};
    java::Analysis ca(cunits);
    ca.run();
    CB_CHECK(ca.cpp_classes);
    CB_CHECK(ca.why.empty());
    // a variadic definition is a reason by itself
    auto va = jv_load("FaithfulVa");
    Node* vunits[] = {va->unit};
    java::Analysis vaa(vunits);
    vaa.run();
    CB_CHECK(std::find(vaa.why.begin(), vaa.why.end(), "a variadic function (total)") != vaa.why.end());
});

// A stand-in for java_objects: writes a fixed text, with the gaps and notes it is told.
class JvFakeIdiomatic : public java::TierWriter {
public:
    JvFakeIdiomatic(std::vector<std::string> gaps, bool refuse) : gaps_(std::move(gaps)), refuse_(refuse) {}
    std::string write() override {
        if (refuse_) throw java::Refused("the object model cannot carry this");
        return "// idiomatic\n";
    }
    const std::vector<std::string>& gaps() const override { return gaps_; }
    const std::vector<std::string>& notes() const override { return notes_; }

private:
    std::vector<std::string> gaps_;
    bool refuse_;
    std::vector<std::string> notes_ = {"made by the fake"};
};

java::TierFactory jv_fake(std::vector<std::string> gaps = {}, bool refuse = false) {
    return [gaps, refuse](std::span<Node* const>, std::string_view, const java::Analysis&) { return std::make_unique<JvFakeIdiomatic>(gaps, refuse); };
}

CB_TEST("translate.java.tier_choice", [] {
    // C++ with classes: the idiomatic tier or nothing
    auto cls = jv_load("ClassPoint");
    Node* cu[] = {cls->unit};
    CB_CHECK_THROWS_AS(java::Refused, java::program(cu, "ClassPoint", {.tier = java::Tier::Faithful}));
    CB_CHECK_THROWS_AS(java::Refused, java::program(cu, "ClassPoint", {}));   // no idiomatic tier given
    java::Program p = java::program(cu, "ClassPoint", {.idiomatic = jv_fake()});
    CB_CHECK(p.tier == java::Tier::Idiomatic);
    CB_CHECK_EQ(p.source, "// idiomatic\n");
    CB_CHECK_EQ(p.notes.size(), std::size_t{1});
    // classes keep the idiomatic tier even with gaps; a refusal from it is the program's
    CB_CHECK(java::program(cu, "ClassPoint", {.idiomatic = jv_fake({"Point::sum (x)"})}).tier == java::Tier::Idiomatic);
    CB_CHECK_THROWS_AS(java::Refused, java::program(cu, "ClassPoint", {.idiomatic = jv_fake({}, true)}));

    // a program that needs the object model's carrying capacity and has none is refused
    auto core = jv_load("FaithfulCore");
    Node* ku[] = {core->unit};
    CB_CHECK_THROWS_AS(java::Refused, java::program(ku, "FaithfulCore", {.tier = java::Tier::Idiomatic, .idiomatic = jv_fake()}));
    // ... while auto takes the faithful tier, and says why
    java::Program f = java::program(ku, "FaithfulCore", {.idiomatic = jv_fake()});
    CB_CHECK(f.tier == java::Tier::Faithful);
    CB_CHECK_EQ(f.notes.back(), "faithful tier: a cast between pointer types, pointer arithmetic, union Pun");

    // a program the analysis lets the idiomatic tier try: its gaps send it back to the faithful tier, its refusal too
    auto ov = jv_load("FaithfulOverloads");
    Node* ou[] = {ov->unit};
    CB_CHECK(java::program(ou, "FaithfulOverloads", {.idiomatic = jv_fake()}).tier == java::Tier::Idiomatic);
    java::Program gapped = java::program(ou, "FaithfulOverloads", {.idiomatic = jv_fake({"twice (a gap)"})});
    CB_CHECK(gapped.tier == java::Tier::Faithful);
    CB_CHECK_EQ(gapped.notes.back(), "faithful tier: the idiomatic tier declined (gaps in the idiomatic tier: twice (a gap))");
    java::Program refused = java::program(ou, "FaithfulOverloads", {.idiomatic = jv_fake({}, true)});
    CB_CHECK(refused.tier == java::Tier::Faithful);
    CB_CHECK_EQ(refused.notes.back(), "faithful tier: the idiomatic tier declined (the object model cannot carry this)");
    // no idiomatic tier at all: the faithful tier, as declined
    CB_CHECK_EQ(java::program(ou, "FaithfulOverloads", {}).notes.back(), "faithful tier: the idiomatic tier declined (the idiomatic tier is not available)");
});

// ---- the library mapping ----------------------------------------------------------------------------------------------------

// Writes `name(args)` for every library call, the arguments converted to the parameter's Java type, and records what it saw.
class JvStubLibrary : public java::LibraryCalls {
public:
    java::Ex call(java::FnWriter& writer, Node& call, std::string_view name, std::span<Node* const> args) const override {
        seen.push_back(std::string(name) + "/" + std::to_string(args.size()));
        std::string text = std::string(name) + "(";
        const ir::Call& c = *ir::get<ir::Call>(&call);
        for (std::size_t i = 0; i < args.size(); ++i) {
            const ir::TypeP pt = i < c.sym->params.size() ? c.sym->params[i] : args[i]->type;
            const ir::Cast* decayed = ir::get<ir::Cast>(java::strip(args[i]));
            const bool is_text = decayed != nullptr && ir::is<ir::Lit>(java::strip(decayed->x));
            text += (i != 0 ? ", " : "") + (is_text ? std::string("<string>") : writer.conv(writer.value(args[i]), args[i]->type, pt).code);
        }
        return {text + ")", java::kPost, writer.p.jt(call.type), false};
    }
    mutable std::vector<std::string> seen;
};

CB_TEST("translate.java.library_calls", [] {
    auto p = jv_load("CallsLib");
    // no mapping: the call is refused, and main is the program's, so the program is
    CB_CHECK_THROWS_AS(java::Refused, jv_write(*p, "CallsLib"));
    try {
        (void)jv_write(*p, "CallsLib");
    } catch (const java::Refused& r) {
        CB_CHECK(std::string(r.what()).find("library function puts") != std::string::npos);
    }
    JvStubLibrary library;
    const java::Program prog = jv_write(*p, "CallsLib", {.library = &library});
    CB_CHECK_EQ(library.seen.size(), std::size_t{2});
    CB_CHECK_EQ(library.seen[0], "puts/1");
    CB_CHECK_EQ(library.seen[1], "abs/1");
    CB_CHECK(prog.source.find("        puts(<string>);\n        return abs(-3);\n") != std::string::npos);
});

CB_TEST("translate.java.gap_for_a_refused_library_call", [] {
    // a library call in a function other than main gaps that function, not the program
    Arena arena;
    auto make = [&](std::vector<Node*> body, const std::string& name, ir::TypeP ret) {
        ir::MethodDecl m;
        m.sym = ir::make_sym(ir::SymKind::Function, name, std::nullopt, true);
        m.sym->ret = ret;
        m.name = name;
        m.kind = "function";
        m.ret = ret;
        m.body = ir::make(arena, ir::Block{.stmts = std::move(body)});
        return ir::make(arena, std::move(m));
    };
    ir::SymP lib = ir::make_sym(ir::SymKind::Function, "mystery", std::nullopt, true);
    lib->ret = ir::i32_type();
    Node* call = ir::make(arena, ir::Call{.sym = lib}, ir::i32_type());
    Node* helper = make({ir::make(arena, ir::Return{.expr = call})}, "helper", ir::i32_type());
    Node* zero = ir::make(arena, ir::Lit{.value = std::int64_t{0}}, ir::i32_type());
    Node* main_fn = make({ir::make(arena, ir::Return{.expr = zero})}, "main", ir::i32_type());
    ir::Unit u;
    u.types = {helper, main_fn};
    Node* unit = ir::make(arena, std::move(u));
    Node* units[] = {unit};
    const java::Program prog = java::program(units, "G", {.tier = java::Tier::Faithful});
    CB_CHECK(prog.source.find("static int helper() {\n        // library function mystery") != std::string::npos);
    CB_CHECK(prog.source.find("static int main() {\n        return 0;\n    }") != std::string::npos);
    CB_CHECK_EQ(prog.notes.front().substr(0, 12), "gap: helper ");
});

// ---- hand-built programs: the errors and the smallest outputs -------------------------------------------------------------------

struct JvBuild {
    Arena arena;
    Node* lit(std::int64_t v) { return ir::make(arena, ir::Lit{.value = v}, ir::i32_type()); }
    Node* ret(Node* e) { return ir::make(arena, ir::Return{.expr = e}); }
    Node* binary(const char* op, Node* a, Node* b, ir::TypeP t = ir::i32_type()) { return ir::make(arena, ir::Binary{.op = op, .left = a, .right = b}, t); }
    Node* paren(Node* x) { return ir::make(arena, ir::Paren{.x = x}, x->type); }
    Node* function(const std::string& name, std::vector<Node*> body, ir::TypeP ret_type = ir::i32_type()) {
        ir::MethodDecl m;
        m.sym = ir::make_sym(ir::SymKind::Function, name, std::nullopt, true);
        m.sym->ret = ret_type;
        m.name = name;
        m.kind = "function";
        m.ret = ret_type;
        m.body = ir::make(arena, ir::Block{.stmts = std::move(body)});
        return ir::make(arena, std::move(m));
    }
    Node* unit(std::vector<Node*> types) {
        ir::Unit u;
        u.types = std::move(types);
        return ir::make(arena, std::move(u));
    }
};

CB_TEST("translate.java.smallest_program", [] {
    JvBuild b;
    // return (1 + 2) * 3 - 4 / 2;
    Node* e = b.binary("-", b.binary("*", b.paren(b.binary("+", b.lit(1), b.lit(2))), b.lit(3)), b.binary("/", b.lit(4), b.lit(2)));
    Node* units[] = {b.unit({b.function("main", {b.ret(e)})})};
    const java::Program p = java::program(units, "Tiny", {.tier = java::Tier::Faithful});
    CB_CHECK_EQ(p.source,
                "// Tiny.java -- translated from C by CodeBreaker (faithful tier: C memory emulated in crt.Mem).\n"
                "import crt.*;\n\nimport static crt.LibC.*;\n\npublic final class Tiny {\n\n"
                "    static int main() {\n        return (1 + 2) * 3 - 4 / 2;\n    }\n\n"
                "    public static void main(String[] args) {\n        exit(main());\n    }\n}\n");
    CB_CHECK(p.tier == java::Tier::Faithful);
    CB_CHECK_EQ(jv_notes_text(p), "faithful tier: chosen\n");
});

CB_TEST("translate.java.errors", [] {
    JvBuild b;
    // no main
    Node* no_main[] = {b.unit({b.function("helper", {b.ret(b.lit(1))})})};
    CB_CHECK_THROWS_AS(java::Refused, java::program(no_main, "X", {.tier = java::Tier::Faithful}));
    // two definitions of one name in C
    Node* twice[] = {b.unit({b.function("main", {b.ret(b.lit(1))}), b.function("main", {b.ret(b.lit(2))})})};
    CB_CHECK_THROWS_AS(java::Refused, java::program(twice, "X", {.tier = java::Tier::Faithful}));
    // a statement the writer has no form for (a Throw) is a gap in a function, and the program's end in main
    Node* thrower = b.function("f", {ir::make(b.arena, ir::Throw{})});
    Node* ok_main = b.function("main", {b.ret(b.lit(0))});
    Node* gapped[] = {b.unit({thrower, ok_main})};
    const java::Program p = java::program(gapped, "X", {.tier = java::Tier::Faithful});
    CB_CHECK_EQ(p.notes.front(), "gap: f (statement Throw)");
    CB_CHECK(p.source.find("CodeBreaker gap: f: statement Throw") != std::string::npos);
    Node* bad_main[] = {b.unit({b.function("main", {ir::make(b.arena, ir::Throw{})})})};
    CB_CHECK_THROWS_AS(java::Refused, java::program(bad_main, "X", {.tier = java::Tier::Faithful}));
    // an expression with no form: Unsupported names its kind and line
    Node* unsupported = ir::make(b.arena, ir::Unsupported{.kind = "StmtExpr"}, ir::i32_type(), 7);
    Node* with_unsupported[] = {b.unit({b.function("main", {b.ret(unsupported)})})};
    try {
        (void)java::program(with_unsupported, "X", {.tier = java::Tier::Faithful});
        CB_CHECK(false);
    } catch (const java::Refused& r) {
        CB_CHECK(std::string(r.what()).find("StmtExpr (line 7)") != std::string::npos);
    }
    // not a Unit
    Node* not_a_unit[] = {b.lit(1)};
    CB_CHECK_THROWS_AS(java::Refused, java::program(not_a_unit, "X", {.tier = java::Tier::Faithful}));
});

// ---- the text helpers ---------------------------------------------------------------------------------------------------------

CB_TEST("translate.java.jname", [] {
    CB_CHECK_EQ(java::jname("count"), "count");
    CB_CHECK_EQ(java::jname("a.b-c"), "a_b_c");
    CB_CHECK_EQ(java::jname(""), "x");
    CB_CHECK_EQ(java::jname("class"), "class_");
    CB_CHECK_EQ(java::jname("Mem"), "Mem_");     // names the runtime owns
    CB_CHECK_EQ(java::jname("frame"), "frame_");
    CB_CHECK_EQ(java::jname("1st"), "1st_");
    CB_CHECK_EQ(java::jname("caf\xC3\xA9"), "caf__");   // (DIVERGES: bytes outside ASCII)
    CB_CHECK_EQ(java::jname("__unnamed_1"), "__unnamed_1");
});

CB_TEST("translate.java.string_literals", [] {
    CB_CHECK_EQ(java::java_string("hi\n\"there\"\\"), "\"hi\\n\\\"there\\\"\\\\\"");
    CB_CHECK_EQ(java::java_string("a\tb\r"), "\"a\\tb\\r\"");
    // a char of a C string is a byte: those outside ASCII are octal escapes of the byte
    CB_CHECK_EQ(java::java_string("\xC3\xA9"), "\"\\351\"");              // the front end's U+00E9 is byte 0xE9
    CB_CHECK_EQ(java::java_string(std::string("\x01\x7f", 2)), "\"\\001\\177\"");
    CB_CHECK_EQ(java::java_wstring("a\xC4\x80\"\\"), "\"a\\u0100\\\"\\\\\"");   // U+0100 as a UTF-16 unit
    CB_CHECK_EQ(java::java_wstring("\xC3\xA9\n"), "\"\\351\\n\"");
    CB_CHECK_EQ(java::char_count("hi"), std::size_t{2});
    CB_CHECK_EQ(java::char_count("\xC3\xA9z"), std::size_t{2});
});

CB_TEST("translate.java.int_literals", [] {
    CB_CHECK_EQ(java::int_lit(5, "int").code, "5");
    CB_CHECK_EQ(java::int_lit(-5, "int").code, "-5");
    CB_CHECK_EQ(java::int_lit(-5, "int").prec, java::kUnary);
    CB_CHECK_EQ(java::int_lit(5, "int").prec, java::kAtom);
    CB_CHECK_EQ(java::int_lit(0xEE6B2800LL, "int").code, "0xEE6B2800");        // reduced to the type's bits, then hex when far from zero
    CB_CHECK_EQ(java::int_lit(0x1FFFFFFFFLL, "int").code, "-1");
    CB_CHECK_EQ(java::int_lit(0x811C9DC5LL, "int").code, "0x811C9DC5");        // large magnitudes in hex
    CB_CHECK_EQ(java::int_lit(-2147483648LL, "int").code, "-2147483648");
    CB_CHECK_EQ(java::int_lit(300, "byte").code, "(byte) 44");
    CB_CHECK_EQ(java::int_lit(0, "byte").code, "0");
    CB_CHECK_EQ(java::int_lit(0, "byte").jt, "int");
    CB_CHECK_EQ(java::int_lit(-1, "short").code, "(short) -1");
    CB_CHECK_EQ(java::int_lit(7, "long").code, "7L");
    CB_CHECK_EQ(java::int_lit(-7, "long").prec, java::kUnary);
    CB_CHECK_EQ(java::int_lit(1LL << 41, "long").code, "0x20000000000L");
    CB_CHECK_EQ(java::int_lit(-1, "long").code, "-1L");
    CB_CHECK_EQ(java::int_lit(std::numeric_limits<std::int64_t>::min(), "long").code, "0x8000000000000000L");
    // Java's char: a printable ASCII one as itself, the others as casts
    CB_CHECK_EQ(java::int_lit('a', "char").code, "'a'");
    CB_CHECK_EQ(java::int_lit('a', "char").prec, java::kAtom);
    CB_CHECK_EQ(java::int_lit('\'', "char").code, "(char) 39");
    CB_CHECK_EQ(java::int_lit('\\', "char").code, "(char) 92");
    CB_CHECK_EQ(java::int_lit('\n', "char").code, "(char) 10");
    CB_CHECK_EQ(java::int_lit(127, "char").code, "(char) 127");               // fixed: DEL had no Java spelling
    CB_CHECK_EQ(java::int_lit(65536 + 65, "char").code, "'A'");
    CB_CHECK_EQ(java::int_lit(-1, "char").code, "(char) 65535");
});

CB_TEST("translate.java.float_literals", [] {
    CB_CHECK_EQ(java::float_lit(2.5, "double").code, "2.5");
    CB_CHECK_EQ(java::float_lit(2.5, "float").code, "2.5f");
    CB_CHECK_EQ(java::float_lit(2.0, "double").code, "2.0");
    CB_CHECK_EQ(java::float_lit(100000.0, "double").code, "100000.0");
    CB_CHECK_EQ(java::float_lit(1e15, "double").code, "1000000000000000.0");
    CB_CHECK_EQ(java::float_lit(1e16, "double").code, "1.0e+16");
    CB_CHECK_EQ(java::float_lit(1.5e-7, "double").code, "1.5e-07");
    CB_CHECK_EQ(java::float_lit(0.0001, "double").code, "0.0001");
    CB_CHECK_EQ(java::float_lit(0.00001, "double").code, "1.0e-05");
    CB_CHECK_EQ(java::float_lit(0.1, "double").code, "0.1");
    CB_CHECK_EQ(java::float_lit(123456789.125, "double").code, "123456789.125");
    CB_CHECK_EQ(java::float_lit(1.2345678901234567e100, "double").code, "1.2345678901234567e+100");
    CB_CHECK_EQ(java::float_lit(0.0, "double").code, "0.0");
    CB_CHECK_EQ(java::float_lit(-0.0, "double").code, "-0.0");
    CB_CHECK_EQ(java::float_lit(-0.0, "double").prec, java::kUnary);
    CB_CHECK_EQ(java::float_lit(-1.5, "float").code, "-1.5f");
    CB_CHECK_EQ(java::float_lit(-1.5, "float").prec, java::kUnary);
    CB_CHECK_EQ(java::float_lit(std::nan(""), "double").code, "Double.NaN");
    CB_CHECK_EQ(java::float_lit(std::nan(""), "float").code, "Float.NaN");
    CB_CHECK_EQ(java::float_lit(HUGE_VAL, "double").code, "Double.POSITIVE_INFINITY");
    CB_CHECK_EQ(java::float_lit(-HUGE_VAL, "float").code, "Float.NEGATIVE_INFINITY");
});

CB_TEST("translate.java.char_literal_text", [] {
    CB_CHECK(java::java_char_text("'a'"));
    CB_CHECK(java::java_char_text("'\\n'"));
    CB_CHECK(java::java_char_text("'\\0'"));
    CB_CHECK(java::java_char_text("'\\377'"));
    CB_CHECK(java::java_char_text("'\\''"));
    CB_CHECK(java::java_char_text("'\\\\'"));
    CB_CHECK(java::java_char_text("'\xC3\xA9'"));
    CB_CHECK(!java::java_char_text("'\\400'"));   // not a byte
    CB_CHECK(!java::java_char_text("'\\x41'"));   // C's escapes Java lacks
    CB_CHECK(!java::java_char_text("'\\a'"));
    CB_CHECK(!java::java_char_text("'\\?'"));
    CB_CHECK(!java::java_char_text("'ab'"));
    CB_CHECK(!java::java_char_text("''"));
    CB_CHECK(!java::java_char_text("'''"));
    CB_CHECK(!java::java_char_text("a"));
    CB_CHECK(!java::java_char_text(""));
});

CB_TEST("translate.java.text_predicates", [] {
    CB_CHECK(java::is_int_text("5"));
    CB_CHECK(java::is_int_text("-5"));
    CB_CHECK(java::is_int_text("(-5)"));
    CB_CHECK(!java::is_int_text("5L"));
    CB_CHECK(!java::is_int_text("(byte) 5"));
    CB_CHECK(!java::is_int_text("-"));
    CB_CHECK(!java::is_int_text(""));
    CB_CHECK_EQ(java::promoted("byte"), "int");
    CB_CHECK_EQ(java::promoted("char"), "int");
    CB_CHECK_EQ(java::promoted("long"), "long");
    CB_CHECK_EQ(java::promoted2("int", "long"), "long");
    CB_CHECK_EQ(java::promoted2("long", "float"), "float");
    CB_CHECK_EQ(java::promoted2("byte", "short"), "int");
    CB_CHECK_EQ(java::promoted2("float", "double"), "double");
    CB_CHECK_EQ(java::promoted2("boolean", "int"), "boolean");
    CB_CHECK(java::binary_precedence("*") == java::kMul);
    CB_CHECK(java::binary_precedence("||") == java::kOr);
    CB_CHECK(!java::binary_precedence("=").has_value());
    const java::Ex e{"a + b", java::kAdd, "int"};
    CB_CHECK_EQ(e.at(java::kAdd), "a + b");
    CB_CHECK_EQ(e.at(java::kMul), "(a + b)");
});

// ---- the C types ----------------------------------------------------------------------------------------------------------------

CB_TEST("translate.java.kinds_and_spelling", [] {
    using ir::Prim;
    // plain char is by its sign (signed unless said otherwise)
    CB_CHECK(java::kind_of(ir::prim_type(Prim::Char, true)) == java::CKind::I8);
    CB_CHECK(java::kind_of(ir::prim_type(Prim::Char, false)) == java::CKind::U8);
    CB_CHECK(java::kind_of(ir::prim_type(Prim::Char)) == java::CKind::I8);
    CB_CHECK(java::kind_of(ir::TypeP{}) == java::CKind::Void);
    CB_CHECK(java::kind_of(ir::any_type()) == java::CKind::Other);
    CB_CHECK(java::kind_of(ir::null_type()) == java::CKind::Null);
    CB_CHECK(java::is_int(ir::prim_type(Prim::U16)));
    CB_CHECK(!java::is_int(ir::bool_type()));
    CB_CHECK(java::is_unsigned(ir::prim_type(Prim::U64)));
    CB_CHECK(!java::is_unsigned(ir::i32_type()));
    CB_CHECK(java::is_float(ir::prim_type(Prim::F80)));
    CB_CHECK(java::is_ptr(ir::pointer_to(ir::i32_type())));
    CB_CHECK(java::is_ptr(ir::null_type()));
    CB_CHECK(!java::is_ptr(ir::i32_type()));
    CB_CHECK(java::is_record(ir::class_type("p.Rec")));
    CB_CHECK(java::is_record(ir::make_type(ir::RecordType{.name = "R"})));
    CB_CHECK(!java::is_record(ir::pointer_to(ir::class_type("p.Rec"))));
    CB_CHECK_EQ(java::bits_of(java::CKind::U32), 32);
    CB_CHECK_EQ(java::bits_of(java::CKind::Bool), 1);
    CB_CHECK_EQ(java::bits_of(java::CKind::F32), 0);
    CB_CHECK_EQ(java::java_primitive(java::CKind::U16), "char");
    CB_CHECK_EQ(java::java_primitive(java::CKind::U8), "byte");
    CB_CHECK_EQ(java::java_primitive(java::CKind::F80), "double");
    CB_CHECK(java::java_primitive(java::CKind::Ptr).empty());

    CB_CHECK_EQ(java::spell(ir::i32_type()), "i32");
    CB_CHECK_EQ(java::spell(ir::TypeP{}), "?");
    CB_CHECK_EQ(java::spell(ir::class_type("geo.Vec")), "geo.Vec");
    CB_CHECK_EQ(java::spell(ir::class_type("Box", {ir::prim_type(Prim::F64)})), "Box<f64>");
    CB_CHECK_EQ(java::spell(ir::class_type("Pair", {ir::i32_type(), ir::class_type("X")})), "Pair<i32, X>");
    CB_CHECK_EQ(java::spell(ir::pointer_to(ir::prim_type(Prim::Char, true))), "char*");
    CB_CHECK_EQ(java::spell(ir::make_type(ir::PtrType{.to = ir::i32_type(), .ref = true})), "i32&");
    CB_CHECK_EQ(java::spell(ir::make_type(ir::ArrayType{.elem = ir::i32_type(), .count = 4})), "i32[4]");
    CB_CHECK_EQ(java::spell(ir::make_type(ir::ArrayType{.elem = ir::i32_type()})), "i32[]");
    ir::FnType fn;
    fn.result.type = ir::i32_type();
    fn.params = {ir::FnParam{.type = ir::i32_type()}, ir::FnParam{.type = ir::pointer_to(ir::i32_type())}};
    const ir::TypeP fn_type = ir::make_type(fn);
    CB_CHECK_EQ(java::spell(fn_type), "i32(i32,i32*)");
    CB_CHECK_EQ(java::spell(ir::make_type(ir::FnPtrType{.fn = fn_type})), "i32(i32,i32*)*");

    CB_CHECK_EQ(java::ctype_text(ir::i32_type(), "x"), "int x");
    CB_CHECK_EQ(java::ctype_text(ir::make_type(ir::ArrayType{.elem = ir::i32_type(), .count = 4}), "t"), "int t[4]");
    CB_CHECK_EQ(java::ctype_text(ir::pointer_to(ir::prim_type(Prim::U8)), "p"), "unsigned char *p");
    CB_CHECK_EQ(java::ctype_text(ir::class_type("Rec"), "r"), "struct Rec r");
    CB_CHECK_EQ(java::ctype_text(ir::prim_type(Prim::Char, true), "c"), "char c");
    CB_CHECK_EQ(java::ctype_text(ir::prim_type(Prim::U64), "n"), "unsigned long long n");
    CB_CHECK(java::pointee(ir::i32_type()).get() == nullptr);
    CB_CHECK(java::pointee(ir::pointer_to(ir::i32_type())) == ir::i32_type());
});

CB_TEST("translate.java.null_constants_and_allocs", [] {
    JvBuild b;
    Node* zero = b.lit(0);
    CB_CHECK(java::is_null_constant(zero));
    CB_CHECK(!java::is_null_constant(b.lit(1)));
    CB_CHECK(java::is_null_constant(ir::make(b.arena, ir::Lit{}, ir::null_type())));
    CB_CHECK(java::is_null_constant(ir::make(b.arena, ir::Cast{.x = ir::make(b.arena, ir::Paren{.x = zero})}, ir::pointer_to(ir::void_type()))));
    CB_CHECK(!java::is_null_constant(ir::make(b.arena, ir::Lit{.value = 0.0}, ir::prim_type(ir::Prim::F64))));
    ir::SymP malloc_sym = ir::make_sym(ir::SymKind::Function, "malloc", std::nullopt, true);
    Node* alloc = ir::make(b.arena, ir::Call{.sym = malloc_sym}, ir::pointer_to(ir::void_type()));
    CB_CHECK(java::from_alloc(alloc));
    CB_CHECK(java::from_alloc(ir::make(b.arena, ir::Paren{.x = alloc})));
    CB_CHECK(!java::from_alloc(zero));
    ir::SymP other = ir::make_sym(ir::SymKind::Function, "puts", std::nullopt, true);
    CB_CHECK(!java::from_alloc(ir::make(b.arena, ir::Call{.sym = other})));
});

// Speed: the whole of FaithfulArith (about 400 small functions) as Java; java.py took 112 ms on the same document (docs/TYPED_CPP_SPEED.md).
CB_TEST("translate.java.speed", [] {
    auto p = jv_load("FaithfulArith");
    double best = 1e9;
    for (int i = 0; i < 5; ++i) {
        const auto t0 = std::chrono::steady_clock::now();
        const java::Program prog = jv_write(*p, "FaithfulArith");
        best = std::min(best, std::chrono::duration<double, std::milli>(std::chrono::steady_clock::now() - t0).count());
        CB_CHECK(prog.source.size() > 1000);
    }
    std::cout << "java.program FaithfulArith: " << best << " ms" << std::endl;
    CB_CHECK(best < 112.0);
});

}  // namespace
