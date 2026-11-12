// cb/translate/frontends/python/lower.hpp -- a typed Python module into L2 (translate/ir.hpp), or a refusal with its reason.
//
// Inference (infer.hpp) has run the whole program; this walks each function again with what it found -- every expression's
// type, every call's target -- and writes the L2 tree. Python's own operations that C++ does not have as such are calls
// owned by "py" (a Sym of kind Function with owner "py"): their names are the API map's (codebreaker/java/api_maps/python.json)
// or the support header's core semantics (truthy, slice, floordiv, ...). A back end writes them; nothing here knows C++.
//
// A function is refused -- left in Python -- when anything in it has no single type, when an integer is not shown to stay
// inside int64 (IntPolicy::Strict; Checked writes overflow-checked operations instead), when it uses a construct the front
// end does not lower, or when it calls a refused function.
//
// Python's model, kept: a list, dict, set, class instance or mutated record is a reference (aliasing is Python's); str,
// bytes, tuples, numbers and records never changed after they are built are values.
//
//     program::Program program;  ... add modules, index, solve(Inference) ...
//     lower::Context ctx(program, library);                              // the program's records, the policy, the arena of the trees
//     auto lowered = lower::lower_functions(ctx, functions);             // {qualified name: Lowered}
//     lower::close_refusals(lowered);                                    // a function that calls a refused one is refused
//     lowered.at("app.f").decl                                           // the MethodDecl, or .refused says why
//
// The module is several files: lower_records (what each container site is in C++), lower_types (inference type -> L2 type),
// lower_locals (where each local is declared), lower_stmt, lower_comp (comprehensions, records), lower_expr, lower_call.
#pragma once

#include <cstdint>
#include <map>
#include <memory>
#include <optional>
#include <set>
#include <span>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "cb/core/arena.hpp"
#include "cb/core/error.hpp"
#include "cb/core/ordered_map.hpp"
#include "cb/translate/frontends/python/infer.hpp"
#include "cb/translate/ir.hpp"

namespace cb::translate::frontends::python::lower {

namespace ir = ::cb::translate::ir;
namespace syntax = ::pyfront;
using program::Class;
using program::Fn;
using program::Module;
using program::Program;
using program::SiteId;
using pytypes::Type;

// ---- why something is not translated ----------------------------------------------------------------------------------

// The classes of infer::IssueClass, and two the lowering adds: a function refused because a function it calls is
// (CallsRefused), and one the front end itself failed on (FrontendError).
enum class RefusalClass { Untyped, Conflict, IntRange, Dynamic, Stdlib, Extern, Syntax, Unresolved, CallsRefused, FrontendError };
std::string_view refusal_class_word(RefusalClass cls);                   // "untyped", "int-range", "calls-refused" ...
std::optional<RefusalClass> refusal_class_from_word(std::string_view word);
RefusalClass refusal_class_of(infer::IssueClass cls);

struct Refusal {
    RefusalClass cls = RefusalClass::Untyped;
    std::string detail;
    std::uint32_t line = 0;
    std::string reason() const;   // "class" or "class:detail"
};

// Thrown inside the lowering of one function wherever it finds it cannot go on; caught where the function is lowered
// (lower_functions, the one isolation boundary), which keeps the Refusal and moves to the next function.
class Refused : public Error {
public:
    Refused(RefusalClass cls, std::string detail = {}, std::uint32_t line = 0);
    explicit Refused(const infer::Issue& issue);
    const Refusal& refusal() const { return refusal_; }
    CB_ERROR_BOILERPLATE(Refused)

private:
    Refusal refusal_;
};

// ---- what every container site of the program is in C++ ---------------------------------------------------------------------

// A record's field: the key, what it holds, and whether every creation of the record sets it.
struct RecordField {
    std::string key;
    Type type;
    bool required = false;
};

// A record struct (named), a dict, a list or a set: what each container site of the program is. A site that is a record
// (a dict whose every use names a constant string key) gets a struct named after what it holds and the function that makes
// it, declared in the module of that function.
class Records {
public:
    explicit Records(Program& program);

    bool has(SiteId root) const { return names_.contains(root); }
    const std::string& name(SiteId root) const { return names_.at(root); }               // the struct's name
    const std::string& module(SiteId root) const { return modules_.at(root); }           // the module it is declared in
    const std::optional<std::string>& owner_function(SiteId root) const { return owners_.at(root); }   // the function whose site created it
    SiteId home(SiteId root) const { return homes_.at(root); }                           // the site that makes it
    // The fields in the order the record's own site writes them, then the others.
    std::vector<RecordField> fields(SiteId root);
    // Whether a record is held by reference: it is changed after it is built.
    bool is_ref(SiteId root);
    // Every record, by root.
    const std::map<SiteId, std::string>& names() const { return names_; }

private:
    Program& p_;
    std::map<SiteId, std::string> names_;
    std::map<SiteId, std::optional<std::string>> owners_;
    std::map<SiteId, std::string> modules_;
    std::map<SiteId, SiteId> homes_;
};

// ---- what lowering the program shares -------------------------------------------------------------------------------------------

// Its records, the types written so far, the policy for integers, and the arena the trees are made in (a tree lives as long
// as the Context does).
class Context {
public:
    explicit Context(Program& program, const infer::Library& library = infer::unmapped_library(),
                     program::IntPolicy int_policy = program::IntPolicy::Strict);
    Context(const Context&) = delete;
    Context& operator=(const Context&) = delete;

