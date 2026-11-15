// cb/translate/frontends/python/infer.hpp -- what each value of a Python program is, found without running it.
//
// An abstract interpreter over the Python syntax tree. Each function is run on types instead of values, flow by flow: a
// branch's condition narrows what it tests (`x is None`, `p < len(data)`, `isinstance`), a loop is run to a fixpoint
// (integer bounds widened over thresholds, then narrowed back), and a successful access bounds the index that made it
// (`data[p]` returning means p is inside data). What a function learns about others -- the arguments it passes, what it
// stores into attributes and containers -- becomes program facts (program::Program), and the program is run round after
// round until no fact changes (`Inference` is the program's evaluator for that).
//
// An integer's interval is what lets it be int64_t in C++. Where none can be shown, its expressions carry an unbounded
// interval and the lowering refuses them (int-range). Counters get one more rule: a variable a loop only ever increases
// by a bounded step, in a loop whose number of passes is bounded (a range, a walk over a sequence -- bounded by the
// address space --, a while loop over a variable it strictly increases toward a bound), is bounded by its start plus
// steps times passes.
//
// Everything the interpreter cannot type is an Issue on the node: (class, detail), the classes named by IssueClass.
//
// What is not here: the types of the builtins, of the methods of the builtin types and of the standard library calls
// (module `methods`). The interpreter asks them through the `Library` it is given; `unmapped_library()` types none of
// them (each call is refused as stdlib:<name>), which is enough to infer a program that uses no library at all.
#pragma once

#include <cstddef>
#include <cstdint>
#include <functional>
#include <optional>
#include <set>
#include <string>
#include <string_view>
#include <unordered_map>
#include <utility>
#include <vector>

#include "cb/translate/frontends/python/program.hpp"

namespace cb::translate::frontends::python::infer {

namespace syntax = ::pyfront;
using program::Class;
using program::FactKey;
using program::Fn;
using program::FnKind;
using program::Module;
using program::Program;
using program::Ref;
using pytypes::Bound;
using pytypes::Kind;
using pytypes::Type;
namespace P = pytypes;

// ---- what the interpreter reports --------------------------------------------------------------------------

//     untyped      a parameter no annotation or call site gives a type
//     conflict     values of two kinds meet in one place (no one C++ type)
//     int-range    an integer not shown to stay inside int64
//     dynamic      a dynamic feature (getattr, *args, a generator, set iteration order, ...)
//     stdlib       a standard library call not mapped (detail: which)
//     extern       a library outside the program and the standard library
//     syntax       a construct the front end does not lower (with, finally, nested def, ...)
//     unresolved   a name not found
enum class IssueClass { Untyped, Conflict, IntRange, Dynamic, Stdlib, Extern, Syntax, Unresolved };
// "untyped", "int-range" ...: the class as the ledgers and refusal reasons write it.
std::string_view issue_class_word(IssueClass cls);

struct Issue {
    IssueClass cls = IssueClass::Untyped;
    std::string detail;
    std::uint32_t line = 0;
    // "class" or "class:detail".
    std::string reason() const;
};

// What a node resolved to, which the lowering reads to know how to write it.
enum class ResKind {
    Local,        // a local variable (read or written)
    Fn,           // `name`: the function's qualified name
    Class,        // `name`: the class's qualified name
    Global,       // `name`: the module, `member`: the name
    Module,       // `name`: a module of the program
    StdModule,    // `name`: a module of the standard library
    StdObj,       // `name`: an object of one (`from os import path`)
    Extern,       // `name`: a library the front end does not map
    Missing,      // `name`: `module.member` that is no name of the program
    Builtin,      // `name`: a builtin function, or a variant of it the lowering writes differently (`name_list`)
    Exc,          // `name`: a builtin exception class
    Const,        // `name`: True, False, None, __name__, __file__, __doc__
    Field,        // `name`: the class that declares the attribute, `member`: the attribute
    ClassAttr,    // `name`: the class, `member`: the attribute
    Property,     // `name`: the property function
    Method,       // `name`: the method called through an instance
    Bound,        // `name`: a method kept as a value
    Ctor,         // `name`: the class constructed
    BaseCall,     // `name`: `Base.method(self, ...)`, the base's own method
    Super,        // `name`: `super().method(...)`
    StdAttr,      // `name`: `os.sep`
    BMethod,      // `name`: "str.split": a method of a builtin type
    Slice,        // `name`: the kind of the sequence sliced
    SliceStore,   // `name`: its kind
    TupleSlice,   // `lo`..`hi`: the bounds of the slice, when constant
    Index,        // `name`: the kind of the sequence indexed
    IndexStore,   // `name`: its kind
    TupleIndex,   // `lo`: the index, when constant
    FieldGet,     // `name`: the key of a record read
    DictGet,
    DictStore,    // `name`: the key of a record written, when constant
    Unpack,
    MixedBool,    // `name`: why the operands have no one type
    Isinstance,   // `flag`: the answer, when the types decide it
    Std           // `name`: a standard library call, `member`: its format when it has one (struct)
};

struct Res {
    ResKind kind = ResKind::Local;
    std::optional<std::string> name;
    std::string member;
    std::optional<Wide> lo, hi;
    bool flag = false;

