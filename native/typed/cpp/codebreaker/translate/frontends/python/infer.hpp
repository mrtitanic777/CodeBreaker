// codebreaker/translate/frontends/python/infer.hpp -- typed C++ from
// codebreaker.translate.frontends.python.infer.
//
// infer.py -- what each value of a Python program is, found without running it.
//
// An abstract interpreter over the Python AST. Each function is run on types instead of values, flow by
// flow: a branch's condition narrows what it tests (`x is None`, `p < len(data)`, `isinstance`), a loop is
// run to a fixpoint (integer bounds widened over thresholds, then narrowed back), and a successful access
// bounds the index that made it (`data[p]` returning means p is inside data). What a function learns about
// others -- the arguments it passes, what it stores into attributes and containers -- becomes program facts
// (program.Program), and the program is run round after round until no fact changes.
//
// An integer's interval is what lets it be int64_t in C++. Where none can be shown, its expressions carry
// an unbounded interval and the lowering refuses them (int-range). Counters get one more rule: a variable a
// loop only ever increases by a bounded step, in a loop whose number of passes is bounded (a range, a walk
// over a sequence -- bounded by the address space --, a while loop over a variable it strictly increases
// toward a bound), is bounded by its start plus steps times passes.
//
// Everything the interpreter cannot type is an Issue on the node: (class, detail). The classes:
//
//     untyped      a parameter no annotation or call site gives a type
//     conflict     values of two kinds meet in one place (no one C++ type)
//     int-range    an integer not shown to stay inside int64
//     dynamic      a dynamic feature (getattr, *args, a generator, set iteration order, ...)
//     stdlib       a standard library call not mapped (detail: which)
//     extern       a library outside the program and the standard library
//     syntax       a construct the front end does not lower (with, finally, nested def, ...)
//     unresolved   a name not found
//
// Notes on the port:
//   * the Python's `state` dict is a `State` (an ordered name -> type map with value semantics); a state that
//     is `None` (unreachable) is an empty std::optional<State>;
//   * an expression's result `t` or `(t, res)` is a `TR`; the optional resolution tuple is a `Tup`;
//   * the arguments of a call are `std::vector<std::optional<T>>` (None for a lambda or generator argument),
//     the keywords a `Kw` (an ordered map keyword -> optional type; the key None is `**spread`);
//   * `types`, `res`, `seen` are keyed by the node's address (Python's id(node)).
#pragma once

#include "codebreaker/translate/frontends/python/program.hpp"

namespace codebreaker::translate::frontends::python::infer {

using program::Big;
using program::Bound;
using program::Fn;
using program::Class;
using program::Module;
using program::Program;
using program::Ref;
using program::Res;
using program::Str;
using program::T;
using program::Tup;
namespace A = pyfront;
namespace P = pytypes;

const std::set<Str>& BUILTIN_FUNCS();
const std::set<Str>& DYNAMIC_BUILTINS();
constexpr int64_t MAX_LOOP = 10;

// _has_top
bool has_top(const T& t, int64_t depth = 0);

struct Issue {
    Str cls;
    Str detail;
    int64_t line;
    Issue(const Str& cls, const Str& detail, int64_t line) : cls(cls), detail(detail), line(line) {}
    Str reason() const { return detail.empty() ? cls : cls + Str(":") + detail; }
    Str repr() const { return Str("Issue(") + reason() + Str(" at ") + py::to_str(line) + Str(")"); }
};

// Every name a function binds (Python's rule: a name assigned anywhere in the body is local).
std::set<Str> assigned_names(const A::FunctionDef* fn_node);

// the type of each local at a point of the flow; the order of binding is kept (a dict)
class State {
  public:
    const T* find(const Str& name) const;
    bool contains(const Str& name) const { return find(name) != nullptr; }
    const T& at(const Str& name) const;  // KeyError when absent
    void set(const Str& name, const T& t);
    void pop(const Str& name);
    // a.keys() == b.keys()
    bool same_keys(const State& o) const;
    const std::vector<std::pair<Str, T>>& items() const { return v_; }
    std::vector<std::pair<Str, T>>& items() { return v_; }

  private:
    std::vector<std::pair<Str, T>> v_;
};
using SP = std::optional<State>;

// an expression's type, and what it resolved to
struct TR {
    T t;
    std::optional<Res> res;
    TR(const T& t) : t(t) {}  // NOLINT
    TR(const T& t, const Res& r) : t(t), res(r) {}
};

// the keywords of a call: name (None for `**spread`) -> its type (None for a lambda or generator)
using Args = std::vector<std::optional<T>>;
using Kw = py::dict<std::optional<Str>, std::optional<T>>;

// One function run on types. With `record`, every node's type and resolution is kept for lowering.
class Eval {
  public:
    Eval(Program& prog, Fn* fn, bool record = false);
    virtual ~Eval();

