// codebreaker/java/transpile.hpp -- typed C++ port of codebreaker.java.transpile.
//
// pipeline.py -- Module 5: the end-to-end converter.
//
//   discover .java -> parse -> build resolver -> emit .h/.cpp -> verify -> report
//
// This is the orchestrator that replaces the monolith's run_conversion(). It is deliberately thin: every
// real decision lives in the modules it calls (parse, types, emit), so this just sequences them and
// collects the ledger.
//
// The verify step runs the crash-class rules (rules.py) over the *emitted* output as an acceptance check --
// a correct generator should produce ~zero findings (arrays get `new T[n]()`, owners get dtor+disabled-copy,
// etc.), in contrast to the legacy output.
//
// Notes on the port:
//   * the profile registry, the project layer and the Minecraft project are the packages' own
//     (java/profiles, java/projects, java/projects/minecraft): convert() reads them through
//     profiles::active / use and projects::choose. What this module owns of them is the `profiles.on_change`
//     observer: switching the active profile reseeds the typed ports that hold profile data (typemap, rules,
//     console/vcxproj) and this module's static-init seed. (java.signatures has no port; its package-role
//     table is not reseeded.)
//   * the module's globals (_TRACE_THROWS, _GL_TO_D3D9, _THIS_ESCAPES, _PROJECT, _STATIC_INIT_ORDER) are
//     namespace-scope variables; convert() holds the one-at-a-time lock as the Python does.
//   * cpp_std is the CppStandard record below; the emitters take the header::CppStd (year, label) of it.
//   * the Java front end is translate::frontends::java (javac's TreeDump -> JSON trees); the Python's
//     `java_units` (cpp03) is `translate::backends::cpp03::java_units(java_dir, classpath)`, which convert() calls.
//   * convert()'s summary dict is the Summary record; `convert`'s `progress_cb` is a std::function of a str
//     (an empty function: no callback); `ledger` is a py::ref<Ledger> (null: a new one).
//   * Python's `Counter.most_common(8)` is spelled out where the progress line is written.
#pragma once

#include <cstdint>
#include <functional>
#include <optional>
#include <tuple>

#include "codebreaker/java/emit/header.hpp"
#include "codebreaker/java/ir.hpp"
#include "codebreaker/java/ledger.hpp"
#include "codebreaker/java/profiles/package.hpp"
#include "codebreaker/java/projects/package.hpp"
#include "codebreaker/java/types.hpp"
#include "codebreaker/translate/ir.hpp"
#include "py.hpp"
#include "py_os.hpp"

