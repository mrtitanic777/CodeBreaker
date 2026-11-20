// java/emit/source.hpp -- render a class's .cpp from the IR and a type resolver.
//
// This is where the console-crash classes are prevented BY CONSTRUCTION:
//   * static and array allocations get value-initialisation `new T[n]()` (Java zero-fills; C++ does not);
//   * every constructor zero-initialises its instance fields in the member-initialiser list (no uninitialised members);
//   * a class that allocates heap in a constructor gets a destructor that frees it (the Rule-of-Three leak fix);
//   * `this(...)` constructor delegation is inlined by the back end, never silently dropped (the Block blockID=0 and
//     BlockContainer flag bugs).
//
// Every body, constructor and initialiser is written from the typed IR by the translator's back end, reached through
// `BodySource`; what it declines is an explicit, ledgered J2X-GAP, never a guess.
//
//     source::Options options;  options.cpp_std = header::CppStd::of_year(2011);  options.bodies = &back_end;
//     const source::Rendered cpp = source::render_source(cls, resolver, options);
//     if (cpp.text) write(cls.name + ".cpp", *cpp.text);
#pragma once

#include <optional>
#include <set>
#include <string>
#include <vector>

#include "java/emit/header.hpp"
#include "java/ir.hpp"
#include "java/ledger.hpp"
#include "java/types.hpp"

namespace cb::java::emit::source {

// The explicit constructor call a Java constructor began with, if any: `super(...)`, `this(...)`, or neither (Java's
// implicit `super()`).
enum class CtorCall { None, Super, This };

// What the back end wrote for one constructor: the call it began with, the C++ arguments of a `super(...)`, and the body's lines.
struct CtorBody {
    CtorCall call = CtorCall::None;
    std::optional<std::string> super_args;   // the arguments to hand the parent's constructor; none: no parent call is written
    std::vector<std::string> lines;
};

// The translator's back end, asked per member. Each answer is nullopt when the back end declined (the output then says so:
// a J2X-GAP that throws if it runs). An answer is lines of C++, without indentation.
class BodySource {
public:
    virtual ~BodySource() = default;

    // The C++ initialiser expression of a static or enum field.
    virtual std::optional<std::string> field_init(const ir::JClass& cls, const ir::Field& field) = 0;
    // The C++ elements of a static field's `new T[]{...}` initialiser.
    virtual std::optional<std::vector<std::string>> array_items(const ir::JClass& cls, const ir::Field& field) = 0;
    // `this->f = init;` for each instance field initialiser and instance `{}` block, in source order.
    virtual std::optional<std::vector<std::string>> instance_inits(const ir::JClass& cls) = 0;
    // A constructor of `cls` (nullptr: the implicit one). `two_phase`: the form that becomes `j2xInit(...)`.
    virtual std::optional<CtorBody> ctor(const ir::JClass& cls, const ir::Method* ctor, bool two_phase) = 0;
    // The C++ arguments of an enum constant's constructor call.
    virtual std::optional<std::vector<std::string>> enum_args(const ir::JClass& cls, const ir::EnumConst& constant) = 0;
    // Each `static {}` block's lines, in source order.
    virtual std::optional<std::vector<std::vector<std::string>>> static_blocks(const ir::JClass& cls) = 0;
    // The body of a method.
    virtual std::optional<std::vector<std::string>> body(const ir::JClass& cls, const ir::Method& method) = 0;
};

struct Options {
    std::optional<header::CppStd> cpp_std;   // none: C++03 in the banner and no standard-specific rewrites
    std::string platform_label;              // for the banner; empty reads "unknown"
    // Classes built in two phases: `C::C()` zeroes the fields and each Java constructor becomes `C* C::j2xInit(params)`.
    std::set<std::string, std::less<>> two_phase;
    ledger::Ledger* ledger = nullptr;        // where the rules that fired and the gaps are recorded; none: not recorded
    BodySource* bodies = nullptr;            // none: every body is a J2X-GAP
};

struct Rendered {
    std::optional<std::string> text;   // the .cpp, or none for an interface or a constant-only enum (header-only)
    // initStatics() has a body (deferred static field initialisers or static{} blocks), so the ordered init driver must call it.
    bool has_init_statics = false;
};

Rendered render_source(const ir::JClass& cls, const types::TypeResolver& resolver, const Options& options = {});

// -- pieces, exposed for their tests ---------------------------------------------------------------------------------------

// True if a static initialiser must run AFTER zero-initialisation, in controlled order, so that the cross-translation-unit
// order of global constructors (which C++ leaves undefined) cannot crash the boot before main(). Deferred initialisers move
// into the ordered initStatics(); the field definition is zero-initialised. Compile-time constants and `const` members (which
// cannot be reassigned) stay eager.
//
// It is order-sensitive when the initialiser constructs an object (`new`), reads another class's statics (`::` or `->`), or
// CALLS a free or static function -- a lowercase-initial identifier (including obfuscated `func_NNNNN`) right before `(`: that
// runs arbitrary code at static-init time that may touch other classes' statics not yet built (StatList's
// `s_mineBlockStatArray = func_25153_a(...)` indexed Block::s_blocksList before its constructors had run: a silent crash
// before main()). Constructor calls (PascalCase type, `JArray<T>(N)`) only allocate and are order-independent.
bool is_deferrable_static_init(std::string_view init, std::string_view field_type);

// Multi-dimensional aggregate `new T[][]{...}` (two or more empty `[]` pairs, then `{`). The one-dimensional form is a flat
// file-static data array (ownership::aggregate_array_body); a nested aggregate cannot be a flat C array, so it builds a nested
// JArrayInit instead (C++03 has no braced new[][]).
bool is_multi_dimensional_aggregate(std::string_view initializer);

// The zero or null literal for a member initialiser of that C++ type: NULL for a pointer, 0 for a primitive, none for a value
// type (its default constructor runs). Java byte (`signed char`) is among the primitives: it was missing once, so every byte
// field -- packet types, yaw/pitch, NBT tags -- held garbage until assigned.
std::optional<std::string> default_value(std::string_view cpp_type);

// The user types a class's .cpp needs the full definition of: its signatures' types (parent, interfaces, fields including
// inherited, parameters, returns), and every capitalised identifier of the body, then their transitive closure over the member
// environment (a chain like `this->mc->gameSettings->keyBindInventory->keyCode` reaches types spelled nowhere in the class).
std::vector<std::string> referenced_types(const ir::JClass& cls, const types::TypeResolver& resolver, std::string_view body_text);

}  // namespace cb::java::emit::source
