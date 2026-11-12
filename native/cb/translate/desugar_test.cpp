// Tests of the lowering pass: each form on a hand-built method (the tree after the pass is read back node by node), the
// forms a back end keeps, what the pass leaves alone, and a real program -- the Java front end's units for a program of
// modern Java, lowered, must equal what the Python implementation lowered them to (testdata/l2_java_units.desugared.json,
// made by codebreaker/translate/desugar.py on the same document).
//
// DIVERGES from desugar.py:
//   * a labelled for-each has its own body lowered before the loop is (translate.desugar.foreach_label_and_nesting): the
//     Python lowered the loop and left a for-each inside it as it was, so the back end declined the method. Fixed in the Python
//     too (tests/translate/test_desugar.py); listed in state/work/typed-port/_bugs/LIST.md;
//   * a method body, branch or loop body that is a bare list of statements (the decompiler's form) is lowered statement by
//     statement; the Python failed with AttributeError on one.
#include "cb/translate/desugar.hpp"

#include <nlohmann/json.hpp>

#include "cb/core/json.hpp"
#include "cb/core/testing.hpp"
#include "cb/translate/l2.hpp"

namespace {

namespace ir = cb::translate::ir;
namespace ds = cb::translate::desugar;
namespace l2 = cb::translate::l2;
using cb::Arena;
using cb::Json;
using ir::Node;

// ---- building trees ---------------------------------------------------------------------------------------------------

struct Build {
    Arena arena;

    template <class K>
    Node* make(K k, ir::TypeP type = {}, std::uint32_t line = 0) {
        return ir::make(arena, std::move(k), std::move(type), line, 1);
    }
    ir::SymP local(const std::string& name, ir::TypeP type) { return ir::make_sym(ir::SymKind::Local, name, std::nullopt, false, std::move(type)); }
    Node* name(const ir::SymP& s) { return make(ir::Name{.sym = s}, s->type); }
    Node* lit(std::int64_t v) { return make(ir::Lit{.value = v}, ir::i32_type()); }
    Node* text(const char* v) { return make(ir::Lit{.value = std::string(v)}, ir::class_type("java.lang.String")); }
    Node* stmt(Node* e) { return make(ir::ExprStmt{.expr = e}); }
    Node* block(std::vector<Node*> s) { return make(ir::Block{.stmts = std::move(s)}); }
    Node* decl(const ir::SymP& s, Node* init = nullptr) { return make(ir::Local{.sym = s, .type = s->type, .init = init}); }
    Node* ret(Node* e) { return make(ir::Return{.expr = e}); }
    Node* brk() { return make(ir::Break{}); }
    Node* yield(Node* e) { return make(ir::Yield{.expr = e}); }
    Node* call(Node* target, const std::string& owner, const std::string& method, std::vector<Node*> args, ir::TypeP type) {
        ir::SymP m = ir::make_sym(ir::SymKind::Method, method, owner);
        return make(ir::Call{.target = target, .sym = m, .args = std::move(args)}, std::move(type));
    }
    Node* case_of(std::vector<Node*> labels, std::vector<Node*> body) {
        return make(ir::Case{.labels = std::move(labels), .body = std::move(body)});
    }
    Node* default_of(std::vector<Node*> body) { return make(ir::Case{.is_default = true, .body = std::move(body)}); }

    // A unit with one class with one method with this body.
    Node* method_in_unit(std::vector<Node*> body, Node** method_out = nullptr) {
        ir::MethodDecl m;
        m.sym = ir::make_sym(ir::SymKind::Method, "m", "p.C");
        m.name = "m";
        m.kind = "method";
        m.body = make(ir::Block{.stmts = std::move(body)});
        Node* mn = make(std::move(m));
        if (method_out != nullptr) *method_out = mn;
        ir::ClassDecl c;
        c.name = "C";
        c.qname = "p.C";
        c.kind = "class";
        c.members = {mn};
        ir::Unit u;
        u.lang = "java";
        u.types = {make(std::move(c))};
        return make(std::move(u));
    }
};

// The statements of a method's body.
std::vector<Node*> body_of(Node* method) { return ir::get<ir::Block>(std::get<Node*>(ir::get<ir::MethodDecl>(method)->body))->stmts; }

std::size_t lower(Build& b, Node* unit, std::vector<ds::Form> keep = {}) {
    const std::vector<Node*> units = {unit};
    return ds::run(b.arena, units, keep);
}

template <class K>
const K* as(Node* n) {
    return ir::get<K>(n);
}

std::size_t count_kind(Node* root, std::string_view kind) {
    std::size_t n = 0;
    for (Node* x : ir::walk(root)) n += ir::kind_name(*x) == kind;
    return n;
}

ir::TypeP list_of(const char* elem) { return ir::class_type("java.util.List", {ir::class_type(elem)}); }

}  // namespace

