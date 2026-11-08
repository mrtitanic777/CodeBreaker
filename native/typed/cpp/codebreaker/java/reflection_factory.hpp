// codebreaker/java/reflection_factory.hpp -- typed C++ port of codebreaker.java.reflection_factory.
//
// reflection_factory.py -- turn Java reflection registries into real C++ factories.
//
// Java code registers a class table and then instantiates by id via reflection:
//
//     addMapping(EntityPig.class, "Pig", 90);              // register
//     (Entity) cls.getConstructor(World.class).newInstance(world);   // reflect-construct
//
// C++ has no `newInstance`, so the converter stubs it to NULL (the classic TileEntity-load /
// Packet-parse / entity-spawn crash). But the registration table lists EVERY concrete class, so the
// factory is fully recoverable: this pass reads the `addMapping(j2xClassOf(typeid(X)), ...)` /
// `addIdClassMapping(..., j2xClassOf(typeid(X)))` table out of the emitted .cpp, generates a
// `<Registry>_createByReflection(Class* c[, Arg* a0 ...])` that switches the Class* to a real
// `new X(args)`, and rewrites the reflection call site to invoke it. NULL stub -> working factory.
//
// Runs as a post-emit pass over the converted tree (also `codebreaker java reflect-factories <dir>`), and is wired
// into `convert()` so every port gets it automatically.
//
// Notes on the port:
//   * the Python's dicts with fixed keys are structs: Factory (registry/ret/args/classes, from process),
//     FactoryRef (name/ret/args, the shared factories), RegistryResult and Reused (run's report);
//   * `process` returns (text, sites, factory-or-None) as a tuple with std::optional<Factory>;
//   * `_REG_LINE` is a module variable that run() reassigns, so it is a plain mutable Pattern;
//   * `registrars=None` is std::nullopt (an empty list counts as None too, as `if registrars` does).
#pragma once

#include <cstdint>
#include <optional>
#include <tuple>

#include "py.hpp"
#include "py_os.hpp"

namespace codebreaker::java::reflection_factory {

// a receiver lvalue: var, var->field, this->field, var[i], var[i]->field, ...
extern const py::str _RECV;

// a reflection instantiation with a ctor arg (EntityList / SpawnerAnimals / MapStorage):
//   (Entity*)recv->getConstructor((JArrayInit<Class* >(), j2xClassOf(typeid(World)))).newInstance((JArrayInit<JObject >(), var1))
extern const py::re::Pattern _WORLD_ARG;

// a plain no-arg reflection instantiation (TileEntity / Packet / BlockSign):  (TileEntity*)recv->newInstance()
extern const py::re::Pattern _PLAIN;

// X.class -> j2xClassOf(typeid(X)) (or the older Class(typeid(X))); the dynamic getClass() form
// ...(typeid(*(var))) is excluded by \w+. The generated factory compares `*c == Class(typeid(X))`
// (a runtime Class value), which is correct whichever spelling the registration table used.
extern const py::re::Pattern _TYPEID;
// registration call lines that carry the concrete class table
extern py::re::Pattern _REG_LINE;      // set per run from the project's registrars

// the factory process() generated for one registry file
struct Factory {
    py::str registry;
    py::str ret;
    py::list<py::str> args;
    py::list<py::str> classes;
};

// a factory known by name (generated now or by a prior run)
struct FactoryRef {
    py::str name;
    py::str ret;
    py::list<py::str> args;
};

// one registry file's entry in run()'s report (the keys of {'file','sites', **factory})
struct RegistryResult {
    py::str file;
    int64_t sites;
    py::str registry;
    py::str ret;
    py::list<py::str> args;
    py::list<py::str> classes;
};

struct Reused {
    py::str file;
    int64_t sites;
};

struct RunResult {
    py::list<RegistryResult> registries;
    py::list<Reused> reused;
    int64_t total_sites;
    int64_t total_classes;
};

// Split a comma-separated arg blob at top-level commas (respecting nested parens/angles).
py::list<py::str> _split_top(const py::str& s);

// Concrete classes to switch over: every typeid(X) on a registration line, minus the ctor arg
// types (e.g. World) and dynamic `typeid(*(...))` lookups (which the \w+ pattern already excludes).
py::list<py::str> _concrete_classes(const py::str& text, const py::set<py::str>& arg_types);

// Drop `// J2X-REVIEW[reflection-unsupported]` lines whose next code line now calls a generated
// factory -- the concern is resolved, so the greppable tag should go.
py::str _strip_resolved_comments(const py::str& text);

// Rewrite reflection sites in `text` (one .cpp, `registry` = its class name) to a generated
// factory. Returns (new_text, sites_rewritten, factory_or_None).
std::tuple<py::str, int64_t, std::optional<Factory>> process(const py::str& text, const py::str& registry);

// Pick the factory whose ctor-arg signature matches this site and whose return type is `cast`
// (or a base of it -- e.g. EntityList's Entity* factory serving an (EntityLiving*) site).
std::optional<FactoryRef> _find_factory(const py::list<FactoryRef>& factories, const py::list<py::str>& arg_types,
                                        const py::str& cast);

// Rewrite reflection sites in a NON-registry file using the shared factories. Returns (text, n).
std::tuple<py::str, int64_t> _reuse(const py::str& text, const py::list<FactoryRef>& factories);

void _write_header(const py::str& out_dir, const py::list<FactoryRef>& factories);

py::str _add_include(const py::str& text);

// Two passes: (1) generate a factory for each registry class from its registration table;
// (2) rewrite reflection sites in OTHER files to reuse those factories via ReflectionFactory.h.
RunResult run(const py::str& out_dir, const std::optional<py::list<py::str>>& registrars = std::nullopt);

}  // namespace codebreaker::java::reflection_factory
