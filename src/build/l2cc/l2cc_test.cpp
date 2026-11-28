// The first vertical slice of the compiler middle on L2 (docs/L2_COMPILER.md): a function held as an L2 tree compiles through
// x86cc's front end, by its semantic actions, to the same object as the C text of the same function.
#include "build/l2cc/l2cc.hpp"

#include <iostream>

#include "core/arena.hpp"
#include "core/fs.hpp"
#include "core/testing.hpp"

namespace stdfs = std::filesystem;
namespace ir = cb::translate::ir;
namespace l2cc = cb::build::l2cc;
namespace x86cc = cb::build::x86cc;

namespace {

// A little tree builder: parameters, names, literals, operators and returns of one function.
struct Fn {
    cb::Arena& arena;
    std::vector<ir::Node*> params;
    std::map<std::string, ir::SymP> syms;

    ir::Node* param(const std::string& name, const ir::TypeP& type) {
        ir::SymP s = ir::make_sym(ir::SymKind::Param, name, std::nullopt, false, type);
        syms[name] = s;
        params.push_back(ir::make(arena, ir::Param{.sym = s, .type = type}));
        return params.back();
    }
    ir::Node* name(const std::string& n) { return ir::make(arena, ir::Name{.sym = syms.at(n)}, syms.at(n)->type); }
    ir::Node* lit(std::int64_t v, const ir::TypeP& type) { return ir::make(arena, ir::Lit{.value = v}, type); }
    ir::Node* bin(const std::string& op, ir::Node* l, ir::Node* r, const ir::TypeP& type) {
        return ir::make(arena, ir::Binary{.op = op, .left = l, .right = r}, type);
    }
    ir::Node* ret(ir::Node* e = nullptr) { return ir::make(arena, ir::Return{.expr = e}); }
    ir::Node* function(const std::string& n, const ir::TypeP& result, std::vector<ir::Node*> body) {
        ir::Node* block = ir::make(arena, ir::Block{.stmts = std::move(body)});
        return ir::make(arena, ir::MethodDecl{.sym = ir::make_sym(ir::SymKind::Function, n), .name = n, .kind = "function",
                                              .params = std::move(params), .ret = result, .body = block});
    }
};

const ir::TypeP i16 = ir::prim_type(ir::Prim::I16);
const ir::TypeP i32 = ir::prim_type(ir::Prim::I32);
const ir::TypeP u32 = ir::prim_type(ir::Prim::U32);
const ir::TypeP i64 = ir::prim_type(ir::Prim::I64);

// The unit compiled both ways, one after the other in one folder, from a file of the same name and time (the object names
// the source by its full path and carries its time): the objects must be the same bytes.
void same_object(const std::string& id, x86cc::Target target, const std::string& c_text, const l2cc::Unit& unit,
                 const std::vector<std::string>& args, bool expect_same = true) {
    const stdfs::path folder = cb::testing::data_path("../work/l2cc-test") / id;
    std::error_code ec;
    stdfs::remove_all(folder, ec);
    stdfs::create_directories(folder);

    const l2cc::Result l2 = l2cc::compile(target, unit, "u.c", args, folder);
    CB_CHECK(l2.declined.empty());
    for (const auto& d : l2.declined) std::cout << "  declined " << d << '\n';
    CB_CHECK_EQ(l2.failed, std::string());
    CB_CHECK_EQ(l2.run.exit_code, 0);
    const auto from_l2 = cb::fs::read_file(folder / "u.obj");
    const auto time = stdfs::last_write_time(folder / "u.c");
    stdfs::rename(folder / "u.obj", folder / "l2.obj");
    stdfs::rename(folder / "u.c", folder / "l2.c");

    cb::fs::write_text(folder / "u.c", c_text);
    stdfs::last_write_time(folder / "u.c", time);
    std::vector<std::string> all = args;
    all.push_back("u.c");
    const x86cc::Run text = x86cc::compile(target, all, folder);
    CB_CHECK_EQ(text.exit_code, 0);
    const auto from_text = cb::fs::read_file(folder / "u.obj");
    CB_CHECK(!from_text.empty());
    if (!expect_same) {   // (the comparison has teeth: another body is another object)
        CB_CHECK(from_text != from_l2);
        stdfs::remove_all(folder, ec);
        return;
    }
    CB_CHECK_EQ(l2.run.err, text.err);
    CB_CHECK(from_text == from_l2);
    if (from_text == from_l2) stdfs::remove_all(folder, ec);   // (kept when they differ, to be looked at)
}

}  // namespace

