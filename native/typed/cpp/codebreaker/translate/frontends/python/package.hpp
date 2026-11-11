// codebreaker/translate/frontends/python/package.hpp -- typed C++ from codebreaker/translate/frontends/python/__init__.py.
//
// The Python front end: CPython's `ast` into L2, typed by inference, refusing what it cannot type.
//
//     auto prog = pyfe::program_of_tree("codebreaker", "codebreaker");   // every module of a package
//     prog->solve();                                                      // types, to a fixpoint
//     auto results = pyfe::translate(*prog, lowering, {{"codebreaker.formats.ne"}});   // {module: ModuleResult}
//     auto r = results.at("codebreaker.formats.ne");
//     r->unit                      an ir.Unit: records, classes, the module's functions and names
//     r->translated / r->refused   {function: MethodDecl} / {function: Issue (class, detail, line)}
//
//     auto [header, source] = cpp17::write_unit(r->unit);
//
// docs/PYTHON_FRONTEND.md says how types are found and what is refused.
//
// Typing and runtime notes:
//   * The lowering of the functions (codebreaker.translate.frontends.python.lower: Context, FnLower, Refused, ModuleResult,
//     lower_functions, close_refusals, lit) has no typed port yet. What this module takes from it is the abstract class
//     `Lowering` below (its Context, Records and FnLower are the classes of the same names), the exception `Refused`
//     (with its `issue`) and the struct `ModuleResult`; a lowering is passed to translate(). Where the Python holds
//     (MethodDecl or Refused, FnLower or None) of one function, `Lowered` does.
//   * `ctx.ty(t, line, what)` raises Refused for a type that has no C++ type; `t` may be None (attr_type's answer for an
//     attribute that is not there): std::optional<T>.
//   * The program, its modules, classes and functions are the typed ports in program.hpp (Program is not copyable: the
//     program_of_* functions return a std::unique_ptr). ast.Constant and ast.walk are pyfront's / pyengine's.
//   * "Qualified names the translations use" are py::set<py::str>; Python's set iteration order is not relied on (the
//     results are closures and membership tests).
#pragma once

#include <cstdint>
#include <memory>
#include <optional>
#include <tuple>
#include <variant>

#include "codebreaker/translate/frontends/python/infer.hpp"
#include "codebreaker/translate/frontends/python/program.hpp"
#include "codebreaker/translate/frontends/python/pytypes.hpp"
#include "codebreaker/translate/ir.hpp"
#include "py.hpp"

