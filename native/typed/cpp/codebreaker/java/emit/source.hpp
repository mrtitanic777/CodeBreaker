// codebreaker/java/emit/source.hpp -- typed C++ port of codebreaker.java.emit.source.
//
// source.py -- render a class's .cpp from IR + a TypeResolver.
//
// This is where the console-crash classes are prevented BY CONSTRUCTION:
//   * static/array allocations get value-init `new T[n]()`  (Java zero-fills; C++ doesn't)
//   * every constructor zero-inits its instance fields in the member-init list
//     (no uninitialized members)
//   * a class that allocates heap in a constructor gets a destructor that frees it
//     (Rule-of-Three leak fix)
//   * `this(...)` constructor delegation is inlined by the back end, never silently dropped (the
//     Block blockID=0 / BlockContainer flag bugs)
//
// Every body, constructor and initialiser is written from the typed IR by the translator's back end
// (codebreaker/translate/backends/cpp03.py); what it declines is an explicit, ledgered J2X-GAP.
//
// Notes on the port:
//   * cls is the typed ir::JClass (a py::ref), resolver the typed types::TypeResolver, ledger a
//     ledger::Ledger (a pointer: null is the Python's `ledger=None`);
//   * cpp_std is codebreaker.java.transpile.CppStandard (no typed port of that module): the record
//     header::CppStd (year and label) stands in for it, as in the header port;
//   * the Python reads `getattr(resolver, 'two_phase', ())` -- an attribute the pipeline sets on the
//     resolver: it is the `two_phase` parameter of render_source (set of class names built in two phases;
//     absent -> none);
//   * TREE_BODIES is the module global the transpiler sets to a translate.backends.cpp03.Bodies; that
//     back end has no typed port yet, so `Bodies` is the interface source.py calls on it (the methods and
//     results of cpp03.Bodies), and TREE_BODIES a pointer to one (null is None).
#pragma once

#include <cstdint>
#include <optional>
#include <tuple>

#include "codebreaker/java/emit/header.hpp"
#include "codebreaker/java/ir.hpp"
#include "codebreaker/java/ledger.hpp"
#include "codebreaker/java/types.hpp"
#include "py.hpp"
#include "py_os.hpp"

