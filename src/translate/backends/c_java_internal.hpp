// translate/backends/c_java_internal.hpp -- what the modules of the C back end for Java share and nobody else needs: the
// program (`Program`: names, types, the tables of a class) and the writer of one function (`MethodWriter`).
#pragma once

#include <array>
#include <cstdint>
#include <functional>
#include <memory>
#include <optional>
#include <span>
#include <string>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "core/arena.hpp"
#include "core/ordered_map.hpp"
#include "translate/backends/c_java.hpp"
#include "translate/backends/cpp03.hpp"
#include "translate/backends/jmodel.hpp"
#include "translate/ir.hpp"

namespace cb::translate::backends::c_java::detail {

namespace jmodel = cb::translate::backends::jmodel;
using jmodel::ClassInfo;
using cpp03::Code;     // an expression's (C text, C type)
using cpp03::Lines;

// The C types the runtime names.
inline constexpr std::string_view kObj = "jobject*";
inline constexpr std::string_view kStr = "jstring*";
inline constexpr std::string_view kArr = "jarray*";
inline constexpr std::string_view kSb = "jrt_StringBuilder*";

// ---- reading the tree (c_java_names.cpp)
// The type of an expression or of a Local or Param; empty for a node that has none.
ir::TypeP type_of(const ir::Node* node);
// The kind of a type as L2 spells it ("i32", "class", "array" ...); "" for no type.
std::string_view kind_of(const ir::TypeP& type);
// The qualified name of a class type; "" for another kind of type.
std::string_view class_name(const ir::TypeP& type);
// An array type's element; empty for another kind.
ir::TypeP elem_of(const ir::TypeP& type);
// The declaration a node of that kind is; a Decline for a node that is not (the front end made a shape this back end does not know).
ir::MethodDecl& method_of(const ir::Node* node);
ir::FieldDecl& field_of(const ir::Node* node);
const ir::Param& param_of(const ir::Node* node);
ir::Sym& need_sym(const ir::SymP& sym, std::string_view what);
// The one statement or expression a slot holds; null when empty. A bare list of statements is not something the Java front end
// makes: a Decline.
ir::Node* single(const ir::Suite& suite);
// The statements of a method body: a Block's. A Decline for a body that is not one.
const std::vector<ir::Node*>& body_statements(const ir::MethodDecl& method);
bool has_body(const ir::MethodDecl& method);
bool is_static(const ir::MethodDecl& method);
bool is_private(const ir::MethodDecl& method);
// toString(), equals(o) and hashCode(): Object's three, which live in the object table, not in a class's own.
bool is_object_method(const ir::MethodDecl& method);

template <class F> void for_each_node(const ir::Node* root, F&& f) {   // pre-order, source order; null: nothing
    if (root == nullptr) return;
    f(root);
    for (const ir::Node* child : ir::children(*root)) for_each_node(child, f);
}
template <class P> bool any_node(const ir::Node* root, P&& pred) {
    if (root == nullptr) return false;
    if (pred(root)) return true;
    for (const ir::Node* child : ir::children(*root)) {
        if (any_node(child, pred)) return true;
    }
    return false;
}

// ---- names, types and literals (c_java_names.cpp)
std::string ident(std::string_view name);                    // a Java name as a C one
std::string_view after_dot(std::string_view qname);          // `java.util.List` -> `List`
std::string join_commas(const std::vector<std::string>& parts);
std::string decl_params(const std::vector<std::string>& params);   // the parameter list, `void` when empty
// A Java string as a char16_t literal and its length in UTF-16 units.
struct Utf16Literal {
    std::string text;
    std::size_t length = 0;
};
Utf16Literal utf16_literal(std::string_view utf8);
// The first code point of UTF-8 text; nullopt for empty text or one that is not valid.
std::optional<std::uint32_t> first_code_point(std::string_view utf8);

std::string_view prim_ctype(std::string_view kind);          // "i32" -> "int32_t"; "" for a kind that is no primitive
std::string_view kind_letter(std::string_view kind);         // "i32" -> "I"; "" for none
std::string_view box_letter(std::string_view qname);         // "java.lang.Integer" -> "I"; "" for none
std::string_view letter_ctype(std::string_view letter);      // "I" -> "int32_t"
std::string_view ctype_letter(std::string_view ctype);       // "int32_t" -> "I"
bool is_numeric(std::string_view ctype);
bool is_integral(std::string_view ctype);
bool is_prim_c(std::string_view ctype);                      // numeric or bool
bool is_pointer_ctype(std::string_view ctype);
// A Java type's letter(s) in a runtime function's name: I J ... Str, O, and arrays as the element's + A.
std::string type_code(const ir::TypeP& type);
// The library static fields the runtime has names for: `Integer.MAX_VALUE`.
const Code* library_static(std::string_view owner, std::string_view name);
// The superclasses the runtime's Throwable stands for.
bool is_library_throwable(std::string_view qname);
// Elements stored in a jarray: the C type, or jobject* for a class pointer.
std::string array_store(std::string_view element_ctype);

// ---- one slot of a class's table, one method of an interface, what a lambda needs
struct Slot {
    std::string name;
    std::vector<std::string> ps;   // C parameters after self
    std::string r;                 // C result
    std::string impl;              // the implementing function, or 0
};
struct IMethod {
    std::string name;              // the slot name
    std::vector<std::string> ps;
    std::string r;
    std::string java;              // the Java name
    std::vector<ir::TypeP> eps;    // erased Java parameters
};
struct Sig {
    std::vector<std::string> ps;
    std::string r;
    std::vector<ir::TypeP> eps;
};
struct SamInfo {                    // the functional interface a lambda implements
    std::string q;                  // qualified name
    std::string slot;
    std::vector<std::string> ps;
    std::string r;
    std::vector<IMethod> ms;
};
struct Cap {                        // what a lambda reads from outside
    std::string name;
    std::string code;
    std::string ct;
};

// ---- the program (c_java_program.cpp)
class Program {
public:
    Program(Arena& arena, std::span<ir::Node* const> units, std::optional<std::string> main);
    Program(const Program&) = delete;
    Program& operator=(const Program&) = delete;