    static Res of(ResKind kind, std::optional<std::string> name = std::nullopt, std::string member = {}) {
        Res r;
        r.kind = kind;
        r.name = std::move(name);
        r.member = std::move(member);
        return r;
    }
};

// A name as the function sees it: a name of the program or an import (a Ref), or one the language provides.
struct Binding {
    enum class Source { Program, Builtin, Exception, Constant };
    Source source = Source::Program;
    Ref ref;                 // for Program
    std::string name;        // for the others
    Res resolution() const;
};

// ---- the interpreter's data ----------------------------------------------------------------------------------

// True when the type, or something it is made of (down six levels), is a Top.
bool has_top(const Type& t, int depth = 0);
bool is_one_of(const Type& t, std::initializer_list<Kind> kinds);
// The names the lowering knows as builtin functions, and those among them no type can follow.
const std::set<std::string>& builtin_functions();
const std::set<std::string>& dynamic_builtins();

constexpr int kMaxLoopPasses = 10;

// The type of each local at a point of the flow; the order of binding is kept.
class State {
public:
    const Type* find(const std::string& name) const;
    bool contains(const std::string& name) const { return find(name) != nullptr; }
    // For a name that must be there (absent is a bug).
    const Type& at(const std::string& name) const;
    void set(const std::string& name, const Type& type);
    void pop(const std::string& name);
    // The two states bind the same names.
    bool same_keys(const State& other) const;
    const std::vector<std::pair<std::string, Type>>& items() const { return items_; }
    std::vector<std::pair<std::string, Type>>& items() { return items_; }

private:
    std::vector<std::pair<std::string, Type>> items_;
};
// A state that is nullopt is unreachable.
using Flow = std::optional<State>;

// An expression's type, and what it resolved to.
struct TR {
    Type t;
    std::optional<Res> res;
    TR(const Type& type) : t(type) {}                            // NOLINT: a result with no resolution
    TR(const Type& type, const Res& r) : t(type), res(r) {}
};

// The arguments of a call: a lambda or a generator (not typed in place) is nullopt.
using Args = std::vector<std::optional<Type>>;
struct Keyword {
    std::optional<std::string> name;   // nullopt: a `**spread`
    std::optional<Type> type;
};
using Kw = std::vector<Keyword>;
const Keyword* find_keyword(const Kw& kw, std::string_view name);

class Eval;

// The types of the builtins, the methods of the builtin types and the standard library calls.
class Library {
public:
    virtual ~Library() = default;
    // recv.attr(args) for a value of a builtin type
    virtual TR method(Eval& ev, const syntax::Call* call, const Type& recv, const std::string& attr, const Args& args,
                      const Kw& kw, const State& state) const = 0;
    // name(args) for a builtin function
    virtual TR builtin(Eval& ev, const syntax::Call* call, const std::string& name, const Args& args, const Kw& kw,
                       const State& state) const = 0;
    // module.name(args) for the standard library ("os.path.join")
    virtual TR std_call(Eval& ev, const syntax::Call* call, const std::string& name, const Args& args, const Kw& kw,
                        const State& state) const = 0;
};
// Types no library call: each is refused as stdlib:<name>.
const Library& unmapped_library();

// ---- the interpreter ---------------------------------------------------------------------------------------

// One function run on types. With `record`, every node's type and resolution is kept for lowering.
class Eval {
public:
    Eval(Program& program, Fn* function, bool record = false, const Library& library = unmapped_library());
    virtual ~Eval() = default;

