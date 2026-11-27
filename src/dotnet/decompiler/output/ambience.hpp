// dotnet/decompiler/output/ambience.hpp -- IAmbience: how a language writes a symbol, a type or a constant as a short piece of text (for a
// tooltip, a list of members, a message), and the ConversionFlags that say what the text includes.
//
//     CSharpAmbience ambience;
//     ambience.set_conversion_flags(ConversionFlags::StandardConversionFlags);
//     std::string text = ambience.convert_symbol(method);        // "public void Run(int count)"
//
// Ported from ILSpy (ICSharpCode.Decompiler.Output: IAmbience, ConversionFlags from IAmbience.cs), Copyright AlphaSierraPapa for the ILSpy
// team, MIT (see NOTICE in dotnet/decompiler).
#pragma once

#include <cstdint>
#include <string>

#include "dotnet/decompiler/typesystem/type_system.hpp"

namespace cb::dotnet::decompiler {

enum class ConversionFlags : std::int32_t {
    None = 0,
    ShowParameterList = 1,
    ShowParameterNames = 2,
    ShowAccessibility = 4,
    ShowDefinitionKeyword = 8,
    ShowDeclaringType = 0x10,
    ShowModifiers = 0x20,
    ShowReturnType = 0x40,
    UseFullyQualifiedTypeNames = 0x80,
    ShowTypeParameterList = 0x100,
    ShowBody = 0x200,
    UseFullyQualifiedEntityNames = 0x400,
    PlaceReturnTypeAfterParameterList = 0x800,
    ShowTypeParameterVarianceModifier = 0x1000,
    ShowParameterModifiers = 0x2000,
    ShowParameterDefaultValues = 0x4000,
    UseNullableSpecifierForValueTypes = 0x8000,
    SupportInitAccessors = 0x10000,
    SupportRecordClasses = 0x20000,
    SupportRecordStructs = 0x40000,
    SupportUnsignedRightShift = 0x80000,
    SupportOperatorChecked = 0x100000,
    UsePrivateProtectedAccessibility = 0x200000,
    SupportExtensionDeclarations = 0x400000,

    StandardConversionFlags = ShowParameterNames | ShowAccessibility | UsePrivateProtectedAccessibility | ShowParameterList | ShowParameterModifiers |
                              ShowParameterDefaultValues | UseNullableSpecifierForValueTypes | ShowReturnType | ShowModifiers | ShowTypeParameterList |
                              ShowTypeParameterVarianceModifier | ShowDefinitionKeyword | ShowBody,

    All = 0x1fffff,
};

constexpr ConversionFlags operator|(ConversionFlags a, ConversionFlags b) {
    return static_cast<ConversionFlags>(static_cast<std::int32_t>(a) | static_cast<std::int32_t>(b));
}
constexpr ConversionFlags operator&(ConversionFlags a, ConversionFlags b) {
    return static_cast<ConversionFlags>(static_cast<std::int32_t>(a) & static_cast<std::int32_t>(b));
}
constexpr ConversionFlags operator~(ConversionFlags a) { return static_cast<ConversionFlags>(~static_cast<std::int32_t>(a)); }
constexpr ConversionFlags& operator|=(ConversionFlags& a, ConversionFlags b) { return a = a | b; }
constexpr ConversionFlags& operator&=(ConversionFlags& a, ConversionFlags b) { return a = a & b; }

// Whether any bit of `flag` is set in `value` (upstream's `(value & flag) != 0`).
constexpr bool has_any(ConversionFlags value, ConversionFlags flag) { return (value & flag) != ConversionFlags::None; }
// Whether all the bits of `flag` are set in `value` (upstream's `(value & flag) == flag`).
constexpr bool has_all(ConversionFlags value, ConversionFlags flag) { return (value & flag) == flag; }

class IAmbience {
public:
    virtual ~IAmbience() = default;

    virtual ConversionFlags conversion_flags() const = 0;
    virtual void set_conversion_flags(ConversionFlags value) = 0;

    virtual std::string convert_symbol(const typesystem::ISymbol& symbol) = 0;
    virtual std::string convert_type(const typesystem::IType& type) = 0;
    virtual std::string convert_constant_value(const typesystem::ConstantValue& constant_value) = 0;
    virtual std::string wrap_comment(const std::string& comment) = 0;
};

}  // namespace cb::dotnet::decompiler