CB_TEST("translate.desugar.foreach_over_an_array", [] {
    Build b;
    const ir::TypeP ints = ir::array_of(ir::i32_type());
    const ir::SymP xs = b.local("xs", ints), x = b.local("x", ir::i32_type()), sum = b.local("sum", ir::i32_type());
    Node* loop = b.make(ir::ForEach{.var = b.decl(x),
                                    .iterable = b.name(xs),
                                    .body = b.stmt(b.make(ir::Assign{.op = "+", .target = b.name(sum), .value = b.name(x)}, ir::i32_type()))});
    Node* method = nullptr;
    Node* unit = b.method_in_unit({b.decl(sum, b.lit(0)), loop, b.ret(b.name(sum))}, &method);
    CB_CHECK_EQ(lower(b, unit), std::size_t(1));
    // { int sum = 0; { int[] j2x_a1 = xs; for (int j2x_i2 = 0; j2x_i2 < j2x_a1.length; j2x_i2++) { int x = j2x_a1[j2x_i2]; sum += x; } } return sum; }
    const std::vector<Node*> body = body_of(method);
    CB_CHECK_EQ(body.size(), std::size_t(3));
    const auto* pair = as<ir::Block>(body[1]);
    CB_CHECK(pair != nullptr && pair->stmts.size() == 2);
    const auto* copy = as<ir::Local>(pair->stmts[0]);
    CB_CHECK(copy != nullptr);
    CB_CHECK_EQ(copy->sym->name, std::string("j2x_a1"));
    CB_CHECK(copy->type == ints);
    CB_CHECK(as<ir::Name>(copy->init)->sym == xs);   // the array is evaluated once, into the temporary
    const auto* f = as<ir::For>(pair->stmts[1]);
    CB_CHECK(f != nullptr);
    CB_CHECK_EQ(as<ir::Local>(f->init.at(0))->sym->name, std::string("j2x_i2"));
    CB_CHECK_EQ(as<ir::Binary>(f->cond)->op, std::string("<"));
    CB_CHECK(as<ir::Length>(as<ir::Binary>(f->cond)->right) != nullptr);
    CB_CHECK(f->cond->type == ir::bool_type());
    const auto* step = as<ir::Unary>(f->update.at(0));
    CB_CHECK(step->op == "++" && step->postfix);
    const auto* inner = as<ir::Block>(std::get<Node*>(f->body));
    CB_CHECK_EQ(inner->stmts.size(), std::size_t(2));
    const auto* element = as<ir::Local>(inner->stmts[0]);
    CB_CHECK(element->sym == x);   // the loop's own variable keeps its symbol
    const auto* at = as<ir::Index>(element->init);
    CB_CHECK(at != nullptr && element->init->type == ir::i32_type());
    CB_CHECK_EQ(as<ir::Name>(std::get<Node*>(at->index))->sym->name, std::string("j2x_i2"));
    CB_CHECK_EQ(count_kind(unit, "ForEach"), std::size_t(0));
    // nothing left to lower
    CB_CHECK_EQ(lower(b, unit), std::size_t(0));
});

CB_TEST("translate.desugar.foreach_over_an_iterable", [] {
    Build b;
    const auto run = [&](ir::TypeP type, ir::TypeP* element_type) {
        const ir::SymP xs = b.local("xs", type), s = b.local("s", ir::class_type("java.lang.String"));
        Node* loop = b.make(ir::ForEach{.var = b.decl(s), .iterable = b.name(xs), .body = b.block({})});
        Node* method = nullptr;
        Node* unit = b.method_in_unit({loop}, &method);
        CB_CHECK_EQ(lower(b, unit), std::size_t(1));
        const std::vector<Node*> body = body_of(method);
        CB_CHECK_EQ(body.size(), std::size_t(1));
        const auto* f = as<ir::For>(body[0]);   // no set-up: the loop stands alone
        CB_CHECK(f != nullptr);
        const auto* it = as<ir::Local>(f->init.at(0));
        CB_CHECK_EQ(it->sym->name, std::string("j2x_it1"));
        CB_CHECK_EQ(ir::to_text(*it->type), std::string("java.util.Iterator<") + ir::to_text(**element_type) + ">");
        const auto* start = as<ir::Call>(it->init);
        CB_CHECK_EQ(start->sym->name, std::string("iterator"));
        CB_CHECK_EQ(start->sym->owner.value_or(""), std::string("java.util.List"));
        CB_CHECK_EQ(as<ir::Call>(f->cond)->sym->name, std::string("hasNext"));
        CB_CHECK(f->update.empty());
        const auto* first = as<ir::Local>(as<ir::Block>(std::get<Node*>(f->body))->stmts.at(0));
        CB_CHECK_EQ(as<ir::Call>(first->init)->sym->name, std::string("next"));
        CB_CHECK(first->init->type == *element_type);
    };
    ir::TypeP string = ir::class_type("java.lang.String");
    run(list_of("java.lang.String"), &string);
    // a wildcard that extends a type iterates as that type
    ir::TypeP number = ir::class_type("java.lang.Number");
    ir::WildcardType wild;
    wild.bound = ir::WildcardBound{ir::BoundMode::Extends, number};
    run(ir::class_type("java.util.List", {ir::make_type(wild)}), &number);
    // a wildcard that is a super bound, a type variable and a raw type iterate as Object
    ir::TypeP object = ir::class_type("java.lang.Object");
    ir::WildcardType super;
    super.bound = ir::WildcardBound{ir::BoundMode::Super, number};
    run(ir::class_type("java.util.List", {ir::make_type(super)}), &object);
    run(ir::class_type("java.util.List", {ir::typevar_type("T")}), &object);
    run(ir::class_type("java.util.List"), &object);
});

