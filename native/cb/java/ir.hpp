// cb/java/ir.hpp -- the Intermediate Representation: what a Java source file becomes.
//
// This is the CONTRACT between the front-end (`parse`) and the back-end (`types`, `emit`). Everything downstream
// depends on these shapes and nothing else, so the parser can be reworked without touching emit as long as it keeps
// producing valid IR -- and emit can be reworked without touching the parser.
//
// The IR is deliberately dumb: it holds *structure*, not decisions. No C++ types, no value/pointer calls, no
// translation. Those belong to `types` and `emit`. Method bodies are kept as raw source lines (with their original
// line numbers, so emit can attribute ledger events back to the Java) until the statement translator consumes them.
//
// Ownership: a JClass owns its fields, methods and bodies by value. The two derived members the pipeline fills in
// from the full class set (`inherited_fields`, `mi_forwards`) point at members of OTHER classes of the same set;
// they stay valid while those classes are neither moved nor have their fields/methods vectors resized, which is the
// case once parsing is done (the pipeline only derives them then).
#pragma once

#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

namespace cb::java::ir {

// Access levels, lowest-visibility last. `Package` = Java default (no modifier).
enum class Access { Public, Protected, Package, Private };

std::string_view to_string(Access a);                       // "public", "protected", "package", "private"
std::optional<Access> parse_access(std::string_view text);  // the inverse; nullopt for any other text

// A Java type reference exactly as written at a use site.
//
// Examples:
//     int                 -> JType{"int"}
//     int[][]             -> JType{"int", 2}
//     List<ItemStack>     -> JType{"List", 0, {JType{"ItemStack"}}}
//     String              -> JType{"String"}
struct JType {
    std::string name;
    std::uint32_t array_dims = 0;
    std::vector<JType> args;

    bool is_array() const { return array_dims > 0; }
    bool is_generic() const { return !args.empty(); }
    // The element type of an array (one dimension peeled); the type itself if scalar.
    JType element() const;
    // Source-like text: `List<ItemStack>[]`.
    std::string to_string() const;
    std::size_t hash() const;

    friend bool operator==(const JType&, const JType&) = default;
};

// One raw source line of a body, with its line number in the Java file.
struct SourceLine {
    std::uint32_t number = 0;
    std::string text;

    friend bool operator==(const SourceLine&, const SourceLine&) = default;
};
using Body = std::vector<SourceLine>;

struct Field {
    std::string name;
    JType type;
    Access access = Access::Package;
    bool is_static = false;
    bool is_final = false;
    std::optional<std::string> initializer;   // raw Java expression
    std::uint32_t line = 0;
};

struct Param {
    std::string name;
    JType type;
    bool is_final = false;
    // Set by the pipeline's collection-out-param pass: a Java collection param the method MUTATES, so the emitter
    // must pass it by C++ reference (JList<T>&) -- a by-value copy loses the callee's writes.
    bool by_ref = false;
};

// A name + parameter-type key, for overload-aware lookups.
struct Signature {
    std::string name;
    std::vector<std::string> param_types;   // JType::to_string of each parameter

    friend bool operator==(const Signature&, const Signature&) = default;
    friend auto operator<=>(const Signature&, const Signature&) = default;
};

struct Method {
    std::string name;
    std::optional<JType> return_type;   // nullopt for constructors; void is JType{"void"}
    std::vector<Param> params;
    Access access = Access::Package;
    bool is_static = false;
    bool is_abstract = false;
    bool is_final = false;
    bool is_ctor = false;
    Body body;
    std::uint32_t line = 0;

    std::size_t arity() const { return params.size(); }
    std::vector<JType> param_types() const;
    Signature signature() const;
};

struct EnumConst {
    std::string name;
    std::vector<std::string> args;   // raw Java argument expressions
};

// An interface method a base class implements, which the derived class must forward to resolve a multiple-
// inheritance diamond (computed by the pipeline from the full class set).
struct MiForward {
    const Method* interface_method = nullptr;
    std::string base_class;
};

// A parsed Java top-level type.
struct JClass {
    std::string name;
    std::optional<std::string> package;
    std::optional<std::string> parent;   // `extends`
    std::vector<std::string> interfaces;
    bool is_interface = false;
    bool is_abstract = false;
    bool is_enum = false;
    std::vector<Field> fields;
    std::vector<Method> methods;
    std::vector<EnumConst> enum_consts;
    // Raw Java bodies of `static { ... }` and instance `{ ... }` initializer blocks, in source order. Static blocks
    // run once at class load; instance blocks run in each constructor.
    std::vector<Body> static_inits;
    std::vector<Body> inst_inits;
    std::optional<std::filesystem::path> source_path;
    // Instance fields inherited from ancestors (filled by the pipeline from the full class set); used for
    // value/pointer access of inherited fields in bodies.
    std::vector<const Field*> inherited_fields;
    // Synthesised MI-diamond forwarding overrides (filled by the pipeline).
    std::vector<MiForward> mi_forwards;
    // Set by the source emitter: initStatics() has a non-empty body (deferred static field inits or static{} blocks),
    // so the pipeline adds the class to the ordered init driver.
    bool has_init_statics = false;

    // -- queries used by types/emit -----------------------------------------
    const Field* field(std::string_view name) const;            // nullptr when there is none
    std::vector<const Field*> instance_fields() const;
    std::vector<const Field*> static_fields() const;
    std::vector<const Method*> constructors() const;
    // First method by that name (use signature() for overload precision); nullptr when none.
    const Method* method(std::string_view name) const;
    // True if any field initializer allocates (a Rule-of-Three signal at the IR level).
    bool owns_heap_field() const;
    // A Java 5 'typed' enum -- constants carry constructor args and/or the enum declares instance state/behaviour
    // (EnumToolMaterial, EnumArt, EnumCreatureType, ...). These must become real objects (a class with a static
    // instance table), NOT bare int constants. A constant-only enum (EnumMobType: `everything, mobs, players;`)
    // stays int constants.
    bool is_typed_enum() const;
};

}  // namespace cb::java::ir

template <>
struct std::hash<cb::java::ir::JType> {
    std::size_t operator()(const cb::java::ir::JType& t) const { return t.hash(); }
};