    Program& p;
    Fn* fn;                  // nullptr for a module's top level
    Module* m;
    bool record;
    const Library& lib;
    int quiet = 0;           // > 0 while a loop's heads are not final: nothing is recorded or contributed
    // Keyed by the node's address.
    std::unordered_map<const void*, Type> types;     // recorded: what each node was
    std::unordered_map<const void*, Res> res;        // recorded: what each node resolved to
    std::unordered_map<const void*, Type> seen;      // every pass: what each node was (what loop bounds read)
    std::vector<Issue> issues;
    Type ret;
    bool returns_value = false;
    Type yielded;   // what `yield` gave, joined: the item type of a generator function
    bool is_generator = false;
    // the (breaks, continues) states of each loop being run
    struct LoopExits {
        std::vector<State> breaks;
        std::vector<State> continues;
    };
    std::vector<LoopExits> loops;
    std::set<std::string> locals;
    OrderedMap<std::string, Ref> local_imports;
    Class* cls;
    const void* builder = nullptr;

    // A loop met again inside an enclosing loop's fixpoint with the same state at entry: its state at exit.
    struct MemoKey {
        const void* st = nullptr;
        std::vector<std::pair<std::string, Type>> items;   // the entry state, sorted by name, containers named by their roots
        bool operator==(const MemoKey& other) const;
    };
    struct MemoKeyHash {
        std::size_t operator()(const MemoKey& key) const;
    };
    std::unordered_map<MemoKey, State, MemoKeyHash> loop_memo;
    std::unordered_map<const void*, Res> res_quiet;   // the calls' Fn/Method resolutions, kept also while not recording

    // -- bookkeeping
    Type note(const syntax::Node* node, const Type& t, const std::optional<Res>& resolution = std::nullopt);
    Type issue(const syntax::Node* node, IssueClass cls, const std::string& detail = {});
    // Facts are contributed only from a pass whose loop heads are final.
    bool live() const { return quiet == 0; }
    void contribute_param(const std::string& fn_name, const std::string& param, const Type& t);
    // An argument: it fits int64, and it is not None where the callee does not expect None.
    Type arg_fit(const syntax::Node* node, const Fn& f, const std::string& param, const Type& t);
    // A value leaving this function (an argument, an attribute, an item): an int must be shown to fit int64 here -- the
    // issue is this function's -- and what arrives is within int64.
    Type fit(const syntax::Node* node, const Type& t, const std::string& what);
    void contribute_attr(const std::string& class_name, const std::string& attr, const Type& t);

    // -- the function
    virtual Eval& run();

    // -- statements
    Flow block(const std::vector<syntax::Stmt*>& stmts, Flow state);
    Flow stmt(const syntax::Stmt* st, const State& state);

    // -- loops
    // A number of passes: an int, or infinity.
    struct Passes {
        bool infinite = false;
        Wide count;
    };
    using BodyFn = std::function<Flow(const State&)>;
    using BoundFn = std::function<std::optional<Wide>(const State&, const State&)>;
    // Run a loop body to a fixpoint; the last pass (with final heads) records and contributes.
    Flow loop(const syntax::Stmt* st, const State& init, const BodyFn& body, const BodyFn& exit_refine, const BoundFn& bound);
    State fixpoint(const syntax::Stmt* st, const State& init, State head, const BodyFn& body, const BoundFn& bound);
    State merge_head(const State& head, State incoming);
    static Type narrow_one(const Type* old_type, const Type& incoming);
    State widen_states(const State& old_state, const State& incoming);
    bool same_state(const State& a, const State& b);
    Flow join_states(const Flow& a, const Flow& b);
    State counter_rule(const syntax::Stmt* st, State head, const State& init, const BoundFn& bound);
    // (most it can increase, most it can decrease) in one pass over stmts -- nullopt if `var` is set any other way or a
    // step is not bounded.
    std::optional<std::pair<Wide, Wide>> step_bound(const std::vector<syntax::Stmt*>& stmts, const std::string& var);
    // passes of `for x in <it>`, or nullopt
    std::optional<Wide> range_count(const syntax::Expr* it);
    std::optional<Wide> while_bound(const syntax::While* st, const State& head, const State& init);
    // (element type, pass bound) of iterating a value
    std::pair<Type, std::optional<Wide>> iteration(const syntax::Expr* node, const Type& t);

