// Tests of cb/translate/ir: the type model (what `==` and the helpers say of a type), symbols, and trees -- built the way a
// front end builds them (here the Java method `int f(int a) { if (a > 0) { return a * 2; } return -a; }`, and a C#
// property, a lambda, a switch expression and a `new` with all its slots) and read back the way a pass reads them: walked
// in source order, rewired in place, cloned. Every kind of node is made by name and named back, which is how L2 documents
// are read.
//
// Design notes: node slots are typed members (so an unknown slot is a compile error); the details of a type are fields of its
// kind (a pointer's space, a function's parameters); a list slot that is empty is the same as one that is absent; a type is
// written with `to_text`; integer constants are 64-bit.
#include "translate/ir.hpp"

#include <iostream>
#include <set>
#include <unordered_set>

#include "core/testing.hpp"

namespace ir = cb::translate::ir;
using ir::Node;

namespace {

ir::TypeP int_type() { return ir::i32_type(); }

// int f(int a) { if (a > 0) { return a * 2; } return -a; }
struct Method {
    cb::Arena arena;
    ir::SymP f = ir::make_sym(ir::SymKind::Method, "f", "Calc", false);
    ir::SymP a = ir::make_sym(ir::SymKind::Param, "a", "Calc.f", false, ir::i32_type());
    Node* decl = nullptr;
    Node* if_stmt = nullptr;
    Node* times = nullptr;
    Node* ret_neg = nullptr;

    Method() {
        auto name_a = [&] { return ir::make(arena, ir::Name{.sym = a}, ir::i32_type(), 1, 4); };
        Node* zero = ir::make(arena, ir::Lit{.value = std::int64_t{0}, .text = "0"}, ir::i32_type());
        Node* two = ir::make(arena, ir::Lit{.value = std::int64_t{2}, .text = "2"}, ir::i32_type());
        Node* test = ir::make(arena, ir::Binary{.op = ">", .left = name_a(), .right = zero}, ir::bool_type(), 2, 8);
        times = ir::make(arena, ir::Binary{.op = "*", .left = name_a(), .right = two}, ir::i32_type());
        Node* ret = ir::make(arena, ir::Return{.expr = times}, {}, 3, 8);
        Node* then = ir::make(arena, ir::Block{.stmts = {ret}});
        if_stmt = ir::make(arena, ir::If{.cond = test, .then = then}, {}, 2, 4);
        Node* neg = ir::make(arena, ir::Unary{.op = "-", .x = name_a()}, ir::i32_type());
        ret_neg = ir::make(arena, ir::Return{.expr = neg}, {}, 5, 4);
        Node* body = ir::make(arena, ir::Block{.stmts = {if_stmt, ret_neg}});
        Node* param = ir::make(arena, ir::Param{.sym = a, .type = ir::i32_type()});
        ir::MethodDecl m;
        m.sym = f;
        m.name = "f";
        m.kind = "method";
        m.params = {param};
        m.ret = ir::i32_type();
        m.body = body;
        m.mods = {"public"};
        decl = ir::make(arena, std::move(m), {}, 1, 0);
    }
};

std::vector<std::string_view> kinds_of(const std::vector<Node*>& nodes) {
    std::vector<std::string_view> out;
    for (const Node* n : nodes) out.push_back(ir::kind_name(*n));
    return out;
}

}  // namespace

CB_TEST("translate.ir.types.equality_is_by_what_they_say", [] {
    CB_CHECK(ir::i32_type() == ir::prim_type(ir::Prim::I32));   // not the same object
    CB_CHECK(ir::i32_type().get() != ir::prim_type(ir::Prim::I32).get());
    CB_CHECK(ir::i32_type() != ir::prim_type(ir::Prim::U32));
    CB_CHECK(ir::prim_type(ir::Prim::Char, true) != ir::prim_type(ir::Prim::Char, false));
    CB_CHECK(ir::prim_type(ir::Prim::Char, true) != ir::prim_type(ir::Prim::Char));       // signedness stated or not
    CB_CHECK(ir::prim_type(ir::Prim::U16, std::nullopt, ir::CharEncoding::Utf16) != ir::prim_type(ir::Prim::U16));
    CB_CHECK(ir::TypeP() == ir::TypeP());                                               // absent equals absent
    CB_CHECK(ir::TypeP() != ir::i32_type());
    // nested and with arguments
    const ir::TypeP list_a = ir::class_type("java.util.List", {ir::class_type("java.lang.String")});
    const ir::TypeP list_b = ir::class_type("java.util.List", {ir::class_type("java.lang.String")});
    const ir::TypeP list_c = ir::class_type("java.util.List", {ir::class_type("java.lang.Integer")});
    CB_CHECK(list_a == list_b);
    CB_CHECK(list_a != list_c);
    CB_CHECK(list_a->hash() == list_b->hash());
    CB_CHECK(list_a != ir::class_type("java.util.List"));
    // a class's facts are part of it
    ir::ClassType nullable;
    nullable.name = "System.String";
    nullable.nullable = true;
    CB_CHECK(ir::make_type(nullable) != ir::class_type("System.String"));
    // pointers: space and segment
    const ir::TypeP near_ds = ir::pointer_to(ir::i32_type(), ir::AddressSpace::Near, ir::SegmentRef{ir::SegmentRegister::Ds});
    CB_CHECK(near_ds == ir::pointer_to(ir::i32_type(), ir::AddressSpace::Near, ir::SegmentRef{ir::SegmentRegister::Ds}));
    CB_CHECK(near_ds != ir::pointer_to(ir::i32_type(), ir::AddressSpace::Near, ir::SegmentRef{ir::SegmentRegister::Es}));
    CB_CHECK(near_ds != ir::pointer_to(ir::i32_type(), ir::AddressSpace::Far));
    CB_CHECK(ir::pointer_to(ir::i32_type()) == ir::pointer_to(ir::i32_type(), ir::AddressSpace::Flat));
    CB_CHECK(ir::pointer_to(ir::i32_type())->hash() == ir::pointer_to(ir::i32_type())->hash());
});

CB_TEST("translate.ir.types.functions_and_records", [] {
    auto fn = [](ir::CallingConvention conv, std::int64_t pops) {
        ir::FnType f;
        f.conv = conv;
        f.distance = ir::AddressSpace::Far;
        f.pops = pops;
        f.params = {{std::string("hwnd"), ir::pointer_to(ir::void_type()), ir::StackSlot{4}},
                    {std::nullopt, ir::i32_type(), ir::RegisterPair{{"dx", "ax"}}}};
        f.result.type = ir::i32_type();
        f.result.at = ir::Register{"ax"};
        f.preserves = {"si", "di"};
        return ir::make_type(f);
    };
    CB_CHECK(fn(ir::CallingConvention::Pascal, 6) == fn(ir::CallingConvention::Pascal, 6));
    CB_CHECK(fn(ir::CallingConvention::Pascal, 6) != fn(ir::CallingConvention::Cdecl, 6));
    CB_CHECK(fn(ir::CallingConvention::Pascal, 6) != fn(ir::CallingConvention::Pascal, 8));
    ir::FnPtrType callback;
    callback.space = ir::AddressSpace::Far;
    callback.fn = fn(ir::CallingConvention::Pascal, 6);
    CB_CHECK(ir::make_type(callback) == ir::make_type(callback));
    ir::RecordType point;
    point.name = "POINT";
    point.size = 4;
    point.role = ir::RecordRole::Struct;
    point.fields = {{"x", 0, ir::prim_type(ir::Prim::I16), false}, {"y", 2, ir::prim_type(ir::Prim::I16), false}};
    ir::RecordType other = point;
    CB_CHECK(ir::make_type(point) == ir::make_type(other));
    other.fields[1].offset = 4;
    CB_CHECK(ir::make_type(point) != ir::make_type(other));
    CB_CHECK_EQ(ir::make_type(point)->simple(), "POINT");
    CB_CHECK_EQ(ir::make_type(point)->kind(), "record");
});

