// cb/java/typemap.hpp -- pure DATA: how Java types map to C++ types.
//
// No logic lives here (that is `types`); these are the lookup tables, lifted from the proven monolith so we keep its
// hard-won knowledge without its inline `/* PORT: */` comment noise. Where the old converter embedded a PORT note
// inside the type string (Object -> "void* /* PORT: was Object */"), we map to the clean type and let `emit` record
// the PORT as a ledger event instead.
//
// Two kinds of data:
//   * the fixed tables of the Java language and of the runtime we ship -- the same for every target; each is a
//     function returning a const table (built once);
//   * `TypeProfile`, the game-/target-specific sets (nullable fields/getters, value-type opt-ins, unsigned-byte
//     overrides). A conversion owns one, taken from its target profile, and hands it to the stages that need it.
//     The pipeline augments `nullable_getters` with corpus-derived getters for the duration of a conversion; it does
//     that on its own copy, so nothing leaks between conversions.
#pragma once

#include <map>
#include <set>
#include <string>
#include <utility>

namespace cb::java::typemap {

// Sets and maps of names. Sorted, so anything that walks them (a report, an emitted list) is deterministic.
using NameSet = std::set<std::string, std::less<>>;
using NameMap = std::map<std::string, std::string, std::less<>>;

// A scalar field of a class: (ClassName, fieldName).
using ClassField = std::pair<std::string, std::string>;

// ---- fixed tables ---------------------------------------------------------------------------------------

// Java language primitives (value types, stack-allocated).
const NameSet& primitives();

// Base Java -> C++ type names.
//
// SIGNED-BYTE POLICY (correct by construction): Java `byte` is SIGNED (-128..127). A SCALAR byte is therefore mapped
// to `signed char` -- the Java-faithful default -- because scalar bytes are routinely used as signed values
// (negative sentinels like -1/-100, `< 0` tests, signed deltas/offsets). Mapping them unsigned silently turns -1
// into 255 and detonates:
//   * Packet1Login.dimension: nether = -1 read as 255 -> getProviderForDimension(255) NULL -> crash.
//   * Packet30Entity x/y/z relative-move deltas: -1 read as +255 -> entities flung into the sky.
//   * live-tree [signedbyte] annotations: side indices, flow-decay sentinels, mouse-invert, door hinge,
//     track-shape ids, model offsets -- all LOCAL byte vars that broke as unsigned.
// EXCEPTION: `byte[]` BUFFERS stay `unsigned char` (chunk/block data, packet payloads, inflate I/O -- read via
// `& 0xFF` and want `unsigned char*`). That split is applied in types::cpp_type: scalar byte -> `signed char`,
// byte[] element -> `unsigned char`. `& 0xFF` still works on signed char (int-promoted), so buffers that also see
// arithmetic are unaffected. This retired the old SIGNED_BYTE_FIELDS whitelist: if a specific scalar byte ever
// needs to be UNSIGNED, add it to TypeProfile::unsigned_byte_scalar_fields (none are known).
// Java `char` is UNSIGNED: `unsigned char`, the same unit JavaString's bytes and charAt (cast to unsigned char)
// give. J2X-BUGFIX char-unsigned: as C++ `char` (signed) every char of 128-255 -- the section sign of colour codes,
// extended Latin -- was negative in arithmetic, comparisons and (int) casts; the behavioural judge found
// (int)(char)255 = -1. Values past 255 cannot occur: strings here are 8-bit.
const NameMap& java_to_cpp();

// Java types that emit warrants a PORT note for (mapped, but lossy / needs review), with the note.
const NameMap& port_note_types();

// Java collection types -> STL. Held BY VALUE as fields by default (std::vector/map).
const NameSet& containers();

// Default element types for a RAW (non-generic) container -> a valid C++ template argument list. A raw Map is
// JMap<JObject, JObject>: a Java raw map mixes key/value types (String/Integer/Character keys; any object/boxed
// value). JObject boxes all of them (and has operator< for the key), so put/get/containsKey/remove all bind
// regardless of the concrete Java types. Keys/values convert in implicitly; reads convert out via the cast the
// converter already emits.
const NameMap& raw_container_default();

// Java stdlib types that become C++ VALUE types (not pointers). ContextCapabilities (an LWJGL GL-caps struct stub)
// is here so `ContextCapabilities c = GLContext.getCapabilities()` is a VALUE local (a copy of the reference the
// stub returns), which both compiles for Indev's assign-then-read and keeps Beta's
// `getCapabilities().GL_ARB_x` working -- see the glcontext-caps-ref bugfix in the runtime.
const NameSet& value_stdlib();

// Boxed primitive wrappers -- map to C++ primitives, so they are VALUE types (not pointers).
const NameSet& boxed_value_types();

// Static-holder externals accessed as Name::member (constants + static functions) that get their own emitted
// header, like GL11. They also need runtime_enum_like (no s_ prefix on members).
const NameSet& runtime_static_holders();

// Runtime-support classes the converter EMITS a header for (see the runtime module) and #includes when referenced
// -- not part of the decompiled corpus. External Java base types our classes extend/implement (Runnable, Thread,
// Comparator ...), the static holders above, NIO buffers, and java.io/awt/net instance stubs.
const NameSet& runtime_headers();

// Java stdlib static holders shimmed in the prelude (JavaRT.h, included everywhere -- so NOT in runtime_headers,
// which would emit a bogus per-type #include). `Name.member` -> `Name::member`.
const NameSet& javart_statics();

// Runtime types whose `Type.MEMBER` (no parens) is a CONSTANT, not an instance/static field -- accessed as
// Type::MEMBER with no s_ prefix (like an enum constant): GL11.GL_TEXTURE_2D, Integer.MAX_VALUE, System.out.
// Color.BLACK / Color.white etc. are static Color CONSTANTS accessed Color::BLACK (no s_).
const NameSet& runtime_enum_like();

// java.lang.Math.<fn> -> C math function. Float-suffixed to match the existing Xbox 360 project (float math); pulled
// in via <math.h>. min/max stay bare (the project provides them).
//
// TRIG is DOUBLE, not float. Java's Math.sin/cos/tan are double, and the game calls them on LARGE arguments -- e.g.
// sin(System.currentTimeMillis()/100.0) for the pulsing block-highlight alpha. `sinf` cannot range-reduce an
// argument that large (it returns NaN on the 360 PPC CRT), which packed the highlight's alpha to 0 -> invisible.
// `sin` (double, native on PPC -> no perf cost) reduces correctly. J2X-BUGFIX trig-double-precision.
// J2X-BUGFIX sqrt-double-precision: Math.sqrt returns DOUBLE in Java. Mapping it to sqrtf (float) made
// `atan2(Math.sqrt(...), doubleExpr)` an ambiguous (float,double) overload (RenderGlobal). A bare `sqrt` doesn't
// fix it either: MSVC's <math.h> has a float sqrt(float) overload, so a float argument still yields float. The
// JavaRT.h shim javaMathSqrt(double) forces the promotion. MathHelper.sqrt_float is a separate key and stays sqrtf.
// Math.round(x) returns an integer (round-half-up); an overloaded free-function shim in the prelude (int for float,
// long long for double) -- there is no single C stdlib call with Java's semantics.
// Math.PI is a member, not a method -> the literal is inlined. A DOUBLE, as in Java. J2X-BUGFIX math-pi-double: it
// was the float 3.1415927F, so `(float)Math.sin(i * Math.PI * 2.0D / 65536.0D)` (MathHelper's SIN_TABLE) was computed
// with a single-precision PI and every MathHelper.sin/cos of a non-zero angle came out a few ulps off Java's.
const NameMap& math_api();

// EMIT form of math_api: global-qualify (`::sin`) ONLY the math functions whose C++ name is UNCHANGED from the Java
// name (sin, cos, tan, abs, ...). Those are the only ones a same-named local can shadow within its own initializer
// -- the `float sin = Math.sin(...)` C2064 from decompiled code (Entity.moveRelative). Names that already differ
// (sqrt->javaMathSqrt, round->javaMathRound) can't collide, and `min`/`max` stay unqualified (they may be MSVC
// min/max MACROS, which `::` breaks). This is the map emit uses.
const NameMap& math_api_emit();

// ---- target-specific data -------------------------------------------------------------------------------

// What a target profile says about nullability, value types and byte signedness. Empty means the generic profile.
struct TypeProfile {
    // Registry arrays whose slots are NULL until an id is registered. Indexing one with an unregistered id and
    // calling through it = a virtual call on NULL = IP 0x0 crash on the console. A deref of these (or any scope
    // array of reference type) is guarded.
    NameSet nullable;
    // Fields that are legitimately NULL at certain points in the lifecycle (set later, cleared on shutdown, etc.). On
    // Xbox 360, ANY dereference of these without a prior null check is a hard crash (no NULL page protection). The
    // converter must guard them.
    NameSet nullable_fields;
    // Getters that can return NULL (a lookup from a sparse registry, or an optional reference). Chaining
    // `getter()->method()` without a null check is a hard crash on the 360.
    NameSet nullable_getters;
    // Return type (a reference/pointer type) of each POINTER-returning nullable getter, so the null-guard hoist can
    // declare a typed temp -- `Type* t = getter();` -- instead of C++11 `auto`. Keyed by getter name; the emitter
    // appends `*`. getSmeltingResultItem is intentionally ABSENT: it returns `int` (a value), so it can never form
    // a `getter()->deref` and never triggers the guard.
    NameMap nullable_getter_return_type;
    // Scalar bytes forced back to unsigned char.
    std::set<ClassField> unsigned_byte_scalar_fields;
    // User-defined C++ VALUE/struct types ('.' access, no heap). EMPTY by default: a Java class is a REFERENCE type
    // and must be a pointer to be universal + faithful (Minecraft's Vec3D / AxisAlignedBB / MovingObjectPosition are
    // reference types AND pooled -- modelling them as values broke nullable override returns at scale, 742
    // conflicting-return errors on alpha). A profile can opt a genuinely-by-value struct in here.
    NameSet default_value_types;
};

}  // namespace cb::java::typemap
