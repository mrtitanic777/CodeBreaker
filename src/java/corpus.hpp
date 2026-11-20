// java/corpus.hpp -- the whole-program passes over the parsed Java classes of one conversion.
//
// Each class is parsed alone; what the emitter needs to know about a class often lives in its neighbours: an inherited
// field, a base class that implements an interface method, a collection that a callee fills in, which getters can answer
// null. These passes read the full class set and record their findings on it (the derived members of `ir::JClass`, the
// by-reference flag of a `Param`) or hand them back (the type resolver, the nullable getters).
//
// The classes are kept in one `std::vector<ir::JClass>`. The derived members point at fields and methods of OTHER classes
// of the same set, so the vector must not grow, shrink or reorder after `resolve_inheritance` and `compute_mi_forwards` have
// run; the pipeline runs them last, once parsing and `drop_synthetic_artifacts` are done.
//
//     auto classes = parse_all(...);
//     corpus::drop_synthetic_artifacts(classes, &ledger);
//     corpus::resolve_inheritance(classes);
//     corpus::compute_mi_forwards(classes);
//     types::TypeResolver resolver = corpus::build_resolver(classes, {.profile = &profile.type, .runtime = &runtime});
#pragma once

#include <cstddef>
#include <filesystem>
#include <optional>
#include <set>
#include <span>
#include <string>
#include <string_view>
#include <vector>

#include "java/ir.hpp"
#include "java/ledger.hpp"
#include "java/runtime.hpp"
#include "java/typemap.hpp"
#include "java/types.hpp"