    // The program's C text.
    std::string write();

    jmodel::Model& model() { return *model_; }
    // The arena of the nodes this writer makes itself (the assignments of initialisers, the lambdas of method references); the
    // caller's units are never given one of them.
    Arena& scratch() { return scratch_; }

    // ---- names and types
    std::string cname(std::string_view qname) const;
    // A method's name in C: its own, or with its parameter codes when the program overloads it.
    std::string mname(std::string_view name, std::span<const ir::TypeP> params) const;
    std::string ct(const ir::TypeP& type) const;
    // &X_class of a class or interface, program or runtime.
    std::string class_ref(std::string_view qname) const;
    Sig sig(const ClassInfo& cls, const ir::Node* method) const;
    std::optional<std::string> base_struct(const ClassInfo& cls) const;
    std::string base_class_ref(const ClassInfo& cls) const;
    bool is_throwable(const ClassInfo& cls) const;
    // A string literal's expression: the slot of the text, made on first use.
    std::string lit(std::string_view text);
    bool needs_clinit(const ClassInfo& cls) const;
    // The class whose C type is `ctype` (`X*`), or null.
    ClassInfo* class_of_ctype(std::string_view ctype) const;

    // ---- the table of virtual slots
    // The slots of class `cls`, base slots first.
    const std::vector<Slot>& slots(const ClassInfo& cls);
    // The implementations of toString, equals, hashCode for class `cls`: {toString, equals, hashCode}.
    std::array<std::string, 3> object_slots(const ClassInfo& cls) const;
    // The methods of an interface, its super-interfaces' first.
    std::vector<IMethod> iface_methods(std::string_view qname) const;
    std::vector<std::string> all_ifaces(const ClassInfo& cls) const;
    // The function implementing an interface method in class `cls` (its own or a base's, or an interface's default), if any.
    std::optional<std::string> find_impl(const ClassInfo& cls, std::string_view java_name, std::size_t parameters) const;
    Lines lambda_tables(const std::string& name, const ClassInfo& iface, std::string_view java_name, std::size_t parameters) const;

    // ---- constructors
    std::string ctor_name(const ClassInfo& cls, const ir::Node* ctor) const;
    std::vector<std::string> ctor_params(const ClassInfo& cls, const ir::MethodDecl& ctor) const;
    std::vector<std::string> new_params(const ClassInfo& cls, const ir::MethodDecl* ctor) const;
    bool has_ctor(const ClassInfo& cls) const;

    // ---- what the text is made of (written as the classes are)
    std::vector<std::string> lambdas;   // closure records: lines of C
    std::size_t lambda_count = 0;
    std::vector<std::string> vfuncs;    // the virtual-call helpers, after the tables

private:
    std::vector<ClassInfo*> ordered() const;
    Lines struct_of(const ClassInfo& cls);
    Lines prototypes(const ClassInfo& cls);
    Lines definitions(const ClassInfo& cls);
    Lines tables(const ClassInfo& cls);