CB_TEST("translate.ir.types.kinds_names_and_text", [] {
    CB_CHECK_EQ(ir::i32_type()->kind(), "i32");
    CB_CHECK_EQ(ir::void_type()->kind(), "void");
    CB_CHECK_EQ(ir::null_type()->kind(), "null");
    CB_CHECK_EQ(ir::any_type()->kind(), "any");
    CB_CHECK_EQ(ir::class_type("a.B")->kind(), "class");
    CB_CHECK_EQ(ir::pointer_to(ir::i32_type())->kind(), "ptr");
    CB_CHECK(ir::i32_type()->is_prim());
    CB_CHECK(!ir::void_type()->is_prim());
    CB_CHECK(!ir::class_type("a.B")->is_prim());
    // what holds a reference
    CB_CHECK(ir::class_type("a.B")->is_ref());
    CB_CHECK(ir::array_of(ir::i32_type())->is_ref());
    CB_CHECK(ir::typevar_type("T")->is_ref());
    CB_CHECK(ir::null_type()->is_ref());
    CB_CHECK(ir::make_type(ir::WildcardType{})->is_ref());
    CB_CHECK(!ir::i32_type()->is_ref());
    CB_CHECK(!ir::pointer_to(ir::i32_type())->is_ref());
    // the unqualified name
    CB_CHECK_EQ(ir::class_type("java.util.List")->simple(), "List");
    CB_CHECK_EQ(ir::class_type("List")->simple(), "List");
    CB_CHECK_EQ(ir::typevar_type("T")->simple(), "T");
    CB_CHECK_EQ(ir::i32_type()->simple(), "i32");
    CB_CHECK_EQ(ir::class_type("")->simple(), "class");
    // text
    CB_CHECK_EQ(ir::to_text(*ir::class_type("java.util.Map", {ir::class_type("K"), ir::array_of(ir::i32_type())})), "java.util.Map<K, i32[]>");
    CB_CHECK_EQ(ir::to_text(*ir::array_of(ir::i32_type(), 2)), "i32[][]");
    ir::WildcardType extends;
    extends.bound = ir::WildcardBound{ir::BoundMode::Extends, ir::typevar_type("T")};
    CB_CHECK_EQ(ir::to_text(*ir::make_type(extends)), "? extends T");
    CB_CHECK_EQ(ir::to_text(*ir::make_type(ir::WildcardType{})), "?");
    CB_CHECK_EQ(ir::to_text(*ir::pointer_to(ir::i32_type())), "ptr");
    // enum names are the schema's
    CB_CHECK_EQ(ir::to_text(ir::Prim::F80), "f80");
    CB_CHECK_EQ(ir::to_text(ir::AddressSpace::Huge), "huge");
    CB_CHECK_EQ(ir::to_text(ir::MemoryModel::Emulated), "emulated");
    CB_CHECK_EQ(ir::to_text(ir::Overflow::Saturate), "saturate");
    CB_CHECK_EQ(ir::to_text(ir::CallingConvention::Fastcall), "fastcall");
    CB_CHECK_EQ(ir::to_text(ir::SymKind::EnumConst), "enumconst");
    CB_CHECK_EQ(ir::to_text(ir::PassMode::ByValue), "");
    CB_CHECK_EQ(ir::to_text(ir::PassMode::Out), "out");
});

CB_TEST("translate.ir.types.arrays", [] {
    const ir::TypeP jagged = ir::array_of(ir::class_type("a.B"), 3);
    const auto [n, inner] = ir::dims(jagged);
    CB_CHECK_EQ(n, std::size_t{3});
    CB_CHECK(inner == ir::class_type("a.B"));
    CB_CHECK_EQ(ir::dims(ir::i32_type()).first, std::size_t{0});
    CB_CHECK(ir::dims(ir::i32_type()).second == ir::i32_type());
    CB_CHECK_EQ(ir::dims(ir::TypeP()).first, std::size_t{0});
    CB_CHECK(ir::array_of(ir::i32_type(), 0) == ir::i32_type());
    // a fixed-size array and a rectangular one are not the plain one
    ir::ArrayType fixed;
    fixed.elem = ir::i32_type();
    fixed.count = 16;
    ir::ArrayType matrix;
    matrix.elem = ir::i32_type();
    matrix.rank = 2;
    CB_CHECK(ir::make_type(fixed) != ir::array_of(ir::i32_type()));
    CB_CHECK(ir::make_type(matrix) != ir::array_of(ir::i32_type()));
});

CB_TEST("translate.ir.sym.owner_simple", [] {
    CB_CHECK_EQ(*ir::make_sym(ir::SymKind::Field, "x", "a.b.Owner")->owner_simple(), "Owner");
    CB_CHECK_EQ(*ir::make_sym(ir::SymKind::Field, "x", "Owner")->owner_simple(), "Owner");
    CB_CHECK(!ir::make_sym(ir::SymKind::Local, "x")->owner_simple().has_value());
    // a symbol is shared: a change through one name is seen through the other
    const ir::SymP s = ir::make_sym(ir::SymKind::Local, "i");
    const ir::SymP alias = s;
    alias->type = ir::i32_type();
    CB_CHECK(s->type == ir::i32_type());
});

CB_TEST("translate.ir.tree.walk_is_depth_first_in_source_order", [] {
    Method m;
    const std::vector<Node*> all = ir::walk(m.decl);
    CB_CHECK(kinds_of(all) == (std::vector<std::string_view>{
                                  "MethodDecl", "Param", "Block", "If", "Binary", "Name", "Lit", "Block", "Return", "Binary", "Name", "Lit",
                                  "Return", "Unary", "Name"}));
    CB_CHECK(all.front() == m.decl);
    CB_CHECK(ir::walk(nullptr).empty());
    // children: only the slots a walk follows, in order
    CB_CHECK(kinds_of(ir::children(*m.decl)) == (std::vector<std::string_view>{"Param", "Block"}));
    CB_CHECK(kinds_of(ir::children(*m.if_stmt)) == (std::vector<std::string_view>{"Binary", "Block"}));   // no else
    CB_CHECK(ir::children(*ir::make(m.arena, ir::Empty{})).empty());
});

CB_TEST("translate.ir.tree.node_places_types_and_kinds", [] {
    Method m;
    CB_CHECK_EQ(m.if_stmt->line, 2u);
    CB_CHECK_EQ(m.if_stmt->col, 4u);
    CB_CHECK(m.times->type == ir::i32_type());
    CB_CHECK(m.if_stmt->type == ir::TypeP());          // a statement has no type
    CB_CHECK_EQ(ir::kind_name(*m.times), "Binary");
    CB_CHECK(ir::is<ir::Binary>(m.times));
    CB_CHECK(!ir::is<ir::Unary>(m.times));
    CB_CHECK(!ir::is<ir::Binary>(nullptr));
    const auto* b = ir::get<ir::Binary>(m.times);
    CB_CHECK(b != nullptr);
    if (b) {
        CB_CHECK_EQ(b->op, "*");
        CB_CHECK_EQ(ir::get<ir::Lit>(b->right)->text.value_or(""), "2");
        CB_CHECK(std::get<std::int64_t>(ir::get<ir::Lit>(b->right)->value) == 2);
    }
    CB_CHECK(ir::category(*m.decl) == ir::Category::Declaration);
    CB_CHECK(ir::category(*m.if_stmt) == ir::Category::Statement);
    CB_CHECK(ir::category(*m.times) == ir::Category::Expression);
    // the symbol is the one the declaration made
    CB_CHECK(ir::get<ir::MethodDecl>(m.decl)->sym == m.f);
    CB_CHECK(ir::get<ir::Name>(ir::get<ir::Binary>(m.times)->left)->sym == m.a);
});