CB_TEST("build.l2cc.add_is_the_same_object_as_its_c", [] {
    if (!x86cc::available()) return;
    cb::Arena arena;
    Fn f{arena};
    f.param("a", i32);
    f.param("b", i32);
    ir::Node* add = f.function("add", i32, {f.ret(f.bin("+", f.name("a"), f.name("b"), i32))});
    CB_CHECK_EQ(l2cc::head(add, x86cc::Target::i386), std::string("int add(int a, int b)"));
    same_object("add", x86cc::Target::i386, "int add(int a, int b)\n{\n    return a + b;\n}\n", {.functions = {add}},
                {"-bt=nt", "-zq"});
    same_object("add_not_sub", x86cc::Target::i386, "int add(int a, int b)\n{\n    return a - b;\n}\n", {.functions = {add}},
                {"-bt=nt", "-zq"}, false);
});

CB_TEST("build.l2cc.operators_are_the_same_objects_as_their_c", [] {
    if (!x86cc::available()) return;
    cb::Arena arena;
    std::vector<ir::Node*> fns;
    {   // a - b * 3, with the optimizer on
        Fn f{arena};
        f.param("a", i32);
        f.param("b", i32);
        fns.push_back(f.function("lin", i32, {f.ret(f.bin("-", f.name("a"), f.bin("*", f.name("b"), f.lit(3, i32), i32), i32))}));
    }
    {   // (x << 2) ^ y on unsigned operands
        Fn f{arena};
        f.param("x", u32);
        f.param("y", u32);
        fns.push_back(f.function("mix", u32, {f.ret(f.bin("^", f.bin("<<", f.name("x"), f.lit(2, i32), u32), f.name("y"), u32))}));
    }
    {   // a / b + a % b, and a comparison
        Fn f{arena};
        f.param("a", i32);
        f.param("b", i32);
        fns.push_back(f.function("dm", i32, {f.ret(f.bin("+", f.bin("/", f.name("a"), f.name("b"), i32),
                                                         f.bin("%", f.name("a"), f.name("b"), i32), i32))}));
    }
    {
        Fn f{arena};
        f.param("a", i32);
        f.param("b", i32);
        fns.push_back(f.function("lt", i32, {f.ret(f.bin("<", f.name("a"), f.name("b"), i32))}));
    }
    {   // 64-bit arithmetic (a run-time call on wcc386)
        Fn f{arena};
        f.param("p", i64);
        f.param("q", i64);
        fns.push_back(f.function("mul64", i64, {f.ret(f.bin("*", f.name("p"), f.name("q"), i64))}));
    }
    {   // a void function's return
        Fn f{arena};
        fns.push_back(f.function("nothing", ir::void_type(), {f.ret()}));
    }
    const std::string c =
        "int lin(int a, int b)\n{\n    return a - b * 3;\n}\n"
        "unsigned int mix(unsigned int x, unsigned int y)\n{\n    return (x << 2) ^ y;\n}\n"
        "int dm(int a, int b)\n{\n    return a / b + a % b;\n}\n"
        "int lt(int a, int b)\n{\n    return a < b;\n}\n"
        "long long mul64(long long p, long long q)\n{\n    return p * q;\n}\n"
        "void nothing(void)\n{\n    return;\n}\n";
    same_object("ops", x86cc::Target::i386, c, {.functions = fns}, {"-bt=nt", "-zq"});
    same_object("ops_ox", x86cc::Target::i386, c, {.functions = fns}, {"-bt=nt", "-zq", "-ox", "-s"});
});

CB_TEST("build.l2cc.a_16_bit_function_is_the_same_object_as_its_c", [] {
    if (!x86cc::available()) return;
    cb::Arena arena;
    Fn f{arena};
    f.param("a", i16);
    f.param("b", i16);
    ir::Node* add = f.function("add", i16, {f.ret(f.bin("+", f.name("a"), f.bin("&", f.name("b"), f.lit(255, i16), i16), i16))});
    CB_CHECK_EQ(l2cc::head(add, x86cc::Target::i86), std::string("int add(int a, int b)"));
    same_object("add16", x86cc::Target::i86, "int add(int a, int b)\n{\n    return a + (b & 255);\n}\n", {.functions = {add}},
                {"-zq", "-ml"});
});

CB_TEST("build.l2cc.what_is_not_lowered_yet_is_declined", [] {
    cb::Arena arena;
    Fn f{arena};
    f.param("a", i32);
    ir::Node* neg = f.function("neg", i32, {f.ret(f.lit(-1, i32))});
    CB_CHECK(!l2cc::declines(neg, x86cc::Target::i386).empty());
    Fn g{arena};
    g.param("a", i32);
    ir::Node* call = g.function("c", i32, {g.ret(ir::make(arena, ir::Unary{.op = "-", .x = g.name("a")}, i32))});
    CB_CHECK(!l2cc::declines(call, x86cc::Target::i386).empty());
});
