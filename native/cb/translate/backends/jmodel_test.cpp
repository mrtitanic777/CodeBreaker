// Tests of cb/translate/backends/jmodel: the object model of a Java program, on a program built the way the Java front end
// builds one (resolved symbols, typed expressions) -- the classes below, in package p:
//
//     interface Shape { double area(); }                       class Circle implements Shape { double area() {..} }
//     class Node implements Comparable<Node> { Node next; int compareTo(Node o); String toString(); private int hidden(); }
//     class Outer { Inner child; class Inner {}  static class Nested {}
//                   void run(int x) { class Loc { int g() { int y; return x; } }  new Runnable() { void run() { x; } }; }
//                   static void sm() { new Runnable() { void run() {} }; } }
//     enum Color { RED { }, GREEN }                            class Bag { List<String> items; }
//     interface Two { void f(); void g(); }                    interface Mixed { void f(); default void g() {..} static void h() {..} }
//
// DIVERGES from the Python model (jmodel.py), by design: classes are found by pointer, not by `id()`; a method or class
// that does not exist is an empty optional or null, not a `None` in a tuple; the model takes the units alone (the Python's
// unused `package_names` flag is gone); a `Name` with a missing type is captured with no type, not an error.
#include "cb/translate/backends/jmodel.hpp"

#include "cb/core/testing.hpp"

namespace {

namespace ir = cb::translate::ir;
namespace jm = cb::translate::backends::jmodel;
using ir::Node;
using ir::TypeP;

TypeP jm_cls(const std::string& name, std::vector<TypeP> args = {}) { return ir::class_type(name, std::move(args)); }

struct JmProgram {
    cb::Arena arena;
    Node* unit = nullptr;
    // the nodes the tests look at
    Node* shape_area = nullptr;
    Node* circle_area = nullptr;
    Node* compare_to = nullptr;
    Node* to_string = nullptr;
    Node* hidden = nullptr;
    Node* outer_run = nullptr;
    ir::SymP x = ir::make_sym(ir::SymKind::Param, "x", "p.Outer.run", false, ir::i32_type());
    ir::SymP compare_sym;

    Node* param(const std::string& name, TypeP type) {
        return ir::make(arena, ir::Param{.sym = ir::make_sym(ir::SymKind::Param, name, "", false, type), .type = type});
    }
    Node* method(const std::string& owner, const std::string& name, std::vector<std::pair<std::string, TypeP>> params, TypeP ret,
                 bool body, std::vector<std::string> mods = {}, std::vector<Node*> stmts = {}, std::string kind = "method") {
        ir::MethodDecl m;
        const bool is_static = std::find(mods.begin(), mods.end(), "static") != mods.end();
        m.sym = ir::make_sym(kind == "ctor" ? ir::SymKind::Ctor : ir::SymKind::Method, name, owner, is_static);
        for (auto& [n, t] : params) {
            m.params.push_back(param(n, t));
            m.sym->params.push_back(t);
        }
        m.sym->ret = ret;
        m.name = name;
        m.kind = kind;
        m.ret = ret;
        m.mods = std::move(mods);
        if (body) m.body = ir::make(arena, ir::Block{.stmts = std::move(stmts)});
        return ir::make(arena, std::move(m));
    }
    Node* field(const std::string& owner, const std::string& name, TypeP type, bool is_static = false, Node* init = nullptr) {
        ir::FieldDecl f;
        f.sym = ir::make_sym(ir::SymKind::Field, name, owner, is_static, type);
        f.type = type;
        f.init = init;
        return ir::make(arena, std::move(f));
    }
    Node* klass(const std::string& name, const std::string& qname, const std::string& kind, std::vector<Node*> members,
                TypeP extends = {}, std::vector<TypeP> implements = {}, std::vector<std::string> mods = {}) {
        ir::ClassDecl c;
        if (!name.empty()) c.name = name;
        if (!qname.empty()) c.qname = qname;
        c.kind = kind;
        c.members = std::move(members);
        c.extends = std::move(extends);
        c.implements = std::move(implements);
        c.mods = std::move(mods);
        return ir::make(arena, std::move(c));
    }
    Node* use_x() { return ir::make(arena, ir::Name{.sym = x}, ir::i32_type()); }
    Node* ret_x() { return ir::make(arena, ir::Return{.expr = use_x()}); }
    Node* anonymous_runnable(Node* body_stmt) {
        Node* run = method("", "run", {}, ir::void_type(), true, {}, {body_stmt});
        ir::New n;
        n.sym = ir::make_sym(ir::SymKind::Ctor, "<init>", "java.lang.Runnable");
        n.body = klass("", "", "class", {run});
        return ir::make(arena, std::move(n), jm_cls("java.lang.Runnable"));
    }