namespace cb::java::corpus {

using typemap::NameSet;

// All .java files under `java_dir`, one per file name: output is keyed by class name, so a name met twice (two packages) keeps
// the first found. Directories are walked top-down, a directory's files before its sub-folders, each in name order; the
// result is sorted by file name.
std::vector<std::filesystem::path> discover(const std::filesystem::path& java_dir);

// What `drop_synthetic_artifacts` found.
struct Dropped {
    NameSet markers;                         // the empty marker classes removed
    std::size_t bridge_constructors = 0;     // the synthetic constructors removed with them
};

// Drops the MCP decompiler artifacts that exist only to satisfy Java access rules, and would emit noise, a false constructor
// stub or an uninitialised-object trap in C++:
//   * empty MARKER classes (`class Empty1 {}`): no fields, methods, parent or interfaces;
//   * synthetic BRIDGE constructors `Cls(EmptyN var1) { this(); }`: a constructor taking exactly one marker-typed parameter
//     (they delegate to the real private constructor and are never meaningfully called).
// A class is a marker only if it is also used as a bridge parameter and never as a base (NoiseGenerator is `class
// NoiseGenerator {}` but is extended by NoiseGeneratorOctaves; EmptyChunk, a real Chunk subclass, is no marker). Records what
// it did on the ledger.
Dropped drop_synthetic_artifacts(std::vector<ir::JClass>& classes, ledger::Ledger* ledger = nullptr);

// Fills each class's `inherited_fields` by walking its parent chain across the full set. Instance fields only; a private
// parent field is skipped (a subclass cannot reference it); own and closer fields shadow farther ones.
void resolve_inheritance(std::vector<ir::JClass>& classes);

// The MULTIPLE-INHERITANCE diamond fix, filling each class's `mi_forwards`. Java lets ONE method satisfy both an interface's
// declaration and a base class's concrete implementation (TileEntityChest: TileEntity declares a concrete
// onInventoryChanged(), IInventory declares it abstractly). In C++ those are distinct vtable slots: the interface's pure
// virtual stays unimplemented -- the class is abstract AND `obj->onInventoryChanged()` is ambiguous. The fix is a forwarding
// override `R m(args){ return Base::m(args); }` in the derived class that unifies both slots; the entry names the interface's
// declaration (the signature's source) and the ancestor whose concrete implementation to call.
void compute_mi_forwards(std::vector<ir::JClass>& classes);

// Each class's fields and method return types flattened over its parent chain (closer definitions win), so a chained
// `recv.field.member` or `Class::s_field.member` gets `.` or `->` right by construction. The runtime's stub classes
// (URL, File, Runtime, Applet, Node ...) are not in the corpus but their method results must resolve chains through them
// (`getDocumentBase()->getHost()`), so they take part: a stub method counts when its return spelling is unambiguous (a single
// reference-type pointer, or `std::string`); anything else (void*, JArray<...>, a primitive) is left out and the chain
// resolver leaves `.` alone rather than guess. A corpus class of the same name wins over a stub.
std::map<std::string, types::MemberEnv, std::less<>> build_member_env(std::span<const ir::JClass> classes, const runtime::Runtime& runtime);

// Per class, the names of its methods that COLLIDE with a field of the same name over the parent chain. Java allows a field
// and a method with one name (a synthetic accessor like NetworkManager.isRunning); C++ does not, so the method is emitted as
// `name_` at its declaration and every call. Also, a STATIC method with the same name and signature as an inherited INSTANCE
// (virtual) method is C++ C4526 (a static cannot override the base virtual): it is renamed too, but only where the signature
// matches (a static helper with a different one legally coexists) and the class has no instance method of the name (the rename
// is by name and would clobber the real override).
std::map<std::string, NameSet, std::less<>> build_method_renames(std::span<const ir::JClass> classes);

// The classes that are (transitively) a Thread subclass or Runnable: an object whose `.start()`/`.run()` runs asynchronously
// and must OUTLIVE the call site. These must never be stack-allocated by the leak transform.
NameSet compute_thread_like(std::span<const ir::JClass> classes);

// WHOLE-PROGRAM pass: marks every collection-typed value parameter that its method mutates with `Param::by_ref`, so the
// emitter passes it by C++ reference instead of copying it. Java passes the collection's reference, so a callee's add() or put()
// writes the CALLER's collection; the converter models collections as value structs, and a by-value parameter silently copies
// -- the getCollidingBoundingBoxes "player falls through the world" bug. Three signals, unioned to a fixpoint:
//   1. DIRECT      -- the body calls a mutator on the parameter (`var6.add(...)`);
//   2. OVERRIDE    -- override signatures MUST match: if any member of an override family has position i out, all do (else an
//                     override that only forwards to super would not match the base's `&`);
//   3. FORWARDING  -- the parameter is passed as an argument into another method's out slot.
// A read-only collection parameter stays by value (a safe copy): only a mutated one flips to `&`, which keeps the change minimal
// and avoids binding temporaries to a non-const reference. Returns how many parameters were marked.
std::size_t compute_collection_out_params(std::vector<ir::JClass>& classes);

// The arguments of every call `name(...)` in `body` (raw Java) with exactly `arity` top-level arguments, each trimmed:
// (position, text) per argument of each such call. Parenthesis-depth aware, so nested calls do not split the list. It shows
// when a method FORWARDS one of its parameters into another method's out-parameter slot.
struct CallArg {
    std::size_t index = 0;
    std::string text;
};
std::vector<CallArg> call_args(std::string_view body, std::string_view name, std::size_t arity);

// The nullable getters DERIVED from the corpus: a method returning a REFERENCE type whose body can `return null;` can hand
// back null, so `getX()->y` on it is a null dereference (a crash on the 360) and must be guarded -- on any corpus, not just the
// Alpha whitelist. High precision (an explicit `return null` and a pointer return), so over-guarding is bounded; a spurious guard
// is at worst a benign extra null check.
//
// AMBIGUITY GUARD: getters and return types are keyed by method NAME (global), and the emitter types a `recv->NAME()` result from
// that map whatever the receiver. An obfuscated name (`a`, `b`) returning DIFFERENT types in different classes would mistype a
// call on one receiver as another's return (EntityMap.a() returns List but Pathfinder.a() returns PathPoint, so
// `entityMap->a()` was typed `PathPoint*` and `.size()` failed to compile). A name whose return type is not consistent across
// every class declaring it is EXCLUDED: a wrong-type hoist is a compile error, far worse than a missed guard.
struct Nullability {
    NameSet getters;
    typemap::NameMap return_types;   // getter -> the reference type it returns
};
Nullability derive_nullability(std::span<const ir::JClass> classes, const types::TypeResolver& resolver);

// The classes whose constructor (or instance initialiser) uses `this` as a value -- stores it, passes it -- and every class below
// one (its constructor runs the parent's). An object of one of these is kept by someone else after `new`, so it must not be freed
// where the `new` is a bare statement. A Thread (or TimerTask, or Runnable) is kept by its running thread.
NameSet compute_this_escapes(std::span<const ir::JClass> classes);

struct ResolverInputs {
    // The class names to treat as C++ value types; none: the target profile's.
    std::optional<NameSet> value_types;
    const typemap::TypeProfile* profile = nullptr;   // none: the generic profile (no value types)
    const runtime::Runtime* runtime = nullptr;       // none: no stub classes in the member environment
    NameSet markers;                                 // the marker classes `drop_synthetic_artifacts` removed
};

// The type resolver of the program: its classes, interfaces, enums (typed ones apart), the member environment, the method
// renames, the thread-like classes, and the whole-program no-GC caller-frees catalog the source emitter consults (owned
// containers get an RAII element-deleter at their call sites; owned scalar returns used inline as arguments are hoisted and
// deleted, but only where the receiving method provably just reads the argument).
types::TypeResolver build_resolver(std::span<const ir::JClass> classes, const ResolverInputs& inputs = {});

}  // namespace cb::java::corpus
