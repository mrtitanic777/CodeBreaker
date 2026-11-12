// cb/translate/frontends/python/package.hpp -- the Python front end: CPython's syntax tree into L2, typed by inference,
// refusing what it cannot type.
//
//     auto program = package::program_of_tree("codebreaker", "codebreaker");   // every module of a package
//     program->solve(inference);                                              // types, to a fixpoint (infer.hpp)
//     package::Translation done = package::translate(*program, {.modules = {{"codebreaker.formats.ne"}}});
//     const lower::ModuleResult& r = *done.find("codebreaker.formats.ne");
//     r.unit                       an ir::Unit: records, classes, the module's functions and names
//     r.translated / r.refused     {function: MethodDecl} / {function: Refusal (class, detail, line)}
//
// A function is lowered by lower.hpp; this module takes what lowering found the program needs and makes the units: the
// module-level names the functions read (each lowered as its module's top level, or refused with it), the records and classes
// every translated function and every such name uses -- and, to a fixpoint, the types of those records' fields and classes'
// attributes -- and then, per module, the declarations: its records, its classes (attributes in the order the methods first
// assign them, `__init__` first), its names and functions. A function that reads a module name that could not be lowered is
// refused with it (calls-refused). The units of the modules a unit names are made too.
//
// docs/PYTHON_FRONTEND.md says how types are found and what is refused.
#pragma once

#include <filesystem>
#include <memory>
#include <optional>
#include <set>
#include <string>
#include <utility>
#include <vector>

#include "cb/core/ordered_map.hpp"
#include "cb/translate/frontends/python/infer.hpp"
#include "cb/translate/frontends/python/lower.hpp"
#include "cb/translate/frontends/python/program.hpp"

namespace cb::translate::frontends::python::package {

// Every module of the package folder `root` (its package name `package`), indexed. A file that does not parse is skipped.
std::unique_ptr<program::Program> program_of_tree(const std::filesystem::path& root, const std::string& package);
// {module name: source text}, in order, indexed (tests).
std::unique_ptr<program::Program> program_of_sources(const std::vector<std::pair<std::string, std::string>>& sources);

struct Options {
    std::optional<std::set<std::string>> modules;     // the modules to translate (nullopt: all); the modules their units name follow
    std::optional<std::set<std::string>> functions;   // the functions to lower, by qualified name (nullopt: all)
    program::IntPolicy int_policy = program::IntPolicy::Strict;
    const infer::Library* library = nullptr;          // the types of the builtins and the library (nullptr: methods::library())
};

// What translating gave: the modules' results, in the order they were made. The trees are in the arena of the context
// this keeps, so they live as long as it does.
class Translation {
public:
    Translation() = default;
    Translation(Translation&&) noexcept = default;
    Translation& operator=(Translation&&) noexcept = default;

    const OrderedMap<std::string, lower::ModuleResult>& modules() const { return modules_; }
    // The module's result, or nullptr when it was not translated.
    const lower::ModuleResult* find(const std::string& module) const { return modules_.find(module); }
    lower::Context& context() { return *context_; }

private:
    friend Translation translate(program::Program& program, const Options& options);
    std::unique_ptr<lower::Context> context_;
    OrderedMap<std::string, lower::ModuleResult> modules_;
};

// Lower the program's functions (those of `options.functions`, or all) and make the units of `options.modules` (or all).
// The program has been solved (infer::Inference run on it with the same library).
Translation translate(program::Program& program, const Options& options = {});

// The names of the program's own types an L2 type mentions (those not owned by "py"), down through its arguments.
std::set<std::string> types_of(const ir::TypeP& type);
// The same over everything a tree holds: the types of its nodes, locals and parameters, and the result type.
std::set<std::string> types_in(ir::Node* method);

}  // namespace cb::translate::frontends::python::package
