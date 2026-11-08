// codebreaker/java/typemap.hpp -- typed C++ from codebreaker.java.typemap.
//
// typemap.py -- pure DATA: how Java types map to C++03 types.
//
// No logic lives here (that's `types.py`); this is the lookup tables, lifted and
// cleaned from the proven monolith so we keep its hard-won knowledge without its
// inline `/* PORT: */` comment noise. Where the old converter embedded a PORT note
// inside the type string (Object -> "void* /* PORT: was Object */"), we map to the
// clean type and let `emit` record the PORT as a ledger event instead.
//
// The game-/target-specific sets below (nullable fields/getters, value-type opt-ins, unsigned-byte
// overrides) are sourced from the active target profile (`codebreaker.java.profiles`) rather than hardcoded, so the
// core stays game-agnostic. They remain mutable module-level names: the pipeline augments
// KNOWN_NULLABLE_GETTERS with corpus-derived getters at convert time and restores them afterward.
//
// codebreaker.java.profiles has no typed port yet, so the profile is taken as a parameter: the
// profile-sourced names are seeded from the generic profile (every field empty, which is what the Python
// has at import) and `_reseed` takes the new profile's data as a `Profile` record. The Python registers
// `_reseed` with `profiles.on_change`; a caller that ports profiles does that wiring.
#pragma once

#include <tuple>

#include "py.hpp"