    // -- assignment
    State assign(const syntax::Expr* target, const Type& t, State state, bool aug = false);
    void builder_keys(const syntax::Assign* st, const Type& t);

    // -- refinement
    // The state after `test` came out `truth`.
    Flow refine(const syntax::Expr* test, const Flow& state, bool truth);
    State refine_cmp(const syntax::Expr* left, syntax::CmpOpKind op, const syntax::Expr* right, State state, bool truth);
    // A successful index bounds the variable that indexed: data[p] returned, so p is inside data.
    Flow after_access(const syntax::Expr* node, const Flow& state);
    std::optional<std::string> res_fn(const syntax::Expr* call);

    // -- expressions
    Type ev(const syntax::Expr* node, const State& state);
    std::optional<Binding> lookup(const std::string& name);
    std::optional<std::string> lookup_class(const syntax::Expr* n);
    Type binding_type(const syntax::Node* n, const Binding& b);
    // the most items a slice can hold: d[a:a+4] four, d[:8] eight, d[2:6] four
    Bound slice_cap(const syntax::Slice* sl, const Bound& cap);
    std::vector<Type> ev_slice_parts(const syntax::Slice* sl, const State& state);
    void need_int_index(const syntax::Node* node, const Type& t);
    program::Origin where(const syntax::Node* n);

    // -- operators
    Type binop(const syntax::Node* n, syntax::OpKind op, Type a, Type b, const syntax::Expr* left, const syntax::Expr* right);
    // an int meeting a float: exact in a double only within 2**53
    void exact_float(const syntax::Node* n, const Type& t);
    void check_percent(const syntax::Node* n, const syntax::Expr* left, const syntax::Expr* right, const Type& b);
    void check_str(const syntax::Node* node, const Type& t, bool as_repr);
    void check_format(const syntax::Node* node, const Type& t, const std::string& spec);
    void check_compare(const syntax::Compare* n, syntax::CmpOpKind op, const Type& a, const Type& b);
    void comparable(const syntax::Node* n, const Type& a, const Type& b, bool eq = false, bool order = false);
    void spread(program::SiteId into, const Type& value_type, const syntax::Node* n);
    State comp_scope(const std::vector<syntax::comprehension*>& gens, const State& state);

    // -- calls
    TR call_value(const syntax::Call* n, const Type& callee, const Args& args, const Kw& kw, const State& state);
    TR construct(const syntax::Call* n, const std::string& class_name, const Args& args, const Kw& kw);
    TR call_fn(const syntax::Call* n, Fn& f, const Args& args, const Kw& kw, bool bound = false);
    void bind_args(const syntax::Call* n, Fn& f, const Args& args, const Kw& kw, bool bound = false);
    TR method_call(const syntax::Call* n, const Type& recv, const std::string& attr, const Args& args, const Kw& kw,
                   const State& state);
    // Is the call the first statement of the function (after a docstring)?
    bool first_stmt(const syntax::Call* call);
    TR super_call(const syntax::Call* n, const std::string& attr, const Args& args, const Kw& kw);
    TR file_method(const syntax::Call* n, const std::string& attr, const Args& args, const Kw& kw);
    // the type a key function returns for these parameter types
    Type lambda_type(const syntax::Expr* lam, const std::vector<Type>& params, const State& state);
    // element type of a generator expression argument
    std::pair<Type, State> gen_elem(const syntax::GeneratorExp* g, const State& state);

private:
    TR e_dispatch(const syntax::Expr* n, const State& state);
    Flow s_dispatch(const syntax::Stmt* st, const State& state);