    Program& p;
    Fn* fn;
    Module* m;
    bool record;
    int64_t quiet = 0;
    std::unordered_map<const void*, T> types;
    std::unordered_map<const void*, Res> res;
    std::unordered_map<const void*, T> seen;
    std::vector<Issue> issues;
    T ret;
    bool returns_value = false;
    // the (breaks, continues) states of each loop being run
    std::vector<std::pair<std::vector<State>, std::vector<State>>> loops;
    std::set<Str> locals;
    py::dict<Str, Ref> local_imports;
    Class* cls;
    const void* builder = nullptr;
    // a loop met again inside an enclosing loop's fixpoint with the same state at entry: its state at exit
    struct MemoKey {
        const void* st;
        std::vector<std::pair<Str, T>> items;  // the entry state, sorted by name, containers named by their roots
        bool operator==(const MemoKey& o) const;
    };
    struct MemoKeyHash {
        size_t operator()(const MemoKey& k) const;
    };
    std::unordered_map<MemoKey, State, MemoKeyHash> loop_memo;
    std::unordered_map<const void*, Res> res_quiet;          // _res_quiet: the calls' ('fn'|'method') resolutions

    // -- bookkeeping
    T note(const A::Node* node, const T& t, const std::optional<Res>& res = std::nullopt);
    T issue(const A::Node* node, const Str& cls, const Str& detail = Str());
    // facts are contributed only from a pass whose loop heads are final
    bool live() const { return quiet == 0; }
    void contribute_param(const Str& fq, const Str& name, const T& t);
    // an argument: it fits int64, and it is not None where the callee does not expect None
    T arg_fit(const A::Node* node, const Fn& f, const Str& name, const T& t);
    // A value leaving this function (an argument, an attribute, an item): an int must be shown to fit
    // int64 here -- the issue is this function's -- and what arrives is within int64.
    T fit(const A::Node* node, const T& t, const Str& what);
    void contribute_attr(const Str& cq, const Str& attr, const T& t);

    // -- the function
    virtual Eval& run();

    // -- statements
    SP block(const std::vector<A::Stmt*>& stmts, SP state);
    SP stmt(const A::Stmt* st, const State& state);

    // -- loops
    // a number of passes: an int, or infinity (float('inf'))
    struct XInt {
        bool inf = false;
        Big v;
    };
    using BodyFn = std::function<SP(const State&)>;
    using BoundFn = std::function<std::optional<Big>(const State&, const State&)>;
    // Run a loop body to a fixpoint; the last pass (with final heads) records and contributes.
    SP loop(const A::Stmt* st, const State& init, const BodyFn& body, const BodyFn& exit_refine, const BoundFn& bound);
    State fixpoint(const A::Stmt* st, const State& init, State head, const BodyFn& body, const BoundFn& bound);
    State merge_head(const State& head, State new_);
    std::optional<T> narrow_one(const T* old, const T& new_);
    State widen_states(const State& old, const State& new_);
    bool same_state(const State& a, const State& b);
    SP join_states(const SP& a, const SP& b);
    State counter_rule(const A::Stmt* st, State head, const State& init, const BoundFn& bound);
    // (most it can increase, most it can decrease) in one pass over stmts -- None if `var` is set any other way
    // or a step is not bounded.
    std::optional<std::pair<Big, Big>> step_bound(const std::vector<A::Stmt*>& stmts, const Str& var);
    // passes of `for x in <it>`, or None
    std::optional<Big> range_count(const A::Expr* it);
    std::optional<Big> while_bound(const A::While* st, const State& head, const State& init);
    // (element type, pass bound) of iterating a value
    std::pair<T, std::optional<Big>> iteration(const A::Expr* node, const T& t, const State& state);

    // -- assignment
    State assign(const A::Expr* tgt, const T& t, State state, const A::Expr* value_node, bool aug = false);
    void builder_keys(const A::Assign* st, const T& t);

    // -- refinement
    // The state after `test` came out `truth`.
    SP refine(const A::Expr* test, const SP& state, bool truth);
    State refine_cmp(const A::Expr* left, A::CmpOpKind op, const A::Expr* right, State state, bool truth);
    // A successful index bounds the variable that indexed: data[p] returned, so p is inside data.
    SP after_access(const A::Expr* node, const SP& state);
    std::optional<Str> res_fn(const A::Expr* call);

    // -- expressions
    T ev(const A::Expr* node, const State& state);
    std::optional<Ref> lookup(const Str& name);
    std::optional<Str> lookup_class(const A::Expr* n);
    T ref_type(const A::Node* n, const Ref& r);
    // the most items a slice can hold: d[a:a+4] four, d[:8] eight, d[2:6] four
    Bound slice_cap(const A::Slice* sl, const Bound& cap);
    std::vector<T> ev_slice_parts(const A::Slice* sl, const State& state);
    void need_int_index(const A::Node* node, const T& t);
    std::tuple<Str, int64_t> where(const A::Node* n);

