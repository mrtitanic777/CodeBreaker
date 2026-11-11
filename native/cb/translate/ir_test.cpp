// Tests of cb/translate/ir: the type model (what `==` and the helpers say of a type), symbols, and trees -- built the way a
// front end builds them (here the Java method `int f(int a) { if (a > 0) { return a * 2; } return -a; }`, and a C#
// property, a lambda, a switch expression and a `new` with all its slots) and read back the way a pass reads them: walked
// in source order, rewired in place, cloned. Every kind of node is made by name and named back, which is how L2 documents
// are read.
//
// DIVERGES from the Python model (translate/ir.py), by design: node slots are typed members, not a dict of whatever a
// front end set (so an unknown slot is a compile error, not a TypeError at run time); the details of a type are fields of
// its kind (a pointer's space, a function's parameters), not an `info` dict; a list slot that is empty is the same as one
// that is absent; there is no `repr` (a type has `to_text`); the Python's integers are 64-bit.
#include "cb/translate/ir.hpp"

#include <unordered_set>

#include "cb/core/testing.hpp"

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
        "Catch", "Labeled", "Goto", "MachineFragment", "Sync", "Empty", "Comment", "LocalClass", "Assert", "CtorCall", "Yield",
        "Lit", "Name", "Select", "TypeName", "This", "Super", "Call", "New", "NewArray", "Index", "Length", "Unary", "Binary",
        "Concat", "Assign", "Cond", "Cast", "InstanceOf", "ClassLit", "Lambda", "MethodRef", "SwitchExpr", "AddressOf",
        "FarAddress", "Piece", "Join", "Paren", "Unsupported"};
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
    CB_CHECK_EQ(seen.size(), std::size_t{63});
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