CB_TEST("translate.desugar.foreach_left_alone", [] {
    Build b;
    // over a type that is neither an array nor a class: left as it was
    const ir::SymP xs = b.local("xs", ir::any_type()), s = b.local("s", ir::i32_type());
    Node* method = nullptr;
    Node* unit = b.method_in_unit({b.make(ir::ForEach{.var = b.decl(s), .iterable = b.name(xs), .body = b.block({})})}, &method);
    CB_CHECK_EQ(lower(b, unit), std::size_t(0));
    CB_CHECK_EQ(count_kind(unit, "ForEach"), std::size_t(1));
    // a form the back end writes itself is kept
    const ir::SymP ys = b.local("ys", ir::array_of(ir::i32_type()));
    Node* unit2 = b.method_in_unit({b.make(ir::ForEach{.var = b.decl(s), .iterable = b.name(ys), .body = b.block({})})});
    CB_CHECK_EQ(lower(b, unit2, {ds::Form::ForEach}), std::size_t(0));
    CB_CHECK_EQ(count_kind(unit2, "ForEach"), std::size_t(1));
    CB_CHECK_EQ(lower(b, unit2), std::size_t(1));
});

CB_TEST("translate.desugar.foreach_label_and_nesting", [] {
    Build b;
    const ir::TypeP ints = ir::array_of(ir::i32_type());
    const ir::SymP xs = b.local("xs", ints), ys = b.local("ys", ints), x = b.local("x", ir::i32_type()), y = b.local("y", ir::i32_type());
    Node* inner = b.make(ir::ForEach{.var = b.decl(y), .iterable = b.name(ys), .body = b.block({b.make(ir::Continue{})})});
    Node* outer = b.make(ir::ForEach{.var = b.decl(x), .iterable = b.name(xs), .body = b.block({inner})});
    Node* labelled = b.make(ir::Labeled{.label = "outer", .body = outer});
    Node* method = nullptr;
    Node* unit = b.method_in_unit({labelled}, &method);
    CB_CHECK_EQ(lower(b, unit), std::size_t(2));   // both loops, the inner one too
    CB_CHECK_EQ(count_kind(unit, "ForEach"), std::size_t(0));
    // the label goes on the loop, not on its set-up: { int[] a = xs; outer: for (...) {...} }
    const auto* wrap = as<ir::Block>(body_of(method).at(0));
    CB_CHECK(wrap != nullptr && wrap->stmts.size() == 2);
    CB_CHECK(as<ir::Local>(wrap->stmts[0]) != nullptr);
    const auto* lab = as<ir::Labeled>(wrap->stmts[1]);
    CB_CHECK(lab != nullptr && lab->label == "outer");
    CB_CHECK(as<ir::For>(std::get<Node*>(lab->body)) != nullptr);
});

