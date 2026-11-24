// translate/backends/rust_java_internal.hpp -- what the modules of the Rust back end for Java share and nobody else needs: the program (`Program`:
// names, types and the tables of a class) and the writer of one function (`Writer`).
#pragma once

#include <cstdint>
#include <functional>
#include <memory>
#include <optional>
#include <set>
#include <span>
#include <string>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "core/arena.hpp"
#include "translate/backends/jmodel.hpp"
#include "translate/backends/rust_java.hpp"
#include "translate/ir.hpp"

namespace cb::translate::backends::rust_java::detail {

namespace jmodel = cb::translate::backends::jmodel;
using jmodel::ClassInfo;
using Lines = std::vector<std::string>;

// How tightly Rust's operators bind (a Code's `prec`): an operand looser than its operator is parenthesised.
inline constexpr int kLow = 0, kCastPrec = 1, kOr = 2, kAnd = 3, kCmp = 4, kBitOr = 5, kBitXor = 6, kBitAnd = 7, kShift = 8, kAdd = 9, kMul = 10, kUnary = 11,
                     kAtom = 12;

// An expression's Rust text and its Java type. `bare` marks an integer literal written without a suffix (it takes the type its place wants).
struct Code {
    std::string text;
    ir::TypeP type;
    int prec = kAtom;
    bool bare = false;
    std::string chr;   // a char literal's own text ('a'), so a conversion can write `'a' as i32`
};

// ---- reading the tree (rust_java_names.cpp)
ir::TypeP type_of(const ir::Node* node);
std::string_view kind_of(const ir::TypeP& type);          // "i32", "class", "array" ...; "" for none
std::string_view class_name(const ir::TypeP& type);       // the qualified name of a class type; "" for another kind
ir::TypeP elem_of(const ir::TypeP& type);
const ir::Node* strip_paren(const ir::Node* node);
ir::MethodDecl& method_of(const ir::Node* node);
ir::FieldDecl& field_of(const ir::Node* node);
const ir::Param& param_of(const ir::Node* node);
ir::Sym& need_sym(const ir::SymP& sym, std::string_view what);
std::vector<ir::Node*> body_of(const ir::MethodDecl& method);
bool is_static(const ir::MethodDecl& method);
bool is_private(const ir::MethodDecl& method);
bool has_mod(const std::vector<std::string>& mods, std::string_view mod);
bool is_prim_kind(std::string_view kind);                  // a Java primitive
bool is_integral_kind(std::string_view kind);              // i8 i16 u16 i32 i64
bool is_float_kind(std::string_view kind);                 // f32 f64
bool is_numeric_kind(std::string_view kind);
// The primitive a java.lang.Integer, Long, Double ... is the box of ("i32" ...); "" for another type.
std::string_view boxed_kind(const ir::TypeP& type);
// The Java primitive type of a kind ("i32", "bool" ...).
ir::TypeP prim_by_kind(std::string_view kind);
// The type binary numeric promotion gives two operand types (i32, i64, f32 or f64).
ir::TypeP promoted(const ir::TypeP& a, const ir::TypeP& b);
ir::TypeP unary_promoted(const ir::TypeP& a);
// Whether evaluating the expression twice is the same as once (names, literals, `this`, fields of those).
bool is_pure(const ir::Node* node);
template <class F> void for_each_node(const ir::Node* root, F&& f) {   // pre-order, source order
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

// ---- names, types and literals (rust_java_names.cpp)
std::string ident(std::string_view name);                      // a local's name as a Rust one (keywords, and what the text uses bare, get a trailing `_`)
std::string member_ident(std::string_view name);               // a method's or field's name (only keywords get a trailing `_`)
std::string rs_string(std::string_view utf8);                     // a Rust string literal ("...") for Java text; a Decline for text Rust cannot hold
std::string float_text(double value, bool single);             // 1.5, 1e-5, 2.0 (f32: with the f32 suffix)
std::string type_suffix(std::string_view kind);                // "i64" -> "i64", "u16" -> "u16"
std::string short_code(const ir::TypeP& type);                 // a type as part of a method's name: i32, str, Animal, i32_arr
std::string join(const std::vector<std::string>& parts, std::string_view sep = ", ");
std::string indent_of(std::size_t depth);

// The text of an expression a method is called on: a literal gets its type, anything looser than an atom its parentheses.
struct Code;
std::string receiver_text(const Code& c);

// ---- the program (rust_java_program.cpp)
struct MethodRef {
    const ClassInfo* cls = nullptr;
    const ir::Node* node = nullptr;   // a MethodDecl
};

class Program {
public:
    Program(Arena& arena, std::span<ir::Node* const> units, std::string main);
    Program(const Program&) = delete;
    Program& operator=(const Program&) = delete;

    std::string write();

    jmodel::Model& model() { return *model_; }
    Arena& scratch() { return scratch_; }

    // ---- types
    std::string rtype(const ir::TypeP& type) const;          // a Java type as Rust writes it (Decline for one it has no word for)
    bool is_throwable_type(std::string_view qname) const;    // Throwable or any subclass, library or program
    bool is_lib_throwable(std::string_view qname) const;
    const ClassInfo* program_class(const ir::TypeP& type) const;   // the program's class a type is, or null

    // ---- classes
    bool static_only(const ClassInfo& cls) const;
    std::string holder(const ClassInfo& cls) const;          // the struct holding a class's statics and method bodies: `Main`, `AnimalData`
    std::string trait_name(const ClassInfo& cls) const { return cls.cname; }
    std::string obj_name(const ClassInfo& cls) const { return cls.cname + "Obj"; }
    std::string fields_fn(const ClassInfo& cls) const { return cls.cname + "_fields"; }
    const ClassInfo* base_class(const ClassInfo& cls) const;                // its program superclass, or null
    std::vector<const ClassInfo*> ancestors(const ClassInfo& cls) const;   // program classes and interfaces above it, nearest first
    std::vector<const ClassInfo*> subclasses_of(const ClassInfo& cls) const;   // concrete classes that are or extend it
    bool is_concrete(const ClassInfo& cls) const;
    bool is_throwable(const ClassInfo& cls) const { return is_throwable_type(cls.qname); }

    // ---- methods
    std::string mname(const ir::MethodDecl& method) const;   // the Rust name of a method (with its parameter codes when the program overloads it)
    bool is_object_method(const ir::MethodDecl& method) const;   // toString(), equals(Object), hashCode()
    bool introduces(const ClassInfo& cls, const ir::MethodDecl& method) const;   // a trait method of its own (it overrides nothing above)
    std::optional<MethodRef> find_impl(const ClassInfo& cls, const std::string& rust_name) const;   // what a concrete class runs for a method
    std::string ctor_suffix(const ClassInfo& cls, const ir::Node* ctor) const;   // "" for the one constructor, "_1" ...
    std::string static_field_name(const ClassInfo& cls, std::string_view field) const { return holder(cls) + "_" + ident(field); }
    bool is_const_field(const ir::FieldDecl& field) const;
    const ClassInfo* main_class() const { return main_cls_; }
    // a cast or instanceof to this program class is written: its caster is made
    void need_caster(const ClassInfo& cls) { casters_.insert(cls.qname); }

private:
    void check_supported();
    void find_main();
    Lines class_text(const ClassInfo& cls);
    Lines static_only_text(const ClassInfo& cls);
    Lines data_struct(const ClassInfo& cls);
    Lines trait_text(const ClassInfo& cls);
    Lines holder_text(const ClassInfo& cls);
    Lines concrete_text(const ClassInfo& cls);
    Lines caster_text(const ClassInfo& cls);
    Lines statics_text(const ClassInfo& cls);
    Lines method_text(const ClassInfo& cls, const ir::Node* method);
    Lines ctor_text(const ClassInfo& cls, const ir::Node* ctor);
    std::string signature(const ClassInfo& cls, const ir::MethodDecl& m, std::string_view self, bool bodyless) const;
    std::string trait_signature(const ir::MethodDecl& m) const;

    Arena scratch_;
    std::unique_ptr<jmodel::Model> model_;
    std::string main_name_;
    const ClassInfo* main_cls_ = nullptr;
    std::unordered_set<std::string> overloaded_;               // method names the program declares with more than one parameter list
    std::unordered_set<std::string> static_only_;
    std::set<std::string> casters_;
    std::vector<ir::Node*> units_;
};

// ---- one function's Rust text (rust_java_stmt.cpp, _expr, _call)
class Writer {
public:
    Writer(Program& program, const ClassInfo* cls, const ir::MethodDecl* method);

    // the statements of a method body (the function's text between its braces), `ret` the Rust return type ("" for void)
    Lines body(std::span<ir::Node* const> stmts, const ir::TypeP& ret, std::size_t depth);
    // one expression as a value of Java type `want` (an initialiser, a static field's value)
    std::string value_of(const ir::Node* e, const ir::TypeP& want);
    // a constructor call this(...) or super(...)
    Lines ctor_call(const ir::CtorCall& call);
    bool is_assigned(const std::string& name) const { return assigned_.contains(name); }
    // the names the body assigns after declaring them (so `mut` is written for those only)
    void note_assigned(std::span<ir::Node* const> stmts);
    void make_static() { static_ = true; }

    // ---- statements (rust_java_stmt.cpp)
    void stmts(std::span<ir::Node* const> list, Lines& out, std::size_t depth);
    void stmt(const ir::Node* s, Lines& out, std::size_t depth);

    // ---- expressions (rust_java_expr.cpp)
    Code expr(const ir::Node* e);
    // an operand of arithmetic: the expression, a boxed number unboxed
    Code numeric(const ir::Node* e);
    Code conv(Code value, const ir::TypeP& dst);
    std::string wrap(const Code& c, int min_prec) const;       // the text, parenthesised when it binds looser than min_prec
    std::string as_arg(const ir::Node* e, const ir::TypeP& param);   // an argument given to a parameter of that type

    std::vector<std::string> call_args(std::span<ir::Node* const> args, const std::vector<ir::TypeP>& params, bool varargs);

private:
    struct Place;   // an assignable thing: a local, a field, an array element
    struct Target {
        enum class Kind { Loop, Switch, Block } kind = Kind::Loop;
        std::optional<std::string> java;        // the Java label
        std::string label;                      // the Rust label ('l1), used when `label_used`
        bool label_used = false;
        bool has_cont = false;                  // the body is wrapped in a block `'cN: { ... }` a continue leaves
        std::string cont;
        bool cont_used = false;
        bool labeled_block = false;             // Rust sees a labeled block here (an unlabeled break through it would be an error)
        std::size_t tries = 0;                  // how many try closures were open around it
    };

    // statements
    void emit_local(const ir::Node& n, const ir::Local& k, Lines& out, std::size_t depth);
    void emit_expr_stmt(const ir::Node* e, Lines& out, std::size_t depth);
    void emit_if(const ir::If& k, Lines& out, std::size_t depth);
    void emit_while(const ir::While& k, std::optional<std::string> label, Lines& out, std::size_t depth);
    void emit_do(const ir::DoWhile& k, std::optional<std::string> label, Lines& out, std::size_t depth);
    void emit_for(const ir::For& k, std::optional<std::string> label, Lines& out, std::size_t depth);
    bool emit_range_for(const ir::For& k, std::optional<std::string> label, Lines& out, std::size_t depth);
    void emit_switch(const ir::Switch& k, Lines& out, std::size_t depth);
    void emit_try(const ir::Try& k, Lines& out, std::size_t depth);
    void emit_return(const ir::Return& k, Lines& out, std::size_t depth);
    void emit_jump(bool is_break, const std::optional<std::string>& label, Lines& out, std::size_t depth);
    void suite(const ir::Suite& s, Lines& out, std::size_t depth);      // the statements of a body, one level in
    void looped(Target t, const ir::Suite& body, std::string_view header, bool wrap_cont, Lines& out, std::size_t depth,
                const std::function<void(Lines&, std::size_t)>& after_body = {});
    std::string cond_text(const ir::Node* e);
    std::string case_label(const ir::Node* label, const ir::TypeP& scrutinee);
    std::string enum_case_label(const ir::Node* label, const ClassInfo& cls);
    bool body_continues(const ir::Suite& body, const std::optional<std::string>& label) const;
    bool terminates(const ir::Node* s) const;

    // expressions
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
    template <class K> Code on(const ir::Node& n, const K&);   // any other kind: a Decline

    Code binary_arith(const std::string& op, Code l, Code r, const ir::TypeP& type);
    Code arith(const std::string& op, const Code& l, const Code& r, const ir::TypeP& type, const ir::Node* right_node);
    Place place_of(const ir::Node* target, std::vector<std::string>& pre);
    Code assign_code(const ir::Node* target, const ir::Node* value, const std::optional<std::string>& op, bool want_value, const ir::TypeP& type);
    Code incdec(const ir::Unary& k, bool want_value);
    Code this_code();
    Code receiver(const ir::Node* target);
    Code method_call(const ir::Node& n, const ir::Call& k);
    Code lib_call(const ir::Node& n, const ir::Call& k);
    Code lib_new(const ir::Node& n, const ir::New& k);
    Code lib_static_field(const ir::Select& sel, const ir::TypeP& type);
    Code class_conv(Code value, const ir::TypeP& dst);
    bool is_subtype(std::string_view a, std::string_view b) const;

    Program& p_;
    const ClassInfo* ci_;
    const ir::MethodDecl* m_;
    bool static_ = false;
    std::set<std::string> assigned_;           // locals and parameters assigned after their declaration
    ir::TypeP ret_;                            // the method's Java return type
    bool has_try_ = false;
    // a try body or a finally's closure: the jumps (break: true, continue: false, and the label) that leave it, made after it ends
    struct TryCtx {
        std::size_t base = 0;   // the targets outside it are those below this
        std::vector<std::pair<bool, std::optional<std::string>>> jumps;
    };
    std::vector<Target> targets_;
    std::vector<TryCtx> try_ctx_;
    std::size_t tries_ = 0;                    // try closures open now
    std::size_t labels_ = 0;
    std::size_t temps_ = 0;
    std::unordered_map<const ir::Node*, ir::TypeP> array_hint_;   // an initialiser's nested array that has no type of its own
};

}  // namespace cb::translate::backends::rust_java::detail