    Program& program() { return p_; }
    const infer::Library& library() const { return library_; }
    program::IntPolicy int_policy() const { return int_policy_; }
    Records& records() { return records_; }
    Arena& arena() { return arena_; }
    // The records whose struct some type written so far names.
    const std::set<SiteId>& used_records() const { return used_records_; }

    // "module.Name" of a record.
    std::string qualified_record(SiteId root);
    // An inference type as an L2 type; Refused when it has none. `what` says what the type is of ("value", "parameter x"),
    // for the reason.
    ir::TypeP type_of(const Type& t, std::uint32_t line = 0, std::string_view what = "value");

private:
    Program& p_;
    const infer::Library& library_;
    program::IntPolicy int_policy_;
    Records records_;
    Arena arena_;
    std::set<SiteId> used_records_;
};

// ---- one function ---------------------------------------------------------------------------------------------------------------

class Lowerer;

// One function of the program into an L2 MethodDecl; or a module's top level, whose names' values are lowered one by one.
class FnLower {
public:
    // `evaluation` is the recording pass of the function (infer::Eval run with record = true), and must outlive this.
    FnLower(Context& context, const Fn& function, infer::Eval& evaluation);
    // A module's top level seen as a function: its names' values are lowered with value_of.
    FnLower(Context& context, const Module& module, infer::Eval& evaluation);
    ~FnLower();
    FnLower(const FnLower&) = delete;
    FnLower& operator=(const FnLower&) = delete;

    // The function as a MethodDecl in the context's arena. Refused when it cannot be translated.
    ir::Node* lower();
    // A value of the module (or function) as an expression of type `want` (an optional unwrapped or wrapped, None typed).
    // Refused; after it, has_pending() says whether the expression needed statements before it.
    ir::Node* value_of(const syntax::Expr* expression, const ir::TypeP& want);
    // Statements the last expression needed before its own statement (a comprehension built into a temporary).
    bool has_pending() const;
    // Keeps the evaluation this lowers alive as long as the lowering is (lower_functions makes both).
    void keep(std::shared_ptr<infer::Eval> evaluation) { kept_ = std::move(evaluation); }

    // What lowering found it needs: the module-level names it reads (module, name), the program's functions it calls, the classes
    // it constructs.
    const std::set<std::pair<std::string, std::string>>& globals_used() const;
    const std::set<std::string>& calls() const;
    const std::set<std::string>& classes_made() const;

private:
    std::unique_ptr<Lowerer> impl_;
    std::shared_ptr<infer::Eval> kept_;
};

// The outcome of lowering one function: its tree, or why it was refused; and the lowering that did it (null when the front end
// could not even run the function).
struct Lowered {
    ir::Node* decl = nullptr;                 // the MethodDecl; null when refused
    std::optional<Refusal> refused;
    std::shared_ptr<FnLower> lowering;
    bool is_refused() const { return refused.has_value(); }
};

// {qualified name: Lowered} for the given functions, in the order given. A function that cannot be run, or that the front end
// fails on, is refused as frontend-error; nothing is thrown for one function.
OrderedMap<std::string, Lowered> lower_functions(Context& context, std::span<Fn* const> functions);

// A function that calls (or constructs through) a refused function is refused too, to a fixpoint.
void close_refusals(OrderedMap<std::string, Lowered>& lowered);

// One module: its L2 unit, what was translated, what was refused and why.
struct ModuleResult {
    std::string name;
    ir::Node* unit = nullptr;
    OrderedMap<std::string, ir::Node*> translated;                      // function qname -> MethodDecl
    OrderedMap<std::string, Refusal> refused;                           // function qname -> why
    OrderedMap<std::string, std::set<std::string>> calls;               // function qname -> the functions it calls
    OrderedMap<std::string, std::set<std::string>> made;                // function qname -> the classes it constructs
    OrderedMap<std::string, std::set<std::pair<std::string, std::string>>> globals;   // function qname -> {(module, name)}
    std::vector<ir::Node*> global_decls;                                // the module's names the translation uses
    OrderedMap<std::string, std::size_t> lines;                         // function qname -> lines
};

}  // namespace cb::translate::frontends::python::lower