namespace codebreaker::translate::frontends::python::pyfe {

namespace P = pytypes;
namespace Pr = program;
namespace ir = codebreaker::translate::ir;
namespace A = pyfront;

// L.Issue: why a function (or name, or type) is not translated: (class, detail, line), and its one-line `reason`
struct Issue {
    py::str klass, detail;
    int64_t line = 0;
    py::str reason;
};

// L.Refused: raised by the lowering (and returned in place of a MethodDecl)
struct Refused : py::Exception {
    Issue issue;
    explicit Refused(const Issue& i) : py::Exception(i.reason.utf8()), issue(i) {}
    const char* type() const override { return "Refused"; }
};

// ctx.records: the records (dict displays with constant keys) the program uses, by root
class Records {
  public:
    virtual ~Records() = default;
    virtual py::dict<int64_t, py::str> names() = 0;                                   // root -> its name
    virtual py::str module(int64_t root) = 0;                                         // the module it is declared in
    virtual py::list<std::tuple<py::str, P::T, bool>> fields(int64_t root) = 0;       // (key, type, required)
    virtual bool is_ref(int64_t root) = 0;
};

// L.Context
class Context {
  public:
    virtual ~Context() = default;
    virtual Records& records() = 0;
    // the L2 type of an inference type; Refused when it has none
    virtual ir::TypeP ty(const std::optional<P::T>& t, int64_t line, const py::str& what = py::str("")) = 0;
};

// L.FnLower: one function's lowering (and the lowering of a module name's value)
class FnLower {
  public:
    virtual ~FnLower() = default;
    py::set<std::tuple<py::str, py::str>> globals_used;      // the module names it reads: (module, name)
    py::list<py::str> calls;                                 // what it calls
    virtual ir::NodeP expr(const A::Expr* node) = 0;
    virtual ir::NodeP coerce(const ir::NodeP& value, const ir::TypeP& t) = 0;
    virtual bool has_pre() = 0;                              // `if fl.pre:` -- it needs statements before the value
};

// a module's top level, seen as a function (to lower its names' values)
struct ModFn {
    py::str qname;
    Pr::Module* module = nullptr;
    Pr::Class* cls = nullptr;
    py::str kind = py::str("function");
    std::vector<py::str> params, kwonly;
    py::str name = py::str("<module>");
    const A::Module* node = nullptr;
    explicit ModFn(Pr::Module& m);
};

// (the lowering of one function, if any: a MethodDecl or Refused) and its FnLower (None: not lowered)
struct Lowered {
    std::variant<py::ref<ir::MethodDecl>, Refused> m;
    std::shared_ptr<FnLower> fl;
};
using LoweredMap = py::dict<py::str, Lowered>;

// L: the lowering
class Lowering {
  public:
    virtual ~Lowering() = default;
    virtual std::shared_ptr<Context> context(Pr::Program& prog, const py::str& int_policy) = 0;       // L.Context(prog, policy)
    virtual LoweredMap lower_functions(Pr::Program& prog, Context& ctx, const std::vector<Pr::Fn*>& todo) = 0;
    virtual std::shared_ptr<FnLower> fn_lower(Context& ctx, const ModFn& fn, infer::Eval& module_eval) = 0;   // L.FnLower(ctx, fn, eval)
    virtual void close_refusals(Pr::Program& prog, LoweredMap& lowered) = 0;
    virtual Issue issue(const py::str& klass, const py::str& detail, int64_t line = 0) = 0;           // L.Refused(klass, detail, line).issue
    virtual ir::NodeP lit(const A::ConstValue& value, const ir::TypeP& t) = 0;                          // L.lit
};

// L.ModuleResult
struct ModuleResult {
    py::str module;
    py::dict<py::str, int64_t> lines;                        // function -> its number of lines
    py::dict<py::str, Issue> refused;
    py::dict<py::str, py::ref<ir::MethodDecl>> translated;
    py::dict<py::str, py::list<py::str>> calls;
    py::ref<ir::Unit> unit;
    explicit ModuleResult(const py::str& module) : module(module) {}
};

std::unique_ptr<Pr::Program> program_of_tree(const py::str& root, const py::str& package);
// {module name: source text} -> an indexed Program (tests)
std::unique_ptr<Pr::Program> program_of_sources(const py::dict<py::str, py::str>& sources);

// Lower the program's functions (those of `modules`, or all) -> {module: ModuleResult}.
py::dict<py::str, std::shared_ptr<ModuleResult>> translate(Pr::Program& prog, Lowering& lowering,
                                                            const std::optional<py::set<py::str>>& modules = std::nullopt,
                                                            const py::str& int_policy = py::str("strict"),
                                                            const std::optional<py::set<py::str>>& fns = std::nullopt);

// the qualified names of the records and classes every translated function (and every module name they read) uses -- and
// the types of those records' fields and classes' attributes, to a fixpoint
py::set<py::str> used_types(Pr::Program& prog, Context& ctx, const LoweredMap& lowered,
                            const py::dict<std::tuple<py::str, py::str>, py::ref<ir::FieldDecl>>& global_decls);

std::shared_ptr<ModuleResult> build_unit(Pr::Program& prog, Context& ctx, Lowering& lowering, const py::str& mod, const LoweredMap& lowered,
                                         const py::dict<std::tuple<py::str, py::str>, py::ref<ir::FieldDecl>>& global_decls,
                                         const std::optional<py::set<py::str>>& used_all = std::nullopt);

// attributes in the order the class's methods first assign them (__init__ first)
int64_t _attr_order(Pr::Program& prog, Pr::Class& c, const py::str& attr);

void _types_of(const ir::TypeP& t, py::set<py::str>& out);
py::set<py::str> _types_of(const ir::TypeP& t);
py::set<py::str> _types_in(const py::ref<ir::MethodDecl>& md);

// a record whose type appears inside another used type (a field of a record that is used)
bool _any_ref(Pr::Program& prog, Context& ctx, int64_t s, const py::set<py::str>& used);

}  // namespace codebreaker::translate::frontends::python::pyfe