    // -- operators
    T binop(const A::Node* n, A::OpKind op, T a, T b, const A::Expr* ln, const A::Expr* rn);
    // an int meeting a float: exact in a double only within 2**53
    void exact_float(const A::Node* n, const T& t);
    void check_percent(const A::Node* n, const A::Expr* ln, const A::Expr* rn, const T& b);
    void check_str(const A::Node* node, const T& t, bool as_repr);
    void check_format(const A::Node* node, const T& t, const Str& spec);
    void check_compare(const A::Compare* n, A::CmpOpKind op, const T& a, const T& b);
    void comparable(const A::Node* n, const T& a, const T& b, bool eq = false, bool order = false);
    void spread(int64_t s, const T& vt, const A::Node* n);
    State comp_scope(const std::vector<A::comprehension*>& gens, const State& state);

    // -- calls
    TR call_value(const A::Call* n, const T& ft, const Args& args, const Kw& kw, const State& state);
    TR construct(const A::Call* n, const Str& cq, const Args& args, const Kw& kw);
    TR call_fn(const A::Call* n, Fn& f, const Args& args, const Kw& kw, bool bound = false);
    void bind_args(const A::Call* n, Fn& f, const Args& args, const Kw& kw, bool bound = false);
    TR method_call(const A::Call* n, const T& recv, const Str& attr, const Args& args, const Kw& kw, const State& state);
    bool first_stmt(const A::Call* call);
    TR super_call(const A::Call* n, const Str& attr, const Args& args, const Kw& kw);
    TR file_method(const A::Call* n, const Str& attr, const Args& args, const Kw& kw);
    TR builtin(const A::Call* n, const Str& name, const Args& args, const Kw& kw, const State& state);
    TR std_call(const A::Call* n, const Str& name, const Args& args, const Kw& kw, const State& state);
    // the type a key function returns for these parameter types
    T lambda_type(const A::Expr* lam, const std::vector<T>& params, const State& state);
    // element type of a generator expression argument
    std::pair<T, State> gen_elem(const A::GeneratorExp* g, const State& state);

  private:
    TR e_dispatch(const A::Expr* n, const State& state);
    SP s_dispatch(const A::Stmt* st, const State& state);

    // statements
    SP s_Expr(const A::ExprStmt* st, const State& state);
    SP s_Assign(const A::Assign* st, const State& state);
    SP s_AnnAssign(const A::AnnAssign* st, const State& state);
    SP s_AugAssign(const A::AugAssign* st, const State& state);
    SP s_Return(const A::Return* st, const State& state);
    SP s_Raise(const A::Raise* st, const State& state);
    SP s_Assert(const A::Assert* st, const State& state);
    SP s_Import(const A::Import* st, const State& state);
    SP s_ImportFrom(const A::ImportFrom* st, const State& state);
    SP s_If(const A::If* st, const State& state);
    SP s_With(const A::With* st, const State& state);
    SP s_Try(const A::Try* st, const State& state);
    bool std_exception(const A::Expr* n);
    SP s_Break(const A::Stmt* st, const State& state);
    SP s_Continue(const A::Stmt* st, const State& state);
    SP s_While(const A::While* st, const State& state);
    SP s_For(const A::For* st, const State& state);

    // loop helpers
    std::optional<std::pair<Big, Big>> step_one(const A::Stmt* st, const Str& var);
    std::optional<Big> cmp_bound(const A::Expr* t, const A::While* st, const State& head, const State& init);
    // the least `var` grows along every path back to the loop's head (0: some path may not grow it; inf: no
    // path loops back)
    XInt min_step(const std::vector<A::Stmt*>& stmts, const Str& var);
    std::optional<std::pair<XInt, XInt>> paths(const std::vector<A::Stmt*>& stmts, const Str& var);
    bool grows_only(const A::Stmt* st, const Str& var);
    bool reads_fields_only(const A::Expr* value, const Str& name);

    // expressions
    TR e_Constant(const A::Constant* n, const State& state);
    TR e_JoinedStr(const A::JoinedStr* n, const State& state);
    TR e_Name(const A::Name* n, const State& state);
    TR e_Attribute(const A::Attribute* n, const State& state);
    TR e_Subscript(const A::Subscript* n, const State& state);
    TR e_UnaryOp(const A::UnaryOp* n, const State& state);
    TR e_BoolOp(const A::BoolOp* n, const State& state);
    TR e_Compare(const A::Compare* n, const State& state);
    TR e_IfExp(const A::IfExp* n, const State& state);
    TR e_Tuple(const A::Tuple* n, const State& state);
    TR e_List(const A::List* n, const State& state);
    TR e_Set(const A::Set* n, const State& state);
    TR e_Dict(const A::Dict* n, const State& state);
    TR e_Call(const A::Call* n, const State& state);
};

// A module's top level: the types of its names, its classes' class-level values and its functions'
// parameter defaults (each evaluated where Python evaluates it, once, at import).
class ModuleEval : public Eval {
  public:
    ModuleEval(Program& prog, Module* module, bool record = false);
    T safe_ev(const A::Expr* v);
    Eval& run() override;
};

// the standard library attributes with a known type: sys.maxsize, os.sep, math.pi ...
const py::dict<Str, T>& STD_ATTRS();

}  // namespace codebreaker::translate::frontends::python::infer