CB_TEST("translate.ir.tree.a_pass_rewires_children_in_place", [] {
    Method m;
    // constant-fold `a * 2` into `a + a`: allocate in the same arena and change the slot
    auto* ret = ir::get<ir::Return>(ir::get<ir::Block>(std::get<Node*>(ir::get<ir::If>(m.if_stmt)->then))->stmts[0]);
    CB_CHECK(ret != nullptr);
    Node* plus = ir::make(m.arena, ir::Binary{.op = "+", .left = ir::get<ir::Binary>(m.times)->left, .right = ir::get<ir::Binary>(m.times)->left},
                          ir::i32_type());
    ret->expr = plus;
    CB_CHECK_EQ(ir::get<ir::Binary>(ir::get<ir::Return>(ir::walk(m.decl)[8])->expr)->op, "+");
    // each_child can rewrite every child slot of a node
    int visited = 0;
    auto* top = ir::get<ir::Binary>(plus);
    top->each_child([&](Node*& child) {
        ++visited;
        child = ir::make(m.arena, ir::Lit{.value = std::int64_t{7}});
    });
    CB_CHECK_EQ(visited, 2);
    CB_CHECK(ir::is<ir::Lit>(top->left) && ir::is<ir::Lit>(top->right));
});

CB_TEST("translate.ir.tree.bodies_are_a_statement_or_a_list", [] {
    cb::Arena arena;
    Node* one = ir::make(arena, ir::Empty{});
    Node* two = ir::make(arena, ir::Break{});
    ir::Suite none;
    ir::Suite single = one;
    ir::Suite list = std::vector<Node*>{one, two};
    ir::Suite empty_list = std::vector<Node*>{};
    CB_CHECK(!ir::present(none));
    CB_CHECK(ir::present(single));
    CB_CHECK(ir::present(list));
    CB_CHECK(!ir::present(empty_list));
    CB_CHECK(ir::statements(none).empty());
    CB_CHECK(ir::statements(single) == (std::vector<Node*>{one}));
    CB_CHECK(ir::statements(list) == (std::vector<Node*>{one, two}));
    // a walk goes through either form
    Node* cond = ir::make(arena, ir::Lit{.value = true});
    Node* loop = ir::make(arena, ir::While{.cond = cond, .body = list});
    CB_CHECK(kinds_of(ir::walk(loop)) == (std::vector<std::string_view>{"While", "Lit", "Empty", "Break"}));
    Node* indexed = ir::make(arena, ir::Index{.arr = ir::make(arena, ir::Name{}), .index = std::vector<Node*>{one, two}});   // a[i, j]
    CB_CHECK(kinds_of(ir::walk(indexed)) == (std::vector<std::string_view>{"Index", "Name", "Empty", "Break"}));
    // a rewrite through each_child reaches the elements of a list body
    int n = 0;
    ir::get<ir::While>(loop)->each_child([&](Node*&) { ++n; });
    CB_CHECK_EQ(n, 3);
});

CB_TEST("translate.ir.tree.children_in_the_order_of_the_source", [] {
    cb::Arena arena;
    auto lit = [&](std::int64_t v) { return ir::make(arena, ir::Lit{.value = v}); };
    // new Outer.Inner(1, 2) { body }: the enclosing instance first, then the arguments, then the anonymous body
    ir::New n;
    n.sym = ir::make_sym(ir::SymKind::Ctor, "Inner", "Outer");
    n.args = {lit(1), lit(2)};
    n.body = ir::make(arena, ir::ClassDecl{.name = "anon"});
    n.outer = ir::make(arena, ir::Name{});
    n.memory = ir::MemoryModel::Gc;
    Node* made = ir::make(arena, std::move(n));
    CB_CHECK(kinds_of(ir::children(*made)) == (std::vector<std::string_view>{"Name", "Lit", "Lit", "ClassDecl"}));
    // a try: resources, body, catches, finally
    ir::Try t;
    t.resources = {ir::make(arena, ir::Local{})};
    t.body = ir::make(arena, ir::Block{});
    t.catches = {ir::make(arena, ir::Catch{.body = ir::make(arena, ir::Block{})})};
    t.finally_body = ir::make(arena, ir::Empty{});
    CB_CHECK(kinds_of(ir::walk(ir::make(arena, std::move(t)))) ==
             (std::vector<std::string_view>{"Try", "Local", "Block", "Catch", "Block", "Empty"}));
    // a for: init, condition, update, body
    ir::For f;
    f.init = {ir::make(arena, ir::Local{})};
    f.cond = ir::make(arena, ir::Name{});
    f.update = {ir::make(arena, ir::Unary{.op = "++"})};
    f.body = ir::make(arena, ir::Block{});
    CB_CHECK(kinds_of(ir::children(*ir::make(arena, std::move(f)))) == (std::vector<std::string_view>{"Local", "Name", "Unary", "Block"}));
    // an instanceof pattern's binding is not walked into
    Node* binding = ir::make(arena, ir::Local{.sym = ir::make_sym(ir::SymKind::Local, "s")});
    Node* test = ir::make(arena, ir::InstanceOf{.x = ir::make(arena, ir::Name{}), .test = ir::class_type("String"), .binding = binding});
    CB_CHECK(kinds_of(ir::walk(test)) == (std::vector<std::string_view>{"InstanceOf", "Name"}));
    // a do-while: body then condition; a lambda: parameters then body; a case: labels then body
    Node* dw = ir::make(arena, ir::DoWhile{.body = ir::make(arena, ir::Block{}), .cond = ir::make(arena, ir::Name{})});
    CB_CHECK(kinds_of(ir::children(*dw)) == (std::vector<std::string_view>{"Block", "Name"}));
    Node* lam = ir::make(arena, ir::Lambda{.params = {ir::make(arena, ir::Param{})}, .body = ir::make(arena, ir::Name{})});
    CB_CHECK(kinds_of(ir::children(*lam)) == (std::vector<std::string_view>{"Param", "Name"}));
    Node* cs = ir::make(arena, ir::Case{.labels = {lit(1)}, .body = {ir::make(arena, ir::Break{})}});
    CB_CHECK(kinds_of(ir::children(*cs)) == (std::vector<std::string_view>{"Lit", "Break"}));
});

CB_TEST("translate.ir.tree.unsupported_constructs_are_found", [] {
    cb::Arena arena;
    Node* bad = ir::make(arena, ir::Unsupported{.kind = "yield-from", .raw = "(yield from x)"}, ir::any_type());
    Node* call = ir::make(arena, ir::Call{.args = {ir::make(arena, ir::Name{}), bad}});
    Node* stmt = ir::make(arena, ir::ExprStmt{.expr = call});
    Node* block = ir::make(arena, ir::Block{.stmts = {stmt, ir::make(arena, ir::Unsupported{.kind = "walrus"})}});
    const std::vector<Node*> found = ir::unsupported(block);
    CB_CHECK_EQ(found.size(), std::size_t{2});
    CB_CHECK(found[0] == bad);
    CB_CHECK_EQ(ir::get<ir::Unsupported>(found[1])->kind.value_or(""), "walrus");
    CB_CHECK(ir::unsupported(ir::make(arena, ir::Empty{})).empty());
    CB_CHECK(ir::unsupported(nullptr).empty());
});