    JmProgram() {
        const TypeP f64 = ir::prim_type(ir::Prim::F64);
        const TypeP node = jm_cls("p.Node");
        shape_area = method("p.Shape", "area", {}, f64, false);
        circle_area = method("p.Circle", "area", {}, f64, true);
        compare_to = method("p.Node", "compareTo", {{"o", node}}, ir::i32_type(), true);
        to_string = method("p.Node", "toString", {}, jm_cls("java.lang.String"), true);
        hidden = method("p.Node", "hidden", {}, ir::i32_type(), true, {"private"});
        compare_sym = ir::get<ir::MethodDecl>(compare_to)->sym;

        // Outer.run: a local class reading x, and an anonymous Runnable reading x
        Node* y = ir::make(arena, ir::Local{.sym = ir::make_sym(ir::SymKind::Local, "y", "p.Outer.run", false, ir::i32_type()),
                                            .type = ir::i32_type()});
        Node* use_y = ir::make(arena, ir::Name{.sym = ir::get<ir::Local>(y)->sym}, ir::i32_type());
        Node* g = method("", "g", {}, ir::i32_type(), true, {}, {y, ir::make(arena, ir::ExprStmt{.expr = use_y}), ret_x()});
        Node* loc = ir::make(arena, ir::LocalClass{.decl = klass("Loc", "", "class", {g})});
        Node* anon = ir::make(arena, ir::ExprStmt{.expr = anonymous_runnable(ir::make(arena, ir::ExprStmt{.expr = use_x()}))});
        outer_run = method("p.Outer", "run", {{"x", ir::i32_type()}}, ir::void_type(), true, {}, {loc, anon});
        Node* sm = method("p.Outer", "sm", {}, ir::void_type(), true, {"static"},
                          {ir::make(arena, ir::ExprStmt{.expr = anonymous_runnable(ir::make(arena, ir::Empty{}))})});
        Node* inner = klass("Inner", "p.Outer.Inner", "class", {});
        Node* nested = klass("Nested", "p.Outer.Nested", "class", {}, {}, {}, {"static"});

        ir::EnumConst red;
        red.sym = ir::make_sym(ir::SymKind::EnumConst, "RED", "p.Color", true);
        red.body = klass("", "", "class", {});
        ir::EnumConst green;
        green.sym = ir::make_sym(ir::SymKind::EnumConst, "GREEN", "p.Color", true);

        ir::Unit u;
        u.package = "p";
        u.lang = "java";
        u.types = {
            klass("Shape", "p.Shape", "interface", {shape_area}),
            klass("Circle", "p.Circle", "class", {circle_area}, {}, {jm_cls("p.Shape")}),
            klass("Node", "p.Node", "class", {field("p.Node", "next", node), compare_to, to_string, hidden}, {},
                  {jm_cls("java.lang.Comparable", {node})}),
            klass("Outer", "p.Outer", "class", {field("p.Outer", "child", jm_cls("p.Outer.Inner")), inner, nested, outer_run, sm}),
            klass("Color", "p.Color", "enum", {ir::make(arena, std::move(red)), ir::make(arena, std::move(green))}),
            klass("Bag", "p.Bag", "class", {field("p.Bag", "items", jm_cls("java.util.List", {jm_cls("java.lang.String")}))}),
            klass("Two", "p.Two", "interface", {method("p.Two", "f", {}, ir::void_type(), false), method("p.Two", "g", {}, ir::void_type(), false)}),
            klass("Mixed", "p.Mixed", "interface",
                  {method("p.Mixed", "f", {}, ir::void_type(), false), method("p.Mixed", "g", {}, ir::void_type(), true),
                   method("p.Mixed", "h", {}, ir::void_type(), true, {"static"})}),
            klass("Leaf", "p.Leaf", "class", {field("p.Leaf", "n", ir::i32_type())}, jm_cls("p.Circle")),
        };
        unit = ir::make(arena, std::move(u));
    }
};

}  // namespace