namespace codebreaker::java::emit::source {

using ClassRef = py::ref<ir::JClass>;
using MethodRef = py::ref<ir::Method>;
using FieldRef = py::ref<ir::Field>;
using JTypeRef = py::ref<ir::JType>;
using types::TypeResolver;
using header::CppStd;

// (kind 'super'/'this'/None, its arguments or None, body lines) of a constructor.
struct CtorParts {
    std::optional<py::str> kind;
    std::optional<py::str> args;
    py::list<py::str> body;
};

// The translator's back end (translate.backends.cpp03.Bodies): every body, constructor and
// initialiser is written from the typed IR. Each lookup returns nullopt when it declined.
class Bodies {
  public:
    virtual ~Bodies() = default;
    // The C++ initialiser expression of field `name`.
    virtual std::optional<py::str> field_init(const py::str& cls_name, const py::str& name) = 0;
    // The C++ elements of static field `name`'s `new T[]{...}` initialiser.
    virtual std::optional<py::list<py::str>> array_items(const py::str& cls_name, const py::str& name) = 0;
    // `this->f = init;` for each instance field initialiser and the instance {} blocks, in source order.
    virtual std::optional<py::list<py::str>> instance_inits(const py::str& cls_name) = 0;
    // The constructor `old` (null: the implicit one) of class `cls_name`.
    virtual std::optional<CtorParts> ctor(const py::str& cls_name, const MethodRef& old, bool two_phase = false) = 0;
    // The C++ arguments of enum constant `const_`'s constructor call.
    virtual std::optional<py::list<py::str>> enum_args(const py::str& cls_name, const py::str& const_) = 0;
    // Each static {} block's lines, in source order.
    virtual std::optional<py::list<py::list<py::str>>> static_blocks(const py::str& cls_name) = 0;
    // Body lines for the method `old` of class `cls_name`.
    virtual std::optional<py::list<py::str>> body(const py::str& cls_name, const MethodRef& old) = 0;
};

// Set by the transpiler for the conversion; null: no back end (everything is a J2X-GAP).
extern Bodies* TREE_BODIES;

// Multi-dimensional aggregate `new T[][]{...}` / `new T[][][]{...}` (2+ empty `[]` dims then `{`).
// aggregate_array_body() intentionally handles only the 1-D `new T[]{...}` file-static-data form;
// nested aggregates can't be a flat C array, so they build a nested JArrayInit instead (C++03 has no
// braced new[][]). One `[]` is NOT matched here -- that stays the ownership.aggregate_array_body path.
extern const py::re::Pattern _NEW_ARRAY_AGG_MULTI;
// zero-initialised in every constructor's member-init list, as Java zeroes every field. FIXED BUG
// byte-fields-uninitialised: 'signed char' (Java byte) was missing when byte stopped being plain char, so
// every byte field -- packet types, yaw/pitch, NBT tags -- held garbage until assigned (the behavioural
// judge: Packet24MobSpawn.type -64, Packet34EntityTeleport.yaw -72 on a fresh packet).
extern const py::set<py::str> _PRIMITIVE_CPP;

py::str _banner(const py::str& cls_name, const std::optional<CppStd>& cpp_std = std::nullopt,
                const std::optional<py::str>& platform_label = std::nullopt);

// The zero/null literal for a member-init, or None for value types (default ctor).
std::optional<py::str> _default_value(const py::str& cpp_type);

// ---------------------------------------------------------------------------
// static field definitions
// ---------------------------------------------------------------------------
// The C++ element spelling for a T[] field, i.e. the JArray<...> template argument. Must MATCH
// types.cpp_type's array handling exactly, or the aggregate data array (`static elem _data[]`) and
// the JArray<elem> field type disagree -- e.g. byte[] is JArray<unsigned char> (a buffer) even though
// a SCALAR byte is signed char, so the element here must be unsigned char too.
py::str _array_elem_cpp(const JTypeRef& jtype, const TypeResolver& resolver);

// J2X-BUGFIX 2026-08-13 static-init-order: a FUNCTION-CALL static field initializer (e.g. StatList's
// `s_mineBlockStatArray = func_25153_a(...)`) leaked out as an eager file-scope global ctor. Global ctors
// run in UNDEFINED cross-TU order BEFORE main()/j2x_initAll(), so func_25153_a() -> Block::s_waterStill->
// blockID derefed a NULL Block static -> silent crash before main() (black screen, no boot markers). Fix:
// also defer lowercase-initial function-call initializers into the ordered initStatics(). See below.
//
// True if a static initializer must run AFTER zero-init, in CONTROLLED order -- so that cross-
// translation-unit global-ctor order (which C++ leaves UNDEFINED) can't crash the boot before main().
// Deferred inits move into the ordered initStatics(); the field def is zero-initialized. Compile-time
// constants and `const` members (can't be reassigned) stay eager.
//
// Order-sensitive when the initializer:
//   - constructs an object (`new`) or reads another class's statics (`::` / `->`); OR
//   - CALLS a free/static function -- a lowercase-initial identifier (incl. obfuscated `func_NNNNN`)
//     immediately before `(`. Such a call runs arbitrary code at static-init time that may touch other
//     classes' not-yet-constructed statics. The real bug this catches: `StatList::s_mineBlockStatArray
//     = func_25153_a("stat.mineBlock", 16777216)` -- func_25153_a() indexes Block::s_blocksList, whose
//     TU may not have run its global ctors yet -> null deref BEFORE main() (silent black screen).
// Constructor calls (PascalCase type, e.g. `JArray<T>(N)`, `JavaString(...)`) only allocate and are
// order-independent, so they stay eager.
bool _is_deferrable_static_init(const py::str& init, const py::str& ft);

// Returns (def_lines, init_lines): the static field DEFINITIONS (zero-init for deferrable ones)
// and the DEFERRED initializer statements that initStatics() must run in dependency order.
std::tuple<py::list<py::str>, py::list<py::str>> _render_statics(const ClassRef& cls, const TypeResolver& resolver,
                                                                  ledger::Ledger* ledger);

// ---------------------------------------------------------------------------
// constructors
// ---------------------------------------------------------------------------
py::list<py::str> _init_list(const ClassRef& cls, const TypeResolver& resolver);

// What the IR's back end could not write: said in the code and the ledger, and thrown if it runs
// -- never a guess.
py::list<py::str> _gap(const ClassRef& cls, ledger::Ledger* ledger, const py::str& what, int64_t line = 0);

// C++ for Java INSTANCE field initializers + instance `{ }` blocks, in declaration order.
// Java runs these at the top of every constructor that does NOT delegate via this(...). Dropping
// them was the null-deref / uninitialized-field crash class (e.g. EntityPlayer.inventory).
py::list<py::str> _instance_init_lines(const ClassRef& cls, const TypeResolver& resolver, ledger::Ledger* ledger);

py::list<py::str> _render_ctor(const ClassRef& cls, const MethodRef& m, const TypeResolver& resolver,
                               ledger::Ledger* ledger);

// A two-phase class (see twophase.py): `C::C()` zeroes the fields, and each Java constructor becomes
// `C* C::j2xInit(params)` -- the parent's j2xInit (Java's super(...)), the instance initialisers, the
// body, `return this`. this(args) is a call to the other j2xInit, which does the parent and initialisers.
// (`two_phase` is resolver.two_phase.)
py::list<py::str> _render_two_phase(const ClassRef& cls, const TypeResolver& resolver, ledger::Ledger* ledger,
                                    const py::set<py::str>& two_phase);

// ---------------------------------------------------------------------------
// destructor (Rule of Three: free heap allocated in constructors)
// ---------------------------------------------------------------------------
py::list<py::str> _render_dtor(const ClassRef& cls, const TypeResolver& resolver, ledger::Ledger* ledger);

// ---------------------------------------------------------------------------
// methods
// ---------------------------------------------------------------------------
py::list<py::str> _render_method(const ClassRef& cls, const MethodRef& m, const TypeResolver& resolver,
                                 ledger::Ledger* ledger);

// A synthesized MI-diamond forwarding override: `R Cls::m(args) { [return] Base::m(args); }`
// so an interface pure-virtual already implemented by a base class is satisfied here too.
py::list<py::str> _render_mi_forward(const ClassRef& cls, const MethodRef& im, const py::str& base,
                                     const TypeResolver& resolver, ledger::Ledger* ledger);

// All known user-type names this class references (for full #includes in the .cpp).
py::list<py::str> _referenced_types(const ClassRef& cls, const TypeResolver& resolver, const py::str& body_text);

// Emit the .cpp for a typed enum: static const/instance defs, the ctor(s)/dtor/methods via the
// normal renderers, and an initStatics() that builds the singleton instance table (in ctor-body
// dependency order via j2x_initAll), assigns ordinals, and fills values().
py::str _render_typed_enum_source(const ClassRef& cls, const TypeResolver& resolver, ledger::Ledger* ledger,
                                  const std::optional<CppStd>& cpp_std, const std::optional<py::str>& platform_label);

// J2X-BUGFIX 2026-08-13 static-init-cycle: Java initializes a class LAZILY on first static access, so a
// self-singleton getter called mid-init (StatList::func_25157_c -> CraftingManager::getInstance(), whose
// instance is built in a LATER-ordered initStatics()) returns NULL and crashes. A linear j2x_initAll can't
// reproduce lazy init for a true cycle. Emulate it: a `X::s_foo = new X(...)` singleton makes its pure
// getter (`return s_foo;`) build on demand, and initStatics() becomes idempotent (the driver may also call
// it). Detects the singleton field from the deferred init lines.
std::optional<py::str> _detect_self_singleton(const py::str& clsname, const py::list<py::str>& static_init_lines);

// If a rendered method is a PURE getter that returns the singleton field, prepend a lazy-init guard so
// first access builds it (Java's <clinit>-on-static-access). Only touches single-statement `return field;`
// bodies -- never a method that does other work.
py::list<py::str> _inject_lazy_singleton(py::list<py::str> mlines, const py::str& clsname, const py::str& field);

// The class's static {} blocks, in source order.
py::list<py::str> _static_block_lines(const ClassRef& cls, ledger::Ledger* ledger);

// Returns the .cpp text, or None for interfaces / constant-only enums (header-only).
std::optional<py::str> render_source(const ClassRef& cls, const TypeResolver& resolver, ledger::Ledger* ledger = nullptr,
                                     const std::optional<CppStd>& cpp_std = std::nullopt,
                                     const std::optional<py::str>& platform_label = std::nullopt,
                                     const std::optional<py::set<py::str>>& two_phase = std::nullopt);

}  // namespace codebreaker::java::emit::source