CB_TEST("translate.ir.tree.clone_is_deep_but_shares_types_and_symbols", [] {
    Method m;
    cb::Arena other;
    Node* copy = ir::clone(other, m.decl);
    CB_CHECK(copy != m.decl);
    CB_CHECK(kinds_of(ir::walk(copy)) == kinds_of(ir::walk(m.decl)));
    const std::vector<Node*> a = ir::walk(m.decl);
    const std::vector<Node*> b = ir::walk(copy);
    for (std::size_t i = 0; i < a.size(); ++i) {
        CB_CHECK(a[i] != b[i]);                      // no node is shared
        CB_CHECK(a[i]->line == b[i]->line && a[i]->col == b[i]->col);
        CB_CHECK(a[i]->type == b[i]->type);
    }
    CB_CHECK(ir::get<ir::MethodDecl>(copy)->sym == m.f);                                   // the symbol is the same object
    CB_CHECK(ir::get<ir::Name>(b[5])->sym == ir::get<ir::Name>(a[5])->sym);
    // changing the copy does not change the original
    ir::get<ir::Binary>(b[4])->op = "<";
    CB_CHECK_EQ(ir::get<ir::Binary>(a[4])->op, ">");
    // the original may go first: the copy is in another arena
    {
        cb::Arena temp;
        Node* t = ir::clone(temp, m.if_stmt);
        CB_CHECK(t != nullptr);
    }
    CB_CHECK(ir::clone(other, nullptr) == nullptr);
    // a pattern binding, which a walk does not enter, is copied too
    Node* binding = ir::make(m.arena, ir::Local{});
    Node* test = ir::make(m.arena, ir::InstanceOf{.x = ir::make(m.arena, ir::Name{}), .binding = binding});
    Node* test_copy = ir::clone(other, test);
    CB_CHECK(ir::get<ir::InstanceOf>(test_copy)->binding != nullptr);
    CB_CHECK(ir::get<ir::InstanceOf>(test_copy)->binding != binding);
});

CB_TEST("translate.ir.tree.every_kind_can_be_made_by_name", [] {
    static const char* const kinds[] = {
        "Module", "Unit", "ClassDecl", "FieldDecl", "MethodDecl", "Param", "EnumConst", "Property", "TypeParam", "Block", "Local",
        "ExprStmt", "If", "While", "DoWhile", "For", "ForEach", "Switch", "Case", "Break", "Continue", "Return", "Throw", "Try",
        "Catch", "Labeled", "Goto", "MachineFragment", "Sync", "Empty", "Comment", "LocalClass", "Assert", "CtorCall", "Yield", "YieldReturn", "YieldBreak", "Await",
        "Lit", "Name", "Select", "TypeName", "This", "Super", "Call", "New", "NewArray", "Index", "Length", "Unary", "Binary",
        "Concat", "Assign", "Cond", "Cast", "InstanceOf", "ClassLit", "Lambda", "MethodRef", "SwitchExpr", "AddressOf",
        "FarAddress", "Piece", "Join", "Paren", "Unsupported",
        "Region", "BlockExpr", "Fixed", "ThrowExpr", "OptionalChain", "OptionalUnwrap", "PatternMatch", "Destructure", "Default", "SizeOf", "StackAlloc", "Intrinsic"};
    cb::Arena arena;
    std::unordered_set<std::string_view> seen;
    for (const char* name : kinds) {
        Node* n = ir::make_node(arena, name);
        CB_CHECK(n != nullptr);
        if (n == nullptr) continue;
        CB_CHECK_EQ(ir::kind_name(*n), std::string_view(name));
        CB_CHECK(ir::children(*n).empty());           // every slot empty
        seen.insert(name);
    }
    CB_CHECK_EQ(seen.size(), std::size_t{78});
    CB_CHECK_EQ(seen.size(), std::variant_size_v<ir::Kind>);   // all of them
    CB_CHECK(ir::make_node(arena, "Nope") == nullptr);
    CB_CHECK(ir::make_node(arena, "") == nullptr);
    CB_CHECK(ir::make_node(arena, "block") == nullptr);        // names are exact
});

CB_TEST("translate.ir.tree.declarations_of_the_other_languages", [] {
    cb::Arena arena;
    // C#: a property with a getter and a setter, the compiler supplying the field
    Node* get = ir::make(arena, ir::MethodDecl{.name = "get_Size", .kind = "get"});
    Node* set = ir::make(arena, ir::MethodDecl{.name = "set_Size", .kind = "set"});
    ir::Property p;
    p.sym = ir::make_sym(ir::SymKind::Property, "Size", "Game.Board");
    p.type = ir::i32_type();
    p.mods = {"public"};
    p.accessors = {get, set};
    Node* prop = ir::make(arena, std::move(p));
    CB_CHECK(kinds_of(ir::children(*prop)) == (std::vector<std::string_view>{"MethodDecl", "MethodDecl"}));
    CB_CHECK(!ir::present(ir::get<ir::MethodDecl>(get)->body));   // no body: the compiler supplies it
    // a C unit with its target, an enum that is a named integer, a struct with its layout
    ir::Unit u;
    u.path = "game.c";
    u.lang = "c";
    ir::CompilerTarget target;
    target.triple = "i686-pc-windows-msvc";
    target.pointer = 32;
    target.long_bits = 32;
    target.char_signed = true;
    u.target = target;
    ir::EnumConst color;
    color.sym = ir::make_sym(ir::SymKind::EnumConst, "RED");
    color.value = 3;
    ir::ClassDecl strukt;
    strukt.name = "POINT";
    strukt.kind = "struct";
    ir::RecordType layout;
    layout.name = "POINT";
    layout.size = 4;
    layout.fields = {{"x", 0, ir::prim_type(ir::Prim::I16), false}, {"", 2, ir::prim_type(ir::Prim::U8), true}};
    strukt.layout = ir::make_type(layout);
    u.types = {ir::make(arena, std::move(strukt)), ir::make(arena, std::move(color))};
    Node* unit = ir::make(arena, std::move(u));
    CB_CHECK(kinds_of(ir::walk(ir::make(arena, ir::Module{.units = {unit}, .header = "typedef int I;"}))) ==
             (std::vector<std::string_view>{"Module", "Unit", "ClassDecl", "EnumConst"}));
    CB_CHECK(*ir::get<ir::Unit>(unit)->target == target);
    // a literal keeps what the source spelled; an integer above INT64_MAX is unsigned
    Node* big = ir::make(arena, ir::Lit{.value = std::uint64_t{0xFFFFFFFFFFFFFFFFull}, .text = "0xFFFFFFFFFFFFFFFFull"}, ir::prim_type(ir::Prim::U64));
    CB_CHECK(std::get<std::uint64_t>(ir::get<ir::Lit>(big)->value) == 0xFFFFFFFFFFFFFFFFull);
    CB_CHECK(std::holds_alternative<std::monostate>(ir::get<ir::Lit>(ir::make(arena, ir::Lit{}))->value));   // null
    // what stays assembly
    Node* asm_fragment = ir::make(arena, ir::MachineFragment{.length = 12, .reason = "hand-written", .assembly = "mov ax, 1", .name = "f", .syntax = "masm"});
    CB_CHECK(ir::category(*asm_fragment) == ir::Category::Statement);
    // a call's generic arguments are types, or what the struct front end puts there
    ir::Call c;
    c.typeargs = {ir::TypeArg{ir::class_type("T")}, ir::TypeArg{std::string("<HH")}, ir::TypeArg{std::int64_t{4}}};
    CB_CHECK_EQ(c.typeargs.size(), std::size_t{3});
});

