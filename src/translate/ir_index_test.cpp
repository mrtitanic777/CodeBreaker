// Tests of translate/ir_index: the derived state of a tree (parents, uses, jumps) on a small lowered-shape method, and that a rewrite
// through the index is seen by the next question. The CIL lane checks the same counts against the ILAst's own on every method of its
// test assemblies (translate.cil.round_trip.*).
#include "translate/ir_index.hpp"

#include "core/testing.hpp"

namespace ir = cb::translate::ir;
using ir::Node;

namespace {

// IL_0000: x = 1; goto IL_0005      IL_0005: if (x) goto IL_0000; f(&x); break IL_0000 (leaves the region)
struct Shape {
    cb::Arena arena;
    ir::SymP x = ir::make_sym(ir::SymKind::Local, "x", std::nullopt, false, ir::i32_type());
    Node* store = nullptr;
    Node* load = nullptr;
    Node* address = nullptr;
    Node* first = nullptr;
    Node* second = nullptr;
    Node* region = nullptr;
    Node* body = nullptr;

    Shape() {
        store = ir::make(arena, ir::Assign{.target = ir::make(arena, ir::Name{.sym = x}), .value = ir::make(arena, ir::Lit{.value = std::int64_t{1}})});
        Node* b0 = ir::make(arena, ir::Block{.stmts = {ir::make(arena, ir::ExprStmt{.expr = store}), ir::make(arena, ir::Goto{.label = "IL_0005"})}});
        first = ir::make(arena, ir::Labeled{.label = "IL_0000", .body = b0});
        load = ir::make(arena, ir::Name{.sym = x});
        address = ir::make(arena, ir::Unary{.op = "&", .x = ir::make(arena, ir::Name{.sym = x})});
        Node* call = ir::make(arena, ir::Call{.args = {address}});
        Node* b1 = ir::make(arena, ir::Block{.stmts = {ir::make(arena, ir::If{.cond = load, .then = ir::make(arena, ir::Goto{.label = "IL_0000"})}),
                                                       ir::make(arena, ir::ExprStmt{.expr = call}), ir::make(arena, ir::Break{.label = "IL_0000"})}});
        second = ir::make(arena, ir::Labeled{.label = "IL_0005", .body = b1});
        region = ir::make(arena, ir::Region{.blocks = {first, second}});
        body = ir::make(arena, ir::Block{.stmts = {region}});
    }
};

}  // namespace

CB_TEST("translate.ir_index.parents_uses_and_jumps", [] {
    Shape s;
    ir::TreeIndex index(s.body);
    CB_CHECK(index.place(s.region).parent == s.body);
    CB_CHECK(index.place(s.second).parent == s.region);
    CB_CHECK_EQ(index.place(s.second).index, 1u);
    CB_CHECK(index.place(s.body).parent == nullptr);
    const ir::TreeIndex::Uses& u = index.uses(s.x.get());
    CB_CHECK_EQ(u.stores.size(), std::size_t{1});
    CB_CHECK(u.stores.front() == s.store);
    CB_CHECK_EQ(u.loads.size(), std::size_t{1});
    CB_CHECK(u.loads.front() == s.load);
    CB_CHECK_EQ(u.addresses.size(), std::size_t{1});
    CB_CHECK(u.addresses.front() == s.address);
    CB_CHECK(!index.is_single_definition(s.x.get()));   // its address is taken
    CB_CHECK(index.block("IL_0005") == s.second);
    CB_CHECK_EQ(index.incoming("IL_0000"), 2u);   // the region's entry and the goto
    CB_CHECK_EQ(index.incoming("IL_0005"), 1u);
    CB_CHECK_EQ(index.leaving("IL_0000"), 1u);
    s.x->entry_used = true;
    CB_CHECK_EQ(index.store_count(s.x.get()), std::size_t{2});
});

CB_TEST("translate.ir_index.sees_its_own_rewrites", [] {
    Shape s;
    ir::TreeIndex index(s.body);
    CB_CHECK_EQ(index.uses(s.x.get()).loads.size(), std::size_t{1});
    const std::uint64_t before = index.generation();
    // the load becomes a constant: the variable has no load left
    Node* one = ir::make(s.arena, ir::Lit{.value = std::int64_t{1}});
    index.replace(s.load, one);
    CB_CHECK(index.generation() != before);
    CB_CHECK(index.uses(s.x.get()).loads.empty());
    CB_CHECK(index.place(one).parent != nullptr);
    CB_CHECK(!index.contains(s.load));
    // a statement put first in the second block: a store more, and the block's statements moved up by one
    Node* again = ir::make(s.arena, ir::Assign{.target = ir::make(s.arena, ir::Name{.sym = s.x}), .value = ir::make(s.arena, ir::Lit{.value = std::int64_t{2}})});
    Node* block = std::get<Node*>(ir::get<ir::Labeled>(s.second)->body);
    index.insert_stmt(block, 0, ir::make(s.arena, ir::ExprStmt{.expr = again}));
    CB_CHECK_EQ(index.uses(s.x.get()).stores.size(), std::size_t{2});
    CB_CHECK(index.uses(s.x.get()).stores.back() == again);
    CB_CHECK_EQ(index.incoming("IL_0000"), 2u);
});
