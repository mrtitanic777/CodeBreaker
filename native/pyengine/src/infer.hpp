// infer.hpp -- what each value of a Python program is, found without running it (the old front
// end's infer.py and methods.py, rule for rule, plus the engine's extensions).
//
// An abstract interpreter over the AST. Each function is run on types instead of values, flow by
// flow: a branch's condition narrows what it tests, a loop is run to a fixpoint (integer bounds
// widened over thresholds, then narrowed back), and a successful access bounds the index that made
// it. What a function learns about others -- arguments, attribute and container stores, returns --
// becomes Program facts, and the program is run round after round until no fact changes.
#pragma once
#include <functional>
#include <map>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

#include "program.hpp"

namespace pyengine {

// a flow state: each local's type (sorted by name); nullopt = unreachable
struct State {
    std::vector<std::pair<std::string, T>> v;
    const T* get(const std::string& k) const;
    bool has(const std::string& k) const { return get(k) != nullptr; }
    void set(const std::string& k, const T& t);
    void erase(const std::string& k);
};
using SP = std::optional<State>;

// an expression's type and what it resolved to
struct TR {
    T t;
    Res r;
    TR(T t_) : t(std::move(t_)) {}                       // NOLINT
    TR(T t_, Res r_) : t(std::move(t_)), r(std::move(r_)) {}
};

class Eval {
public:
    Eval(Program& p, Fn* fn, bool record = false);
    virtual ~Eval() = default;
    virtual Eval& run();

    Program& p;
    Fn* fn;
    Module* m;
    bool record;
    int quiet = 0;
    std::unordered_map<const void*, T> types, seen;
    std::unordered_map<const void*, Res> res, res_quiet;
    std::vector<Issue> issues;
    T ret_t;
    T yield_t;                                // (new) a generator: the join of what it yields
    bool returns_value = false;
    std::vector<std::pair<std::vector<State>, std::vector<State>>> loops;
    std::set<std::string> locals;
    std::map<std::string, Ref> local_imports;
    Class* cls;
    const void* builder = nullptr;
    std::unordered_map<std::string, State> loop_memo;
    // (new) a nested function sees its definer's names (closure); the definer's final state
    std::map<std::string, T> bound_types;     // (new) every value bound to each local (for nested functions)

    // bookkeeping
    T note(const void* node, const T& t, const Res* r = nullptr);
    T issue(AnyNode node, const std::string& cls, const std::string& detail = "");
    bool live() const { return quiet == 0; }
    void contribute_param(const std::string& fq, const std::string& name, const T& t);
    T arg_fit(AnyNode node, Fn* f, const std::string& name, const T& t);
    T fit(AnyNode node, const T& t, const std::string& what);
    void contribute_attr(const std::string& cq, const std::string& attr, const T& t);

    // statements
    SP block(const std::vector<A::Stmt*>& stmts, SP state);
    SP stmt(const A::Stmt* st, const State& state);
    SP loop(const A::Stmt* st, const State& init, const std::function<SP(const State&)>& body,
            const std::function<SP(const State&)>& exit_refine,
            const std::function<std::optional<Big>(const State&, const State&)>& bound);
    State fixpoint(const A::Stmt* st, const State& init, State head, const std::function<SP(const State&)>& body,
                   const std::function<std::optional<Big>(const State&, const State&)>& bound);
    SP join_states(const SP& a, const SP& b);
    bool same_state(const State& a, const State& b);
    T narrow(const T* old, const T& nw);
    State counter_rule(const A::Stmt* st, State head, const State& init,
                       const std::function<std::optional<Big>(const State&, const State&)>& bound);
    std::optional<std::pair<Big, Big>> step_bound(const std::vector<A::Stmt*>& stmts, const std::string& var);
    std::optional<std::pair<Big, Big>> step_one(const A::Stmt* st, const std::string& var);
    std::optional<Big> range_count(const A::Expr* it);
    std::optional<Big> while_bound(const A::While* st, const State& head, const State& init);
    std::optional<Big> cmp_bound(const A::Expr* t, const A::While* st, const State& head, const State& init);
    std::pair<T, std::optional<Big>> iteration(const A::Expr* node, const T& t, const State& state);
    State assign(const A::Expr* tgt, const T& t, State state, const A::Expr* value_node, bool aug = false);
    void builder_keys(const A::Assign* st, const T& t);
    SP refine(const A::Expr* test, const SP& state, bool truth);
    State refine_cmp(const A::Expr* left, A::CmpOpKind op, const A::Expr* right, State state, bool truth);
    SP after_access(const A::Expr* node, const SP& state);
    std::string res_fn(const A::Expr* call);

