// translate/backends/go_java_internal.hpp -- what the modules of the Go back end for Java share and nobody else needs: the program (`Program`: names,
// types and the tables of a class) and the writer of one function (`Writer`).
#pragma once

#include <cstdint>
#include <functional>
#include <map>
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
#include "translate/backends/go_java.hpp"
#include "translate/backends/jmodel.hpp"
#include "translate/ir.hpp"

namespace cb::translate::backends::go_java::detail {

namespace jmodel = cb::translate::backends::jmodel;
using jmodel::ClassInfo;
using Lines = std::vector<std::string>;

// How tightly Go's operators bind (a Code's `prec`): an operand looser than its operator is parenthesised.
inline constexpr int kLow = 0, kOr = 1, kAnd = 2, kCmp = 3, kAdd = 4, kMul = 5, kUnary = 6, kAtom = 7;

// An expression's Go text and its Java type. `bare` marks a number written as an untyped Go constant (it takes the type its place wants).
struct Code {
    std::string text;
    ir::TypeP type;
    int prec = kAtom;
    bool bare = false;
};

// ---- reading the tree and naming (go_java_program.cpp)
ir::TypeP type_of(const ir::Node* node);
std::string_view kind_of(const ir::TypeP& type);          // "i32", "class", "array" ...; "" for none
std::string_view class_name(const ir::TypeP& type);       // the qualified name of a class type; "" for another kind
ir::TypeP elem_of(const ir::TypeP& type);
const ir::Node* strip_paren(const ir::Node* node);
ir::MethodDecl& method_of(const ir::Node* node);
const ir::Param& param_of(const ir::Node* node);
ir::Sym& need_sym(const ir::SymP& sym, std::string_view what);
std::vector<ir::Node*> body_of(const ir::MethodDecl& method);
bool is_static(const ir::MethodDecl& method);
bool is_private(const ir::MethodDecl& method);
bool has_mod(const std::vector<std::string>& mods, std::string_view mod);
bool is_prim_kind(std::string_view kind);
bool is_integral_kind(std::string_view kind);
bool is_float_kind(std::string_view kind);
bool is_numeric_kind(std::string_view kind);
std::string_view boxed_kind(const ir::TypeP& type);       // the primitive a java.lang.Integer ... is the box of; "" otherwise
ir::TypeP prim_by_kind(std::string_view kind);
ir::TypeP promoted(const ir::TypeP& a, const ir::TypeP& b);
ir::TypeP unary_promoted(const ir::TypeP& a);
bool is_pure(const ir::Node* node);
template <class F> void for_each_node(const ir::Node* root, F&& f) {   // pre-order, source order
    if (root == nullptr) return;
    f(root);
    for (const ir::Node* child : ir::children(*root)) for_each_node(child, f);
}
template <class P> bool any_node(const ir::Node* root, P&& pred) {
    if (root == nullptr) return false;
    if (pred(root)) return true;
    for (const ir::Node* child : ir::children(*root))
        if (any_node(child, pred)) return true;
    return false;
}

std::string ident(std::string_view name);                 // a local's name as a Go one (keywords and predeclared names get a trailing `_`)
std::string field_ident(std::string_view name);           // a field's name (only keywords get a trailing `_`)
std::string capital(std::string_view name);               // a method's name: first letter upper case
std::string go_string(std::string_view utf8);             // a Go string literal for Java text; a Decline for text Go cannot hold
std::string float_text(double value, bool single);
std::string join(const std::vector<std::string>& parts, std::string_view sep = ", ");
std::string indent_of(std::size_t depth);
std::string short_code(const ir::TypeP& type);            // a type as part of a method's name: int, str, Animal, int_arr
// a number or other literal with its type spelled out
std::string typed_text(const Code& c);

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