namespace codebreaker::java::typemap {

// The fields of a codebreaker.java.profiles.Profile that this module reads.
struct Profile {
    py::set<py::str> nullable;
    py::set<py::str> nullable_fields;
    py::set<py::str> nullable_getters;
    py::dict<py::str, py::str> nullable_getter_return_type;
    // (ClassName, fieldName) pairs
    py::set<std::tuple<py::str, py::str>> unsigned_byte_scalar_fields;
    py::set<py::str> default_value_types;
};

// Java language primitives (value types, stack-allocated).
extern const py::set<py::str> PRIMITIVES;

// Base Java -> C++03 type names. Profile extras merge on top of this at runtime.
// SIGNED-BYTE POLICY (correct by construction): Java `byte` is SIGNED (-128..127). A SCALAR byte is
// therefore mapped to `signed char` -- the Java-faithful default -- because scalar bytes are routinely
// used as signed values (negative sentinels like -1/-100, `< 0` tests, signed deltas/offsets). Mapping
// them unsigned silently turns -1 into 255 and detonates:
//   * Packet1Login.dimension: nether = -1 read as 255 -> getProviderForDimension(255) NULL -> crash.
//   * Packet30Entity x/y/z relative-move deltas: -1 read as +255 -> entities flung into the sky.
//   * live-tree [signedbyte] annotations: side indices, flow-decay sentinels, mouse-invert, door hinge,
//     track-shape ids, model offsets -- all LOCAL byte vars that broke as unsigned.
// EXCEPTION: `byte[]` BUFFERS stay `unsigned char` (chunk/block data, packet payloads, inflate I/O --
// read via `& 0xFF` and want `unsigned char*`). That split is applied in types.cpp_type: scalar byte ->
// `signed char`, byte[] element -> `unsigned char`. `& 0xFF` still works on signed char (int-promoted),
// so buffers that also see arithmetic are unaffected. This retires the old SIGNED_BYTE_FIELDS whitelist.
// Java `char` is UNSIGNED: `unsigned char`, the same unit JavaString's bytes and charAt (cast to unsigned
// char) give. J2X-BUGFIX char-unsigned: as C++ `char` (signed) every char of 128-255 -- the section sign of
// colour codes, extended Latin -- was negative in arithmetic, comparisons and (int) casts; the behavioural
// judge found (int)(char)255 = -1. Values past 255 cannot occur: strings here are 8-bit.
extern const py::dict<py::str, py::str> JAVA_TO_CPP;

// RETIRED: SIGNED_BYTE_FIELDS whitelist. Scalar `byte` is now `signed char` by DEFAULT (Java-faithful),
// so per-field curation is no longer needed -- Packet1Login.dimension, Packet30Entity deltas, and every
// unlisted scalar byte are all correctly signed by construction. Only `byte[]` buffers stay unsigned
// (see JAVA_TO_CPP note + types.cpp_type). If a specific scalar byte ever needs to be UNSIGNED, add it
// to UNSIGNED_BYTE_SCALAR_FIELDS below (empty -- none known).
// (ClassName, fieldName) scalar bytes forced back to unsigned char
extern py::set<std::tuple<py::str, py::str>> UNSIGNED_BYTE_SCALAR_FIELDS;

// Java types that emit warrants a PORT note for (mapped, but lossy / needs review).
extern const py::dict<py::str, py::str> PORT_NOTE_TYPES;

// Java collection types -> STL. Held BY VALUE as fields by default (std::vector/map).
extern const py::set<py::str> CONTAINERS;

// default element types for a RAW (non-generic) container -> a valid C++ template
extern const py::dict<py::str, py::str> RAW_CONTAINER_DEFAULT;

// Java stdlib types that become C++ VALUE types (not pointers). ContextCapabilities (an LWJGL GL-caps
// struct stub) is here so `ContextCapabilities c = GLContext.getCapabilities()` is a VALUE local (a copy
// of the reference the stub returns), which both compiles for Indev's assign-then-read and keeps Beta's
// `getCapabilities().GL_ARB_x` working -- see the glcontext-caps-ref bugfix in runtime.py.
extern const py::set<py::str> VALUE_STDLIB;

// Boxed primitive wrappers -- map to C++ primitives, so they're VALUE types (not pointers).
extern const py::set<py::str> BOXED_VALUE_TYPES;

// Registry arrays whose slots are NULL until an id is registered. Indexing one with
// an unregistered id and calling through it = a virtual call on NULL = IP 0x0 crash
// on the console. A deref of these (or any scope array of reference type) is guarded.
extern py::set<py::str> KNOWN_NULLABLE;

// Fields that are legitimately NULL at certain points in the lifecycle (set later,
// cleared on shutdown, etc.). On Xbox 360, ANY dereference of these without a prior
// null check is a hard crash (no NULL page protection). The converter must guard them.
extern py::set<py::str> KNOWN_NULLABLE_FIELDS;

// Getters that can return NULL (return a lookup from a sparse registry, or an
// optional reference). Chaining `getter()->method()` without a null check is a
// hard crash on the 360.
extern py::set<py::str> KNOWN_NULLABLE_GETTERS;

// Return type (a reference/pointer type) of each POINTER-returning nullable getter, so the null-guard
// hoist of the old text translator could declare a typed temp in C++03 -- `Type* t = getter();` -- instead of
// C++11 `auto`. Keyed by getter name; the emitter appends `*`. Verified against the Alpha source.
// getSmeltingResultItem is intentionally ABSENT: it returns `int` (a value), so it can never form a
// `getter()->deref` and never triggers the guard.
extern py::dict<py::str, py::str> NULLABLE_GETTER_RETURN_TYPE;

// Runtime-support classes the converter EMITS a header for (see runtime.py) and
// #includes when referenced -- not part of the decompiled corpus. Includes the external
// Java base types our classes extend/implement (Runnable/Thread/Comparator/...).
// Static-holder externals accessed as Name::member (constants + static functions) that get
// their own emitted header. Like GL11. Also need RUNTIME_ENUM_LIKE (no s_ prefix on members).
extern const py::set<py::str> RUNTIME_STATIC_HOLDERS;

// Runtime-support classes the converter EMITS a header for (see runtime.py) and #includes when
// referenced -- not part of the decompiled corpus. External Java base types our classes
// extend/implement, the static holders above, NIO buffers, and java.io/awt/net instance stubs.
extern const py::set<py::str> RUNTIME_HEADERS;

// Java stdlib static holders shimmed in the prelude (JavaRT.h, included everywhere -- so NOT
// in RUNTIME_HEADERS, which would emit a bogus per-type #include). `Name.member` -> `Name::member`.
extern const py::set<py::str> JAVART_STATICS;

// Runtime types whose `Type.MEMBER` (no parens) is a CONSTANT, not an instance/static field
// -- accessed as Type::MEMBER with no s_ prefix (like an enum constant). e.g. GL11.GL_TEXTURE_2D,
// Integer.MAX_VALUE, System.out.
// Color.BLACK / Color.white etc. are static Color CONSTANTS accessed Color::BLACK (no s_).
extern const py::set<py::str> RUNTIME_ENUM_LIKE;

// java.lang.Math.<fn> -> C math function. Float-suffixed to match the existing
// Xbox 360 project (float math); pulled in via <math.h>. min/max stay bare
// (the project provides them). Returns the C name for a given Java Math method.
extern const py::dict<py::str, py::str> MATH_API;

// EMIT form: global-qualify (`::sin`) ONLY the math functions whose C++ name is UNCHANGED from the Java
// name (sin, cos, tan, abs, ...). Those are the only ones a same-named local can shadow within its own
// initializer -- the `float sin = Math.sin(...)` C2064 from decompiled code (Entity.moveRelative). Names
// that already differ (sqrt->sqrtf, round->javaMathRound) can't collide, and `min`/`max` stay unqualified
// (they may be MSVC min/max MACROS, which `::` breaks). This is the map emit uses.
extern const py::dict<py::str, py::str> MATH_API_EMIT;

// User-defined C++ VALUE/struct types ('.' access, no heap). EMPTY by default: a Java class
// is a REFERENCE type and must be a pointer to be universal + faithful (Minecraft's Vec3D /
// AxisAlignedBB / MovingObjectPosition are reference types AND pooled -- modeling them as
// values broke nullable override returns at scale, 742 conflicting-return errors on alpha).
// The value-type mechanism still exists; a profile/caller can opt a genuinely-by-value struct
// in via TypeResolver(value_types=...).
extern py::set<py::str> DEFAULT_VALUE_TYPES;

// Re-derive every profile-sourced global from ``p`` when the active target profile changes.
//
// Registered as a `profiles.on_change` observer so `profiles.use()` actually retargets the
// transpiler (the module-level assignments above only seed the IMPORT-TIME profile). Each global is
// rebuilt as a fresh COPY -- never an alias of the profile's own collection -- because the pipeline
// mutates some of these in place at runtime (e.g. `KNOWN_NULLABLE_GETTERS |= corpus_getters`), and
// aliasing would leak that mutation back into the shared Profile object and across conversions.
void _reseed(const Profile& p);

}  // namespace codebreaker::java::typemap