namespace codebreaker::java::transpile {

using ClassRef = py::ref<ir::JClass>;
using MethodRef = py::ref<ir::Method>;
using Progress = std::function<void(const py::str&)>;

// ---------------------------------------------------------------------------------------------- the module
// Set True by convert() for platforms where a C++ `throw` is a no-op that does not unwind (xbox360), so
// _finalize_emitted prefixes every emitted throw with a location tracer (see JavaRT.h).
extern bool _TRACE_THROWS;
// Set True by convert() for the Xbox 360: every OpenGL call in the converted game is converted to the native
// Direct3D 9 layer (namespace d3d9).
extern bool _GL_TO_D3D9;
// OpenGL API namespaces that the registry maps to the Direct3D 9 layer 'd3d9'.
extern py::list<py::str> _GL_NAMESPACES;
// classes whose construction hands `this` to someone (a registry, a list, a listener) -- set per convert
extern py::set<py::str> _THIS_ESCAPES;
// the project of this conversion, set per convert
extern std::shared_ptr<projects::Project> _PROJECT;

// Rewrite OpenGL call sites (GL11::glX, GLU::gluX, GL11::GL_CONST) in emitted game code to the Direct3D 9
// layer namespace 'd3d9'. Only the `Ns::` qualified form is touched.
py::str _convert_gl_to_d3d9(const py::str& text);

// Prefix every LINE-LEADING `throw` statement with a separate `J2X_GUEST_THROW();` statement so a fired
// throw names its file:line.
py::str _trace_guest_throws(const py::str& text);

// Global text finalizers applied to EVERY emitted .h/.cpp.
py::str _finalize_emitted(const py::str& text);

// All .java under java_dir, de-duplicated by basename (output is keyed by class name).
py::list<py::str> discover(const py::str& java_dir);

// Drop MCP decompiler artifacts (empty marker classes, synthetic bridge constructors). Returns the kept
// classes and the markers dropped.
std::tuple<py::list<ClassRef>, py::set<py::str>> drop_synthetic_artifacts(const py::list<ClassRef>& classes,
                                                                          ledger::Ledger* ledger = nullptr);

// Populate each class's inherited_fields by walking its parent chain across the full class set.
void resolve_inheritance(const py::list<ClassRef>& classes);

// MULTIPLE-INHERITANCE diamond fix: attaches `cls.mi_forwards`.
void compute_mi_forwards(const py::list<ClassRef>& classes);

// Flatten each class's fields (static + instance) and method return types over its parent chain.
py::dict<py::str, types::MemberEnv> build_member_env(const py::list<ClassRef>& classes);

// {class: set(method names that COLLIDE with a same-named field, over the parent chain)}.
py::dict<py::str, py::set<py::str>> build_method_renames(const py::list<ClassRef>& classes);

// Names of classes that are (transitively) a Thread subclass or Runnable.
py::set<py::str> compute_thread_like(const py::list<ClassRef>& classes);

extern const py::re::Pattern& _RETURN_NULL();
// Java collection mutators: calling one on a collection PARAM mutates the CALLER's collection.
extern const py::list<py::str> _COLL_MUTATORS;

// (arg_index, arg_text) for every call to `name(...)` in `body` whose top-level argument count == arity.
py::list<std::tuple<int64_t, py::str>> _iter_call_args(const py::str& body, const py::str& name, int64_t arity);

// WHOLE-PROGRAM pass: mark every collection-typed value param that its method mutates with Param.by_ref.
// Returns the number of params marked.
int64_t compute_collection_out_params(const py::list<ClassRef>& classes, const types::TypeResolver& resolver);

// DERIVE nullable getters from the corpus. Returns (getter_names, {getter: retType}).
std::tuple<py::set<py::str>, py::dict<py::str, py::str>> derive_nullability(const py::list<ClassRef>& classes,
                                                                          const types::TypeResolver& resolver);

// The classes whose constructor (or instance initialiser) uses `this` as a value, and every class below one.
py::set<py::str> compute_this_escapes(const py::list<ClassRef>& classes);

py::ref<types::TypeResolver> build_resolver(const py::list<ClassRef>& classes,
                                            const std::optional<py::set<py::str>>& value_types = std::nullopt);

// Lightweight descriptor for the target C++ standard revision.
struct CppStandard {
    int64_t year = 0;
    py::str label;
    explicit CppStandard(int64_t year_);
    bool has_nullptr() const { return year >= 2011; }
    bool has_auto() const { return year >= 2011; }
    bool has_range_for() const { return year >= 2011; }
    bool has_override() const { return year >= 2011; }
    bool has_initializer_list() const { return year >= 2011; }
    bool has_enum_class() const { return year >= 2011; }
    bool has_constexpr() const { return year >= 2011; }
    bool has_structured_bindings() const { return year >= 2017; }
    bool has_optional() const { return year >= 2017; }
    bool has_string_view() const { return year >= 2017; }
    emit::header::CppStd std() const { return emit::header::CppStd(year, label); }
};

extern const py::dict<py::str, int64_t> _STD_MAP;
CppStandard _parse_std_version(const py::str& label);

// Top-level symbols a translation unit DEFINES (class/struct names, the owner class of every `X::` method
// definition, free-function names).
py::set<py::str> _defined_symbols(const py::str& text);

// Fold pure FORWARDER headers into their targets. Returns the removed filenames.
py::list<py::str> _coalesce_forwarders(const py::str& out_dir, const Progress& prog);

// Full-product prune -- automatic dead-subsystem cut. Returns the dropped filenames.
py::list<py::str> _prune_unreferenced(const py::str& out_dir, const py::set<py::str>& game_classes, const Progress& prog,
                                      const py::list<py::str>& overlay_files = py::list<py::str>());

// What convert() answers (a dict in Python).
struct Summary {
    int64_t parsed = 0;
    int64_t headers = 0;
    int64_t sources = 0;
    py::list<py::str> markers;                               // empty decompiler marker classes, dropped on purpose
    std::tuple<int64_t, py::dict<py::str, int64_t>> tree_bodies;   // (written, declined by reason)
    int64_t rule_findings = 0;
    int64_t events = 0;
    int64_t high = 0;
    std::optional<py::str> report;
    py::ref<ledger::Ledger> ledger;
    py::str cpp_std;
    py::str platform;
    py::str profile;                                         // the target profile active DURING this conversion
};

// Convert a Java source tree to C++ (see _convert). The per-conversion module state -- the active edition
// profile and the derived nullable-getter sets -- is snapshotted here and restored when it ends, so a
// conversion that RAISES midway can't leak its profile into the next in-process convert. Conversions in one
// process run ONE AT A TIME.
Summary convert(const py::str& java_dir, const py::str& out_dir, const std::optional<py::set<py::str>>& value_types = std::nullopt,
                const py::ref<ledger::Ledger>& ledger = nullptr, bool write_report = true, const py::str& cpp_std = py::str("C++03"),
                const std::optional<py::str>& platform = std::nullopt, const Progress& progress_cb = nullptr,
                bool platform_refs = false, const std::optional<py::str>& sdk_override = std::nullopt, bool emit_debug = true,
                const std::optional<py::str>& overlay_dir = std::nullopt, const std::optional<py::str>& edition = std::nullopt,
                const std::optional<py::str>& mc_version = std::nullopt);

// The seed of the dependency order for static initialization (the active profile's).
extern py::list<py::str> _STATIC_INIT_ORDER;
// Re-derive the static-init seed when the active target profile changes.
void _reseed_static_init_order(const profiles::Profile& p);

// Order classes' initStatics() so a class runs AFTER every class whose statics it reads.
py::list<py::str> compute_static_init_order(const py::list<ClassRef>& classes, const py::set<py::str>& callable_init);

// Emit j2x_init.h / j2x_init.cpp / j2x_rtti.h / j2x_rtti.cpp -- the ordered static-init entry point.
void _write_init_driver(const py::str& out_dir, const py::list<py::str>& init_classes, const py::list<ClassRef>& classes);

void _write(const py::str& path, const py::str& text);
py::str _read(const py::str& path);

}  // namespace codebreaker::java::transpile