    // ---- types
    std::string gtype(const ir::TypeP& type) const;          // a Java type as Go writes it (Decline for one it has no word for)
    bool is_lib_throwable(std::string_view qname) const;
    bool is_throwable_type(std::string_view qname) const;
    const ClassInfo* program_class(const ir::TypeP& type) const;

    // ---- classes and their names
    bool static_only(const ClassInfo& cls) const;
    std::string iface(const ClassInfo& cls) const { return cls.cname; }
    std::string obj(const ClassInfo& cls) const { return cls.cname + "Obj"; }
    std::string accessor(const ClassInfo& cls) const { return "as" + cls.cname; }
    std::string marker(const ClassInfo& cls) const { return "is" + cls.cname; }
    std::string static_name(const ClassInfo& cls, std::string_view member) const { return cls.cname + "_" + std::string(member); }
    const ClassInfo* base_class(const ClassInfo& cls) const;
    std::vector<const ClassInfo*> ancestors(const ClassInfo& cls) const;
    bool is_concrete(const ClassInfo& cls) const;
    bool is_throwable(const ClassInfo& cls) const { return is_throwable_type(cls.qname); }
    const ClassInfo* declaring_class(const ClassInfo& cls, std::string_view field) const;

    // ---- methods
    std::string mname(const ir::MethodDecl& method) const;   // the Go name of a method (with its parameter codes when the program overloads it)
    bool is_object_method(const ir::MethodDecl& method) const;
    bool introduces(const ClassInfo& cls, const ir::MethodDecl& method) const;
    // the parameter and result types a method has in Go: the topmost declaration's, as javac's bridge methods give them
    jmodel::Override signature(const ClassInfo& cls, const ir::Node* method) const;
    std::string ctor_suffix(const ClassInfo& cls, const ir::Node* ctor) const;
    bool is_const_field(const ir::FieldDecl& field) const;
    const ClassInfo* main_class() const { return main_cls_; }
    void need_math() { math_ = true; }
    std::string param_list(const ClassInfo& cls, const ir::Node* method, bool names) const;
    std::string result_text(const jmodel::Override& sig) const;

private:
    void check_supported();
    void find_main();
    Lines class_text(const ClassInfo& cls);
    Lines interface_text(const ClassInfo& cls);
    Lines struct_text(const ClassInfo& cls);
    Lines method_text(const ClassInfo& cls, const ir::Node* method);
    Lines ctor_text(const ClassInfo& cls, const ir::Node* ctor);
    Lines new_text(const ClassInfo& cls, const ir::Node* ctor);
    Lines enum_text(const ClassInfo& cls);
    Lines default_method_text(const ClassInfo& cls);
    void static_init_text(const ClassInfo& cls, Lines& out);

    Arena& arena_;
    std::unique_ptr<jmodel::Model> model_;
    std::string main_name_;
    const ClassInfo* main_cls_ = nullptr;
    std::unordered_set<std::string> overloaded_;
    std::unordered_set<std::string> static_only_;
    bool math_ = false;
    std::vector<ir::Node*> units_;
};

// ---- one function's Go text (go_java_stmt.cpp, go_java_expr.cpp)
class Writer {
public:
    Writer(Program& program, const ClassInfo* cls, const ir::MethodDecl* method);

    // the statements of a method body (between the braces), `ret` the Java return type
    Lines body(std::span<ir::Node* const> stmts, const ir::TypeP& ret, std::size_t depth);
    std::string value_of(const ir::Node* e, const ir::TypeP& want);
    Lines ctor_call(const ir::CtorCall& call);
    void make_static() { static_ = true; }
    void find_reads(std::span<ir::Node* const> stmts);   // which locals the body reads (Go refuses ones it never does)

    void stmts(std::span<ir::Node* const> list, Lines& out, std::size_t depth);
    void stmt(const ir::Node* s, Lines& out, std::size_t depth);