    Arena scratch_;
    std::unique_ptr<jmodel::Model> model_;
    std::optional<std::string> main_;
    OrderedMap<std::string, std::string> lits_;                       // literal text -> slot name
    std::unordered_set<std::string> overloaded_;
    std::unordered_set<std::string> static_names_;   // static fields and enum constants: a method of the same name is written apart
    std::unordered_map<const ClassInfo*, std::vector<Slot>> slots_;   // per class, made on first use
    std::unordered_map<std::string, ClassInfo*> by_cname_;            // `X` -> the class (first of that name)
};

// ---- one function's C text (c_java_method.cpp, _stmt, _expr, _call, _closure)
class MethodWriter {
public:
    MethodWriter(Program& program, const ClassInfo* cls, const ir::Node* method = nullptr);

    // ---- members (c_java_method.cpp)
    Lines define();
    Lines ctor_call(const ir::CtorCall& call, std::string_view ind);
    // X_new for a class with no constructor of its own (an anonymous class): the super's constructor with the arguments, then
    // the field initialisers.
    Lines anon_new();
    Lines finit();
    Lines clinit();
    std::string constant(const ir::FieldDecl& field);

    // ---- statements (c_java_stmt.cpp)
    void stmts(std::span<ir::Node* const> list, Lines& out, std::string_view ind);
    void block(const ir::Node* s, Lines& out, std::string_view ind);
    void loop_body(const ir::Node* body, Lines& out, std::string_view ind);
    void stmt(const ir::Node* s, Lines& out, std::string_view ind);
    std::string cond(const ir::Node* e);
    std::string decl(std::string_view ct, std::string_view name, const std::optional<std::string>& init = std::nullopt) const;
    // jrt_pop for each handler opened since `depth` (innermost first).
    std::string pops(std::size_t depth) const;

    // ---- expressions -> (code, C type) (c_java_expr.cpp)
    Code expr(const ir::Node* e);
    std::string conv(const std::string& code, std::string_view src, std::string_view dst) const;
    // A value put in a place of Java type `type`: a constant given to a Short, Byte or Character is boxed as that, not as an int.
    std::string conv_to(const Code& value, const ir::TypeP& type, std::string_view dst) const;

    // ---- closures (c_java_closure.cpp)
    Code closure(const ir::Node* e);

private:
    // statements
    void emit(const ir::Node& n, const ir::Block& k, Lines& out, std::string_view ind);
    void emit(const ir::Node& n, const ir::Local& k, Lines& out, std::string_view ind);
    void emit(const ir::Node& n, const ir::ExprStmt& k, Lines& out, std::string_view ind);
    void emit(const ir::Node& n, const ir::If& k, Lines& out, std::string_view ind);
    void emit(const ir::Node& n, const ir::While& k, Lines& out, std::string_view ind);
    void emit(const ir::Node& n, const ir::DoWhile& k, Lines& out, std::string_view ind);
    void emit(const ir::Node& n, const ir::For& k, Lines& out, std::string_view ind);
    void emit(const ir::Node& n, const ir::Labeled& k, Lines& out, std::string_view ind);
    void emit(const ir::Node& n, const ir::Return& k, Lines& out, std::string_view ind);
    void emit(const ir::Node& n, const ir::Break& k, Lines& out, std::string_view ind);
    void emit(const ir::Node& n, const ir::Continue& k, Lines& out, std::string_view ind);
    void emit(const ir::Node& n, const ir::Throw& k, Lines& out, std::string_view ind);
    void emit(const ir::Node& n, const ir::Switch& k, Lines& out, std::string_view ind);
    void emit(const ir::Node& n, const ir::Try& k, Lines& out, std::string_view ind);
    void emit(const ir::Node& n, const ir::Empty& k, Lines& out, std::string_view ind);
    void emit(const ir::Node& n, const ir::Sync& k, Lines& out, std::string_view ind);
    void emit(const ir::Node& n, const ir::Assert& k, Lines& out, std::string_view ind);
    void emit(const ir::Node& n, const ir::LocalClass& k, Lines& out, std::string_view ind);
    void emit(const ir::Node& n, const ir::CtorCall& k, Lines& out, std::string_view ind);
    template <class K> void emit(const ir::Node& n, const K&, Lines&, std::string_view);   // any other kind: a Decline
    void jump(bool is_break, const std::optional<std::string>& label, Lines& out, std::string_view ind);
    std::string case_label(const ir::Node* label, const ClassInfo* enum_class);
    void try_catch(const ir::Try& s, Lines& out, std::string_view ind);
    // A try's body inside its resources, from the `at`-th on.
    void try_body(const ir::Try& s, std::size_t at, Lines& out, std::string_view ind);
    std::string close_call(const std::string& resource, const ir::TypeP& type);
    bool must_finish(std::size_t depth) const;   // a handler since `depth` has a finally or a resource
    // What a jump out of the handlers since `depth` does, innermost first: each is popped, then its finally or close runs.
    void unwind(std::size_t depth, Lines& out, std::string_view ind);

