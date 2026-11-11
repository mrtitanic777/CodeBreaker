// codebreaker/translate/frontends/python/lower.hpp -- typed C++ from
// codebreaker.translate.frontends.python.lower.
//
// lower.py -- a typed Python module into L2 (codebreaker/translate/ir.py), or a refusal with its reason.
//
// Inference (infer.py) has run the whole program; this walks each function again with what it found --
// every expression's type, every call's target -- and writes the L2 tree. Python's own operations that C++
// does not have as such are calls owned by "py" (Sym('function', name, owner='py')): their names are the
// API map's (codebreaker/java/api_maps/python.json) or the support header's core semantics (truthy, slice,
// floordiv, ...). A back end writes them; nothing here knows C++.
//
// A function is refused -- left in Python -- when anything in it has no single type, when an integer is not
// shown to stay inside int64 (int_policy 'strict'; 'checked' writes overflow-checked operations instead),
// when it uses a construct the front end does not lower, or when it calls a refused function.
//
// Python's model, kept: a list, dict, set, class instance or mutated record is a reference (aliasing is
// Python's); str, bytes, tuples, numbers and records never changed after they are built are values.
//
// Notes on the port:
//   * Refused is an exception carrying the infer::Issue the Python's `.issue` is;
//   * the Python's `(MethodDecl | Refused, FnLower | None)` pairs of lower_functions are a `Lowered`;
//   * the Python `ast` is native/pyfront's tree and a node's identity (id(node)) its address, as in infer;
//     a resolution `r` is `Res`, and r[0] its `kind`, r[i] its items (Tup::s(i) for a str);
//   * a statement lowering returns the list of IR statements the Python returns as a node or a list
//     (stmt() wraps the node in a list, so every s_* returns a list here);
//   * Python's `_ModFn` (a module's top level seen as a function, python/__init__.py) is the second
//     FnLower constructor: it takes a program::Module;
//   * Python ints in the IR are int64_t (a literal of the source beyond int64 is refused before it is
//     built, as in the Python; a bigint default or range step would not be);
//   * private names keep their meaning: _camel -> camel, _singular -> singular, _str_const / _int_const are
//     program::str_const / int_const; `T` the method (type of a node) is `Tn`.
#pragma once

#include <functional>
#include <map>
#include <memory>
#include <optional>
#include <set>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "codebreaker/translate/frontends/python/infer.hpp"
#include "codebreaker/translate/ir.hpp"

namespace codebreaker::translate::frontends::python::lower {

namespace ir = codebreaker::translate::ir;
namespace A = pyfront;
namespace P = pytypes;
using program::Class;
using program::Fn;
using program::Module;
using program::Program;
using program::Res;
using program::Str;
using program::T;
using ir::NodeP;
using ir::SymP;
using ir::TypeP;
using ir::Val;

extern const char* const PY;   // "py"

// I64 / BOOL / F64 / VOID / STR / BYTES
const TypeP& I64();
const TypeP& BOOL();
const TypeP& F64();
const TypeP& VOID();
const TypeP& STR();
const TypeP& BYTES();

// a function or type the program's own code could not give a single type or a lowering
struct Refused : py::Exception {
    infer::Issue issue;
    Refused(const Str& cls, const Str& detail = Str(), int64_t line = 0)
        : py::Exception((cls + Str(":") + detail).utf8()), issue(cls, detail, line) {}
    const char* type() const override { return "Refused"; }
};

// ir.Call(target, Sym('function', name, owner='py'), args, typeargs, type, line)
NodeP pyfn(const Str& name, const std::vector<NodeP>& args, const TypeP& t, const NodeP& target = nullptr,
           int64_t line = 0, const py::list<Val>& typeargs = py::list<Val>());

// ir.Lit(value, text, type)
NodeP lit(const Val& v, const TypeP& t, const std::optional<Str>& text = std::nullopt);

Str camel(const Str& s);
Str singular(const Str& w);

// What every container site of the program is in C++: a record struct (named), a dict, a list, a set.
class Records {
  public:
    explicit Records(Program& prog);

    struct Field {
        Str key;
        T type;
        bool required;
    };

    Program* p;
    std::map<int64_t, Str> names;                    // root -> struct name
    std::map<int64_t, std::optional<Str>> owner_fn;  // root -> the function whose site created it
    std::map<int64_t, Str> module;                   // root -> module qname it is declared in
    std::map<int64_t, int64_t> home;                 // root -> the site that makes it (where its struct is declared)

    // [(key, T, required)]: in the order the record's own site writes them, then the others
    std::vector<Field> fields(int64_t s);
    bool is_ref(int64_t s);

  private:
    std::map<Str, std::unordered_map<int64_t, Str>> fn_lines_;
    void fn_lines();
    Str name_(int64_t s, const std::optional<Str>& fq);
    bool holds_(const T& t, int64_t s, int64_t depth = 0);
};

// What lowering the program shares: its records, the types written so far, and the policy.
class Context {
  public:
    explicit Context(Program& prog, const Str& int_policy = Str("strict"));

