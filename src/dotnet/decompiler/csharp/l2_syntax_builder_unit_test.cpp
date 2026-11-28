// cb-unity: alone
// Tests of the C# back end's plain lowering (dotnet/decompiler/csharp/l2_syntax_builder), the one C# back end of every front end with no
// IL facts, on programs built the way the C# and Java front ends build them (resolved symbols, typed expressions, one Unit per source
// file). They are the old writer's tests (translate/backends/csharp, retired in stage 15), moved onto this path. The expected C# was
// reviewed against what the old writer wrote: the same program, with the layout and literal spelling of ILSpy's output visitor (no blank
// line between the types of a namespace, `: base(...)` on its own line, `where` on the declaration's line, a block lambda, a switch
// expression and an object initialiser one item to a line, `7uL`, `3f`, `- -a`, `(long)-1`, text that is not ASCII as it is).
// Java units go through the rule set java (translate/rules/java_csharp) when the options give the API-mapping registry; without it
// they are declined, class by class.
//
// Behaviour pinned here: a statement list where a statement is expected (a bare list of statements as a body) is written as a
// block; a float literal reads back as the same value, and a double a float cannot hold is declined (not written as an infinity);
// constant folding for `unchecked` is exact in 64 bits; two units of one file stem get `_2`, not one file over the other; a u64
// literal above int64's maximum is written.
#include "dotnet/decompiler/csharp/l2_syntax_builder.hpp"
#include "translate/l2.hpp"

#include <algorithm>
#include <chrono>
#include <cstdio>
#include <limits>

#include <nlohmann/json.hpp>

#include "core/fs.hpp"
#include "core/json.hpp"
#include "core/testing.hpp"

namespace {

namespace cs = cb::dotnet::decompiler::csharp;
namespace ir = cb::translate::ir;
using ir::Node;
using ir::TypeP;

TypeP t_int() { return ir::i32_type(); }
TypeP t_str() { return ir::class_type("System.String"); }
TypeP t_dbl() { return ir::prim_type(ir::Prim::F64); }
TypeP t_obj() { return ir::class_type("System.Object"); }

// Builds trees in one arena.
struct B {
    cb::Arena arena;

    template <class K> Node* n(K kind, TypeP type = {}) { return ir::make(arena, std::move(kind), std::move(type)); }

    Node* i(std::int64_t v, TypeP t = ir::i32_type()) { return n(ir::Lit{.value = v}, std::move(t)); }
    Node* str(std::string s) { return n(ir::Lit{.value = std::move(s)}, t_str()); }
    Node* flag(bool v) { return n(ir::Lit{.value = v}, ir::bool_type()); }
    Node* null() { return n(ir::Lit{.value = std::monostate{}}, ir::null_type()); }
    Node* name(const ir::SymP& s) { return n(ir::Name{.sym = s}, s->type); }
    Node* bin(std::string op, Node* l, Node* r, TypeP t = ir::i32_type()) { return n(ir::Binary{.op = std::move(op), .left = l, .right = r}, std::move(t)); }
    Node* cmp(std::string op, Node* l, Node* r) { return bin(std::move(op), l, r, ir::bool_type()); }
    Node* expr_stmt(Node* e) { return n(ir::ExprStmt{.expr = e}); }
    Node* ret(Node* e = nullptr) { return n(ir::Return{.expr = e}); }
    Node* block(std::vector<Node*> stmts) { return n(ir::Block{.stmts = std::move(stmts)}); }
    Node* local(const ir::SymP& s, Node* init = nullptr) { return n(ir::Local{.sym = s, .type = s->type, .init = init}); }
    Node* assign(Node* target, Node* value, std::optional<std::string> op = std::nullopt) {
        return n(ir::Assign{.op = std::move(op), .target = target, .value = value}, target->type);
    }
    Node* param(const ir::SymP& s) { return n(ir::Param{.sym = s, .type = s->type}); }

    ir::SymP local_sym(const std::string& name, TypeP t, const std::string& owner = "Prog.M") {
        return ir::make_sym(ir::SymKind::Local, name, owner, false, std::move(t));
    }
    ir::SymP param_sym(const std::string& name, TypeP t, const std::string& owner = "Prog.M") {
        return ir::make_sym(ir::SymKind::Param, name, owner, false, std::move(t));
    }

    Node* method(const std::string& name, TypeP ret, std::vector<Node*> params, Node* body, std::vector<std::string> mods = {"static"},
                 const std::string& kind = "method") {
        ir::MethodDecl m;
        m.sym = ir::make_sym(ir::SymKind::Method, name, "Prog", false);
        m.name = name;
        m.kind = kind;
        m.params = std::move(params);
        m.ret = std::move(ret);
        m.body = body;
        m.mods = std::move(mods);
        return n(std::move(m));
    }

    Node* method(const std::string& name, std::nullptr_t, std::vector<Node*> params, Node* body, std::vector<std::string> mods = {"static"},
                 const std::string& kind = "method") {
        return method(name, TypeP(), std::move(params), body, std::move(mods), kind);
    }

    Node* klass(const std::string& qname, std::vector<Node*> members, std::vector<std::string> mods = {"public"}, const std::string& kind = "class") {
        ir::ClassDecl c;
        const auto dot = qname.rfind('.');
        c.name = dot == std::string::npos ? qname : qname.substr(dot + 1);
        c.qname = qname;
        c.kind = kind;
        c.mods = std::move(mods);
        c.members = std::move(members);
        return n(std::move(c));
    }

    Node* unit(const std::string& path, std::vector<Node*> types, const std::string& lang = "csharp") {
        ir::Unit u;
        u.path = path;
        u.lang = lang;
        u.types = std::move(types);
        return n(std::move(u));
    }
};

std::string write_one(Node* unit) {
    Node* units[] = {unit};
    cs::L2CSharpProgram p(units);
    return p.files().at(0).text;
}

// Whether `text` holds `part`; when it does not, the text is printed, to read what was written instead.
bool l2u_holds(const std::string& text, std::string_view part) {
    if (text.find(part) != std::string::npos) return true;
    std::fprintf(stderr, "not in the text: [%.*s]\n----\n%s----\n", static_cast<int>(part.size()), part.data(), text.c_str());
    return false;
}

// The text a single expression is written as: `T x = <text>;` in the one method of a class.
std::string render(B& b, Node* e) {
    const ir::SymP x = b.local_sym("x", e->type);
    Node* m = b.method("M", nullptr, {}, b.block({b.local(x, e)}));
    const std::string file = write_one(b.unit("P.cs", {b.klass("Prog", {m}, {})}));
    const std::string head = " x = ";
    const std::string tail = ";\n    }\n}\n";   // (the one statement of the one method, then the end of the class)
    const auto at = file.find(head);
    const auto end = file.rfind(tail);
    CB_CHECK(at != std::string::npos && end != std::string::npos);
    if (at == std::string::npos || end == std::string::npos) return file;
    return file.substr(at + head.size(), end - at - head.size());
}

}  // namespace

// ---- the file ------------------------------------------------------------------------------------------------------------

CB_TEST("dotnet.l2_csharp.unit.class_in_a_namespace", [] {
    B b;
    // namespace Geo { public class Circle { public double R; public Circle(double r) { R = r; }
    //                 public double Area => R * R * 3.14159; public override string ToString() { return "Circle " + R; } } }
    const auto field_r = ir::make_sym(ir::SymKind::Field, "R", "Geo.Circle", false, t_dbl());
    ir::FieldDecl fd{.sym = field_r, .type = t_dbl(), .mods = {"public"}};
    Node* field = b.n(std::move(fd));
    const auto pr = b.param_sym("r", t_dbl(), "Geo.Circle..ctor");
    Node* self = b.n(ir::This{}, ir::class_type("Geo.Circle"));
    Node* ctor = b.method("Circle", nullptr, {b.param(pr)},
                          b.block({b.expr_stmt(b.assign(b.n(ir::Select{.obj = self, .sym = field_r}, t_dbl()), b.name(pr)))}), {"public"}, "ctor");
    const auto area_sym = ir::make_sym(ir::SymKind::Property, "Area", "Geo.Circle", false, t_dbl());
    Node* r1 = b.n(ir::Select{.obj = self, .sym = field_r}, t_dbl());
    Node* r2 = b.n(ir::Select{.obj = self, .sym = field_r}, t_dbl());
    Node* area = b.bin("*", b.bin("*", r1, r2, t_dbl()), b.n(ir::Lit{.value = 3.14159}, t_dbl()), t_dbl());
    ir::MethodDecl getter;
    getter.kind = "get";
    getter.body = b.block({b.ret(area)});
    ir::Property property{.sym = area_sym, .type = t_dbl(), .mods = {"public"}, .accessors = {b.n(std::move(getter))}};
    Node* to_string = b.method("ToString", t_str(), {}, b.block({b.ret(b.n(ir::Concat{.parts = {b.str("Circle "), b.n(ir::Select{.obj = self, .sym = field_r}, t_dbl())}}, t_str()))}),
                               {"public", "override"});
    Node* circle = b.klass("Geo.Circle", {field, ctor, b.n(std::move(property)), to_string});
    const std::string expected =
        "// Translated by CodeBreaker from Geo.cs (csharp).\n"
        "namespace Geo\n"
        "{\n"
        "    public class Circle\n"
        "    {\n"
        "        public double R;\n"
        "\n"
        "        public Circle(double r)\n"
        "        {\n"
        "            R = r;\n"
        "        }\n"
        "\n"
        "        public double Area => R * R * 3.14159;\n"
        "\n"
        "        public override string ToString() => \"Circle \" + R;\n"
        "    }\n"
        "}\n";
    CB_CHECK_EQ(write_one(b.unit("src/Geo.cs", {circle})), expected);
});