CB_TEST("translate.desugar.switch_expression_as_a_value", [] {
    Build b;
    const ir::SymP k = b.local("k", ir::i32_type()), r = b.local("r", ir::i32_type());
    Node* sw = b.make(ir::SwitchExpr{.expr = b.name(k),
                                     .cases = {b.case_of({b.lit(1)}, {b.yield(b.lit(10))}), b.default_of({b.yield(b.lit(20))})}},
                      ir::i32_type());
    Node* method = nullptr;
    Node* unit = b.method_in_unit({b.decl(r, sw)}, &method);
    CB_CHECK_EQ(lower(b, unit), std::size_t(1));
    // int r; j2x_sw1: switch (k) { case 1: { r = 10; break j2x_sw1; } default: { r = 20; break j2x_sw1; } }
    const std::vector<Node*> body = body_of(method);
    CB_CHECK_EQ(body.size(), std::size_t(2));
    CB_CHECK(as<ir::Local>(body[0])->init == nullptr);
    CB_CHECK(as<ir::Local>(body[0])->sym == r);
    const auto* lab = as<ir::Labeled>(body[1]);
    CB_CHECK(lab != nullptr && lab->label == "j2x_sw1");
    const auto* st = as<ir::Switch>(std::get<Node*>(lab->body));
    CB_CHECK(st != nullptr && st->cases.size() == 2);
    const auto* first = as<ir::Case>(st->cases[0]);
    CB_CHECK_EQ(first->labels.size(), std::size_t(1));
    const auto* arm = as<ir::Block>(first->body.at(0));
    CB_CHECK_EQ(arm->stmts.size(), std::size_t(2));
    const auto* assign = as<ir::Assign>(as<ir::ExprStmt>(arm->stmts[0])->expr);
    CB_CHECK(assign != nullptr && !assign->op && as<ir::Name>(assign->target)->sym == r);
    CB_CHECK_EQ(std::get<std::int64_t>(as<ir::Lit>(assign->value)->value), std::int64_t(10));
    CB_CHECK(as<ir::Break>(arm->stmts[1])->label == "j2x_sw1");
    CB_CHECK(as<ir::Case>(st->cases[1])->is_default);
    CB_CHECK_EQ(count_kind(unit, "SwitchExpr"), std::size_t(0));
    CB_CHECK_EQ(count_kind(unit, "Yield"), std::size_t(0));
});

CB_TEST("translate.desugar.switch_expression_returned_and_assigned", [] {
    Build b;
    const ir::SymP k = b.local("k", ir::i32_type()), t = b.local("t", ir::i32_type());
    const auto make_switch = [&] {
        return b.make(ir::SwitchExpr{.expr = b.name(k), .cases = {b.default_of({b.yield(b.lit(1))})}}, ir::i32_type());
    };
    Node* method = nullptr;
    Node* unit = b.method_in_unit({b.decl(t), b.stmt(b.make(ir::Assign{.target = b.name(t), .value = make_switch()}, ir::i32_type())),
                                   b.ret(make_switch())},
                                  &method);
    CB_CHECK_EQ(lower(b, unit), std::size_t(2));
    const std::vector<Node*> body = body_of(method);
    CB_CHECK_EQ(body.size(), std::size_t(3));
    // an assignment of the value goes straight into the target's switch
    CB_CHECK(as<ir::Labeled>(body[1]) != nullptr);
    // a return gets a temporary, then returns it
    const auto* wrap = as<ir::Block>(body[2]);
    CB_CHECK(wrap != nullptr && wrap->stmts.size() == 3);
    const auto* tmp = as<ir::Local>(wrap->stmts[0]);
    CB_CHECK(tmp->sym->name.starts_with("j2x_sw"));
    CB_CHECK(as<ir::Labeled>(wrap->stmts[1]) != nullptr);
    CB_CHECK(as<ir::Name>(as<ir::Return>(wrap->stmts[2])->expr)->sym == tmp->sym);
});

CB_TEST("translate.desugar.switch_expression_inside_an_expression", [] {
    Build b;
    const ir::SymP k = b.local("k", ir::i32_type()), list = b.local("list", list_of("java.lang.Integer"));
    const auto make_switch = [&] {
        return b.make(ir::SwitchExpr{.expr = b.name(k), .cases = {b.default_of({b.yield(b.lit(1))})}}, ir::i32_type());
    };
    // list.add(switch ...): the call is the only thing that runs, after it, so the switch may come out before the statement
    {
        Node* method = nullptr;
        Node* unit = b.method_in_unit({b.stmt(b.call(b.name(list), "java.util.List", "add", {make_switch()}, ir::bool_type()))}, &method);
        CB_CHECK_EQ(lower(b, unit), std::size_t(1));
        const std::vector<Node*> body = body_of(method);
        CB_CHECK_EQ(body.size(), std::size_t(1));
        const auto* wrap = as<ir::Block>(body[0]);
        CB_CHECK(wrap != nullptr && wrap->stmts.size() == 3);   // temporary, switch, the statement
        CB_CHECK(as<ir::ExprStmt>(wrap->stmts[2]) != nullptr);
        CB_CHECK_EQ(count_kind(body[0], "SwitchExpr"), std::size_t(0));
        const auto* arg = as<ir::Name>(as<ir::Call>(as<ir::ExprStmt>(wrap->stmts[2])->expr)->args.at(0));
        CB_CHECK(arg != nullptr && arg->sym->name.starts_with("j2x_sw"));
    }
    // another call among the operands: the order of effects would change, so it is left for the back end to decline
    {
        Node* unit = b.method_in_unit({b.stmt(b.call(b.name(list), "java.util.List", "add",
                                                      {b.call(nullptr, "p.C", "next", {}, ir::i32_type()), make_switch()},
                                                      ir::bool_type()))});
        CB_CHECK_EQ(lower(b, unit), std::size_t(0));
        CB_CHECK_EQ(count_kind(unit, "SwitchExpr"), std::size_t(1));
    }
    // two switches in one statement
    {
        Node* unit = b.method_in_unit({b.stmt(b.call(b.name(list), "java.util.List", "add", {make_switch(), make_switch()}, ir::bool_type()))});
        CB_CHECK_EQ(lower(b, unit), std::size_t(0));
    }
});

