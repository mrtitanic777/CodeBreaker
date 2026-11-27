// dotnet/decompiler/ilspycmd/dotnet_enum.hpp -- .NET's Enum.ToString and Enum.TryParse over the enums the metadata table dump and the
// command line print and read: a table of names and values (in the order .NET keeps them), and the same decisions .NET makes for a
// [Flags] enum (names joined with ", ", the number when a bit has no name) and for one that is not (the name, else the number).
//
//     enum_to_string(enums::type_attributes(), 0x100101) == "Public, Sealed, BeforeFieldInit"
//     enum_to_string(enums::assembly_flags(), 0) == "0"
//     std::uint64_t v; enum_try_parse(enums::table_index(), "typedef", true, 8, false, &v);   // v == 2
//
// Written for ilspycmd (ICSharpCode.ILSpyCmd: MetadataTableDumper, whose output prints these enums), MIT, Copyright AlphaSierraPapa for
// the ILSpy team (see NOTICE in dotnet/decompiler). The tables were read from .NET's own enums (Enum.GetNames and GetValues), so the
// order of equal values is .NET's: of several names with one value a [Flags] enum prints the last, one that is not prints the first,
// and the value 0 prints the first name that has it.
#pragma once

#include <cstdint>
#include <span>
#include <string>
#include <string_view>

namespace cb::dotnet::decompiler::ilspycmd {

struct EnumName {
    const char* name;
    std::uint64_t value;
};

struct EnumInfo {
    std::span<const EnumName> names;   // by value, ties in .NET's order
    bool flags;                        // [Flags]
};

// Enum.ToString(): `value` is the enum's value as an unsigned number of its underlying type's width.
std::string enum_to_string(const EnumInfo& info, std::uint64_t value);

// Enum.TryParse(text, ignoreCase, out value): white space around the text and each comma separated name is ignored; a number (an
// optional sign, then digits) is any value of the underlying type (`bits` wide, `is_signed`), defined or not; names join by OR.
// Returns false for text that is none of that.
bool enum_try_parse(const EnumInfo& info, std::string_view text, bool ignore_case, int bits, bool is_signed, std::uint64_t* value);

namespace enums {
const EnumInfo& type_attributes();                // System.Reflection.TypeAttributes
const EnumInfo& field_attributes();               // System.Reflection.FieldAttributes
const EnumInfo& method_impl_attributes();         // System.Reflection.MethodImplAttributes (not [Flags])
const EnumInfo& method_attributes();              // System.Reflection.MethodAttributes
const EnumInfo& parameter_attributes();           // System.Reflection.ParameterAttributes
const EnumInfo& event_attributes();               // System.Reflection.EventAttributes
const EnumInfo& property_attributes();            // System.Reflection.PropertyAttributes
const EnumInfo& manifest_resource_attributes();   // System.Reflection.ManifestResourceAttributes
const EnumInfo& generic_parameter_attributes();   // System.Reflection.GenericParameterAttributes
const EnumInfo& assembly_flags();                 // System.Reflection.AssemblyFlags
const EnumInfo& method_import_attributes();       // System.Reflection.MethodImportAttributes
const EnumInfo& assembly_hash_algorithm();        // System.Reflection.AssemblyHashAlgorithm (not [Flags])
const EnumInfo& constant_type_code();             // System.Reflection.Metadata.ConstantTypeCode (not [Flags])
const EnumInfo& declarative_security_action();    // System.Reflection.DeclarativeSecurityAction (not [Flags])
const EnumInfo& method_semantics_attributes();    // System.Reflection.MethodSemanticsAttributes
const EnumInfo& table_index();                    // System.Reflection.Metadata.Ecma335.TableIndex (not [Flags])
const EnumInfo& language_version();               // ICSharpCode.Decompiler.CSharp.LanguageVersion (not [Flags])
}  // namespace enums

}  // namespace cb::dotnet::decompiler::ilspycmd