CB_TEST("translate.ir.operator_names_of_the_java_front_end", [] {
    CB_CHECK_EQ(ir::binary_operator("PLUS").value_or(""), "+");
    CB_CHECK_EQ(ir::binary_operator("UNSIGNED_RIGHT_SHIFT").value_or(""), ">>>");
    CB_CHECK_EQ(ir::binary_operator("CONDITIONAL_AND").value_or(""), "&&");
    CB_CHECK_EQ(ir::binary_operator("GREATER_THAN_EQUAL").value_or(""), ">=");
    CB_CHECK_EQ(ir::binary_operator("REMAINDER").value_or(""), "%");
    CB_CHECK(!ir::binary_operator("plus").has_value());
    CB_CHECK(!ir::binary_operator("").has_value());
    const auto post = ir::unary_operator("POSTFIX_INCREMENT");
    CB_CHECK(post.has_value());
    if (post) {
        CB_CHECK_EQ(post->op, "++");
        CB_CHECK(post->postfix);
    }
    CB_CHECK(!ir::unary_operator("PREFIX_DECREMENT")->postfix);
    CB_CHECK_EQ(ir::unary_operator("LOGICAL_COMPLEMENT")->op, "!");
    CB_CHECK_EQ(ir::unary_operator("BITWISE_COMPLEMENT")->op, "~");
    CB_CHECK(!ir::unary_operator("PLUS").has_value());
});

CB_TEST("translate.ir.tree.a_node_of_the_common_kinds_is_small", [] {
    // the statements and expressions of a tree are what there are millions of: the big declarations are boxed
    static_assert(sizeof(ir::Node) <= 160, "a node must not be as big as a ClassDecl");
    CB_CHECK(sizeof(ir::Node) <= 160);
    // a boxed kind reads and writes like any other
    cb::Arena arena;
    ir::ClassDecl c;
    c.name = "Board";
    c.members = {ir::make(arena, ir::Empty{})};
    ir::Node* decl = ir::make(arena, std::move(c));
    CB_CHECK_EQ(ir::kind_name(*decl), "ClassDecl");
    CB_CHECK(ir::category(*decl) == ir::Category::Declaration);
    ir::get<ir::ClassDecl>(decl)->mods.push_back("public");
    CB_CHECK_EQ(ir::get<ir::ClassDecl>(decl)->mods.size(), std::size_t{1});
    CB_CHECK(!ir::is<ir::MethodDecl>(decl));
    // a copy of a boxed node owns its own copy of what it holds
    ir::Node copy = *decl;
    ir::get<ir::ClassDecl>(&copy)->name = "Other";
    CB_CHECK_EQ(*ir::get<ir::ClassDecl>(decl)->name, "Board");
    CB_CHECK_EQ(*ir::get<ir::ClassDecl>(&copy)->name, "Other");
    // visiting opens the box
    std::string seen;
    ir::visit(*decl, [&](const auto& k) { seen = std::string(std::decay_t<decltype(k)>::kName); });
    CB_CHECK_EQ(seen, "ClassDecl");
});

CB_TEST("translate.ir.tree.where_a_node_came_from_is_shared_and_optional", [] {
    cb::Arena arena;
    ir::Node* a = ir::make(arena, ir::Empty{});
    CB_CHECK(a->origin == nullptr);   // source has none
    a->origin = std::make_shared<const ir::Origin>(ir::OriginVa{"lz32.dll", 0x1000});
    ir::Node* b = ir::make(arena, ir::Empty{});
    b->origin = a->origin;            // the statements made from one function's instruction share it
    CB_CHECK(std::get<ir::OriginVa>(*b->origin).va == 0x1000u);
    CB_CHECK(a->origin == b->origin);
    ir::Node* copy = ir::clone(arena, a);
    CB_CHECK(copy->origin != nullptr && *copy->origin == *a->origin);
});

// ---- what a lowered bytecode states (docs: work/ilspy-port/l2/ILAST_TO_L2.md): stack kinds, prefix flags, entities, info, and the kinds
// the CIL lane needs. Every one is optional: a tree that states none of it is the tree it was.

namespace {

// Stands for the CLR lane's entity (the real one holds an IType, IMethod ...).
class IrTestEntity : public ir::Entity {
public:
    IrTestEntity(ir::EntityKind kind, std::string key) : kind_(kind), key_(std::move(key)) {}
    std::string_view family() const override { return "test"; }
    ir::EntityKind kind() const override { return kind_; }
    std::string key() const override { return key_; }

private:
    ir::EntityKind kind_;
    std::string key_;
};

ir::EntityP entity(ir::EntityKind kind, std::string key) { return std::make_shared<const IrTestEntity>(kind, std::move(key)); }

template <class E, class Last> void all_enum_values_round_trip(Last last) {
    for (std::size_t i = 0; i <= static_cast<std::size_t>(last); ++i) {
        const E value = static_cast<E>(i);
        const std::string_view text = ir::to_text(value);
        CB_CHECK(!text.empty());
        const auto back = ir::from_text<E>(text);
        CB_CHECK(back.has_value() && *back == value);
    }
    CB_CHECK(!ir::from_text<E>("no such name").has_value());
    CB_CHECK(!ir::from_text<E>("").has_value());
}

}  // namespace

// A CIL `int` (an entity of the CLR's System.Int32, a fact of its own) and the Java front end's `i32` are the same type: equal, and
// equal in hash -- `==` compares what a type says, not which type system made it.
CB_TEST("translate.ir.cil.a_cil_int_and_the_java_i32_are_equal_and_hash_equal", [] {
    ir::InfoP info;
    info.set("cil.native_int", false);
    const ir::TypeP cil_int = ir::make_type(ir::PrimType{ir::Prim::I32, std::nullopt, std::nullopt}, entity(ir::EntityKind::Type, "T:System.Runtime|System.Int32"), info);
    const ir::TypeP java_i32 = ir::prim_type(ir::Prim::I32);
    CB_CHECK(cil_int == java_i32);
    CB_CHECK(*cil_int == *java_i32);
    CB_CHECK(java_i32 == cil_int);
    CB_CHECK_EQ(cil_int->hash(), java_i32->hash());
    CB_CHECK(cil_int.get() != java_i32.get());
    CB_CHECK(cil_int->entity != nullptr && cil_int->info);
    CB_CHECK(java_i32->entity == nullptr && !java_i32->info);
    CB_CHECK(cil_int->info.get_as<bool>("cil.native_int") != nullptr);
    // the entity is compared by `same_entity`, by what it is (family, kind, key), not by object
    const ir::TypeP same = ir::make_type(ir::PrimType{ir::Prim::I32, std::nullopt, std::nullopt}, entity(ir::EntityKind::Type, "T:System.Runtime|System.Int32"));
    CB_CHECK(ir::same_entity(cil_int, same));
    CB_CHECK(!ir::same_entity(cil_int, java_i32));      // one has none
    CB_CHECK(!ir::same_entity(java_i32, java_i32));     // none is not "the same"
    CB_CHECK(!ir::same_entity(cil_int, ir::make_type(ir::PrimType{ir::Prim::I32, std::nullopt, std::nullopt}, entity(ir::EntityKind::Type, "T:Other|X"))));
    // a class from the CLR and from Java: equal by name; other entities do not make types unequal, other bodies do
    const ir::TypeP cil_string = ir::make_type(ir::ClassType{.name = "System.String"}, entity(ir::EntityKind::Type, "T:System.Runtime|System.String"));
    CB_CHECK(cil_string == ir::class_type("System.String"));
    CB_CHECK(cil_string != ir::class_type("java.lang.String"));
    CB_CHECK(cil_int != ir::prim_type(ir::Prim::U32));
    // an array of a CIL int is the array of an i32
    CB_CHECK(ir::array_of(cil_int) == ir::array_of(java_i32));
    CB_CHECK_EQ(ir::array_of(cil_int)->hash(), ir::array_of(java_i32)->hash());
    // decimal is a class type of its own name, not a primitive: System.Decimal
    CB_CHECK(ir::decimal_type() == ir::class_type("System.Decimal"));
});