CB_TEST("translate.jmodel.classes_and_c_names", [] {
    JmProgram prog;
    jm::Model model(std::span<Node* const>(&prog.unit, 1));
    CB_CHECK(model.packages() == std::vector<std::string>{"p"});
    const jm::ClassInfo* inner = model.info("p.Outer.Inner");
    CB_CHECK(inner != nullptr);
    CB_CHECK_EQ(inner->cname, std::string("Outer_Inner"));
    CB_CHECK(inner->inner && inner->needs_outer);
    CB_CHECK(inner->outer == model.info("p.Outer"));
    const jm::ClassInfo* nested = model.info("p.Outer.Nested");
    CB_CHECK(!nested->inner && !nested->needs_outer);
    CB_CHECK_EQ(model.info("p.Color")->cname, std::string("Color"));
    CB_CHECK(model.info("p.Nothing") == nullptr);
    CB_CHECK(model.info("") == nullptr);
    CB_CHECK(model.of_decl(inner->node) == inner);
    CB_CHECK(model.of_type(jm_cls("p.Node")) == model.info("p.Node"));
    CB_CHECK(model.of_type(ir::i32_type()) == nullptr);
    // every class, in the order found: the top-level ones, each followed by what it holds
    std::vector<std::string> keys;
    for (const auto& entry : model.classes()) keys.push_back(entry.first);
    const std::vector<std::string> expected = {"p.Shape", "p.Circle", "p.Node", "p.Outer", "p.Outer.Inner", "p.Outer.Nested",
                                               "<Outer_anon1>", "<Outer_Loc2>", "<Outer_anon3>", "p.Color", "<Color_anon4>",
                                               "p.Bag", "p.Two", "p.Mixed", "p.Leaf"};
    CB_CHECK(keys == expected);
});

CB_TEST("translate.jmodel.supertypes", [] {
    JmProgram prog;
    jm::Model model(std::span<Node* const>(&prog.unit, 1));
    CB_CHECK(model.supers("p.Circle") == (std::vector<std::string>{"java.lang.Object", "p.Shape"}));
    CB_CHECK(model.supers("p.Shape").empty());   // an interface has no base
    CB_CHECK(model.supers("p.Color") == (std::vector<std::string>{"java.lang.Enum"}));
    CB_CHECK(model.supers("java.lang.RuntimeException") == (std::vector<std::string>{"java.lang.Exception", "java.lang.Object"}));
    CB_CHECK(model.supers("java.lang.Object").empty());
    CB_CHECK(model.ancestors("p.Leaf") ==
             (std::vector<std::string>{"p.Leaf", "p.Circle", "java.lang.Object", "p.Shape"}));
    CB_CHECK(model.is_sub("p.Leaf", "p.Shape"));
    CB_CHECK(model.is_sub("p.Leaf", "java.lang.Object"));
    CB_CHECK(!model.is_sub("p.Shape", "p.Leaf"));
    CB_CHECK(model.is_sub("java.lang.ArrayIndexOutOfBoundsException", "java.lang.RuntimeException"));
    CB_CHECK(model.is_interface("p.Shape") && !model.is_interface("p.Circle"));
    CB_CHECK(model.is_interface("java.lang.Runnable") && model.is_interface("java.util.List"));
    CB_CHECK(!model.is_interface("java.lang.String"));
    CB_CHECK(model.subclassed("p.Circle") && !model.subclassed("p.Leaf"));
});