CB_TEST("dotnet.l2_csharp.unit.usings_and_nested_names", [] {
    B b;
    // Outer.Inner is named from inside Outer by its own name, from outside by Outer.Inner; a library class gets a using.
    Node* inner = b.klass("N.Outer.Inner", {}, {"public"});
    ir::get<ir::ClassDecl>(inner)->outer = "N.Outer";
    const auto li = b.local_sym("a", ir::class_type("N.Outer.Inner"), "N.Outer.Run");
    const auto ll = b.local_sym("l", ir::class_type("System.Collections.Generic.List", {t_int()}), "N.Outer.Run");
    Node* run = b.method("Run", nullptr, {}, b.block({b.local(li), b.local(ll)}));
    Node* outer = b.klass("N.Outer", {inner, run});
    Node* other_unit_class = b.klass("N.User", {b.method("F", nullptr, {}, b.block({b.local(b.local_sym("q", ir::class_type("N.Outer.Inner"), "N.User.F"))}))});
    const std::string file = write_one(b.unit("A.cs", {outer, other_unit_class}));
    CB_CHECK(l2u_holds(file, "using System.Collections.Generic;"));
    CB_CHECK(l2u_holds(file, "        public class Inner\n"));
    CB_CHECK(l2u_holds(file, "            Inner a;\n"));                // inside Outer: the nested name alone
    CB_CHECK(l2u_holds(file, "            List<int> l;\n"));
    CB_CHECK(l2u_holds(file, "            Outer.Inner q;\n"));          // from another class: through its outer class
    CB_CHECK_EQ(std::count(file.begin(), file.end(), '{'), std::count(file.begin(), file.end(), '}'));
    CB_CHECK_EQ(file.find("namespace N\n"), file.rfind("namespace N\n"));              // one namespace block for both classes
});

CB_TEST("dotnet.l2_csharp.unit.keywords_are_escaped", [] {
    B b;
    const auto s = b.local_sym("event", t_int());
    const auto k1 = b.local_sym("class", t_int());
    const auto k2 = b.local_sym("object", t_int());
    const auto k3 = b.local_sym("Class", t_int());
    Node* m = b.method("M", nullptr, {}, b.block({b.local(s, b.i(1)), b.expr_stmt(b.assign(b.name(s), b.i(2))), b.local(k1, b.i(3)), b.local(k2, b.i(4)), b.local(k3, b.i(5))}));
    const std::string file = write_one(b.unit("K.cs", {b.klass("Prog", {m})}));
    CB_CHECK(l2u_holds(file, "int @event = 1;"));
    CB_CHECK(l2u_holds(file, "@event = 2;"));
    CB_CHECK(l2u_holds(file, "int @class = 3;"));
    CB_CHECK(l2u_holds(file, "int @object = 4;"));
    CB_CHECK(l2u_holds(file, "int Class = 5;"));   // (C#'s keywords are lower case)
});

// ---- literals ----------------------------------------------------------------------------------------------------------------

CB_TEST("dotnet.l2_csharp.literals.integers", [] {
    B b;
    CB_CHECK_EQ(render(b, b.i(5)), "5");
    CB_CHECK_EQ(render(b, b.i(-5)), "-5");
    CB_CHECK_EQ(render(b, b.i(std::numeric_limits<std::int32_t>::max())), "int.MaxValue");
    CB_CHECK_EQ(render(b, b.i(std::numeric_limits<std::int32_t>::min())), "int.MinValue");
    CB_CHECK_EQ(render(b, b.i(7, ir::prim_type(ir::Prim::I64))), "7L");
    CB_CHECK_EQ(render(b, b.i(std::numeric_limits<std::int64_t>::min(), ir::prim_type(ir::Prim::I64))), "long.MinValue");
    CB_CHECK_EQ(render(b, b.i(std::numeric_limits<std::int64_t>::max(), ir::prim_type(ir::Prim::I64))), "long.MaxValue");
    CB_CHECK_EQ(render(b, b.i(7, ir::prim_type(ir::Prim::U32))), "7u");
    CB_CHECK_EQ(render(b, b.i(4294967295LL, ir::prim_type(ir::Prim::U32))), "uint.MaxValue");
    CB_CHECK_EQ(render(b, b.i(7, ir::prim_type(ir::Prim::U64))), "7uL");   // (the output visitor's spelling of a ulong)
    CB_CHECK_EQ(render(b, b.n(ir::Lit{.value = std::uint64_t{18446744073709551615ULL}}, ir::prim_type(ir::Prim::U64))), "ulong.MaxValue");
    CB_CHECK_EQ(render(b, b.n(ir::Lit{.value = std::uint64_t{18446744073709551614ULL}}, ir::prim_type(ir::Prim::U64))), "18446744073709551614uL");
    CB_CHECK_EQ(render(b, b.i(3, ir::prim_type(ir::Prim::U8))), "3");
});

CB_TEST("dotnet.l2_csharp.literals.floats_bools_null", [] {
    B b;
    const auto f64 = [&](double v) { return render(b, b.n(ir::Lit{.value = v}, t_dbl())); };
    CB_CHECK_EQ(f64(1.5), "1.5");
    CB_CHECK_EQ(f64(100.0), "100.0");
    CB_CHECK_EQ(f64(-0.25), "-0.25");
    CB_CHECK_EQ(f64(1e22), "1E+22");
    CB_CHECK_EQ(f64(2500000000.0), "2500000000.0");                                       // positional up to 1e16, as a person writes it
    CB_CHECK_EQ(f64(123456789.125), "123456789.125");
    CB_CHECK_EQ(f64(0.0001), "0.0001");
    CB_CHECK_EQ(f64(1.5e-5), "1.5E-05");
    CB_CHECK_EQ(f64(1e16), "10000000000000000.0");
    CB_CHECK_EQ(f64(0.0), "0.0");
    CB_CHECK_EQ(f64(1.7976931348623157e308), "1.7976931348623157E+308");
    CB_CHECK_EQ(f64(std::numeric_limits<double>::quiet_NaN()), "double.NaN");
    CB_CHECK_EQ(f64(std::numeric_limits<double>::infinity()), "double.PositiveInfinity");
    CB_CHECK_EQ(f64(-std::numeric_limits<double>::infinity()), "double.NegativeInfinity");
    const TypeP f32 = ir::prim_type(ir::Prim::F32);
    CB_CHECK_EQ(render(b, b.n(ir::Lit{.value = 0.1}, f32)), "0.1f");                       // the shortest text that is this float
    CB_CHECK_EQ(render(b, b.n(ir::Lit{.value = 3.0}, f32)), "3f");
    CB_CHECK_EQ(render(b, b.n(ir::Lit{.value = 1e10}, f32)), "1E+10f");
    CB_CHECK_EQ(render(b, b.n(ir::Lit{.value = 3.4028234663852886e38}, f32)), "3.4028235E+38f");
    CB_CHECK_EQ(render(b, b.n(ir::Lit{.value = std::numeric_limits<double>::quiet_NaN()}, f32)), "float.NaN");
    CB_CHECK_EQ(render(b, b.n(ir::Lit{.value = std::int64_t{2}}, t_dbl())), "2.0");        // an integer in a double's place
    CB_CHECK_EQ(render(b, b.flag(true)), "true");
    CB_CHECK_EQ(render(b, b.flag(false)), "false");
    CB_CHECK_EQ(render(b, b.null()), "null");
    // a double beyond float's range is declined, and the method is a gap
    const ir::SymP x = b.local_sym("x", f32);
    const std::string gap = write_one(b.unit("F.cs", {b.klass("Prog", {b.method("M", nullptr, {}, b.block({b.local(x, b.n(ir::Lit{.value = 1e300}, f32))}))})}));
    CB_CHECK(l2u_holds(gap, "CB-GAP: a float literal beyond the range of float"));
});