CB_TEST("translate.desugar.switch_on_a_string", [] {
    Build b;
    const ir::TypeP string = ir::class_type("java.lang.String");
    const ir::SymP s = b.local("s", string), n = b.local("n", ir::i32_type());
    const auto assign = [&](std::int64_t v) { return b.stmt(b.make(ir::Assign{.target = b.name(n), .value = b.lit(v)}, ir::i32_type())); };
    // switch (s) { case "a": case "b": n = 1; break; default: n = 9; }   (the labels of "a" fall into "b": no, "a" is empty -> a fall-through)
    {
        Node* sw = b.make(ir::Switch{.expr = b.name(s),
                                     .cases = {b.case_of({b.text("a")}, {assign(1), b.brk()}),
                                               b.case_of({b.text("b"), b.text("c")}, {assign(2), b.brk()}), b.default_of({assign(9)})}});
        Node* method = nullptr;
        Node* unit = b.method_in_unit({b.decl(n, b.lit(0)), sw}, &method);
        CB_CHECK_EQ(lower(b, unit), std::size_t(1));
        const std::vector<Node*> body = body_of(method);
        CB_CHECK_EQ(body.size(), std::size_t(2));
        // { String j2x_s2 = s; j2x_sw1: { if (j2x_s2.equals("a")) {...} else if (j2x_s2.equals("b") || j2x_s2.equals("c")) {...} else {...} } }
        const auto* wrap = as<ir::Block>(body[1]);
        CB_CHECK(wrap != nullptr && wrap->stmts.size() == 2);
        const auto* sel = as<ir::Local>(wrap->stmts[0]);
        CB_CHECK_EQ(sel->sym->name, std::string("j2x_s2"));   // (the label is numbered first)
        CB_CHECK(as<ir::Name>(sel->init)->sym == s);
        const auto* lab = as<ir::Labeled>(wrap->stmts[1]);
        CB_CHECK(lab != nullptr && lab->label == "j2x_sw1");
        const auto* chain = as<ir::Block>(std::get<Node*>(lab->body));
        const auto* first = as<ir::If>(chain->stmts.at(0));
        CB_CHECK(first != nullptr);
        CB_CHECK_EQ(as<ir::Call>(first->cond)->sym->name, std::string("equals"));
        CB_CHECK(as<ir::Call>(first->cond)->sym->owner == "java.lang.String");
        const auto* second = as<ir::If>(std::get<Node*>(first->orelse));
        CB_CHECK(second != nullptr);
        CB_CHECK_EQ(as<ir::Binary>(second->cond)->op, std::string("||"));
        CB_CHECK(as<ir::Block>(std::get<Node*>(second->orelse)) != nullptr);   // the default arm
        // the breaks leaving the switch name its label
        bool labelled = false;
        for (Node* x : ir::walk(body[1])) {
            if (const auto* br = ir::get<ir::Break>(x)) labelled = br->label == "j2x_sw1";
        }
        CB_CHECK(labelled);
        CB_CHECK_EQ(count_kind(body[1], "Switch"), std::size_t(0));
    }
    // a fall-through (an arm that does not leave) is not lowered
    {
        Node* sw = b.make(ir::Switch{.expr = b.name(s), .cases = {b.case_of({b.text("a")}, {assign(1)}), b.case_of({b.text("b")}, {assign(2), b.brk()})}});
        Node* unit = b.method_in_unit({sw});
        CB_CHECK_EQ(lower(b, unit), std::size_t(0));
        CB_CHECK_EQ(count_kind(unit, "Switch"), std::size_t(1));
    }
    // a default sharing labels with another is not lowered either
    {
        ir::Case both;
        both.labels = {b.text("a")};
        both.is_default = true;
        both.body = {assign(1), b.brk()};
        Node* sw = b.make(ir::Switch{.expr = b.name(s), .cases = {b.make(std::move(both))}});
        Node* unit = b.method_in_unit({sw});
        CB_CHECK_EQ(lower(b, unit), std::size_t(0));
    }
    // a break of an inner loop is not the switch's
    {
        Node* loop = b.make(ir::While{.cond = b.make(ir::Lit{.value = true}, ir::bool_type()), .body = b.block({b.brk()})});
        Node* sw = b.make(ir::Switch{.expr = b.name(s), .cases = {b.case_of({b.text("a")}, {loop, b.brk()})}});
        Node* unit = b.method_in_unit({sw});
        CB_CHECK_EQ(lower(b, unit), std::size_t(1));
        std::size_t unlabelled = 0;
        for (Node* x : ir::walk(unit)) {
            if (const auto* br = ir::get<ir::Break>(x)) unlabelled += !br->label;
        }
        CB_CHECK_EQ(unlabelled, std::size_t(1));   // the loop's own
    }
    // the same form with the forms kept: untouched
    {
        Node* sw = b.make(ir::Switch{.expr = b.name(s), .cases = {b.case_of({b.text("a")}, {b.brk()})}});
        Node* unit = b.method_in_unit({sw});
        CB_CHECK_EQ(lower(b, unit, {ds::Form::StringSwitch}), std::size_t(0));
    }
});