    // expressions
    T ev(const A::Expr* node, const State& state);
    TR ev_inner(const A::Expr* node, const State& state);
    Ref lookup(const std::string& name);
    std::string lookup_class(const A::Expr* n);
    T ref_type(AnyNode n, const Ref& r);
    Res ref_res(const Ref& r);
    std::optional<Big> slice_cap(const A::Slice* sl, const std::optional<Big>& cap);
    std::vector<T> ev_slice_parts(const A::Slice* sl, const State& state);
    void need_int_index(const A::Expr* node, const T& t);
    std::pair<std::string, int> where(const A::Node* n);
    TR binop(AnyNode n, A::OpKind op, T a, T b, const A::Expr* ln, const A::Expr* rn);
    void exact_float(AnyNode n, const T& t);
    void check_percent(AnyNode n, const A::Expr* ln, const A::Expr* rn, const T& b);
    void check_str(AnyNode node, const T& t, bool as_repr);
    void check_format(AnyNode node, const T& t, const std::string& spec);
    void check_compare(AnyNode n, A::CmpOpKind op, const T& a, const T& b);
    void comparable(AnyNode n, const T& a, const T& b, bool eq = false, bool order = false);
    void spread(int s, const T& vt, AnyNode n);
    State comp_scope(const std::vector<A::comprehension*>& gens, const State& state);
    TR call_value(const A::Call* n, const T& ft, const std::vector<std::optional<T>>& args,
                  const OMap<std::optional<T>>& kw, const State& state);
    TR construct(const A::Call* n, const std::string& cq, const std::vector<std::optional<T>>& args,
                 const OMap<std::optional<T>>& kw);
    TR call_fn(const A::Call* n, Fn* f, const std::vector<std::optional<T>>& args, const OMap<std::optional<T>>& kw,
               bool bound = false);
    void bind_args(const A::Call* n, Fn* f, const std::vector<std::optional<T>>& args, const OMap<std::optional<T>>& kw,
                   bool bound = false);
    TR method_call(const A::Call* n, const T& recv, const std::string& attr, const std::vector<std::optional<T>>& args,
                   const OMap<std::optional<T>>& kw, const State& state);
    bool first_stmt(const A::Call* call);
    TR super_call(const A::Call* n, const std::string& attr, const std::vector<std::optional<T>>& args,
                  const OMap<std::optional<T>>& kw);
    T lambda_type(const A::Expr* lam, const std::vector<T>& params, const State& state);
    std::pair<T, State> gen_elem(const A::GeneratorExp* g, const State& state);

    // methods.cc: builtins, builtin types' methods, the standard library
    TR builtin(const A::Call* n, const std::string& name, const std::vector<std::optional<T>>& args,
               const OMap<std::optional<T>>& kw, const State& state);
    TR bmethod(const A::Call* n, const T& recv, const std::string& attr, const std::vector<std::optional<T>>& args,
               const OMap<std::optional<T>>& kw, const State& state);
    TR std_call(const A::Call* n, const std::string& name, const std::vector<std::optional<T>>& args,
                const OMap<std::optional<T>>& kw, const State& state);

    // statement kinds
    SP s_Assign(const A::Assign* st, const State& state);
    SP s_AugAssign(const A::AugAssign* st, const State& state);
    SP s_Try(const A::Try* st, const State& state);
    SP s_While(const A::While* st, const State& state);
    SP s_For(const A::For* st, const State& state);
    SP s_With(const A::With* st, const State& state);
    SP s_Import(const A::Import* st, const State& state);
    SP s_ImportFrom(const A::ImportFrom* st, const State& state);
    SP s_FunctionDef(const A::FunctionDef* st, const State& state);

    // (new) extensions: the stdlib rows the old engine refused, re, files, closures
    TR ext_std(const A::Call* n, const std::string& name, const std::vector<std::optional<T>>& args,
               const OMap<std::optional<T>>& kw, const State& state, bool& handled);
    TR ext_match_index(const A::Subscript* n, const std::string& pat, bool verbose);
    TR ext_method(const A::Call* n, const T& recv, const std::string& attr, const std::vector<std::optional<T>>& args,
                  const OMap<std::optional<T>>& kw, const State& state, bool& handled);
};

class ModuleEval : public Eval {
public:
    ModuleEval(Program& p, Module* m, bool record = false);
    Eval& run() override;
    T safe_ev(const A::Expr* v);
};

// (new) a regular expression's groups, read from its pattern (ext.cc): how many, their names, and
// which may be left unset (inside ?, *, {0,}, an alternative, a negative look-around, a conditional)
struct ReInfo {
    bool ok = true;
    int groups = 0;
    std::map<std::string, int> names;
    std::vector<bool> optional;    // [1..groups]
};
ReInfo re_analyze(const std::string& pat, bool verbose);

std::string fmt_big(const std::optional<Big>& b);
std::string py_repr(const std::string& s);       // repr() of a str (UTF-8)
std::string py_repr_list(const std::vector<std::string>& v);

}  // namespace pyengine