CB_TEST("dotnet.l2_csharp.literals.text_and_chars", [] {
    B b;
    CB_CHECK_EQ(render(b, b.str("plain")), "\"plain\"");
    CB_CHECK_EQ(render(b, b.str("a\"b\\c\n\r\t")), "\"a\\\"b\\\\c\\n\\r\\t\"");
    CB_CHECK_EQ(render(b, b.str(std::string("nul\0z", 5))), "\"nul\\0z\"");
    CB_CHECK_EQ(render(b, b.str("caf\xC3\xA9")), "\"caf\xC3\xA9\"");                         // not ASCII: as it is (the file is UTF-8)
    CB_CHECK_EQ(render(b, b.str("\xF0\x9F\x98\x80")), "\"\\ud83d\\ude00\"");               // beyond the BMP: a surrogate pair
    CB_CHECK_EQ(render(b, b.str("\xED\xA0\x80")), "\"\\ud800\"");                          // half of a pair, as a Java char keeps it
    const TypeP ch = ir::prim_type(ir::Prim::U16, std::nullopt, ir::CharEncoding::Utf16);
    CB_CHECK_EQ(render(b, b.n(ir::Lit{.value = std::string("a")}, ch)), "'a'");
    CB_CHECK_EQ(render(b, b.n(ir::Lit{.value = std::string("'")}, ch)), "'\\''");
    CB_CHECK_EQ(render(b, b.n(ir::Lit{.value = std::string("\"")}, ch)), "'\"'");
    CB_CHECK_EQ(render(b, b.n(ir::Lit{.value = std::int64_t{65}}, ch)), "'A'");             // a number in a char's place
    // a u16 with no encoding is a ushort, and its literal a number
    CB_CHECK_EQ(render(b, b.n(ir::Lit{.value = std::string("a")}, ir::prim_type(ir::Prim::U16))), "97");
});

// ---- operators and precedence -----------------------------------------------------------------------------------------------

CB_TEST("dotnet.l2_csharp.expr.parentheses_only_where_needed", [] {
    B b;
    const auto a = b.local_sym("a", t_int());
    const auto c = b.local_sym("c", t_int());
    const auto d = b.local_sym("d", t_int());
    CB_CHECK_EQ(render(b, b.bin("*", b.bin("+", b.name(a), b.name(c)), b.name(d))), "(a + c) * d");
    CB_CHECK_EQ(render(b, b.bin("+", b.bin("*", b.name(a), b.name(c)), b.name(d))), "a * c + d");
    CB_CHECK_EQ(render(b, b.bin("-", b.name(a), b.bin("-", b.name(c), b.name(d)))), "a - (c - d)");   // a right operand of equal precedence
    CB_CHECK_EQ(render(b, b.bin("-", b.bin("-", b.name(a), b.name(c)), b.name(d))), "a - c - d");
    CB_CHECK_EQ(render(b, b.bin("<<", b.bin("+", b.name(a), b.i(1)), b.i(2))), "a + 1 << 2");           // + binds tighter than <<
    CB_CHECK_EQ(render(b, b.bin("+", b.bin("<<", b.name(a), b.i(1)), b.i(2))), "(a << 1) + 2");
    CB_CHECK_EQ(render(b, b.bin("&", b.name(a), b.bin("|", b.name(c), b.name(d)))), "a & (c | d)");
    CB_CHECK_EQ(render(b, b.n(ir::Paren{.x = b.bin("+", b.name(a), b.name(c))}, t_int())), "a + c");   // grouping is rebuilt from precedence
    CB_CHECK_EQ(render(b, b.n(ir::Unary{.op = "-", .x = b.n(ir::Unary{.op = "-", .x = b.name(a)}, t_int())}, t_int())), "- -a");
    CB_CHECK_EQ(render(b, b.n(ir::Unary{.op = "!", .x = b.cmp("==", b.name(a), b.name(c))}, ir::bool_type())), "!(a == c)");
    CB_CHECK_EQ(render(b, b.n(ir::Cast{.x = b.bin("+", b.name(a), b.name(c))}, ir::prim_type(ir::Prim::I64))), "(long)(a + c)");
    CB_CHECK_EQ(render(b, b.n(ir::Cast{.x = b.name(a)}, ir::prim_type(ir::Prim::F64))), "(double)a");
    CB_CHECK_EQ(render(b, b.n(ir::Cast{.x = b.i(-1)}, ir::prim_type(ir::Prim::I64))), "(long)-1");
    Node* post = b.n(ir::Unary{.op = "++", .x = b.name(a), .postfix = true}, t_int());
    CB_CHECK_EQ(render(b, post), "a++");
    Node* cond = b.n(ir::Cond{.cond = b.cmp(">", b.name(a), b.i(0)), .then = b.name(a), .orelse = b.n(ir::Unary{.op = "-", .x = b.name(a)}, t_int())}, t_int());
    CB_CHECK_EQ(render(b, cond), "a > 0 ? a : -a");
    Node* nested = b.n(ir::Cond{.cond = b.cmp(">", b.name(a), b.i(0)), .then = b.name(c), .orelse = b.n(ir::Cond{.cond = b.cmp(">", b.name(a), b.i(1)), .then = b.name(d), .orelse = b.i(0)}, t_int())}, t_int());
    CB_CHECK_EQ(render(b, nested), "a > 0 ? c : a > 1 ? d : 0");
    // a `??` chain is right-associative
    const auto s1 = b.local_sym("s1", t_str());
    const auto s2 = b.local_sym("s2", t_str());
    CB_CHECK_EQ(render(b, b.bin("??", b.name(s1), b.bin("??", b.name(s2), b.str("-"), t_str()), t_str())), "s1 ?? s2 ?? \"-\"");
    CB_CHECK_EQ(render(b, b.bin("??", b.bin("??", b.name(s1), b.name(s2), t_str()), b.str("-"), t_str())), "(s1 ?? s2) ?? \"-\"");
    // checked arithmetic
    ir::Binary checked_add{.op = "+", .left = b.name(a), .right = b.name(c), .overflow = ir::Overflow::Trap};
    CB_CHECK_EQ(render(b, b.n(std::move(checked_add), t_int())), "checked(a + c)");
});

CB_TEST("dotnet.l2_csharp.expr.constants_that_overflow_are_unchecked", [] {
    B b;
    // Java wraps a constant expression; C# refuses to compile one that leaves its type's range
    const auto max = [&] { return b.i(std::numeric_limits<std::int32_t>::max()); };
    CB_CHECK_EQ(render(b, b.bin("+", max(), b.i(1))), "unchecked(int.MaxValue + 1)");
    CB_CHECK_EQ(render(b, b.bin("+", max(), b.i(0))), "int.MaxValue + 0");
    CB_CHECK_EQ(render(b, b.bin("*", b.i(65536), b.i(65536))), "unchecked(65536 * 65536)");
    CB_CHECK_EQ(render(b, b.n(ir::Unary{.op = "-", .x = b.i(std::numeric_limits<std::int32_t>::min())}, t_int())), "unchecked(-int.MinValue)");
    const TypeP i64 = ir::prim_type(ir::Prim::I64);
    CB_CHECK_EQ(render(b, b.bin("*", b.i(4000000000LL, i64), b.i(4000000000LL, i64), i64)), "unchecked(4000000000L * 4000000000L)");
    CB_CHECK_EQ(render(b, b.bin("*", b.i(40000, i64), b.i(40000, i64), i64)), "40000L * 40000L");
    // a value that is not a constant is left alone
    const auto v = b.local_sym("v", t_int());
    CB_CHECK_EQ(render(b, b.bin("+", b.name(v), b.i(1))), "v + 1");
    // division by zero in a constant is not folded (the compiler's to refuse)
    CB_CHECK_EQ(render(b, b.bin("/", b.i(1), b.i(0))), "1 / 0");
});

CB_TEST("dotnet.l2_csharp.expr.string_concatenation", [] {
    B b;
    const auto n = b.local_sym("n", t_int());
    const auto s = b.local_sym("s", t_str());
    // two non-strings start from "" so that + is not an addition
    CB_CHECK_EQ(render(b, b.n(ir::Concat{.parts = {b.name(n), b.name(n)}}, t_str())), "\"\" + n + n");
    CB_CHECK_EQ(render(b, b.n(ir::Concat{.parts = {b.name(s), b.name(n)}}, t_str())), "s + n");
    // a C# concatenation of three or more parts with text is written interpolated
    CB_CHECK_EQ(render(b, b.n(ir::Concat{.parts = {b.str("n = "), b.name(n), b.str("{!}")}}, t_str())), "$\"n = {n}{{!}}\"");
    // an expression with `:` or `?` in an interpolation hole is parenthesised
    Node* c = b.n(ir::Cond{.cond = b.cmp(">", b.name(n), b.i(0)), .then = b.str("a"), .orelse = b.str("b")}, t_str());
    CB_CHECK_EQ(render(b, b.n(ir::Concat{.parts = {b.str("x"), c, b.str("y")}}, t_str())), "$\"x{(n > 0 ? \"a\" : \"b\")}y\"");
    // a literal with a newline is not interpolated
    CB_CHECK_EQ(render(b, b.n(ir::Concat{.parts = {b.str("a\n"), b.name(n), b.str("b")}}, t_str())), "\"a\\n\" + n + \"b\"");
});

