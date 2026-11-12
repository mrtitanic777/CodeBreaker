// Tests of the pass tools: matching shapes (names by symbol name, calls on this.<field>, null tests), walking a unit's methods
// and statement lists, rebuilding a subtree bottom-up, and the builders. Hand-built trees: the tools know nothing of a source
// language.
//
// DIVERGES from rewrite.py: `methods` skips a unit's types that are not classes (the Python took a method or field there for a
// class and failed with AttributeError); `replace` and `is_name`/`is_call` accept a null node (a null slot has no children and
// matches nothing).
#include "cb/translate/rewrite.hpp"

#include "cb/core/arena.hpp"
#include "cb/core/testing.hpp"

namespace {

using cb::Arena;
namespace ir = cb::translate::ir;
namespace rw = cb::translate::rewrite;
using ir::Node;

ir::SymP rt_sym(ir::SymKind kind, std::string name, ir::TypeP type = {}) { return ir::make_sym(kind, std::move(name), std::nullopt, false, std::move(type)); }
Node* rt_name(Arena& a, const ir::SymP& s) { return ir::make(a, ir::Name{.sym = s}, s->type); }
Node* rt_int(Arena& a, std::int64_t v) { return ir::make(a, ir::Lit{.value = v}, ir::i32_type()); }
Node* rt_call(Arena& a, Node* target, const std::string& method, std::vector<Node*> args = {}) {
    return ir::make(a, ir::Call{.target = target, .sym = rt_sym(ir::SymKind::Method, method), .args = std::move(args)});
}
Node* rt_this_field(Arena& a, const std::string& field) {
    return ir::make(a, ir::Select{.obj = ir::make(a, ir::This{}), .sym = rt_sym(ir::SymKind::Field, field)});
}
Node* rt_stmt(Arena& a, Node* e) { return ir::make(a, ir::ExprStmt{.expr = e}); }
Node* rt_block(Arena& a, std::vector<Node*> stmts) { return ir::make(a, ir::Block{.stmts = std::move(stmts)}); }
Node* rt_method(Arena& a, const std::string& name, Node* body) {
    ir::MethodDecl m;
    m.sym = rt_sym(ir::SymKind::Method, name);
    m.name = name;
    m.body = body;
    return ir::make(a, std::move(m));
}
Node* rt_class(Arena& a, const std::string& name, std::vector<Node*> members) {
    ir::ClassDecl c;
    c.name = name;
    c.members = std::move(members);
    return ir::make(a, std::move(c));
}
Node* rt_unit(Arena& a, std::vector<Node*> types) {
    ir::Unit u;
    u.types = std::move(types);
    return ir::make(a, std::move(u));
}

CB_TEST("translate.rewrite.strip_and_names", [] {
    Arena a;
    ir::SymP v = rt_sym(ir::SymKind::Local, "var1");
    Node* n = rt_name(a, v);
    Node* wrapped = ir::make(a, ir::Paren{.x = ir::make(a, ir::Paren{.x = n})});
    CB_CHECK(rw::strip(wrapped) == n);
    CB_CHECK(rw::strip(n) == n);
    CB_CHECK(rw::strip(nullptr) == nullptr);
    CB_CHECK(rw::is_name(wrapped, *v));
    // symbols are compared by name: each use carries its own symbol
    CB_CHECK(rw::is_name(rt_name(a, rt_sym(ir::SymKind::Local, "var1")), *v));
    CB_CHECK(!rw::is_name(rt_name(a, rt_sym(ir::SymKind::Local, "var2")), *v));
    CB_CHECK(!rw::is_name(rt_int(a, 1), *v));
    CB_CHECK(!rw::is_name(nullptr, *v));
});

CB_TEST("translate.rewrite.calls_and_null_tests", [] {
    Arena a;
    ir::SymP v = rt_sym(ir::SymKind::Local, "var7");
    Node* on_field = rt_call(a, rt_this_field(a, "soundPoolSounds"), "getRandomSoundFromSoundPool");
    CB_CHECK(rw::is_call(on_field, "getRandomSoundFromSoundPool"));
    CB_CHECK(rw::is_call(on_field, "getRandomSoundFromSoundPool", "soundPoolSounds"));
    CB_CHECK(!rw::is_call(on_field, "getRandomSoundFromSoundPool", "soundPoolMusic"));
    CB_CHECK(!rw::is_call(on_field, "other"));
    // a call on something that is not this.<field>
    Node* on_local = rt_call(a, rt_name(a, rt_sym(ir::SymKind::Local, "x")), "getRandomSound");
    CB_CHECK(rw::is_call(on_local, "getRandomSound"));
    CB_CHECK(!rw::is_call(on_local, "getRandomSound", "soundPoolMusic"));
    CB_CHECK(!rw::is_call(rt_int(a, 3), "getRandomSound"));
    // an own-method call has no target, so it is not on any field
    CB_CHECK(!rw::is_call(rt_call(a, nullptr, "f"), "f", "field"));
    CB_CHECK(rw::is_call(rt_call(a, nullptr, "f"), "f"));

    Node* ne = rw::binary(a, "!=", rt_name(a, v), rw::null_lit(a), ir::bool_type());
    Node* eq = rw::binary(a, "==", rt_name(a, v), rw::null_lit(a), ir::bool_type());
    CB_CHECK(rw::is_null_test(ne, *v));
    CB_CHECK(!rw::is_null_test(ne, *v, "=="));
    CB_CHECK(rw::is_null_test(eq, *v, "=="));
    CB_CHECK(!rw::is_null_test(rw::binary(a, "!=", rt_name(a, v), rt_int(a, 0), ir::bool_type()), *v));
    CB_CHECK(!rw::is_null_test(ne, *rt_sym(ir::SymKind::Local, "other")));
    CB_CHECK(!rw::is_null_test(rt_int(a, 0), *v));
});

CB_TEST("translate.rewrite.methods_and_statement_lists", [] {
    Arena a;
    ir::SymP v = rt_sym(ir::SymKind::Local, "x");
    Node* inner_if = ir::make(a, ir::If{.cond = rt_name(a, v), .then = rt_block(a, {rt_stmt(a, rt_call(a, nullptr, "g"))})});
    Node* body = rt_block(a, {rt_stmt(a, rt_call(a, nullptr, "f")), inner_if});
    Node* method = rt_method(a, "bar", body);
    Node* unit = rt_unit(a, {rt_class(a, "Foo", {method})});
    const auto ms = rw::methods({unit});
    CB_CHECK_EQ(ms.size(), std::size_t{1});
    CB_CHECK_EQ(*ms[0].cls->name, "Foo");
    CB_CHECK_EQ(*ms[0].method->name, "bar");
    CB_CHECK(ms[0].method_node == method);
    // the method body's own block, then the block of the if: both hold statements
    const auto lists = rw::stmt_lists(body);
    CB_CHECK_EQ(lists.size(), std::size_t{2});
    CB_CHECK(lists[0] == &ir::get<ir::Block>(body)->stmts);
    CB_CHECK_EQ(lists[0]->size(), std::size_t{2});
    CB_CHECK_EQ(lists[1]->size(), std::size_t{1});
    // a case holds statements too, after the switch's own subject
    Node* sw = ir::make(a, ir::Switch{.expr = rt_name(a, v), .cases = {ir::make(a, ir::Case{.body = {rt_stmt(a, rt_call(a, nullptr, "h"))}})}});
    CB_CHECK_EQ(rw::stmt_lists(rt_block(a, {sw})).size(), std::size_t{2});
    // the lists are the tree's own: a statement added to one is in the tree
    lists[0]->push_back(rt_stmt(a, rt_call(a, nullptr, "added")));
    CB_CHECK_EQ(ir::get<ir::Block>(body)->stmts.size(), std::size_t{3});
    // a unit with no types, a method without a body, a type that is no class: nothing
    CB_CHECK(rw::methods({rt_unit(a, {})}).empty());
    CB_CHECK(rw::methods({}).empty());
    ir::MethodDecl bodiless;
    bodiless.name = "abstract_one";
    CB_CHECK(rw::methods({rt_unit(a, {rt_class(a, "A", {ir::make(a, std::move(bodiless))})})}).empty());
    CB_CHECK(rw::methods({rt_unit(a, {method})}).empty());   // (a method at the top of a unit is not in a class)
    CB_CHECK(rw::stmt_lists(nullptr).empty());
});

CB_TEST("translate.rewrite.nested_classes_come_after_their_outer", [] {
    Arena a;
    Node* inner = rt_class(a, "Inner", {rt_method(a, "inner_method", rt_block(a, {}))});
    Node* outer = rt_class(a, "Outer", {inner, rt_method(a, "outer_method", rt_block(a, {}))});
    const auto ms = rw::methods({rt_unit(a, {outer})});
    CB_CHECK_EQ(ms.size(), std::size_t{2});
    CB_CHECK_EQ(*ms[0].method->name, "outer_method");   // a class's own methods come before its nested classes'
    CB_CHECK_EQ(*ms[0].cls->name, "Outer");
    CB_CHECK_EQ(*ms[1].method->name, "inner_method");
    CB_CHECK_EQ(*ms[1].cls->name, "Inner");
});

CB_TEST("translate.rewrite.replace_bottom_up", [] {
    Arena a;
    ir::SymP v = rt_sym(ir::SymKind::Local, "var1");
    Node* inner = rt_call(a, nullptr, "f", {rt_name(a, v), rt_int(a, 5)});
    Node* stmt = rt_stmt(a, inner);
    // every int literal becomes 0; the root itself is not replaced
    int visited = 0;
    rw::replace(stmt, [&](Node* n) -> Node* {
        ++visited;
        if (ir::is<ir::Lit>(n)) return rt_int(a, 0);
        return nullptr;
    });
    CB_CHECK_EQ(visited, 3);   // the call, the name, the literal
    const ir::Call* c = ir::get<ir::Call>(inner);
    CB_CHECK(std::get<std::int64_t>(ir::get<ir::Lit>(c->args[1])->value) == 0);
    CB_CHECK(ir::is<ir::Name>(c->args[0]));
    CB_CHECK(rw::mentions(stmt, *v));
    CB_CHECK(!rw::mentions(stmt, *rt_sym(ir::SymKind::Local, "other")));
    rw::replace(nullptr, [](Node*) -> Node* { return nullptr; });   // nothing to do, nothing to fail

    // a replacement is put in the parent's slot and is not visited again; children are visited before their parent
    Node* sum = rw::binary(a, "+", rt_int(a, 1), rw::binary(a, "*", rt_int(a, 2), rt_int(a, 3), ir::i32_type()), ir::i32_type());
    Node* ret = ir::make(a, ir::Return{.expr = sum});
    std::vector<std::string> order;
    rw::replace(ret, [&](Node* n) -> Node* {
        if (const ir::Binary* b = ir::get<ir::Binary>(n)) {
            order.push_back(b->op);
            if (b->op == "*") return rt_int(a, 6);   // folded: the `+` then sees a literal where the product was
        }
        return nullptr;
    });
    CB_CHECK_EQ(order.size(), std::size_t{2});
    CB_CHECK_EQ(order[0], "*");
    CB_CHECK_EQ(order[1], "+");
    CB_CHECK(ir::is<ir::Lit>(ir::get<ir::Binary>(ir::get<ir::Return>(ret)->expr)->right));
    // slots that hold a statement list are rebuilt too
    Node* two = rt_block(a, {rt_stmt(a, rt_int(a, 1)), rt_stmt(a, rt_int(a, 2))});
    rw::replace(two, [&](Node* n) -> Node* { return ir::is<ir::Lit>(n) ? rt_int(a, 9) : nullptr; });
    for (Node* s : ir::get<ir::Block>(two)->stmts) CB_CHECK_EQ(std::get<std::int64_t>(ir::get<ir::Lit>(ir::get<ir::ExprStmt>(s)->expr)->value), std::int64_t{9});
});

CB_TEST("translate.rewrite.builders", [] {
    Arena a;
    Node* cond = rw::binary(a, "!=", rw::name_of(a, rt_sym(ir::SymKind::Local, "w", ir::i32_type())), rw::null_lit(a), ir::bool_type());
    Node* wrapped = rw::if_then(a, cond, rt_stmt(a, rt_call(a, nullptr, "f")));
    const ir::If* i = ir::get<ir::If>(wrapped);
    CB_CHECK(i->cond == cond);
    const auto stmts = rw::block_or_single(i->then);
    CB_CHECK_EQ(stmts.size(), std::size_t{1});
    CB_CHECK(ir::is<ir::ExprStmt>(stmts[0]));
    // a statement that is already a block is not wrapped twice
    Node* b = rt_block(a, {rt_stmt(a, rt_call(a, nullptr, "g")), rt_stmt(a, rt_call(a, nullptr, "h"))});
    const ir::If* j = ir::get<ir::If>(rw::if_then(a, cond, b));
    CB_CHECK_EQ(rw::block_or_single(j->then).size(), std::size_t{2});
    // a single statement that is no block is its own list; a bare list of statements is itself
    Node* one = rt_stmt(a, rt_int(a, 1));
    CB_CHECK_EQ(rw::block_or_single(ir::Suite{one}).size(), std::size_t{1});
    CB_CHECK_EQ(rw::block_or_single(ir::Suite{std::vector<Node*>{one, one}}).size(), std::size_t{2});
    CB_CHECK(rw::block_or_single(ir::Suite{static_cast<Node*>(nullptr)}).empty());
    CB_CHECK(rw::name_of(a, rt_sym(ir::SymKind::Local, "w", ir::i32_type()))->type);   // the type comes from the symbol
    CB_CHECK(rw::name_of(a, rt_sym(ir::SymKind::Local, "w", ir::i32_type()), ir::bool_type())->type == ir::bool_type());   // or is given
    CB_CHECK(rw::true_lit(a)->type == ir::bool_type());
    CB_CHECK(std::get<bool>(ir::get<ir::Lit>(rw::true_lit(a))->value));
    CB_CHECK(rw::null_lit(a)->type == ir::null_type());
    CB_CHECK(rw::lit_int(a, 8, ir::i32_type())->type == ir::i32_type());
    CB_CHECK_EQ(std::get<std::int64_t>(ir::get<ir::Lit>(rw::lit_int(a, 8, ir::i32_type()))->value), std::int64_t{8});
});

}  // namespace