CB_TEST("translate.jmodel.overrides", [] {
    JmProgram prog;
    jm::Model model(std::span<Node* const>(&prog.unit, 1));
    // a program interface's method: the root's signature
    const auto area = model.overrides(*model.info("p.Circle"), prog.circle_area);
    CB_CHECK(area.has_value() && area->params.empty());
    CB_CHECK(area->result == ir::prim_type(ir::Prim::F64));
    // a library root: Comparable.compareTo(T) -- the type variable is what the root declares
    const auto cmp = model.overrides(*model.info("p.Node"), prog.compare_to);
    CB_CHECK(cmp.has_value() && cmp->params.size() == 1 && cmp->params[0] == ir::typevar_type("T"));
    CB_CHECK(cmp->result == ir::i32_type());
    // Object.toString
    const auto str = model.overrides(*model.info("p.Node"), prog.to_string);
    CB_CHECK(str.has_value() && str->result == jm_cls("java.lang.String"));
    // private, static, constructors, and what overrides nothing
    CB_CHECK(!model.overrides(*model.info("p.Node"), prog.hidden));
    CB_CHECK(!model.overrides(*model.info("p.Outer"), prog.outer_run));
    const auto* sm = model.info("p.Outer")->methods("sm").front();
    CB_CHECK(!model.overrides(*model.info("p.Outer"), sm));
    CB_CHECK(!model.overrides(*model.info("p.Shape"), nullptr));
});

CB_TEST("translate.jmodel.method_decl", [] {
    JmProgram prog;
    jm::Model model(std::span<Node* const>(&prog.unit, 1));
    // a call of Node.compareTo(Node), by its resolved symbol; erased parameter types decide
    const auto found = model.method_decl(*prog.compare_sym);
    CB_CHECK(found.has_value() && found->cls == model.info("p.Node") && found->method == prog.compare_to);
    // a call through a subclass finds the declaration above it
    ir::Sym via_leaf = *prog.compare_sym;
    via_leaf.owner = "p.Leaf";
    CB_CHECK(!model.method_decl(via_leaf));   // Leaf extends Circle, which has no compareTo
    ir::Sym area = *ir::get<ir::MethodDecl>(prog.circle_area)->sym;
    area.owner = "p.Leaf";
    const auto up = model.method_decl(area);
    CB_CHECK(up.has_value() && up->cls == model.info("p.Circle"));
    // wrong arity or type: none; no owner: none
    ir::Sym wrong = *prog.compare_sym;
    wrong.params = {ir::i32_type()};
    CB_CHECK(!model.method_decl(wrong));
    wrong.owner.reset();
    CB_CHECK(!model.method_decl(wrong));
});

CB_TEST("translate.jmodel.local_and_anonymous_classes", [] {
    JmProgram prog;
    jm::Model model(std::span<Node* const>(&prog.unit, 1));
    // the classes are numbered in the order they are met: a method's code is searched from its last statement back
    const jm::ClassInfo* loc = model.info("<Outer_Loc2>");
    CB_CHECK(loc != nullptr && loc->local && !loc->anon);
    CB_CHECK_EQ(loc->cname, std::string("Outer_Loc2"));
    CB_CHECK(loc->method == prog.outer_run && loc->needs_outer);   // an instance method has `this`
    // what it reads from the method, and only that: x, typed; not its own local y
    CB_CHECK(loc->captures.size() == 1 && loc->captures[0].name == "x" && loc->captures[0].type == ir::i32_type());

    const jm::ClassInfo* anon = model.info("<Outer_anon1>");
    CB_CHECK(anon != nullptr && anon->anon && anon->creation != nullptr);
    CB_CHECK_EQ(anon->cname, std::string("Outer_anon1"));
    CB_CHECK(anon->base == std::string("java.lang.Object") && anon->ifaces == std::vector<std::string>{"java.lang.Runnable"});
    CB_CHECK(anon->needs_outer && anon->captures.size() == 1 && anon->captures[0].name == "x");

    // in a static method there is no outer instance
    const jm::ClassInfo* in_static = model.info("<Outer_anon3>");
    CB_CHECK(in_static != nullptr && !in_static->needs_outer);
    CB_CHECK(in_static->captures.empty());

    // an enum constant's body is a subclass of the enum, with no outer instance
    const jm::ClassInfo* body = model.info("<Color_anon4>");
    CB_CHECK(body != nullptr && body->enum_const != nullptr && !body->needs_outer);
    CB_CHECK(body->base == std::string("p.Color"));
});