    Program& p;
    Records records;
    Str int_policy;
    std::set<int64_t> used_records;

    Str qual_record(int64_t s);
    // a pytypes.T as an ir.Type; Refused when it has none
    TypeP ty(const T& t, int64_t line = 0, const Str& what = Str("value"));
};

bool is_opt(const TypeP& t);

// One function of the program into an ir.MethodDecl.
class FnLower {
  public:
    FnLower(Context& ctx, Fn& f, std::shared_ptr<infer::Eval> ev);
    // a module's top level seen as a function (python/__init__.py's _ModFn)
    FnLower(Context& ctx, Module& m, std::shared_ptr<infer::Eval> ev);

    Context& ctx;
    Program& p;
    Fn* f = nullptr;
    std::shared_ptr<infer::Eval> ev;
    // the function's own facts (those of a _ModFn for a module's top level)
    Str qname;
    Module* module = nullptr;
    Class* cls = nullptr;
    Str kind;
    std::vector<Str> params;
    std::vector<Str> kwonly;
    Str src;
    std::vector<NodeP> pre;                       // statements an expression needs before its statement
    int64_t tmp = 0;
    std::unordered_map<const void*, std::vector<Str>> decl_at;      // id(stmt) -> [names declared just before it]
    std::unordered_map<const void*, Str> decl_init;                 // id(Assign stmt) -> name declared by it
    std::unordered_map<const void*, std::set<Str>> decl_tuple;      // id(Assign stmt) -> names an unpacking declares
    std::unordered_map<Str, TypeP, py::str_hash> local_types;
    std::unordered_set<Str, py::str_hash> declared;
    std::unordered_set<Str, py::str_hash> loopvars;
    std::set<Str> calls;                          // qualnames of the program's functions it calls
    std::set<Str> classes_made;
    std::set<std::tuple<Str, Str>> globals_used;
    std::set<int64_t> records_used;
    TypeP ret;
    const A::Node* in_cond = nullptr;

    // -- types and positions
    const T& Tn(const A::Expr* node);
    const T& Tn(const A::Stmt* node);
    bool has_type(const void* node) const;
    TypeP ty(const T& t, const A::Node* node = nullptr, const Str& what = Str("value"));
    std::optional<Res> R(const void* node) const;
    int64_t line(const A::Node* n) const;

    // -- the function
    py::ref<ir::MethodDecl> lower();
    const std::vector<A::Stmt*>& body_stmts_of(const std::vector<A::Stmt*>& stmts, std::vector<A::Stmt*>& store) const;

    // -- where each local is declared
    void plan_locals();
    std::vector<Str> target_names(const A::Expr* t);
    std::vector<const A::Node*> own_nodes(const A::Stmt* st);

    // -- statements
    py::ref<ir::Block> block(const std::vector<A::Stmt*>& stmts);
    std::vector<NodeP> stmt(const A::Stmt* st);
    std::vector<NodeP> s_Expr(const A::ExprStmt* st);
    std::vector<NodeP> s_Assign(const A::Assign* st);
    std::vector<NodeP> s_AnnAssign(const A::AnnAssign* st);
    std::vector<NodeP> s_AugAssign(const A::AugAssign* st);
    NodeP expr_of_target(const A::Expr* tgt, const NodeP& lowered);
    std::vector<NodeP> store(const A::Expr* tgt, const NodeP& value, const A::Stmt* st);
    std::vector<NodeP> unpack(const A::Expr* tgt, const NodeP& value, const A::Stmt* st);
    NodeP name_expr(const Str& name);
    NodeP target_expr(const A::Expr* tgt);
    TypeP elem_type(const TypeP& t, int64_t line);
    std::vector<NodeP> s_Return(const A::Return* st);
    std::vector<NodeP> s_Raise(const A::Raise* st);
    std::vector<NodeP> s_Assert(const A::Assert* st);
    std::vector<NodeP> s_If(const A::If* st);
    std::vector<NodeP> s_While(const A::While* st);
    std::vector<NodeP> s_For(const A::For* st);
    bool loop_local(const A::Stmt* st, const Str& name);
    std::vector<NodeP> range_loop(const A::For* st);
    bool pure(const A::Expr* node, const A::Stmt* loop);
    std::vector<NodeP> s_Try(const A::Try* st);
    std::vector<NodeP> s_With(const A::With* st);

    // -- comprehensions
    Str new_tmp(const Str& base);
    NodeP empty(const TypeP& t);
    std::vector<NodeP> comp_into(const A::Assign* st, const A::Name* tgt, const A::Expr* comp);
    std::vector<NodeP> comp_loops(const A::Expr* comp, const Str& into, const TypeP& t);
    std::vector<NodeP> gen_loops(const std::vector<A::comprehension*>& gens, size_t k, const std::function<NodeP()>& innermost);
    NodeP hoist_comp(const A::Expr* node, const char* kind);
    NodeP gen_as_list(const A::GeneratorExp* gen);
    NodeP any_all(const A::Call* node, const char* which);