CB_TEST("dotnet.l2_csharp.expr.calls_members_indexers_and_lambdas", [] {
    B b;
    const auto list = b.local_sym("list", ir::class_type("System.Collections.Generic.List", {t_int()}));
    const auto n = b.local_sym("n", t_int());
    const auto add = ir::make_sym(ir::SymKind::Method, "Add", "System.Collections.Generic.List", false);
    add->params = {t_int()};
    Node* call = b.n(ir::Call{.target = b.name(list), .sym = add, .args = {b.name(n)}}, ir::prim_type(ir::Prim::Bool));
    CB_CHECK_EQ(render(b, call), "list.Add(n)");
    // a generic method's type arguments are written when C# cannot infer them
    const auto make = ir::make_sym(ir::SymKind::Method, "Make", "Util", true);
    Node* typed = b.n(ir::Call{.target = b.n(ir::TypeName{}, ir::class_type("Util")), .sym = make, .typeargs = {ir::TypeArg{t_int()}, ir::TypeArg{t_str()}}}, t_int());
    CB_CHECK_EQ(render(b, typed), "Util.Make<int, string>()");
    // an operator method is an operator
    const auto plus = ir::make_sym(ir::SymKind::Method, "op_Addition", "V", true);
    plus->flags = {"operator"};
    const auto v1 = b.local_sym("v1", ir::class_type("V"));
    const auto v2 = b.local_sym("v2", ir::class_type("V"));
    Node* sum = b.n(ir::Call{.sym = plus, .args = {b.name(v1), b.name(v2)}}, ir::class_type("V"));
    CB_CHECK_EQ(render(b, sum), "v1 + v2");
    // an extension method is written on its receiver, with a using for its class
    const auto first = ir::make_sym(ir::SymKind::Method, "First", "System.Linq.Enumerable", true);
    first->flags = {"extension"};
    first->params = {ir::class_type("System.Collections.Generic.List", {t_int()})};
    Node* ext = b.n(ir::Call{.target = b.n(ir::TypeName{}, ir::class_type("System.Linq.Enumerable")), .sym = first, .args = {b.name(list)}}, t_int());
    CB_CHECK_EQ(render(b, ext), "list.First()");
    // an array: length, index, a new array of sizes, and one with items
    const auto arr = b.local_sym("arr", ir::array_of(t_int()));
    CB_CHECK_EQ(render(b, b.n(ir::Length{.arr = b.name(arr)}, t_int())), "arr.Length");
    CB_CHECK_EQ(render(b, b.n(ir::Index{.arr = b.name(arr), .index = b.i(2)}, t_int())), "arr[2]");
    CB_CHECK_EQ(render(b, b.n(ir::NewArray{.elem = t_int(), .dims = {b.name(n)}}, ir::array_of(t_int()))), "new int[n]");
    CB_CHECK_EQ(render(b, b.n(ir::NewArray{.elem = t_int(), .init = {b.i(1), b.i(2)}}, ir::array_of(t_int()))), "new int[] { 1, 2 }");
    CB_CHECK_EQ(render(b, b.n(ir::NewArray{.elem = t_int(), .dims = {b.i(2)}}, ir::array_of(ir::array_of(t_int())))), "new int[2][]");
    TypeP matrix = ir::make_type(ir::ArrayType{.elem = t_int(), .rank = 2});
    CB_CHECK_EQ(render(b, b.n(ir::NewArray{.elem = t_int(), .dims = {b.i(2), b.i(3)}}, matrix)), "new int[2, 3]");
    CB_CHECK_EQ(render(b, b.n(ir::Index{.arr = b.n(ir::Name{.sym = b.local_sym("m", matrix)}, matrix), .index = std::vector<Node*>{b.i(1), b.i(2)}}, t_int())), "m[1, 2]");
    // a lambda, with a parameter of its own
    const auto p = b.param_sym("p", t_int(), "Prog.M.lambda");
    Node* lam = b.n(ir::Lambda{.params = {b.param(p)}, .body = b.bin("*", b.name(p), b.i(2))}, ir::class_type("System.Func", {t_int(), t_int()}));
    CB_CHECK_EQ(render(b, lam), "p => p * 2");
    const auto q = b.param_sym("q", t_int(), "Prog.M.lambda");
    Node* lam2 = b.n(ir::Lambda{.params = {b.param(p), b.param(q)}, .body = b.block({b.ret(b.bin("+", b.name(p), b.name(q)))})}, ir::class_type("System.Func", {t_int(), t_int(), t_int()}));
    CB_CHECK_EQ(render(b, lam2), "(p, q) =>\n        {\n            return p + q;\n        }");   // (a block lambda is indented with its statement)
    // is-patterns and typeof
    const auto o = b.local_sym("o", t_obj());
    const auto bound = b.local_sym("s", t_str());
    Node* is = b.n(ir::InstanceOf{.x = b.name(o), .test = t_str(), .binding = b.local(bound)}, ir::bool_type());
    CB_CHECK_EQ(render(b, is), "o is string s");
    CB_CHECK_EQ(render(b, b.n(ir::ClassLit{.of = t_str()}, ir::class_type("System.Type"))), "typeof(string)");
    // a switch expression
    const auto k = b.local_sym("k", t_int());
    ir::Case c1{.labels = {b.i(1)}, .body = {b.n(ir::Yield{.expr = b.str("one")})}};
    ir::Case c2{.is_default = true, .body = {b.n(ir::Yield{.expr = b.str("many")})}};
    Node* sw = b.n(ir::SwitchExpr{.expr = b.name(k), .cases = {b.n(std::move(c1)), b.n(std::move(c2))}}, t_str());
    CB_CHECK_EQ(render(b, sw), "k switch\n        {\n            1 => \"one\",\n            _ => \"many\",\n        }");   // (one arm to a line)
    // `new`, with its arguments coerced to the parameters
    const auto ctor = ir::make_sym(ir::SymKind::Ctor, "Pair", "Pair", false);
    ctor->params = {t_int(), t_str()};
    CB_CHECK_EQ(render(b, b.n(ir::New{.sym = ctor, .args = {b.i(1), b.str("x")}}, ir::class_type("Pair"))), "new Pair(1, \"x\")");
});

CB_TEST("dotnet.l2_csharp.expr.null_conditional_and_ref_arguments", [] {
    B b;
    const auto r = b.local_sym("r", ir::class_type("Node"));
    const auto next = ir::make_sym(ir::SymKind::Field, "Next", "Node", false, ir::class_type("Node"));
    Node* access = b.n(ir::Select{.obj = b.name(r), .sym = next}, ir::class_type("Node"));
    Node* cond = b.n(ir::Cond{.cond = b.cmp("!=", b.name(r), b.null()), .then = access, .orelse = b.null()}, ir::class_type("Node"));
    CB_CHECK_EQ(render(b, cond), "r?.Next");
    // out and ref arguments carry their keyword
    const auto try_get = ir::make_sym(ir::SymKind::Method, "TryGet", "Cache", true);
    try_get->params = {t_int(), t_int()};
    try_get->refs = {ir::PassMode::ByValue, ir::PassMode::Out};
    const auto key = b.local_sym("key", t_int());
    const auto val = b.local_sym("val", t_int());
    Node* call = b.n(ir::Call{.target = b.n(ir::TypeName{}, ir::class_type("Cache")), .sym = try_get, .args = {b.name(key), b.name(val)}}, ir::bool_type());
    CB_CHECK_EQ(render(b, call), "Cache.TryGet(key, out val)");
});

// ---- statements ---------------------------------------------------------------------------------------------------------------

