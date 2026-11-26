// dotnet/decompiler/disassembler/disassembler_helpers.hpp -- the small pieces of IL text: offsets (IL_002a), names quoted where ILAsm
// needs it, string and number operands, parameter and local references, exception handlers.
//
//     escape("Main") == "Main";   escape("<Main>$") == "'<Main>$'";   offset_to_string(42) == "IL_002a";
//
// Ported from ILSpy (ICSharpCode.Decompiler.Disassembler: ILNameSyntax, DisassemblerHelpers), Copyright AlphaSierraPapa for the ILSpy
// team, MIT (see NOTICE in dotnet/decompiler). Names are UTF-8; whether a character is a letter is decided on UTF-16 code units, by .NET's
// rules (util/char_info). An operand of upstream's type object is a metadata ConstantValue.
#pragma once

#include <optional>
#include <string>
#include <string_view>

#include "dotnet/decompiler/metadata/metadata_file.hpp"
#include "dotnet/decompiler/output/text_output.hpp"
#include "dotnet/metadata/blob_reader.hpp"

namespace cb::dotnet::decompiler::disassembler {

namespace srm = cb::dotnet::metadata;

enum class ILNameSyntax : std::uint8_t {
    Signature,                       // a type in a signature (class/valuetype written, type parameters by name)
    SignatureNoNamedTypeParameters,  // the same, type parameters by number
    TypeName,                        // a type as a name (no class/valuetype)
    ShortTypeName,                   // the name without namespace
};

std::string offset_to_string(long long offset);
void write_offset_reference(ITextOutput& writer, std::optional<int> offset);
void write_to(const srm::ExceptionRegion& handler, const metadata::MetadataFile& module, const metadata::MetadataGenericContext& context,
              ITextOutput& writer);

// A name as it is, when ILAsm reads it as an identifier; else in single quotes, escaped.
std::string escape(std::string_view identifier);
// A string's characters for a quoted IL string (\" \\ \0 \a ... and \uXXXX for control, surrogate and uncommon space characters).
std::string escape_string(std::u16string_view str);
std::string escape_string(std::string_view utf8);

void write_parameter_reference(ITextOutput& writer, const srm::MetadataReader& metadata, srm::MethodDefinitionHandle handle, int index);
void write_variable_reference(ITextOutput& writer, const srm::MetadataReader& metadata, srm::MethodDefinitionHandle handle, int index);

// An operand of any type (upstream's WriteOperand(object)): a string quoted, a char as its number, a bool as true/false, numbers in the
// invariant culture. UsageError for null (upstream's ArgumentNullException).
void write_operand(ITextOutput& writer, const srm::ConstantValue& operand);
void write_operand(ITextOutput& writer, long long value);
void write_operand(ITextOutput& writer, float value);
void write_operand(ITextOutput& writer, double value);
void write_operand(ITextOutput& writer, std::u16string_view operand);

// "int8", "int32", "native int" ... for System.SByte, System.Int32, System.IntPtr ...; nullopt for the others.
std::optional<std::string> primitive_type_name(std::string_view full_name);

}  // namespace cb::dotnet::decompiler::disassembler