CB_TEST("translate.jmodel.functional_interfaces", [] {
    JmProgram prog;
    jm::Model model(std::span<Node* const>(&prog.unit, 1));
    const auto shape = model.functional("p.Shape");
    CB_CHECK(shape.has_value() && shape->name == "area" && shape->params.empty());
    CB_CHECK(!model.functional("p.Two"));                // two abstract methods
    const auto mixed = model.functional("p.Mixed");      // a default and a static method do not count
    CB_CHECK(mixed.has_value() && mixed->name == "f");
    CB_CHECK(!model.functional("p.Circle"));             // not an interface
    CB_CHECK(!model.functional("p.Missing"));
    const auto runnable = model.functional("java.lang.Runnable");
    CB_CHECK(runnable.has_value() && runnable->name == "run" && runnable->result == ir::void_type());
    const auto binary = model.functional("java.util.function.IntBinaryOperator");
    CB_CHECK(binary.has_value() && binary->params.size() == 2 && binary->result == ir::i32_type());
});

CB_TEST("translate.jmodel.cycles", [] {
    JmProgram prog;
    jm::Model model(std::span<Node* const>(&prog.unit, 1));
    // Node.next leads back to Node; Outer.child is an Inner that holds its Outer; Bag holds a List, which may hold anything
    // (so does a local or anonymous class that needs its outer instance, which the outer may hold)
    const auto cycles = model.cycles();
    const std::vector<std::string> expected = {"Bag", "Node", "Outer", "Outer_Inner"};
    CB_CHECK(cycles == expected);
});

CB_TEST("translate.jmodel.name_anonymous", [] {
    JmProgram prog;
    // javac owns an anonymous class's members by '': a Call-like symbol inside the body, and a `this` typed with no name
    jm::Model model(std::span<Node* const>(&prog.unit, 1));
    const jm::ClassInfo* anon = model.info("<Outer_anon1>");
    ir::Node* run = anon->methods("run").front();
    auto sym = ir::get<ir::MethodDecl>(run)->sym;
    sym->owner = "";
    ir::Node* self = ir::make(prog.arena, ir::This{}, ir::class_type(""));
    ir::get<ir::MethodDecl>(run)->body = ir::make(prog.arena, ir::Block{.stmts = {ir::make(prog.arena, ir::ExprStmt{.expr = self})}});
    model.name_anonymous();
    CB_CHECK(sym->owner == std::string("<Outer_anon1>"));
    CB_CHECK(self->type == ir::class_type("<Outer_anon1>"));
    // a symbol that has a real owner is left alone
    CB_CHECK(ir::get<ir::MethodDecl>(prog.compare_to)->sym->owner == std::string("p.Node"));
});

CB_TEST("translate.jmodel.erase", [] {
    CB_CHECK(jm::erase({}) == ir::void_type());
    CB_CHECK(jm::erase(ir::typevar_type("T")) == jm_cls("java.lang.Object"));
    CB_CHECK(jm::erase(ir::any_type()) == jm_cls("java.lang.Object"));
    CB_CHECK(jm::erase(ir::array_of(ir::typevar_type("E"))) == ir::array_of(jm_cls("java.lang.Object")));
    CB_CHECK(jm::erase(jm_cls("java.util.List", {jm_cls("java.lang.String")})) == jm_cls("java.util.List"));
    CB_CHECK(jm::erase(ir::i32_type()) == ir::i32_type());
});