    // statements
    Flow s_expr(const syntax::ExprStmt* st, const State& state);
    Flow s_assign(const syntax::Assign* st, const State& state);
    Flow s_ann_assign(const syntax::AnnAssign* st, const State& state);
    Flow s_aug_assign(const syntax::AugAssign* st, const State& state);
    Flow s_return(const syntax::Return* st, const State& state);
    Flow s_raise(const syntax::Raise* st, const State& state);
    Flow s_assert(const syntax::Assert* st, const State& state);
    Flow s_import(const syntax::Import* st, const State& state);
    Flow s_import_from(const syntax::ImportFrom* st, const State& state);
    Flow s_if(const syntax::If* st, const State& state);
    Flow s_with(const syntax::With* st, const State& state);
    Flow s_try(const syntax::Try* st, const State& state);
    bool std_exception(const syntax::Expr* n);
    Flow s_break(const State& state);
    Flow s_continue(const State& state);
    Flow s_while(const syntax::While* st, const State& state);
    Flow s_for(const syntax::For* st, const State& state);

    // loop helpers
    std::optional<std::pair<Wide, Wide>> step_one(const syntax::Stmt* st, const std::string& var);
    std::optional<Wide> cmp_bound(const syntax::Expr* t, const syntax::While* st, const State& init);
    // the least `var` grows along every path back to the loop's head (0: some path may not grow it; infinite: no path
    // loops back)
    Passes min_step(const std::vector<syntax::Stmt*>& stmts, const std::string& var);
    std::optional<std::pair<Passes, Passes>> paths(const std::vector<syntax::Stmt*>& stmts, const std::string& var);
    bool grows_only(const syntax::Stmt* st, const std::string& var);
    bool reads_fields_only(const syntax::Expr* value, const std::string& name);

    // expressions
    TR e_constant(const syntax::Constant* n);
    TR e_joined_str(const syntax::JoinedStr* n, const State& state);
    TR e_name(const syntax::Name* n, const State& state);
    TR e_attribute(const syntax::Attribute* n, const State& state);
    TR e_subscript(const syntax::Subscript* n, const State& state);
    TR e_unary_op(const syntax::UnaryOp* n, const State& state);
    TR e_bool_op(const syntax::BoolOp* n, const State& state);
    TR e_compare(const syntax::Compare* n, const State& state);
    TR e_if_exp(const syntax::IfExp* n, const State& state);
    TR e_tuple(const syntax::Tuple* n, const State& state);
    TR e_list(const syntax::List* n, const State& state);
    TR e_set(const syntax::Set* n, const State& state);
    TR e_dict(const syntax::Dict* n, const State& state);
    TR e_call(const syntax::Call* n, const State& state);
};

// A module's top level: the types of its names, its classes' class-level values and its functions' parameter defaults
// (each evaluated where Python evaluates it, once, at import).
class ModuleEval : public Eval {
public:
    ModuleEval(Program& program, Module* module, bool record = false, const Library& library = unmapped_library());
    Eval& run() override;

private:
    // The type of a module-level value; a value the front end cannot follow is a Top saying so.
    Type safe_ev(const syntax::Expr* value);
};

// The standard library attributes with a known type: sys.maxsize, os.sep, math.pi ...
const std::unordered_map<std::string, Type>& std_attrs();

// The evaluator `Program::solve` runs: every function and every module's top level on types.
class Inference : public program::Evaluator {
public:
    explicit Inference(const Library& library = unmapped_library()) : lib_(library) {}
    void run_module(Program& program, Module& module) override;
    void run_function(Program& program, Fn& fn) override;

private:
    const Library& lib_;
};

// Names of the types' `name` field by kind: "std:os.path" (a standard library module or object), "exc:ValueError" (an
// exception class), "bound:<fn>" (a method kept as a value), "builtin:len", "bmethod:str.split".
inline constexpr std::string_view kStdPrefix = "std:";
inline constexpr std::string_view kExcPrefix = "exc:";
inline constexpr std::string_view kBoundPrefix = "bound:";
inline constexpr std::string_view kBuiltinPrefix = "builtin:";
inline constexpr std::string_view kBMethodPrefix = "bmethod:";

}  // namespace cb::translate::frontends::python::infer