CB_TEST("dotnet.l2_csharp.stmt.a_method_of_every_statement", [] {
    B b;
    const TypeP ints = ir::array_of(t_int());
    const auto xs = b.param_sym("xs", ints, "Prog.Run");
    const auto name = b.param_sym("name", t_str(), "Prog.Run");
    const auto total = b.local_sym("total", t_int(), "Prog.Run");
    const auto i = b.local_sym("i", t_int(), "Prog.Run");
    const auto x = b.local_sym("x", t_int(), "Prog.Run");
    const auto e = b.local_sym("e", ir::class_type("System.FormatException"), "Prog.Run");
    // int total = 0;
    // for (int i = 0; i < xs.Length; i++) { total += xs[i]; }
    Node* for_stmt = b.n(ir::For{.init = {b.local(i, b.i(0))},
                                 .cond = b.cmp("<", b.name(i), b.n(ir::Length{.arr = b.name(xs)}, t_int())),
                                 .update = {b.n(ir::Unary{.op = "++", .x = b.name(i), .postfix = true}, t_int())},
                                 .body = b.block({b.expr_stmt(b.assign(b.name(total), b.n(ir::Index{.arr = b.name(xs), .index = b.name(i)}, t_int()), "+"))})});
    // foreach (int x in xs) { if (x > 3) { total -= x; } else if (x < 0) { continue; } else { break; } }
    Node* inner_if = b.n(ir::If{.cond = b.cmp("<", b.name(x), b.i(0)), .then = b.block({b.n(ir::Continue{})}), .orelse = b.block({b.n(ir::Break{})})});
    Node* big_if = b.n(ir::If{.cond = b.cmp(">", b.name(x), b.i(3)), .then = b.block({b.expr_stmt(b.assign(b.name(total), b.name(x), "-"))}), .orelse = inner_if});
    Node* each = b.n(ir::ForEach{.var = b.local(x), .iterable = b.name(xs), .body = b.block({big_if})});
    // while (total > 100) total = total / 2;
    Node* loop = b.n(ir::While{.cond = b.cmp(">", b.name(total), b.i(100)), .body = b.expr_stmt(b.assign(b.name(total), b.bin("/", b.name(total), b.i(2))))});
    // switch (total) { case 1: case 2: total++; break; case 3: total--; default: return 0; }
    ir::Case case12{.labels = {b.i(1), b.i(2)}, .body = {b.expr_stmt(b.n(ir::Unary{.op = "++", .x = b.name(total), .postfix = true}, t_int())), b.n(ir::Break{})}};
    ir::Case case3{.labels = {b.i(3)}, .body = {b.expr_stmt(b.n(ir::Unary{.op = "--", .x = b.name(total), .postfix = true}, t_int()))}};
    ir::Case dflt{.is_default = true, .body = {b.ret(b.i(0))}};
    Node* sw = b.n(ir::Switch{.expr = b.name(total), .cases = {b.n(std::move(case12)), b.n(std::move(case3)), b.n(std::move(dflt))}});
    // try { total = int.Parse(name); } catch (FormatException e) { throw; } finally { total++; }
    const auto parse = ir::make_sym(ir::SymKind::Method, "Parse", "System.Int32", true);
    parse->params = {t_str()};
    Node* parse_call = b.n(ir::Call{.target = b.n(ir::TypeName{}, ir::class_type("System.Int32")), .sym = parse, .args = {b.name(name)}}, t_int());
    ir::Catch handler{.sym = e, .types = {ir::class_type("System.FormatException")}, .body = b.block({b.n(ir::Throw{})})};
    Node* try_stmt = b.n(ir::Try{.body = b.block({b.expr_stmt(b.assign(b.name(total), parse_call))}),
                                 .catches = {b.n(std::move(handler))},
                                 .finally_body = b.block({b.expr_stmt(b.n(ir::Unary{.op = "++", .x = b.name(total), .postfix = true}, t_int()))})});
    Node* body = b.block({b.local(total, b.i(0)), for_stmt, each, loop, sw, try_stmt, b.ret(b.name(total))});
    Node* run = b.method("Run", t_int(), {b.param(xs), b.param(name)}, body);
    const std::string expected =
        "// Translated by CodeBreaker from Prog.cs (csharp).\n"
        "using System;\n"
        "\n"
        "public class Prog\n"
        "{\n"
        "    static int Run(int[] xs, string name)\n"
        "    {\n"
        "        int total = 0;\n"
        "        for (int i = 0; i < xs.Length; i++)\n"
        "        {\n"
        "            total += xs[i];\n"
        "        }\n"
        "        foreach (int x in xs)\n"
        "        {\n"
        "            if (x > 3)\n"
        "            {\n"
        "                total -= x;\n"
        "            }\n"
        "            else if (x < 0)\n"
        "            {\n"
        "                continue;\n"
        "            }\n"
        "            else\n"
        "            {\n"
        "                break;\n"
        "            }\n"
        "        }\n"
        "        while (total > 100)\n"
        "        {\n"
        "            total = total / 2;\n"
        "        }\n"
        "        switch (total)\n"
        "        {\n"
        "            case 1:\n"
        "            case 2:\n"
        "                total++;\n"
        "                break;\n"
        "            case 3:\n"
        "                total--;\n"
        "                goto default;\n"
        "            default:\n"
        "                return 0;\n"
        "        }\n"
        "        try\n"
        "        {\n"
        "            total = int.Parse(name);\n"
        "        }\n"
        "        catch (FormatException e)\n"
        "        {\n"
        "            throw;\n"
        "        }\n"
        "        finally\n"
        "        {\n"
        "            total++;\n"
        "        }\n"
        "        return total;\n"
        "    }\n"
        "}\n";
    CB_CHECK_EQ(write_one(b.unit("Prog.cs", {b.klass("Prog", {run})})), expected);
});

CB_TEST("dotnet.l2_csharp.stmt.names_declared_in_inner_blocks_are_renamed", [] {
    B b;
    // { int a = 1; } int a = 2;  -- Java's scope of a local begins at its declaration; C#'s is its whole block
    const auto first = b.local_sym("a", t_int());
    const auto second = b.local_sym("a", t_int());
    Node* body = b.block({b.block({b.local(first, b.i(1))}), b.local(second, b.i(2))});
    const std::string file = write_one(b.unit("R.cs", {b.klass("Prog", {b.method("M", nullptr, {}, body)})}));
    CB_CHECK(l2u_holds(file, "            int a2 = 1;\n"));
    CB_CHECK(l2u_holds(file, "        int a = 2;\n"));
});

CB_TEST("dotnet.l2_csharp.stmt.hoisted_initialisers_are_put_back", [] {
    B b;
    // var __t1 = new List<int>(); __t1.Add(1); __t1.Add(2); Use(__t1);   ->   Use(new List<int> { 1, 2 });
    const TypeP list = ir::class_type("System.Collections.Generic.List", {t_int()});
    const auto t1 = b.local_sym("__t1", list);
    const auto ctor = ir::make_sym(ir::SymKind::Ctor, "List", "System.Collections.Generic.List", false);
    const auto add = ir::make_sym(ir::SymKind::Method, "Add", "System.Collections.Generic.List", false);
    add->params = {t_int()};
    const auto use = ir::make_sym(ir::SymKind::Method, "Use", "Prog", true);
    use->params = {list};
    const auto adds = [&](int v) { return b.expr_stmt(b.n(ir::Call{.target = b.name(t1), .sym = add, .args = {b.i(v)}}, ir::bool_type())); };
    Node* use_call = b.expr_stmt(b.n(ir::Call{.sym = use, .args = {b.name(t1)}}));
    // and an object initialiser: var __t2 = new Pt(); __t2.X = 1; __t2.Y = 2; Show(__t2);
    const TypeP pt = ir::class_type("Pt");
    const auto t2 = b.local_sym("__t2", pt);
    const auto fx = ir::make_sym(ir::SymKind::Property, "X", "Pt", false, t_int());
    const auto fy = ir::make_sym(ir::SymKind::Property, "Y", "Pt", false, t_int());
    const auto sets = [&](const ir::SymP& f, int v) { return b.expr_stmt(b.assign(b.n(ir::Select{.obj = b.name(t2), .sym = f}, t_int()), b.i(v))); };
    const auto show = ir::make_sym(ir::SymKind::Method, "Show", "Prog", true);
    show->params = {pt};
    Node* show_call = b.expr_stmt(b.n(ir::Call{.sym = show, .args = {b.name(t2)}}));
    Node* body = b.block({b.local(t1, b.n(ir::New{.sym = ctor}, list)), adds(1), adds(2), use_call,
                          b.local(t2, b.n(ir::New{.sym = ir::make_sym(ir::SymKind::Ctor, "Pt", "Pt", false)}, pt)), sets(fx, 1), sets(fy, 2), show_call});
    const std::string file = write_one(b.unit("I.cs", {b.klass("Prog", {b.method("M", nullptr, {}, body)})}));
    CB_CHECK(l2u_holds(file, "        Use(new List<int> { 1, 2 });\n"));
    CB_CHECK(l2u_holds(file, "        Show(new Pt\n        {\n            X = 1,\n            Y = 2\n        });\n"));
    CB_CHECK(file.find("__t") == std::string::npos);
    CB_CHECK(l2u_holds(file, "using System.Collections.Generic;"));
});