CB_TEST("translate.jmodel.library_tables", [] {
    CB_CHECK(jm::library_supers("java.util.LinkedList") == (std::vector<std::string>{"java.util.List", "java.util.Deque"}));
    CB_CHECK(jm::library_supers("p.Unknown").empty());
    const auto& throwables = jm::library_throwables();
    CB_CHECK(throwables.size() == 15 && throwables.front() == "java.lang.Exception");
    CB_CHECK(std::find(throwables.begin(), throwables.end(), "java.lang.Throwable") == throwables.end());
    CB_CHECK(jm::library_functional("java.util.Comparator")->name == "compare");
    CB_CHECK(jm::library_functional("java.util.List") == nullptr);
    // the one-method interfaces' method comes after the ones the table lists by hand
    CB_CHECK(jm::library_methods("java.lang.Comparable").size() == 1);
    CB_CHECK(jm::library_methods("java.lang.Object").size() == 3);
    CB_CHECK(jm::library_methods("java.lang.Runnable").size() == 1);
    CB_CHECK(jm::library_methods("p.Unknown").empty());
    // the runtime's names: listed ones, the functional interfaces' and the exceptions' simple names
    CB_CHECK(jm::runtime_name("java.util.Map.Entry") == std::string_view("MapEntry"));
    CB_CHECK(jm::runtime_name("java.util.Deque") == std::string_view("LinkedList"));
    CB_CHECK(jm::runtime_name("java.util.function.BiFunction") == std::string_view("BiFunction"));
    CB_CHECK(jm::runtime_name("java.io.IOException") == std::string_view("IOException"));
    CB_CHECK(jm::runtime_name("java.lang.Throwable") == std::string_view("Throwable"));
    CB_CHECK(!jm::runtime_name("p.Unknown"));
});

CB_TEST("translate.jmodel.literals", [] {
    CB_CHECK_EQ(jm::char_literal('a'), std::string("u'a'"));
    CB_CHECK_EQ(jm::char_literal(' '), std::string("u' '"));
    CB_CHECK_EQ(jm::char_literal('\\'), std::string("(char16_t)92"));
    CB_CHECK_EQ(jm::char_literal('\''), std::string("(char16_t)39"));
    CB_CHECK_EQ(jm::char_literal('\n'), std::string("(char16_t)10"));
    CB_CHECK_EQ(jm::char_literal(0x263a), std::string("(char16_t)9786"));
    CB_CHECK_EQ(jm::char_literal(127), std::string("(char16_t)127"));

    JmProgram prog;
    auto lit = [&](std::int64_t v) { return ir::make(prog.arena, ir::Lit{.value = v}, ir::i32_type()); };
    auto field = [&](Node* init, TypeP type) {
        ir::FieldDecl f;
        f.type = std::move(type);
        f.init = init;
        return f;
    };
    CB_CHECK(jm::constant_init(field(lit(5), ir::i32_type())));
    CB_CHECK(jm::constant_init(field(ir::make(prog.arena, ir::Paren{.x = lit(5)}), ir::i32_type())));
    CB_CHECK(jm::constant_init(field(ir::make(prog.arena, ir::Unary{.op = "-", .x = lit(5)}, ir::i32_type()), ir::i32_type())));
    // a string literal, a non-literal, no initialiser, and a reference type are not constants
    CB_CHECK(!jm::constant_init(field(ir::make(prog.arena, ir::Lit{.value = std::string("s")}), ir::i32_type())));
    CB_CHECK(!jm::constant_init(field(ir::make(prog.arena, ir::Unary{.op = "~", .x = lit(5)}, ir::i32_type()), ir::i32_type())));
    CB_CHECK(!jm::constant_init(field(ir::make(prog.arena, ir::Lit{}), ir::i32_type())));
    CB_CHECK(!jm::constant_init(field(nullptr, ir::i32_type())));
    CB_CHECK(!jm::constant_init(field(lit(5), jm_cls("java.lang.Integer"))));
});