CB_TEST("translate.desugar.string_switch_expression", [] {
    // a switch expression on a String lowers to the if-chain inside its label, the yields as assignments
    Build b;
    const ir::TypeP string = ir::class_type("java.lang.String");
    const ir::SymP s = b.local("s", string), r = b.local("r", ir::i32_type());
    Node* sw = b.make(ir::SwitchExpr{.expr = b.name(s), .cases = {b.case_of({b.text("a")}, {b.yield(b.lit(1))}), b.default_of({b.yield(b.lit(2))})}},
                      ir::i32_type());
    Node* method = nullptr;
    Node* unit = b.method_in_unit({b.decl(r, sw)}, &method);
    CB_CHECK_EQ(lower(b, unit), std::size_t(2));   // the expression, and the string switch it became
    CB_CHECK_EQ(count_kind(unit, "Switch"), std::size_t(0));
    CB_CHECK_EQ(count_kind(unit, "Yield"), std::size_t(0));
    CB_CHECK_EQ(count_kind(unit, "If"), std::size_t(1));
});

CB_TEST("translate.desugar.instanceof_patterns", [] {
    Build b;
    const ir::TypeP object = ir::class_type("java.lang.Object"), string = ir::class_type("java.lang.String");
    const ir::SymP o = b.local("o", object), str = b.local("str", string);
    // if (o instanceof String str && str.isEmpty()) return;
    Node* test = b.make(ir::InstanceOf{.x = b.name(o), .test = string, .binding = b.decl(str)}, ir::bool_type());
    Node* cond = b.make(ir::Binary{.op = "&&", .left = test, .right = b.call(b.name(str), "java.lang.String", "isEmpty", {}, ir::bool_type())},
                        ir::bool_type());
    Node* method = nullptr;
    Node* unit = b.method_in_unit({b.make(ir::If{.cond = cond, .then = b.block({b.make(ir::Return{})})})}, &method);
    CB_CHECK_EQ(lower(b, unit), std::size_t(1));
    const std::vector<Node*> body = body_of(method);
    CB_CHECK_EQ(body.size(), std::size_t(2));   // the variable declared before the statement
    const auto* declared = as<ir::Local>(body[0]);
    CB_CHECK(declared != nullptr && declared->sym == str && declared->init == nullptr);
    const auto* i = as<ir::If>(body[1]);
    const auto* both = as<ir::Binary>(i->cond);
    // (o instanceof String && (str = (String) o) != null) && str.isEmpty()
    const auto* group = as<ir::Paren>(both->left);
    CB_CHECK(group != nullptr);
    const auto* conj = as<ir::Binary>(group->x);
    CB_CHECK_EQ(conj->op, std::string("&&"));
    const auto* plain_test = as<ir::InstanceOf>(conj->left);
    CB_CHECK(plain_test != nullptr && plain_test->binding == nullptr && plain_test->test == string);
    const auto* bind = as<ir::Binary>(conj->right);
    CB_CHECK_EQ(bind->op, std::string("!="));
    const auto* assign = as<ir::Assign>(as<ir::Paren>(bind->left)->x);
    CB_CHECK(assign != nullptr && as<ir::Name>(assign->target)->sym == str);
    const auto* cast = as<ir::Cast>(assign->value);
    CB_CHECK(cast != nullptr && cast->x != nullptr && assign->value->type == string);
    CB_CHECK(as<ir::Name>(cast->x)->sym == o);   // a copy of the operand: it is read twice
    CB_CHECK(as<ir::Lit>(bind->right) != nullptr && bind->right->type == ir::null_type());
    CB_CHECK_EQ(count_kind(unit, "InstanceOf"), std::size_t(1));
    // run again: nothing to do (the pattern has no binding now)
    CB_CHECK_EQ(lower(b, unit), std::size_t(0));
});