CB_TEST("dotnet.l2_csharp.stmt.using_lock_assert_and_labels", [] {
    B b;
    const auto res = b.local_sym("r", ir::class_type("System.IO.StringReader"));
    const auto lockee = b.local_sym("gate", t_obj());
    const auto ctor = ir::make_sym(ir::SymKind::Ctor, "StringReader", "System.IO.StringReader", false);
    ctor->params = {t_str()};
    Node* make = b.n(ir::New{.sym = ctor, .args = {b.str("")}}, ir::class_type("System.IO.StringReader"));
    Node* using_stmt = b.n(ir::Try{.resources = {b.local(res, make)}, .body = b.block({b.n(ir::Empty{})})});
    Node* lock_stmt = b.n(ir::Sync{.lock = b.name(lockee), .body = b.block({b.n(ir::Empty{})})});
    Node* assert_stmt = b.n(ir::Assert{.cond = b.flag(true)});
    Node* labelled = b.n(ir::Labeled{.label = "again", .body = b.n(ir::Empty{})});
    Node* go = b.n(ir::Goto{.label = "again"});
    Node* note = b.n(ir::Comment{.text = "done"});
    const std::string file = write_one(b.unit("U.cs", {b.klass("Prog", {b.method("M", nullptr, {}, b.block({using_stmt, lock_stmt, assert_stmt, labelled, go, note}))})}));
    CB_CHECK(l2u_holds(file, "using (var r = new StringReader(\"\"))\n        {\n            ;\n        }\n"));
    CB_CHECK(l2u_holds(file, "lock (gate)\n        {\n            ;\n        }\n"));
    CB_CHECK(l2u_holds(file, "Debug.Assert(true);"));
    CB_CHECK(l2u_holds(file, "using System.Diagnostics;"));
    CB_CHECK(l2u_holds(file, "        again:\n        ;\n        goto again;\n        // done\n"));
});

CB_TEST("dotnet.l2_csharp.stmt.a_bare_statement_list_is_a_block", [] {
    B b;
    // the decompiler's and the Python front end's bodies are lists, not Blocks
    const auto v = b.local_sym("v", t_int());
    ir::MethodDecl m;
    m.sym = ir::make_sym(ir::SymKind::Method, "M", "Prog", true);
    m.name = "M";
    m.kind = "method";
    m.mods = {"static"};
    m.body = std::vector<Node*>{b.local(v, b.i(1)), b.ret()};
    ir::If branch{.cond = b.flag(true), .then = std::vector<Node*>{b.ret()}};
    ir::MethodDecl m2;
    m2.sym = ir::make_sym(ir::SymKind::Method, "N", "Prog", true);
    m2.name = "N";
    m2.kind = "method";
    m2.body = std::vector<Node*>{b.n(std::move(branch))};
    const std::string file = write_one(b.unit("L.cs", {b.klass("Prog", {b.n(std::move(m)), b.n(std::move(m2))})}));
    CB_CHECK(l2u_holds(file, "    static void M()\n    {\n        int v = 1;\n        return;\n    }\n"));
    CB_CHECK(l2u_holds(file, "    void N()\n    {\n        if (true)\n        {\n            return;\n        }\n    }\n"));
});

// ---- declarations -------------------------------------------------------------------------------------------------------------

CB_TEST("dotnet.l2_csharp.decl.enums_interfaces_records_delegates_generics", [] {
    B b;
    // enum Color { Red = 0, Green = 1 }
    ir::EnumConst red{.sym = ir::make_sym(ir::SymKind::EnumConst, "Red", "Color", true), .value = 0};
    ir::EnumConst green{.sym = ir::make_sym(ir::SymKind::EnumConst, "Green", "Color", true), .value = 1};
    Node* color = b.klass("Color", {b.n(std::move(red)), b.n(std::move(green))}, {"public"}, "enum");
    // interface IShape { double Area(); }
    Node* area = b.method("Area", t_dbl(), {}, nullptr, {});
    Node* shape = b.klass("IShape", {area}, {"public"}, "interface");
    // delegate int Op(int a, int b);
    const auto da = b.param_sym("a", t_int(), "Op.Invoke");
    const auto db = b.param_sym("b", t_int(), "Op.Invoke");
    Node* op = b.klass("Op", {b.method("Invoke", t_int(), {b.param(da), b.param(db)}, nullptr, {})}, {"public"}, "delegate");
    // public struct Box<T> where T : class { }
    ir::TypeParam tp{.name = "T", .flags = {"class"}};
    Node* box = b.klass("Box", {}, {"public"}, "struct");
    auto* box_decl = ir::get<ir::ClassDecl>(box);
    box_decl->typeparams = {"T"};
    box_decl->tparams = {b.n(std::move(tp))};
    // class Derived : Base, IShape { }
    Node* derived = b.klass("Derived", {}, {"public"});
    auto* d = ir::get<ir::ClassDecl>(derived);
    d->extends = ir::class_type("Base");
    d->implements = {ir::class_type("IShape")};
    const std::string file = write_one(b.unit("D.cs", {color, shape, op, box, derived}));
    const std::string expected =
        "// Translated by CodeBreaker from D.cs (csharp).\n"
        "public enum Color\n"
        "{\n"
        "    Red = 0,\n"
        "    Green = 1\n"
        "}\n"
        "public interface IShape\n"
        "{\n"
        "    double Area();\n"
        "}\n"
        "public delegate int Op(int a, int b);\n"
        "public struct Box<T> where T : class\n"
        "{\n"
        "}\n"
        "public class Derived : Base, IShape\n"
        "{\n"
        "}\n";
    CB_CHECK_EQ(file, expected);
});

CB_TEST("dotnet.l2_csharp.decl.operators_properties_and_constructors", [] {
    B b;
    // public static V operator +(V a, V b) { return a; }  public int this[int i] { get; set; }  public int N { get; set; } = 3;
    const auto pa = b.param_sym("a", ir::class_type("V"), "V.op");
    const auto pb = b.param_sym("b", ir::class_type("V"), "V.op");
    Node* plus = b.method("op_Addition", ir::class_type("V"), {b.param(pa), b.param(pb)}, b.block({b.ret(b.name(pa))}), {"public", "static", "operator"});
    const auto pi = b.param_sym("i", t_int(), "V.this");
    ir::MethodDecl get{};
    get.kind = "get";
    ir::MethodDecl set{};
    set.kind = "set";
    ir::Property indexer{.sym = ir::make_sym(ir::SymKind::Property, "Item", "V", false, t_int()), .type = t_int(), .mods = {"public"},
                         .params = {b.param(pi)}, .accessors = {b.n(std::move(get)), b.n(std::move(set))}};
    ir::MethodDecl get2{};
    get2.kind = "get";
    ir::MethodDecl set2{};
    set2.kind = "set";
    ir::Property auto_prop{.sym = ir::make_sym(ir::SymKind::Property, "N", "V", false, t_int()), .type = t_int(), .mods = {"public"},
                           .accessors = {b.n(std::move(get2)), b.n(std::move(set2))}, .init = b.i(3)};
    // a constructor chaining to the base: public V(int n) : base(n) { }
    const auto pn = b.param_sym("n", t_int(), "V..ctor");
    const auto base_ctor = ir::make_sym(ir::SymKind::Ctor, "Base", "Base", false);
    base_ctor->params = {t_int()};
    Node* chain = b.n(ir::CtorCall{.kind = "base", .sym = base_ctor, .args = {b.name(pn)}});
    Node* ctor = b.method("V", nullptr, {b.param(pn)}, b.block({chain}), {"public"}, "ctor");
    const std::string file = write_one(b.unit("V.cs", {b.klass("V", {plus, b.n(std::move(indexer)), b.n(std::move(auto_prop)), ctor})}));
    CB_CHECK(l2u_holds(file, "    public static V operator +(V a, V b) => a;\n"));
    CB_CHECK(l2u_holds(file, "    public int this[int i] { get; set; }\n"));
    CB_CHECK(l2u_holds(file, "    public int N { get; set; } = 3;\n"));
    CB_CHECK(l2u_holds(file, "    public V(int n)\n        : base(n)\n    {\n    }\n"));
});

// ---- what is declined ----------------------------------------------------------------------------------------------------------

CB_TEST("dotnet.l2_csharp.decline.a_method_with_an_unsupported_node_is_a_gap", [] {
    B b;
    ir::Unsupported await{.kind = "await", .raw = "await is not carried"};
    Node* bad = b.method("Later", nullptr, {}, b.block({b.expr_stmt(b.n(std::move(await)))}));
    Node* good = b.method("Fine", nullptr, {}, b.block({b.ret()}));
    Node* units[] = {b.unit("G.cs", {b.klass("Prog", {bad, good})})};
    cs::L2CSharpProgram p(units);
    const std::string& text = p.files().at(0).text;
    CB_CHECK(l2u_holds(text, "    static void Later()\n    {\n        throw new System.NotSupportedException(\"CB-GAP: await is not carried\");\n    }\n"));
    CB_CHECK(l2u_holds(text, "    static void Fine()\n    {\n        return;\n    }\n"));   // the next method is written
    CB_CHECK_EQ(p.ledger().size(), std::size_t{1});
    CB_CHECK(p.ledger().at(0) == (cs::L2Gap{"Prog.Later", "await", "await is not carried"}));
});

