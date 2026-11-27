// dotnet/decompiler/csharp/syntax/modifiers.hpp -- the C# modifiers of a declaration (public, static, readonly ...) as flags, and the
// keyword of each.
//
//     Modifiers m = Modifiers::Public | Modifiers::Static;
//     if (has_flag(m, Modifiers::Static)) ...
//     std::string_view text = CSharpModifiers::get_modifier_name(Modifiers::Static);   // "static"
//     for (Modifiers each : CSharpModifiers::all_modifiers()) ...                        // in the order they are written
//
// Modifiers::Any is for patterns only: it matches whatever modifiers a declaration has.
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.Syntax: Modifiers, CSharpModifiers, from Modifiers.cs), Copyright AlphaSierraPapa
// for the ILSpy team, MIT (see NOTICE in dotnet/decompiler).
#pragma once

#include <array>
#include <cstdint>
#include <string_view>

namespace cb::dotnet::decompiler::csharp::syntax {

enum class Modifiers : std::int32_t {
    None = 0,

    Private = 0x0001,
    Internal = 0x0002,
    Protected = 0x0004,
    Public = 0x0008,

    Abstract = 0x0010,
    Virtual = 0x0020,
    Sealed = 0x0040,
    Static = 0x0080,
    Override = 0x0100,
    Readonly = 0x0200,
    Const = 0x0400,
    New = 0x0800,
    Partial = 0x1000,

    Extern = 0x2000,
    Volatile = 0x4000,
    Unsafe = 0x8000,
    Async = 0x10000,
    Ref = 0x20000,
    Required = 0x40000,

    VisibilityMask = Private | Internal | Protected | Public,

    // The pattern value that matches any modifiers (the sign bit, as upstream's `unchecked((int)0x80000000)`).
    Any = static_cast<std::int32_t>(0x80000000u),
};

constexpr Modifiers operator|(Modifiers a, Modifiers b) { return static_cast<Modifiers>(static_cast<std::int32_t>(a) | static_cast<std::int32_t>(b)); }
constexpr Modifiers operator&(Modifiers a, Modifiers b) { return static_cast<Modifiers>(static_cast<std::int32_t>(a) & static_cast<std::int32_t>(b)); }
constexpr Modifiers operator^(Modifiers a, Modifiers b) { return static_cast<Modifiers>(static_cast<std::int32_t>(a) ^ static_cast<std::int32_t>(b)); }
constexpr Modifiers operator~(Modifiers a) { return static_cast<Modifiers>(~static_cast<std::int32_t>(a)); }
constexpr Modifiers& operator|=(Modifiers& a, Modifiers b) { return a = a | b; }
constexpr Modifiers& operator&=(Modifiers& a, Modifiers b) { return a = a & b; }
constexpr Modifiers& operator^=(Modifiers& a, Modifiers b) { return a = a ^ b; }

// Whether all the bits of `flag` are set in `value` (upstream's `(value & flag) == flag`).
constexpr bool has_flag(Modifiers value, Modifiers flag) { return (value & flag) == flag; }

struct CSharpModifiers {
    // Every modifier in the order modifiers are written when generating code (sorted: public, private, protected, internal, new,
    // unsafe, static, abstract, virtual, sealed, override, required, readonly, volatile, ref, extern, partial, const, async), then Any.
    static const std::array<Modifiers, 20>& all_modifiers();

    // The C# keyword of a single modifier ("static"); "any" for Modifiers::Any. Throws std::invalid_argument for anything else.
    static std::string_view get_modifier_name(Modifiers modifier);
};

}  // namespace cb::dotnet::decompiler::csharp::syntax