CB_TEST("translate.ir.cil.a_decimal_literal_keeps_its_exact_text", [] {
    cb::Arena arena;
    ir::Node* a = ir::make_decimal(arena, "1.50");
    ir::Node* b = ir::make_decimal(arena, "1.5");
    CB_CHECK(a->type == ir::decimal_type());
    CB_CHECK_EQ(*ir::get<ir::Lit>(a)->text, std::string("1.50"));
    CB_CHECK_EQ(std::get<std::string>(ir::get<ir::Lit>(a)->value), std::string("1.50"));
    CB_CHECK_NE(*ir::get<ir::Lit>(a)->text, *ir::get<ir::Lit>(b)->text);   // the scale is kept
    CB_CHECK(a->type == b->type);
});

// What a node costs: the stack kind, the flags and a null pointer to the info; nothing is allocated for the info until a key is set.
CB_TEST("translate.ir.cil.a_node_grows_by_16_bytes_and_info_is_free_until_set", [] {
    std::cout << "sizeof: Node " << sizeof(ir::Node) << ", Type " << sizeof(ir::Type) << ", Sym " << sizeof(ir::Sym) << ", Kind " << sizeof(ir::Kind)
              << ", InfoP " << sizeof(ir::InfoP) << ", Origin " << sizeof(ir::Origin) << "\n";
    static_assert(sizeof(ir::InfoP) == sizeof(void*), "info is one pointer");
    static_assert(sizeof(ir::Kind) <= 96, "the new slots must not grow the largest kind (it sets the size of every node)");
    static_assert(sizeof(ir::Node) <= 152, "a node: 136 bytes before the lowered-bytecode facts, +8 for stack and flags, +8 for the info pointer");
    static_assert(sizeof(ir::Sym) <= 232, "a symbol: 200 bytes before, +8 index, +16 entity, +8 info; role, stack and the flags sit in the padding");
    static_assert(sizeof(ir::Type) <= 176, "a type: 152 bytes before, +16 entity, +8 info");
    CB_CHECK(sizeof(ir::Node) <= 152);
    ir::Node n;
    CB_CHECK(!n.info);
    CB_CHECK(n.stack == ir::Stack::Unknown && n.flags == 0);
    // copying a node copies its info: the copy is another
    n.info.set("cil.code_size", std::int64_t{12});
    ir::Node copy = n;
    copy.info.set("cil.code_size", std::int64_t{99});
    CB_CHECK_EQ(*n.info.get_as<std::int64_t>("cil.code_size"), std::int64_t{12});
    CB_CHECK_EQ(*copy.info.get_as<std::int64_t>("cil.code_size"), std::int64_t{99});
    // the largest kinds stay inside the variant's size: Case and Assign are the ones the new slots made bigger
    static_assert(sizeof(ir::Case) <= 88 && sizeof(ir::Assign) <= 88 && sizeof(ir::Binary) <= 88 && sizeof(ir::Call) <= 88);
});

CB_TEST("translate.ir.cil.info_is_a_sorted_keyed_bag_that_copies_what_it_holds", [] {
    ir::InfoP info;
    CB_CHECK(!info);
    CB_CHECK(info.find("cil.code_size") == nullptr);
    info.set("cil.severity", std::int64_t{2});
    info.set("cil.code_size", std::int64_t{7});
    info.set("cil.mono_state_machine", true);
    CB_CHECK(info);
    std::vector<std::string> keys;
    for (const auto& item : info.get()->items()) keys.push_back(item.first);
    CB_CHECK((keys == std::vector<std::string>{"cil.code_size", "cil.mono_state_machine", "cil.severity"}));   // sorted
    info.set("cil.code_size", std::int64_t{8});                                                              // replaced, not added
    CB_CHECK_EQ(info.get()->items().size(), std::size_t{3});
    CB_CHECK_EQ(*info.get_as<std::int64_t>("cil.code_size"), std::int64_t{8});
    CB_CHECK(info.get_as<std::string>("cil.code_size") == nullptr);   // another kind: none
    // a copy is its own
    ir::InfoP other = info;
    other.erase("cil.severity");
    CB_CHECK(info.find("cil.severity") != nullptr && other.find("cil.severity") == nullptr);
    CB_CHECK(info != other);
    other.set("cil.severity", std::int64_t{2});
    other.set("cil.code_size", std::int64_t{8});
    CB_CHECK(info == other);
    // erasing the last key frees it
    ir::InfoP one;
    one.set("cil.pinned", true);
    one.erase("cil.pinned");
    CB_CHECK(!one);
    one.erase("cil.pinned");   // (and erasing what is not there is nothing)
    // entities in an info are compared by what they are
    ir::InfoP e1, e2;
    e1.set("cil.move_next", entity(ir::EntityKind::Method, "M:A|N#06000001"));
    e2.set("cil.move_next", entity(ir::EntityKind::Method, "M:A|N#06000001"));
    CB_CHECK(e1 == e2);
    e2.set("cil.move_next", entity(ir::EntityKind::Method, "M:A|N#06000002"));
    CB_CHECK(e1 != e2);
});

// The tables are data: sorted (they are searched), complete, and each row says why.
CB_TEST("translate.ir.cil.the_info_and_intrinsic_tables_are_well_formed", [] {
    std::string_view last;
    for (const ir::InfoKey& k : ir::info_keys()) {
        CB_CHECK(k.key.starts_with("cil."));
        CB_CHECK(last < k.key);   // sorted, no duplicates
        last = k.key;
        CB_CHECK(!k.doc.empty());
        CB_CHECK(ir::find_info_key(k.key) == &k);
        if (k.on == ir::InfoOn::Node) {
            CB_CHECK(!k.kinds.empty());
            std::size_t at = 0;
            cb::Arena arena;
            while (at < k.kinds.size()) {   // every kind a key may sit on is a kind
                std::size_t end = k.kinds.find(' ', at);
                if (end == std::string_view::npos) end = k.kinds.size();
                CB_CHECK(ir::make_node(arena, k.kinds.substr(at, end - at)) != nullptr);
                at = end + 1;
            }
        } else {
            CB_CHECK(k.kinds.empty());
        }
    }
    CB_CHECK(ir::find_info_key("cil.nope") == nullptr);
    CB_CHECK(ir::find_info_key("") == nullptr);
    CB_CHECK_EQ(ir::info_keys().size(), std::size_t{31});

    last = {};
    std::set<std::string_view> flag_names;
    for (ir::NodeFlag f : ir::all_node_flags()) flag_names.insert(ir::to_text(f));
    for (const ir::IntrinsicName& n : ir::intrinsic_names()) {
        CB_CHECK(last < n.name);   // sorted, no duplicates
        last = n.name;
        CB_CHECK(!n.operands.empty() && !n.result.empty() && !n.why.empty() && !n.lanes.empty() && !n.test.empty());   // the binding rule
        CB_CHECK(ir::find_intrinsic(n.name) == &n);
        // every flag it names is a flag; a name with no cil. / cs. prefix is used by two lanes at least
        std::size_t at = 0;
        while (at < n.flags.size()) {
            std::size_t end = n.flags.find(' ', at);
            if (end == std::string_view::npos) end = n.flags.size();
            CB_CHECK(flag_names.contains(n.flags.substr(at, end - at)));
            at = end + 1;
        }
        if (!n.name.starts_with("cil.") && !n.name.starts_with("cs.")) CB_CHECK(n.lanes.find(' ') != std::string_view::npos);
    }
    CB_CHECK(ir::find_intrinsic("cil.nope") == nullptr);
    CB_CHECK(ir::find_intrinsic("mem.copy") != nullptr);
    CB_CHECK_EQ(ir::intrinsic_names().size(), std::size_t{25});
});