    Code expr(const ir::Node* e);
    Code numeric(const ir::Node* e);
    Code conv(Code value, const ir::TypeP& dst);
    std::string wrap(const Code& c, int min_prec) const { return c.prec >= min_prec ? c.text : "(" + c.text + ")"; }
    std::vector<std::string> call_args(std::span<ir::Node* const> args, const std::vector<ir::TypeP>& params);

private:
    struct Place {
        std::string read, lvalue;
        ir::TypeP type;
    };
    struct Target {
        bool loop = false;                      // a loop (a switch is not)
        std::optional<std::string> java;        // the Java label
        bool used = false;
        std::size_t closures = 0;               // try closures open around it
    };
    struct TryFrame {
        std::string flow;
        bool handler = false;
        bool returns = false;
        std::vector<std::pair<bool, std::optional<std::string>>> jumps;
    };

    // statements
    void emit_local(const ir::Local& k, Lines& out, std::size_t depth);
    void emit_expr_stmt(const ir::Node* e, Lines& out, std::size_t depth);
    void emit_if(const ir::If& k, Lines& out, std::size_t depth);
    void emit_loop(const ir::Node* loop, std::optional<std::string> label, Lines& out, std::size_t depth);
    void emit_foreach(const ir::ForEach& k, std::optional<std::string> label, Lines& out, std::size_t depth);
    void emit_switch(const ir::Switch& k, Lines& out, std::size_t depth);
    void emit_try(const ir::Try& k, Lines& out, std::size_t depth);
    void emit_return(const ir::Return& k, Lines& out, std::size_t depth);
    void emit_return_text(const std::string& value, Lines& out, std::size_t depth);
    void emit_jump(bool is_break, const std::optional<std::string>& label, Lines& out, std::size_t depth);
    void emit_assign_stmt(const ir::Assign& a, Lines& out, std::size_t depth);
    void suite(const ir::Suite& s, Lines& out, std::size_t depth);
    void block_body(const ir::Node* n, Lines& out, std::size_t depth);
    void looped(Target t, const std::string& header, const std::function<void(Lines&, std::size_t)>& body, Lines& out, std::size_t depth);
    std::string cond_text(const ir::Node* e);
    std::string simple_stmt(const ir::Node* e);   // an expression statement as one Go simple statement (for init and update clauses)
    void declare(const ir::Sym& sym, const ir::TypeP& type, const std::string& init, bool has_init, Lines& out, std::size_t depth);

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
    template <class K> Code on(const ir::Node& n, const K&);

    Code arith(const std::string& op, Code l, Code r);
    Place place_of(const ir::Node* target);
    std::string assign_text(const ir::Node* target, const ir::Node* value, const std::optional<std::string>& op);
    Code assign_value(const ir::Node* target, const ir::Node* value, const std::optional<std::string>& op);
    std::string incdec_text(const ir::Unary& k);
    Code incdec_value(const ir::Unary& k);
    void check_order(const ir::Node* target);
    Code this_code();
    Code method_call(const ir::Node& n, const ir::Call& k);
    Code lib_call(const ir::Node& n, const ir::Call& k);
    Code lib_new(const ir::Node& n, const ir::New& k);
    Code lib_static_field(const ir::Select& sel, const ir::TypeP& type);
    Code from_any(Code c, const ir::TypeP& want);   // an erased value used as the type javac says it has
    std::string exact_type_name(const ir::TypeP& t);
    std::optional<std::int64_t> fold(const ir::Node* e);
    Code int_literal(std::int64_t v, const ir::TypeP& type);

    Program& p_;
    const ClassInfo* ci_;
    const ir::MethodDecl* m_;
    bool static_ = false;
    std::set<std::string> read_;            // locals and parameters the body reads
    ir::TypeP ret_;                            // the method's Java return type
    bool uses_retv_ = false;
    std::vector<Target> targets_;
    std::vector<TryFrame> tries_;
    std::size_t temps_ = 0;
    const ir::Node* cur_stmt_ = nullptr;
};

}  // namespace cb::translate::backends::go_java::detail