CB_TEST("translate.desugar.instanceof_pattern_names", [] {
    Build b;
    const ir::TypeP object = ir::class_type("java.lang.Object"), string = ir::class_type("java.lang.String"), integer = ir::class_type("java.lang.Integer");
    const ir::SymP o = b.local("o", object), v1 = b.local("v", string), v2 = b.local("v", integer);
    const auto pattern = [&](const ir::SymP& v, const ir::TypeP& t) {
        return b.make(ir::If{.cond = b.make(ir::InstanceOf{.x = b.name(o), .test = t, .binding = b.decl(v)}, ir::bool_type()), .then = b.block({})});
    };
    // the same variable of the same type twice in a list: declared once
    {
        Node* method = nullptr;
        Node* unit = b.method_in_unit({pattern(v1, string), pattern(v1, string)}, &method);
        CB_CHECK_EQ(lower(b, unit), std::size_t(2));
        CB_CHECK_EQ(body_of(method).size(), std::size_t(3));
    }
    // the same name under another type: left as it is (a back end declines the method)
    {
        Node* method = nullptr;
        Node* unit = b.method_in_unit({pattern(v1, string), pattern(v2, integer)}, &method);
        CB_CHECK_EQ(lower(b, unit), std::size_t(1));
        CB_CHECK_EQ(count_kind(unit, "InstanceOf"), std::size_t(2));
        std::size_t with_binding = 0;
        for (Node* x : ir::walk(unit)) {
            if (const auto* io = ir::get<ir::InstanceOf>(x)) with_binding += io->binding != nullptr;
        }
        CB_CHECK_EQ(with_binding, std::size_t(1));
    }
    // the operand is not plain (a call): not lowered
    {
        Node* test = b.make(ir::InstanceOf{.x = b.call(nullptr, "p.C", "get", {}, object), .test = string, .binding = b.decl(v1)}, ir::bool_type());
        Node* unit = b.method_in_unit({b.make(ir::If{.cond = test, .then = b.block({})})});
        CB_CHECK_EQ(lower(b, unit), std::size_t(0));
    }
    // kept
    {
        Node* unit = b.method_in_unit({pattern(v1, string)});
        CB_CHECK_EQ(lower(b, unit, {ds::Form::Pattern}), std::size_t(0));
    }
});

CB_TEST("translate.desugar.bodies_that_are_bare_lists", [] {
    // the decompiler's and the Python front end's bodies are lists of statements, not blocks
    Build b;
    const ir::TypeP ints = ir::array_of(ir::i32_type());
    const ir::SymP xs = b.local("xs", ints), x = b.local("x", ir::i32_type());
    Node* loop = b.make(ir::ForEach{.var = b.decl(x), .iterable = b.name(xs), .body = b.block({})});
    Node* inner = b.make(ir::ForEach{.var = b.decl(x), .iterable = b.name(xs), .body = b.block({})});
    Node* branch = b.make(ir::If{.cond = b.make(ir::Lit{.value = true}, ir::bool_type()), .then = std::vector<Node*>{inner}});
    Node* unit = b.method_in_unit({});
    Node* method = ir::get<ir::ClassDecl>(ir::get<ir::Unit>(unit)->types.at(0))->members.at(0);
    ir::get<ir::MethodDecl>(method)->body = std::vector<Node*>{loop, branch};
    CB_CHECK_EQ(lower(b, unit), std::size_t(2));
    CB_CHECK_EQ(count_kind(unit, "ForEach"), std::size_t(0));
    const auto& body = std::get<std::vector<Node*>>(ir::get<ir::MethodDecl>(method)->body);
    CB_CHECK_EQ(body.size(), std::size_t(2));   // still a list
    CB_CHECK(std::holds_alternative<std::vector<Node*>>(ir::get<ir::If>(body[1])->then));
});