CB_TEST("translate.ir.cil.the_new_enums_and_flags_have_names_that_read_back", [] {
    all_enum_values_round_trip<ir::Stack>(ir::Stack::Void);
    all_enum_values_round_trip<ir::Lift>(ir::Lift::ThreeValued);
    all_enum_values_round_trip<ir::Coalesce>(ir::Coalesce::NullableValue);
    all_enum_values_round_trip<ir::ConvKind>(ir::ConvKind::ExprTree);
    all_enum_values_round_trip<ir::Eval>(ir::Eval::New);
    all_enum_values_round_trip<ir::Via>(ir::Via::Dynamic);
    all_enum_values_round_trip<ir::CallForm>(ir::CallForm::Indirect);
    all_enum_values_round_trip<ir::RegionKind>(ir::RegionKind::Switch);
    all_enum_values_round_trip<ir::EmptyKind>(ir::EmptyKind::Pop);
    all_enum_values_round_trip<ir::VarRole>(ir::VarRole::DeconTemp);
    CB_CHECK_EQ(ir::to_text(ir::CallForm::Default), std::string_view("default"));
    CB_CHECK_EQ(ir::to_text(ir::Stack::I4), std::string_view("i4"));
    // flags: one bit each, distinct names, the names read back
    std::uint32_t seen = 0;
    for (ir::NodeFlag f : ir::all_node_flags()) {
        CB_CHECK(ir::bit(f) != 0 && (ir::bit(f) & (ir::bit(f) - 1)) == 0);   // a power of two
        CB_CHECK((seen & ir::bit(f)) == 0);
        seen |= ir::bit(f);
        CB_CHECK(ir::from_text<ir::NodeFlag>(ir::to_text(f)) == f);
    }
    CB_CHECK_EQ(ir::all_node_flags().size(), std::size_t{17});
    CB_CHECK(!ir::from_text<ir::NodeFlag>("nope").has_value());
    cb::Arena arena;
    ir::Node* n = ir::make(arena, ir::Empty{});
    CB_CHECK(!ir::has_flag(*n, ir::NodeFlag::Volatile));
    ir::set_flag(*n, ir::NodeFlag::Volatile);
    ir::set_flag(*n, ir::NodeFlag::Tail);
    CB_CHECK(ir::has_flag(*n, ir::NodeFlag::Volatile) && ir::has_flag(*n, ir::NodeFlag::Tail) && !ir::has_flag(*n, ir::NodeFlag::ReadOnly));
    ir::set_flag(*n, ir::NodeFlag::Volatile, false);
    CB_CHECK(!ir::has_flag(*n, ir::NodeFlag::Volatile) && ir::has_flag(*n, ir::NodeFlag::Tail));
});

CB_TEST("translate.ir.cil.the_stack_kind_is_stated_or_derived_from_the_type", [] {
    cb::Arena arena;
    auto of = [&](ir::TypeP t, ir::Stack stated = ir::Stack::Unknown) {
        ir::Node* n = ir::make(arena, ir::Empty{}, std::move(t));
        n->stack = stated;
        return ir::stack_of(*n);
    };
    CB_CHECK(of(ir::bool_type()) == ir::Stack::I4);
    CB_CHECK(of(ir::i32_type()) == ir::Stack::I4);
    CB_CHECK(of(ir::prim_type(ir::Prim::Char)) == ir::Stack::I4);
    CB_CHECK(of(ir::prim_type(ir::Prim::U64)) == ir::Stack::I8);
    CB_CHECK(of(ir::prim_type(ir::Prim::F32)) == ir::Stack::F4);
    CB_CHECK(of(ir::prim_type(ir::Prim::F64)) == ir::Stack::F8);
    CB_CHECK(of(ir::void_type()) == ir::Stack::Void);
    CB_CHECK(of(ir::class_type("System.String")) == ir::Stack::O);
    CB_CHECK(of(ir::array_of(ir::i32_type())) == ir::Stack::O);
    CB_CHECK(of(ir::null_type()) == ir::Stack::O);
    CB_CHECK(of(ir::pointer_to(ir::i32_type())) == ir::Stack::I);
    ir::PtrType managed;
    managed.to = ir::i32_type();
    managed.managed = true;
    CB_CHECK(of(ir::make_type(managed)) == ir::Stack::Ref);
    CB_CHECK(of(ir::any_type()) == ir::Stack::Unknown);
    CB_CHECK(of({}) == ir::Stack::Unknown);
    // what the lane stated wins: a bool that is on the stack as a native int, a string that is a ref
    CB_CHECK(of(ir::bool_type(), ir::Stack::I) == ir::Stack::I);
    CB_CHECK(of(ir::class_type("System.String"), ir::Stack::Ref) == ir::Stack::Ref);
});

CB_TEST("translate.ir.cil.the_new_type_kinds", [] {
    ir::TupleType tuple;
    tuple.elems = {ir::i32_type(), ir::class_type("System.String")};
    tuple.names = {std::string("id"), std::nullopt};
    const ir::TypeP t = ir::make_type(tuple);
    CB_CHECK_EQ(t->kind(), std::string_view("tuple"));
    CB_CHECK_EQ(ir::to_text(*t), std::string("(i32 id, System.String)"));
    ir::TupleType other = tuple;
    CB_CHECK(t == ir::make_type(other));
    other.names[1] = "name";
    CB_CHECK(t != ir::make_type(other));
    CB_CHECK_EQ(t->hash(), ir::make_type(tuple)->hash());
    ir::ModType mod;
    mod.inner = ir::i32_type();
    mod.required = true;
    mod.modifier = ir::class_type("System.Runtime.CompilerServices.IsVolatile");
    const ir::TypeP m = ir::make_type(mod);
    CB_CHECK_EQ(m->kind(), std::string_view("mod"));
    mod.required = false;
    CB_CHECK(m != ir::make_type(mod));
    // a type variable now says where it is declared; one that says nothing is as before
    CB_CHECK(ir::typevar_type("T") == ir::typevar_type("T"));
    ir::TypeVarType v;
    v.name = "T";
    v.index = 0;
    v.owner = "System.Collections.Generic.List`1";
    CB_CHECK(ir::make_type(v) != ir::typevar_type("T"));
    v.of_method = true;
    CB_CHECK(ir::make_type(v) != ir::make_type(ir::TypeVarType{"T", 0, "System.Collections.Generic.List`1", false}));
    // managed and plain pointers differ; a member function's type differs from a free one
    ir::PtrType managed;
    managed.to = ir::i32_type();
    ir::PtrType plain = managed;
    managed.managed = true;
    CB_CHECK(ir::make_type(managed) != ir::make_type(plain));
    ir::FnType f;
    ir::FnType g = f;
    g.has_this = true;
    CB_CHECK(ir::make_type(f) != ir::make_type(g));
    g = f;
    g.explicit_this = true;
    CB_CHECK(ir::make_type(f) != ir::make_type(g));
});