CB_TEST("dotnet.l2_csharp.decline.constructs_without_a_csharp_form", [] {
    B b;
    const auto v = b.local_sym("v", t_int());
    ir::LocalClass lc{.decl = b.klass("Prog.Local", {})};
    Node* anon = b.n(ir::New{.sym = ir::make_sym(ir::SymKind::Ctor, "Runnable", "Runnable", false), .body = b.klass("Prog.1", {})}, ir::class_type("Runnable"));
    Node* inner_new = b.n(ir::New{.sym = ir::make_sym(ir::SymKind::Ctor, "Inner", "Outer.Inner", false), .outer = b.n(ir::This{}, ir::class_type("Outer"))}, ir::class_type("Outer.Inner"));
    ir::MachineFragment fragment{.reason = "hand-written"};
    ir::Binary weird{.op = "<=>", .left = b.name(v), .right = b.i(1)};
    ir::MethodRef ctor_ref{.target = b.n(ir::TypeName{}, ir::class_type("Pair")), .sym = ir::make_sym(ir::SymKind::Ctor, "Pair", "Pair", false), .mode = "new"};
    Node* bodies[] = {b.block({b.n(std::move(lc))}), b.block({b.local(b.local_sym("a", ir::class_type("Runnable")), anon)}), b.block({b.local(b.local_sym("b", ir::class_type("Outer.Inner")), inner_new)}),
                      b.block({b.n(std::move(fragment))}), b.block({b.expr_stmt(b.n(std::move(weird), t_int()))}),
                      b.block({b.local(b.local_sym("c", t_obj()), b.n(std::move(ctor_ref), t_obj()))})};
    std::vector<Node*> methods;
    int k = 0;
    for (Node* body : bodies) methods.push_back(b.method("M" + std::to_string(k++), nullptr, {}, body));
    Node* units[] = {b.unit("X.cs", {b.klass("Prog", methods)})};
    cs::L2CSharpProgram p(units);
    p.files();
    std::vector<std::string> kinds;
    for (const cs::L2Gap& g : p.ledger()) kinds.push_back(g.kind);
    const std::vector<std::string> want = {"local-class", "anonymous-class", "inner-class", "machine", "operator", "method-ref"};
    CB_CHECK(kinds == want);
    CB_CHECK(p.ledger().at(3).why == "machine code (hand-written)");
});

CB_TEST("dotnet.l2_csharp.decline.a_class_that_cannot_be_written_is_a_comment_and_the_rest_is_kept", [] {
    B b;
    Node* odd = b.klass("Odd", {}, {"public"}, "annotation");
    Node* fine = b.klass("Fine", {});
    Node* units[] = {b.unit("O.cs", {odd, fine})};
    cs::L2CSharpProgram p(units);
    const std::string& text = p.files().at(0).text;
    CB_CHECK(l2u_holds(text, "// CB-GAP: Odd was not translated: a annotation\n"));
    CB_CHECK(l2u_holds(text, "public class Fine\n{\n}\n"));
    CB_CHECK_EQ(p.ledger().size(), std::size_t{1});
    CB_CHECK_EQ(p.ledger()[0].kind, "class-kind");
    // a field whose initialiser is declined keeps the field
    B c;
    const auto fld = ir::make_sym(ir::SymKind::Field, "F", "Prog", false, t_int());
    Node* field = c.n(ir::FieldDecl{.sym = fld, .type = t_int(), .init = c.n(ir::Unsupported{.kind = "stackalloc", .raw = "stackalloc"}, t_int())});
    const std::string file = write_one(c.unit("F.cs", {c.klass("Prog", {field})}));
    CB_CHECK(l2u_holds(file, "    int F = default(int)/* CB-GAP: stackalloc */;\n"));
});

CB_TEST("dotnet.l2_csharp.decline.only_unit_nodes_are_written", [] {
    B b;
    Node* units[] = {b.klass("Prog", {})};
    CB_CHECK_THROWS_AS(cb::UsageError, (void)cs::L2CSharpProgram(units));
});

// ---- the program: files, names, runtime, Java units -------------------------------------------------------------------------

namespace {

const cb::java::apimap::Registry& l2u_registry() {
    static const cb::java::apimap::Registry reg = cb::java::apimap::Registry::load(cb::testing::data_path("../config/java/api_maps"));
    return reg;
}

cs::L2CSharpOptions l2u_java_options() {
    cs::L2CSharpOptions options;
    options.java_registry = &l2u_registry();
    options.runtime_dir = cb::testing::data_path("../config/translate/dotnet/runtime");
    return options;
}

}  // namespace

CB_TEST("dotnet.l2_csharp.program.a_java_unit_is_restated_by_the_rule_set", [] {
    // package pkg; public class App implements java.io.Serializable {
    //     public static void main(String msg) { System.out.println(msg); }  void helper() { return; }  static void scale() { double d = 2.5; } }
    B b;
    const auto msg = b.param_sym("msg", ir::class_type("java.lang.String"), "pkg.App.main");
    const auto println = ir::make_sym(ir::SymKind::Method, "println", "java.io.PrintStream", false);
    println->params = {ir::class_type("java.lang.String")};
    const auto out = ir::make_sym(ir::SymKind::Field, "out", "java.lang.System", true, ir::class_type("java.io.PrintStream"));
    Node* print = b.expr_stmt(b.n(ir::Call{.target = b.n(ir::Select{.obj = b.n(ir::TypeName{}, ir::class_type("java.lang.System")), .sym = out}, ir::class_type("java.io.PrintStream")),
                                           .sym = println, .args = {b.name(msg)}}));
    Node* main_m = b.method("main", nullptr, {b.param(msg)}, b.block({print}), {"public", "static"});
    Node* helper = b.method("helper", nullptr, {}, b.block({b.ret()}), {});
    const auto d = ir::make_sym(ir::SymKind::Local, "d", "pkg.App.scale", false, t_dbl());
    Node* with_double = b.method("scale", nullptr, {}, b.block({b.local(d, b.n(ir::Lit{.value = 2.5}, t_dbl()))}), {"static"});
    Node* app = b.klass("pkg.App", {main_m, helper, with_double});
    ir::get<ir::ClassDecl>(app)->implements = {ir::class_type("java.io.Serializable")};
    Node* units[] = {b.unit("pkg/App.java", {app}, "java")};
    cs::L2CSharpProgram p(units, l2u_java_options());
    const auto& files = p.files();
    CB_CHECK(!files.empty());
    if (files.empty()) return;
    CB_CHECK_EQ(files[0].name, std::string("App.cs"));
    const std::string expected =
        "// Translated by CodeBreaker from App.java (java).\n"
        "using System;\n"
        "\n"
        "namespace pkg\n"
        "{\n"
        "    public class App\n"                                  // (Serializable has no C# form: dropped)
        "    {\n"
        "        public static void Main(string msg)\n"           // main is Main
        "        {\n"
        "            Console.WriteLine(JavaRt.Str(msg));\n"       // System.out.println, a null String printed as Java prints it
        "        }\n"
        "\n"
        "        internal virtual void helper()\n"                // Java's package access, dispatched virtually
        "        {\n"
        "            return;\n"
        "        }\n"
        "\n"
        "        internal static void scale()\n"
        "        {\n"
        "            double d = 2.5;\n"
        "        }\n"
        "    }\n"
        "}\n";
    CB_CHECK_EQ(files[0].text, expected);
    CB_CHECK(p.ledger().empty());
    CB_CHECK(p.java_rules());
    // the units given are not changed: the rule set restates a copy
    CB_CHECK(ir::get<ir::MethodDecl>(main_m)->name == std::optional<std::string>("main"));
});

CB_TEST("dotnet.l2_csharp.program.java_with_no_registry_is_declined_class_by_class", [] {
    B b;
    Node* units[] = {b.unit("pkg/Plain.java", {b.klass("pkg.Plain", {b.method("f", nullptr, {}, b.block({b.ret()}))})}, "java"),
                     b.unit("pkg/Other.cs", {b.klass("Other", {})}, "csharp")};
    cs::L2CSharpProgram p(units);
    CB_CHECK_EQ(p.files().size(), std::size_t{2});
    CB_CHECK(l2u_holds(p.files()[0].text, "// CB-GAP: Plain was not translated: no rule set for java source in the plain lowering yet\n"));
    CB_CHECK(l2u_holds(p.files()[1].text, "public class Other\n{\n}\n"));   // the C# unit is unaffected
    CB_CHECK_EQ(p.ledger().size(), std::size_t{1});
    if (!p.ledger().empty()) CB_CHECK_EQ(p.ledger()[0].kind, std::string("library"));
    CB_CHECK(!p.java_rules());
});