    // expressions
    std::string sequenced(const ir::Node* e);
    bool concat_in_turn(const ir::Concat& k) const;
    bool is_sub(std::string_view src, std::string_view dst) const;
    Code this_ref();
    Code outer_of(std::string_view qname);
    Code receiver(const ir::Node* obj, const ir::SymP& sym);
    Code unboxed(const std::string& code, std::string_view ct, const ir::TypeP& type) const;
    std::string as_string(const ir::Node* x);
    Code on(const ir::Node& n, const ir::Paren& k);
    Code on(const ir::Node& n, const ir::Lit& k);
    Code on(const ir::Node& n, const ir::Name& k);
    Code on(const ir::Node& n, const ir::This& k);
    Code on(const ir::Node& n, const ir::Select& k);
    Code on(const ir::Node& n, const ir::Length& k);
    Code on(const ir::Node& n, const ir::Index& k);
    Code on(const ir::Node& n, const ir::Unary& k);
    Code on(const ir::Node& n, const ir::Binary& k);
    Code on(const ir::Node& n, const ir::Concat& k);
    Code on(const ir::Node& n, const ir::Assign& k);
    Code on(const ir::Node& n, const ir::Cond& k);
    Code on(const ir::Node& n, const ir::Cast& k);
    Code on(const ir::Node& n, const ir::InstanceOf& k);
    Code on(const ir::Node& n, const ir::Call& k);
    Code on(const ir::Node& n, const ir::New& k);
    Code on(const ir::Node& n, const ir::NewArray& k);
    Code on(const ir::Node& n, const ir::Lambda& k);
    Code on(const ir::Node& n, const ir::MethodRef& k);
    Code on(const ir::Node& n, const ir::SwitchExpr& k);
    Code on(const ir::Node& n, const ir::ClassLit& k);
    Code on(const ir::Node& n, const ir::TypeName& k);
    std::string args(std::span<ir::Node* const> args, const std::vector<std::string>& params, const ir::SymP& sym, bool lib = false);
    Code lib_call(const ir::Node& n, const ir::Call& k);
    // The topmost class whose table first has the slot (its _v helper is the one written).
    const ClassInfo* slot_owner(const ClassInfo* cls, std::string_view name);
    Code new_class(const ClassInfo* cls, const ir::Node& n, const ir::New& k);
    Code lib_new(const ir::Node& n, const ir::New& k);

    // closures
    SamInfo sam(const ir::TypeP& type);
    std::vector<ir::Node*> ref_args(std::span<ir::Node* const> names, std::span<const ir::TypeP> param_types);
    Arena& scratch() { return p_.scratch(); }

    Program& p_;
    const ClassInfo* ci_;
    const ir::Node* node_;                    // the MethodDecl, or null (an initialiser, a constructor made up)
    const ir::MethodDecl* m_;
    std::unordered_map<std::string, std::string> vars_;   // the locals in scope: name -> C type
    std::string ret_ = "void";
    Lines pending_;                           // temporaries to declare before the statement
    std::size_t temps_ = 0;
    bool order_ = false;                      // sequence the operands whose order Java fixes
    std::unordered_map<const ir::Node*, Code> subst_;   // expressions already evaluated into a temporary
    std::optional<std::string> cont_next_;
    struct Handler {
        std::string name;
        std::size_t loops = 0;
        const ir::Node* finally_body = nullptr;   // what a jump out of it runs: a finally,
        std::string close;                        // or the closing of a resource
        std::size_t real_loops = 0;               // the loops around it, without the switches (what a continue counts)
    };
    std::vector<Handler> tries_;              // the handlers open around the statement
    std::unordered_map<std::string, std::size_t> label_depth_;   // how many handlers were open at each label
    std::size_t loops_ = 0;
    std::size_t real_loops_ = 0;
    bool vol_ = false;                        // locals volatile (the function has a try)
    std::unordered_map<std::string, std::string> captures_;
    bool static_ = false;
    std::optional<std::unordered_map<std::string, Code>> lam_;   // in a closure: what it captured, and '#this'
    std::unordered_map<const ir::Node*, ir::TypeP> array_types_;   // an initialiser's nested array that has no type of its own
};

}  // namespace cb::translate::backends::c_java::detail