CB_TEST("translate.desugar.lambdas_and_anonymous_classes", [] {
    Build b;
    const ir::TypeP ints = ir::array_of(ir::i32_type());
    const ir::SymP xs = b.local("xs", ints), x = b.local("x", ir::i32_type());
    // a lambda whose body has a for-each, and an anonymous class with a method that has one
    Node* lam = b.make(ir::Lambda{.body = b.block({b.make(ir::ForEach{.var = b.decl(x), .iterable = b.name(xs), .body = b.block({})})})});
    ir::MethodDecl inner_method;
    inner_method.name = "run";
    inner_method.kind = "method";
    inner_method.body = b.block({b.make(ir::ForEach{.var = b.decl(x), .iterable = b.name(xs), .body = b.block({})})});
    ir::ClassDecl anon;
    anon.name = "";
    anon.members = {b.make(std::move(inner_method))};
    Node* made = b.make(ir::New{.body = b.make(std::move(anon))});
    Node* unit = b.method_in_unit({b.stmt(lam), b.stmt(made)});
    CB_CHECK_EQ(lower(b, unit), std::size_t(2));
    CB_CHECK_EQ(count_kind(unit, "ForEach"), std::size_t(0));
    CB_CHECK_EQ(count_kind(unit, "For"), std::size_t(2));
});

CB_TEST("translate.desugar.nested_statements_and_try", [] {
    // loops inside an if, a try's body, catch and finally, a sync, an inner class, an enum constant's body
    Build b;
    const ir::TypeP ints = ir::array_of(ir::i32_type());
    const ir::SymP xs = b.local("xs", ints), x = b.local("x", ir::i32_type());
    const auto each = [&] { return b.make(ir::ForEach{.var = b.decl(x), .iterable = b.name(xs), .body = b.block({})}); };
    ir::Catch c;
    c.body = b.block({each()});
    Node* t = b.make(ir::Try{.body = b.block({each()}), .catches = {b.make(std::move(c))}, .finally_body = b.block({each()})});
    Node* s = b.make(ir::Sync{.lock = b.name(xs), .body = b.block({each()})});
    Node* i = b.make(ir::If{.cond = b.make(ir::Lit{.value = true}, ir::bool_type()), .then = each(), .orelse = b.block({each()})});
    Node* w = b.make(ir::While{.cond = b.make(ir::Lit{.value = true}, ir::bool_type()), .body = each()});
    ir::MethodDecl local_method;
    local_method.name = "g";
    local_method.kind = "method";
    local_method.body = b.block({each()});
    ir::ClassDecl local_class;
    local_class.name = "L";
    local_class.members = {b.make(std::move(local_method))};
    Node* lc = b.make(ir::LocalClass{.decl = b.make(std::move(local_class))});
    Node* unit = b.method_in_unit({t, s, i, w, lc});
    CB_CHECK_EQ(lower(b, unit), std::size_t(8));
    CB_CHECK_EQ(count_kind(unit, "ForEach"), std::size_t(0));
    // a loop that stood alone as a branch is one statement again (its set-up and loop in one block), not a list
    CB_CHECK(ir::is<ir::Block>(std::get<Node*>(as<ir::If>(i)->then)));
    CB_CHECK(ir::is<ir::Block>(std::get<Node*>(as<ir::While>(w)->body)));
});

CB_TEST("translate.desugar.a_real_program", [] {
    // The Java front end's units for a program of modern Java (for-each, switch expressions, a string switch, patterns),
    // lowered here and by codebreaker/translate/desugar.py: the same documents.
    const Json units = cb::json::load(cb::testing::data_path("translate/testdata/l2_java_units.json"));
    const Json want = cb::json::load(cb::testing::data_path("translate/testdata/l2_java_units.desugared.json"));
    Arena arena;
    std::vector<Node*> trees;
    for (const Json& u : units) trees.push_back(l2::from_json(arena, u));
    std::size_t before = 0;
    for (Node* t : trees) before += count_kind(t, "ForEach") + count_kind(t, "SwitchExpr");
    CB_CHECK(before > 0);
    const std::size_t lowered = ds::run(arena, trees);
    CB_CHECK_EQ(lowered, want.at("lowered").get<std::size_t>());
    for (Node* t : trees) CB_CHECK_EQ(count_kind(t, "ForEach"), std::size_t(0));
    // (the two documents as the Python wrote them and as this wrote them: key order and null slots do not count)
    Json got = Json::array();
    for (Node* t : trees) got.push_back(l2::to_json(*t));
    Arena reread;
    Json again = Json::array();
    for (const Json& d : want.at("units")) again.push_back(l2::to_json(*l2::from_json(reread, d)));
    CB_CHECK(got == again);
    // lowering the lowered program lowers nothing
    CB_CHECK_EQ(ds::run(arena, trees), std::size_t(0));
});