    // -- records
    bool is_record_literal(const A::Expr* node);
    std::vector<NodeP> record_builder(const A::Assign* st, const A::Name* tgt);
    NodeP record_value(const A::Expr* node);
    std::vector<std::pair<std::optional<Str>, NodeP>> spread_fields(const NodeP& src, const A::Expr* node);
    NodeP field_ref(const NodeP& obj, const Str& key, const A::Expr* node, int64_t line, bool store = false, bool raw = false);

    // -- expressions
    NodeP cond(const A::Expr* node);
    NodeP truthy(const NodeP& e);
    NodeP coerce(const NodeP& e, const TypeP& want);
    NodeP expr(const A::Expr* node);
    NodeP e_Constant(const A::Constant* n);
    NodeP e_Name(const A::Name* n);
    NodeP narrowed(const NodeP& e, const TypeP& flow);
    NodeP e_Attribute(const A::Attribute* n);
    NodeP e_Subscript(const A::Subscript* n);
    NodeP e_BinOp(const A::BinOp* n);
    NodeP binop(const A::Node* n, A::OpKind op, const NodeP& a, const NodeP& b, const T& ta, const T& tb, const T& tr);
    NodeP as_double(const NodeP& e);
    NodeP int_op(const A::Node* n, const char* o, const NodeP& a, const NodeP& b, const T& ta, const T& tb, const T& tr,
                 const TypeP& rt);
    NodeP e_UnaryOp(const A::UnaryOp* n);
    NodeP paren(const NodeP& x);
    NodeP e_BoolOp(const A::BoolOp* n);
    NodeP paren_bool(const NodeP& e, const char* op);
    NodeP e_Compare(const A::Compare* n);
    NodeP compare(const A::Node* n, A::CmpOpKind op, const NodeP& a, const NodeP& b, const A::Expr* an, const A::Expr* bn);
    NodeP e_IfExp(const A::IfExp* n);
    NodeP e_JoinedStr(const A::JoinedStr* n);
    NodeP e_Tuple(const A::Tuple* n);
    NodeP e_List(const A::List* n);
    NodeP e_Set(const A::Set* n);
    NodeP e_Dict(const A::Dict* n);
    NodeP lambda_expr(const A::Lambda* lam, const std::vector<TypeP>& ptypes);
    bool effects(const NodeP& e);

    // -- calls
    NodeP e_Call(const A::Call* n);
    TypeP call_ret(Fn* f, const A::Call* n);
    std::vector<NodeP> call_args(const A::Call* n, Fn* f, bool bound);
    NodeP default_lit(const A::Constant* d);
    NodeP builtin_call(const A::Call* n, const Str& name, const T& t);
    NodeP method_call(const A::Call* n, const Str& name, const T& t);
    NodeP builtin_call_sort(const A::Call* n, const NodeP& recv);
    NodeP std_call(const A::Call* n, const Res& r, const T& t);

  private:
    int64_t cur_line_ = 0;
    std::string utf8_src_;
    std::vector<size_t> line_starts_;
    bool have_src_ = false;
    std::optional<Str> source_segment(const A::Node* n);
    NodeP expr_dispatch(const A::Expr* node);
};

// the outcome of lowering one function: its tree, or why it was refused; and the FnLower that did it
struct Lowered {
    py::ref<ir::MethodDecl> decl;          // null when refused
    std::optional<Refused> refused;
    std::shared_ptr<FnLower> fl;           // null when the front end could not even run the function
    bool is_refused() const { return refused.has_value(); }
};

// One module: its L2 unit, what was translated, what was refused and why.
struct ModuleResult {
    Str name;
    py::ref<ir::Unit> unit;
    py::dict<Str, py::ref<ir::MethodDecl>> translated;       // fn qname -> ir.MethodDecl
    py::dict<Str, infer::Issue> refused;                      // fn qname -> Issue
    py::dict<Str, std::set<Str>> calls;                       // fn qname -> set of fn qnames it calls
    py::dict<Str, std::set<Str>> made;                        // fn qname -> classes it constructs
    py::dict<Str, std::set<std::tuple<Str, Str>>> globals;    // fn qname -> {(module, name)}
    py::list<py::ref<ir::FieldDecl>> global_decls;            // the module's names the translation uses
    py::dict<Str, int64_t> lines;                             // fn qname -> lines
    explicit ModuleResult(const Str& name) : name(name) {}
};

// {qname: Lowered} for the given functions
py::dict<Str, Lowered> lower_functions(Program& prog, Context& ctx, const std::vector<Fn*>& fns);

// a function that calls (or constructs through) a refused function is refused too
void close_refusals(Program& prog, py::dict<Str, Lowered>& lowered);

}  // namespace codebreaker::translate::frontends::python::lower