// The new kinds: made by name (the list above), in the categories the writer uses, with their children in the order of the lane's tree.
CB_TEST("translate.ir.cil.the_new_kinds_and_slots_walk_in_the_order_of_the_ilast", [] {
    cb::Arena arena;
    auto leaf = [&](const char* name) { return ir::make(arena, ir::Name{.sym = ir::make_sym(ir::SymKind::Local, name)}); };
    // categories
    for (const char* name : {"Region", "Fixed", "Destructure"}) CB_CHECK(ir::category(*ir::make_node(arena, name)) == ir::Category::Statement);
    for (const char* name : {"BlockExpr", "ThrowExpr", "OptionalChain", "OptionalUnwrap", "PatternMatch", "Default", "SizeOf", "StackAlloc", "Intrinsic"}) {
        CB_CHECK(ir::category(*ir::make_node(arena, name)) == ir::Category::Expression);
    }
    // Try: body, catches, finally, then the fault block; Catch: the filter before the body
    Node *body = leaf("body"), *handler = leaf("handler"), *fin = leaf("finally"), *fault = leaf("fault"), *filter = leaf("filter"), *catch_body = leaf("catch_body");
    ir::Catch c;
    c.body = catch_body;
    c.filter = filter;
    Node* catch_node = ir::make(arena, std::move(c));
    ir::Try t;
    t.body = body;
    t.catches = {catch_node};
    t.finally_body = fin;
    t.fault_body = fault;
    Node* try_node = ir::make(arena, std::move(t));
    CB_CHECK((ir::children(*try_node) == std::vector<Node*>{body, catch_node, fin, fault}));
    CB_CHECK((ir::children(*catch_node) == std::vector<Node*>{filter, catch_body}));
    (void)handler;
    // Break leaves with a value; a method declares nested local functions after its body
    Node* value = leaf("value");
    Node* brk = ir::make(arena, ir::Break{.label = "IL_0000", .value = value});
    CB_CHECK((ir::children(*brk) == std::vector<Node*>{value}));
    Node *p = leaf("p"), *b = leaf("b"), *local_fn = ir::make(arena, ir::MethodDecl{});
    ir::MethodDecl md;
    md.params = {p};
    md.body = b;
    md.nested = {local_fn};
    Node* md_node = ir::make(arena, std::move(md));
    CB_CHECK((ir::children(*md_node) == std::vector<Node*>{p, b, local_fn}));
    // Region: its blocks; BlockExpr: statements then the result; PatternMatch: the tested operand then the sub-patterns (the variable is a symbol)
    Node *b1 = ir::make(arena, ir::Labeled{.label = "IL_0000"}), *b2 = ir::make(arena, ir::Labeled{.label = "IL_0004"});
    CB_CHECK((ir::children(*ir::make(arena, ir::Region{.kind = ir::RegionKind::Loop, .blocks = {b1, b2}})) == std::vector<Node*>{b1, b2}));
    Node *s1 = leaf("s1"), *res = leaf("res");
    CB_CHECK((ir::children(*ir::make(arena, ir::BlockExpr{.role = "array_init", .stmts = {s1}, .result = res})) == std::vector<Node*>{s1, res}));
    Node *tested = leaf("tested"), *sub1 = leaf("sub1"), *sub2 = leaf("sub2");
    ir::PatternMatch pm;
    pm.var = ir::make_sym(ir::SymKind::Local, "x");
    pm.tested = tested;
    pm.subs = {sub1, sub2};
    CB_CHECK((ir::children(*ir::make(arena, std::move(pm))) == std::vector<Node*>{tested, sub1, sub2}));
    Node *pat = leaf("pat"), *conv = leaf("conv"), *asg = leaf("asg");
    CB_CHECK((ir::children(*ir::make(arena, ir::Destructure{.pattern = pat, .conversions = conv, .assignments = asg})) == std::vector<Node*>{pat, conv, asg}));
    Node* count = leaf("count");
    CB_CHECK((ir::children(*ir::make(arena, ir::StackAlloc{.elem = ir::i32_type(), .count = count, .span = true})) == std::vector<Node*>{count}));
    Node *a0 = leaf("a0"), *a1 = leaf("a1");
    ir::Intrinsic in;
    in.name = "mem.copy";
    in.args = {a0, a1};
    CB_CHECK((ir::children(*ir::make(arena, std::move(in))) == std::vector<Node*>{a0, a1}));
    Node* x = leaf("x");
    CB_CHECK((ir::children(*ir::make(arena, ir::ThrowExpr{.x = x})) == std::vector<Node*>{x}));
    CB_CHECK((ir::children(*ir::make(arena, ir::OptionalChain{.x = x})) == std::vector<Node*>{x}));
    CB_CHECK((ir::children(*ir::make(arena, ir::OptionalUnwrap{.x = x, .ref_in = true})) == std::vector<Node*>{x}));
    Node *fv = leaf("fv"), *fb = leaf("fb");
    CB_CHECK((ir::children(*ir::make(arena, ir::Fixed{.var = fv, .body = fb})) == std::vector<Node*>{fv, fb}));
    CB_CHECK(ir::children(*ir::make(arena, ir::Default{})).empty());
    CB_CHECK(ir::children(*ir::make(arena, ir::SizeOf{.of = ir::i32_type()})).empty());
});

// A tree that states none of it is the tree it was: the old slots, designated, still make the same nodes and clone the same.
CB_TEST("translate.ir.cil.clone_keeps_the_stack_the_flags_the_info_and_the_span", [] {
    cb::Arena arena;
    cb::Arena other;
    ir::SymP v = ir::make_sym(ir::SymKind::Local, "num", std::nullopt, false, ir::i32_type());
    v->role = ir::VarRole::Temp;
    v->index = 3;
    v->stack = ir::Stack::I4;
    Node* n = ir::make(arena, ir::Name{.sym = v}, ir::i32_type());
    n->stack = ir::Stack::I4;
    ir::set_flag(*n, ir::NodeFlag::StackEmpty);
    n->info.set("cil.code_size", std::int64_t{4});
    n->origin = std::make_shared<const ir::Origin>(ir::OriginSpan{4, 9});
    Node* copy = ir::clone(other, n);
    CB_CHECK(copy->stack == ir::Stack::I4 && ir::has_flag(*copy, ir::NodeFlag::StackEmpty));
    CB_CHECK(copy->info == n->info);
    CB_CHECK(std::get<ir::OriginSpan>(*copy->origin) == (ir::OriginSpan{4, 9}));
    copy->info.set("cil.code_size", std::int64_t{5});   // the copy's info is its own
    CB_CHECK_EQ(*n->info.get_as<std::int64_t>("cil.code_size"), std::int64_t{4});
    CB_CHECK(ir::get<ir::Name>(copy)->sym == v);        // the symbol is shared, as before
});

// Boxed kinds still read and write like any other (Module and Lambda grew, so they are boxed: a tree is made of nodes of the common size).
CB_TEST("translate.ir.cil.module_lambda_and_the_new_big_kinds_are_boxed", [] {
    cb::Arena arena;
    ir::Lambda l;
    l.scope = "IL_0000";
    l.locals = {ir::make_sym(ir::SymKind::Local, "a")};
    Node* lam = ir::make(arena, std::move(l));
    CB_CHECK_EQ(ir::kind_name(*lam), std::string_view("Lambda"));
    ir::Node copy = *lam;
    ir::get<ir::Lambda>(&copy)->scope = "IL_0004";
    CB_CHECK_EQ(*ir::get<ir::Lambda>(lam)->scope, std::string("IL_0000"));   // a copy of a boxed node owns its own
    ir::Module m;
    m.scope_refs = {"System.Private.CoreLib"};
    Node* mod = ir::make(arena, std::move(m));
    CB_CHECK(ir::get<ir::Module>(mod)->scope == nullptr);
    CB_CHECK_EQ(ir::kind_name(*ir::make_node(arena, "PatternMatch")), std::string_view("PatternMatch"));
    CB_CHECK_EQ(ir::kind_name(*ir::make_node(arena, "Intrinsic")), std::string_view("Intrinsic"));
});
