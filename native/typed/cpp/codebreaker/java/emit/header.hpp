// codebreaker/java/emit/header.hpp -- typed C++ port of codebreaker.java.emit.header.
//
// header.py -- render a class's .h from IR + a TypeResolver.
//
// Decides includes vs forward declarations correctly:
//   * parent / interfaces  -> #include (need the full definition to inherit)
//   * value-type members / by-value params/returns -> #include (need full def)
//   * reference-type (pointer) members/params/returns -> forward `class X;`
//   * std-mapped types -> the right <string>/<vector>/<map>/<set> system header
//
// Every lossy mapping (Object -> void*, NIO buffers) is recorded on the ledger as
// an ANNOTATED event so the report shows it.
//
// Notes on the port:
//   * cls is the typed ir::JClass (a py::ref), resolver the typed types::TypeResolver, ledger a
//     ledger::Ledger (a pointer: null is the Python's `ledger=None`);
//   * cpp_std is codebreaker.java.transpile.CppStandard (no typed port of that module): only its `year` and
//     `label` are read, so a small CppStd record stands in for it (`std::optional`: None -> no standard);
//   * the Python reads `getattr(resolver, 'two_phase', ())` -- an attribute the pipeline sets on the resolver
//     (types.TypeResolver has no such member in the typed port): it is the `two_phase` parameter, the set of
//     class names built in two phases (twophase.compute); absent -> none.
#pragma once

#include <cstdint>
#include <optional>
#include <tuple>

#include "codebreaker/java/ir.hpp"
#include "codebreaker/java/ledger.hpp"
#include "codebreaker/java/types.hpp"
#include "py.hpp"

namespace codebreaker::java::emit::header {

using ClassRef = py::ref<ir::JClass>;
using MethodRef = py::ref<ir::Method>;
using FieldRef = py::ref<ir::Field>;
using JTypeRef = py::ref<ir::JType>;
using types::TypeResolver;

// What header.py reads of a transpile.CppStandard: its year and its label ('C++%02d' % (year % 100)).
struct CppStd {
    int64_t year = 0;
    py::str label;

    CppStd() = default;
    explicit CppStd(int64_t year_);   // label computed as CppStandard.__init__ does
    CppStd(int64_t year_, const py::str& label_) : year(year_), label(label_) {}
};

// The banner: '// Auto-generated from <cls_name>.java\n// Target: <platform> / <C++ standard>'.
py::str _banner(const py::str& cls_name, const std::optional<CppStd>& cpp_std = std::nullopt,
                const std::optional<py::str>& platform_label = std::nullopt);

// The #include operand (quoted or in angle brackets) for a std-mapped C++ type, or None.
// Object -> JObject (the universal boxed value type) is a name-changing mapping, so it lands here
// rather than as a user X.h. JObject lives in the runtime prelude; a header that takes/returns it
// BY VALUE (e.g. `equals(JObject)`) needs the full definition, so pull in JavaRT.h. Without this the
// header has neither an include nor a forward-decl for JObject and fails to compile standalone (the
// only reason it ever worked was the .cpp happening to #include JavaRT.h before the class header).
std::optional<py::str> _std_header_for(const py::str& cpp_base);

// C++ base-class spelling for a Java class that EXTENDS a container (e.g. `extends ArrayList`).
// ArrayList/List -> JList<void* > (JList.h is always included); HashMap/Map -> std::map<JObject,
// JObject>; HashSet/Set/TreeSet -> std::set<void* >. Non-container parents are returned unchanged.
py::str _base_type_spelling(const py::str& parent);

// (system_includes, quoted_includes, forward_decls)
using Dependencies = std::tuple<py::set<py::str>, py::set<py::str>, py::set<py::str>>;

// Return (system_includes, quoted_includes, forward_decls).
Dependencies _collect_dependencies(const ClassRef& cls, const TypeResolver& resolver);

py::str _method_decl(const ClassRef& cls, const MethodRef& m, const TypeResolver& resolver);

// C++ type for an instance field. Scalar Java `byte` -> `signed char` (Java-faithful; resolver
// handles it) and `byte[]` -> unsigned-char buffer -- see the signed-byte policy in typemap. The
// only override is the (empty by default) UNSIGNED_BYTE_SCALAR_FIELDS escape hatch.
py::str _field_type(const ClassRef& cls, const FieldRef& f, const TypeResolver& resolver);

// A TYPED enum (constants carry ctor args / the enum has instance fields or methods) becomes a
// real class: instance fields + ctor + methods are preserved, and each constant is a singleton
// `static EnumX* NAME` built in initStatics(). All statics use BARE names (no s_), because a typed
// enum stays in the holder set so `EnumX.NAME` resolves to `EnumX::NAME` (see types.py).
py::str _render_typed_enum_header(const ClassRef& cls, const TypeResolver& resolver, ledger::Ledger* ledger,
                                  const std::optional<CppStd>& cpp_std, const std::optional<py::str>& platform_label);

py::str render_header(const ClassRef& cls, const TypeResolver& resolver, ledger::Ledger* ledger = nullptr,
                      const std::optional<CppStd>& cpp_std = std::nullopt,
                      const std::optional<py::str>& platform_label = std::nullopt,
                      const std::optional<py::set<py::str>>& two_phase = std::nullopt);

void _report(const ClassRef& cls, const TypeResolver& resolver, ledger::Ledger* ledger);

}  // namespace codebreaker::java::emit::header
