#include "cb/core/arena.hpp"
#include "cb/core/testing.hpp"
#include "cb/java/projects/minecraft/ir_edit.hpp"
#include "cb/java/projects/minecraft/ir_test_support.hpp"

namespace {

using namespace cb;
namespace ir = cb::translate::ir;
namespace edit = cb::java::projects::minecraft::ir_edit;
namespace ts = cb::java::projects::minecraft::ir_test_support;

CB_TEST("java.minecraft.ir_edit.strip_and_names", [] {
    Arena a;
    ir::SymP v = ts::sym(ir::SymKind::Local, "var1");
    ir::Node* n = ts::name(a, v);
    ir::Node* wrapped = ir::make(a, ir::Paren{.x = ir::make(a, ir::Paren{.x = n})});
    CB_CHECK(edit::strip(wrapped) == n);
    CB_CHECK(edit::strip(n) == n);
    CB_CHECK(edit::strip(nullptr) == nullptr);
    CB_CHECK(edit::is_name(wrapped, *v));
    // symbols are compared by name: each use carries its own symbol
    CB_CHECK(edit::is_name(ts::name(a, ts::sym(ir::SymKind::Local, "var1")), *v));
    CB_CHECK(!edit::is_name(ts::name(a, ts::sym(ir::SymKind::Local, "var2")), *v));
    CB_CHECK(!edit::is_name(ts::int_lit(a, 1), *v));
});

CB_TEST("java.minecraft.ir_edit.calls_and_null_tests", [] {
    Arena a;
    ir::SymP v = ts::sym(ir::SymKind::Local, "var7");
    ir::Node* on_field = ts::call(a, ts::this_field(a, "soundPoolSounds"), "getRandomSoundFromSoundPool");
    CB_CHECK(edit::is_call(on_field, "getRandomSoundFromSoundPool"));
    CB_CHECK(edit::is_call(on_field, "getRandomSoundFromSoundPool", "soundPoolSounds"));
    CB_CHECK(!edit::is_call(on_field, "getRandomSoundFromSoundPool", "soundPoolMusic"));
    CB_CHECK(!edit::is_call(on_field, "other"));
    // a call on something that is not this.<field>
    ir::Node* on_local = ts::call(a, ts::name(a, ts::sym(ir::SymKind::Local, "x")), "getRandomSound");
    CB_CHECK(edit::is_call(on_local, "getRandomSound"));
    CB_CHECK(!edit::is_call(on_local, "getRandomSound", "soundPoolMusic"));
    CB_CHECK(!edit::is_call(ts::int_lit(a, 3), "getRandomSound"));

    ir::Node* ne = ts::binary(a, "!=", ts::name(a, v), ts::null_lit(a));
    ir::Node* eq = ts::binary(a, "==", ts::name(a, v), ts::null_lit(a));
    CB_CHECK(edit::is_null_test(ne, *v));
    CB_CHECK(!edit::is_null_test(ne, *v, "=="));
    CB_CHECK(edit::is_null_test(eq, *v, "=="));
    CB_CHECK(!edit::is_null_test(ts::binary(a, "!=", ts::name(a, v), ts::int_lit(a, 0)), *v));
    CB_CHECK(!edit::is_null_test(ne, *ts::sym(ir::SymKind::Local, "other")));
});

CB_TEST("java.minecraft.ir_edit.methods_and_statement_lists", [] {
    Arena a;
    ir::SymP v = ts::sym(ir::SymKind::Local, "x");
    ts::Program p = ts::program(a, "Foo", "bar", {ts::expr_stmt(a, ts::call(a, nullptr, "f")),
                                                  ts::if_(a, ts::name(a, v), ts::block(a, {ts::expr_stmt(a, ts::call(a, nullptr, "g"))}))});
    const auto ms = edit::methods({p.unit});
    CB_CHECK_EQ(ms.size(), std::size_t{1});
    CB_CHECK_EQ(*ms[0].cls->name, "Foo");
    CB_CHECK_EQ(*ms[0].method->name, "bar");
    // the method body's own block, then the block of the if: both hold statements
    const auto lists = edit::stmt_lists(p.body_node);
    CB_CHECK_EQ(lists.size(), std::size_t{2});
    CB_CHECK(lists[0] == &p.body->stmts);
    CB_CHECK_EQ(lists[0]->size(), std::size_t{2});
    CB_CHECK_EQ(lists[1]->size(), std::size_t{1});
    // a unit with no types and a method without a body give nothing
    ir::Node* empty_unit = ir::make(a, ir::Unit{});
    CB_CHECK(edit::methods({empty_unit}).empty());
    CB_CHECK(edit::methods({}).empty());
});

CB_TEST("java.minecraft.ir_edit.nested_classes_come_after_their_outer", [] {
    Arena a;
    ir::MethodDecl outer_m;
    outer_m.name = "outer_method";
    outer_m.body = ts::block(a, {});
    ir::MethodDecl inner_m;
    inner_m.name = "inner_method";
    inner_m.body = ts::block(a, {});
    ir::ClassDecl inner;
    inner.name = "Inner";
    inner.members = {ir::make(a, std::move(inner_m))};
    ir::ClassDecl outer;
    outer.name = "Outer";
    outer.members = {ir::make(a, std::move(inner)), ir::make(a, std::move(outer_m))};
    ir::Unit u;
    u.types = {ir::make(a, std::move(outer))};
    const auto ms = edit::methods({ir::make(a, std::move(u))});
    CB_CHECK_EQ(ms.size(), std::size_t{2});
    CB_CHECK_EQ(*ms[0].method->name, "outer_method");   // a class's own methods come before its nested classes'
    CB_CHECK_EQ(*ms[0].cls->name, "Outer");
    CB_CHECK_EQ(*ms[1].method->name, "inner_method");
    CB_CHECK_EQ(*ms[1].cls->name, "Inner");
});

CB_TEST("java.minecraft.ir_edit.replace_bottom_up", [] {
    Arena a;
    ir::SymP v = ts::sym(ir::SymKind::Local, "var1");
    ir::Node* inner = ts::call(a, nullptr, "f", {ts::name(a, v), ts::int_lit(a, 5)});
    ir::Node* stmt = ts::expr_stmt(a, inner);
    // every int literal becomes 0; the root itself is not replaced
    int visited = 0;
    edit::replace(stmt, [&](ir::Node* n) -> ir::Node* {
        ++visited;
        if (ir::is<ir::Lit>(n)) return ts::int_lit(a, 0);
        return nullptr;
    });
    CB_CHECK_EQ(visited, 3);   // the call, the name, the literal
    const ir::Call* c = ir::get<ir::Call>(inner);
    const ir::Lit* l = ir::get<ir::Lit>(c->args[1]);
    CB_CHECK(std::get<std::int64_t>(l->value) == 0);
    CB_CHECK(ir::is<ir::Name>(c->args[0]));
    CB_CHECK(edit::mentions(stmt, *v));
    CB_CHECK(!edit::mentions(stmt, *ts::sym(ir::SymKind::Local, "other")));
});

CB_TEST("java.minecraft.ir_edit.builders", [] {
    Arena a;
    ir::Node* cond = edit::binary(a, "!=", edit::name_of(a, ts::sym(ir::SymKind::Local, "w", ir::i32_type())), edit::null_lit(a),
                                  ir::bool_type());
    ir::Node* wrapped = edit::if_then(a, cond, ts::expr_stmt(a, ts::call(a, nullptr, "f")));
    const ir::If* i = ir::get<ir::If>(wrapped);
    CB_CHECK(i->cond == cond);
    const auto stmts = edit::block_or_single(i->then);
    CB_CHECK_EQ(stmts.size(), std::size_t{1});
    CB_CHECK(ir::is<ir::ExprStmt>(stmts[0]));
    // a statement that is already a block is not wrapped twice
    ir::Node* b = ts::block(a, {ts::expr_stmt(a, ts::call(a, nullptr, "g")), ts::expr_stmt(a, ts::call(a, nullptr, "h"))});
    const ir::If* j = ir::get<ir::If>(edit::if_then(a, cond, b));
    CB_CHECK_EQ(edit::block_or_single(j->then).size(), std::size_t{2});
    CB_CHECK(edit::name_of(a, ts::sym(ir::SymKind::Local, "w", ir::i32_type()))->type);   // the type comes from the symbol
    CB_CHECK(edit::true_lit(a)->type == ir::bool_type());
    CB_CHECK(edit::lit_int(a, 8, ir::i32_type())->type == ir::i32_type());
});

}  // namespace