CB_TEST("dotnet.l2_csharp.program.file_names_and_writing", [] {
    B b;
    Node* units[] = {b.unit("a/Same.cs", {b.klass("A", {})}), b.unit("b/Same.cs", {b.klass("B", {})}), b.unit("", {b.klass("C", {})})};
    cs::L2CSharpProgram p(units);
    const auto& files = p.files();
    CB_CHECK_EQ(files.size(), std::size_t{3});
    if (files.size() != 3) return;
    CB_CHECK_EQ(files[0].name, std::string("Same.cs"));
    CB_CHECK_EQ(files[1].name, std::string("Same_2.cs"));   // not one file over the other
    CB_CHECK_EQ(files[2].name, std::string("Unit.cs"));
    cb::fs::ScopedTempDir dir("cb-l2cs-test");
    const auto paths = p.write(dir.path() / "out" / "deep");
    CB_CHECK_EQ(paths.size(), std::size_t{3});
    if (paths.size() == 3) CB_CHECK_EQ(cb::fs::read_text(paths[1]), files[1].text);
    // a runtime file that is not there is an IoError, not a silent omission
    B c;
    const auto d = ir::make_sym(ir::SymKind::Local, "d", "R.f", false, t_dbl());
    const auto s = ir::make_sym(ir::SymKind::Local, "s", "R.f", false, ir::class_type("java.lang.String"));
    Node* text = c.n(ir::Concat{.parts = {c.str("d = "), c.name(d)}}, ir::class_type("java.lang.String"));   // (a double as text: JavaRt.Str)
    Node* f = c.method("f", nullptr, {}, c.block({c.local(d, c.n(ir::Lit{.value = 0.5}, t_dbl())), c.local(s, text)}), {"static"});
    Node* units2[] = {c.unit("R.java", {c.klass("R", {f})}, "java")};
    cs::L2CSharpOptions options = l2u_java_options();
    options.runtime_dir = dir.path();
    cs::L2CSharpProgram q(units2, options);
    CB_CHECK_THROWS_AS(cb::Error, q.files());
});

CB_TEST("dotnet.l2_csharp.program.class_index", [] {
    B b;
    Node* base = b.klass("p.Base", {b.method("f", nullptr, {}, b.block({}), {"public"})});
    Node* nested = b.klass("p.Derived.In", {});
    ir::get<ir::ClassDecl>(nested)->outer = "p.Derived";
    Node* derived = b.klass("p.Derived", {b.method("f", nullptr, {}, b.block({}), {"public"}), nested});
    ir::get<ir::ClassDecl>(derived)->extends = ir::class_type("p.Base");
    Node* units[] = {b.unit("P.cs", {base, derived})};
    cs::L2CSharpProgram p(units);
    CB_CHECK(p.class_named("p.Base") == ir::get<ir::ClassDecl>(base));
    CB_CHECK(p.class_named("p.Derived.In") == ir::get<ir::ClassDecl>(nested));   // a nested class is indexed too
    CB_CHECK(p.class_named("p.Nope") == nullptr);
    CB_CHECK(p.base_of(*ir::get<ir::ClassDecl>(derived)) == ir::get<ir::ClassDecl>(base));
    CB_CHECK(p.base_of(*ir::get<ir::ClassDecl>(base)) == nullptr);
});

// ---- Java units: what the rule set says in the text (the javac front end's units of translate/testdata/csharp_java) ----------------
//
// The whole text of each program is pinned by dotnet.l2_csharp.java.* (l2_syntax_builder_test.cpp); these name what a reader looks for.

namespace {

struct L2uUnits {
    cb::Arena arena;
    std::vector<Node*> units;
};

std::unique_ptr<L2uUnits> l2u_load(const std::string& name) {
    auto u = std::make_unique<L2uUnits>();
    const cb::Json docs = cb::json::load(cb::testing::data_path("translate/testdata/csharp_java/" + name + ".l2.json"));
    for (const cb::Json& d : docs) u->units.push_back(cb::translate::l2::from_json(u->arena, d));
    return u;
}

bool l2u_runtime_file(const std::string& name) {
    return name == "JavaRt.cs" || name == "JavaCollections.cs" || name == "JavaStreams.cs" || name == "JavaCharTypes.cs";
}

std::string l2u_text(cs::L2CSharpProgram& p) {
    std::string out;
    for (const cs::L2File& f : p.files()) {
        if (!l2u_runtime_file(f.name)) out += "// ==== " + f.name + " ====\n" + f.text;
    }
    return out;
}

bool l2u_has_runtime(cs::L2CSharpProgram& p) {
    return std::any_of(p.files().begin(), p.files().end(), [](const cs::L2File& f) { return l2u_runtime_file(f.name); });
}

}  // namespace

CB_TEST("dotnet.l2_csharp.java.semantics_in_the_text", [] {
    auto u = l2u_load("features");
    cs::L2CSharpProgram p(u->units, l2u_java_options());
    const std::string text = l2u_text(p);
    const auto has = [&](std::string_view s) {
        const bool found = text.find(s) != std::string::npos;
        if (!found) std::fprintf(stderr, "not in the text: %.*s\n", static_cast<int>(s.size()), s.data());
        return found;
    };
    CB_CHECK(has("public override string ToString()"));                  // toString is ToString
    CB_CHECK(has("public override bool Equals(object o)"));
    CB_CHECK(has("public override int GetHashCode()"));
    CB_CHECK(has("public virtual int CompareTo(Shape o)"));              // Comparable
    CB_CHECK(has("class Resource : IDisposable"));                       // AutoCloseable
    CB_CHECK(has("public virtual void Dispose()"));                      // close
    CB_CHECK(has("public abstract string greet(string name);"));         // the abstract class declares what its interface leaves
    CB_CHECK(has("public override string greet(string name)"));
    CB_CHECK(has("static Features()\n"));                                // static initialisers in one static constructor
    CB_CHECK(has("internal const int LIMIT = 10;"));
    CB_CHECK(has("(object)a == (object)b"));                             // Java compares the references
    CB_CHECK(has("JavaRt.Str(d)"));                                      // a double as text
    CB_CHECK(has("JLib.CloseResource(r, j_thrown1);"));   // (a close that throws must not replace the exception on its way, as C#'s using lets it)
    CB_CHECK(has("public static void Main(string[] args)"));
    CB_CHECK(p.ledger().empty());
    CB_CHECK(l2u_has_runtime(p));
});

CB_TEST("dotnet.l2_csharp.java.what_it_once_declined", [] {
    {   // a box that may be null is a nullable value, and only that one
        auto u = l2u_load("boxed_null");
        cs::L2CSharpProgram p(u->units, l2u_java_options());
        const std::string written = l2u_text(p);
        CB_CHECK(p.ledger().empty());
        CB_CHECK(written.find("int? absent()") != std::string::npos);
        CB_CHECK(written.find("int x = 5;") != std::string::npos);
    }
    {   // an enum of objects is a class
        auto u = l2u_load("enum_object");
        cs::L2CSharpProgram p(u->units, l2u_java_options());
        CB_CHECK(p.ledger().empty());
        CB_CHECK(l2u_text(p).find(": JEnum") != std::string::npos);
    }
    {   // an exception made from a cause keeps it
        auto u = l2u_load("exception_cause");
        cs::L2CSharpProgram p(u->units, l2u_java_options());
        CB_CHECK(p.ledger().empty());
        CB_CHECK(l2u_text(p).find("new Exception(") != std::string::npos);
    }
    {   // the instance around an inner class is a field, given to the constructor
        auto u = l2u_load("inner_outer");
        cs::L2CSharpProgram p(u->units, l2u_java_options());
        CB_CHECK(p.ledger().empty());
        CB_CHECK(l2u_text(p).find("outer_") != std::string::npos);
    }
    {   // a HashMap is Java's table, so a walk of it is in Java's order
        auto u = l2u_load("hash_order");
        cs::L2CSharpProgram p(u->units, l2u_java_options());
        CB_CHECK(p.ledger().empty());
        CB_CHECK(l2u_text(p).find("JHashMap<") != std::string::npos);
    }
});

CB_TEST("dotnet.l2_csharp.java.a_unit_of_another_language_is_not_restated", [] {
    // only Java units go through the rule set: a C# unit (with the registry given) is written as C#'s own
    auto u = l2u_load("calc");
    for (Node* unit : u->units) ir::get<ir::Unit>(unit)->lang = "csharp";
    cs::L2CSharpProgram p(u->units, l2u_java_options());
    CB_CHECK(!p.files().empty());
    CB_CHECK(!l2u_has_runtime(p));
});

CB_TEST("dotnet.l2_csharp.java.a_java_unit_without_the_registry_is_declined", [] {
    auto u = l2u_load("calc");
    cs::L2CSharpProgram p(u->units);
    CB_CHECK(!p.ledger().empty());
    if (!p.ledger().empty()) CB_CHECK_EQ(p.ledger()[0].kind, std::string("library"));
});

// The speed of the writing, the rule set included: the best of 20, printed.
CB_TEST("dotnet.l2_csharp.java.timing", [] {
    for (const char* name : {"modern", "calc", "features"}) {
        auto u = l2u_load(name);
        std::int64_t best_us = std::numeric_limits<std::int64_t>::max();
        for (int rep = 0; rep < 20; ++rep) {
            const auto t0 = std::chrono::steady_clock::now();
            cs::L2CSharpProgram p(u->units, l2u_java_options());
            (void)p.files();
            best_us = std::min(best_us, std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - t0).count());
        }
        std::fprintf(stderr, "l2_csharp java %s: %lld us\n", name, static_cast<long long>(best_us));
    }
});
