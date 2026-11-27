// dotnet/decompiler/csharp/output_visitor/csharp_ambience.hpp -- CSharpAmbience: writes a symbol (type, member, namespace), a type or a
// constant as C# text, as short or as complete as the ConversionFlags ask: `public void Run(int count)`, `List<T>`, `System.String.Length`.
//
//     CSharpAmbience ambience;
//     ambience.set_conversion_flags(ConversionFlags::StandardConversionFlags);
//     std::string text = ambience.convert_symbol(*method);
//     ambience.convert_type(*type);                           // "Dictionary<string, int>"
//     ambience.convert_constant_value(ConstantValue(std::int32_t(5)));    // "5"
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.OutputVisitor: CSharpAmbience), Copyright AlphaSierraPapa for the ILSpy team, MIT (see
// NOTICE in dotnet/decompiler).
#pragma once

#include <string>

#include "dotnet/decompiler/csharp/output_visitor/csharp_formatting_options.hpp"
#include "dotnet/decompiler/csharp/output_visitor/token_writer.hpp"
#include "dotnet/decompiler/csharp/syntax/type_system_ast_builder.hpp"
#include "dotnet/decompiler/output/ambience.hpp"

namespace cb::dotnet::decompiler::csharp::output_visitor {

class CSharpAmbience final : public IAmbience {
public:
    ConversionFlags conversion_flags() const override { return conversion_flags_; }
    void set_conversion_flags(ConversionFlags value) override { conversion_flags_ = value; }

    // The symbol written with empty formatting options.
    std::string convert_symbol(const typesystem::ISymbol& symbol) override;
    // The symbol written to `writer`.
    void convert_symbol(const typesystem::ISymbol& symbol, TokenWriter& writer, const CSharpFormattingOptions& formatting_policy);

    std::string convert_variable(const typesystem::IVariable& variable);
    std::string convert_type(const typesystem::IType& type) override;
    std::string convert_constant_value(const typesystem::ConstantValue& constant_value) override;
    std::string wrap_comment(const std::string& comment) override;

private:
    bool is_extension(const typesystem::ITypeDefinition* type_def, syntax::ExtensionGroup* extension_group) const;
    bool show_parameter_list(const typesystem::ISymbol& symbol) const;
    syntax::TypeSystemAstBuilder create_ast_builder() const;
    void write_type_declaration_name(const typesystem::ITypeDefinition& type_def, TokenWriter& writer, const CSharpFormattingOptions& formatting_policy);
    void write_member_declaration_name(const typesystem::IMember& member, TokenWriter& writer, const CSharpFormattingOptions& formatting_policy);
    void write_type_parameters(const std::vector<syntax::TypeParameterDeclaration*>& type_parameters, TokenWriter& writer,
                               const CSharpFormattingOptions& formatting_policy);
    void print_modifiers(syntax::Modifiers modifiers, TokenWriter& writer);
    void write_qualified_name(const std::string& name, TokenWriter& writer, const CSharpFormattingOptions& formatting_policy);
    void convert_type(const typesystem::IType& type, TokenWriter& writer, const CSharpFormattingOptions& formatting_policy);
    const typesystem::IType* get_explicit_interface_type(const typesystem::IMember& member);

    ConversionFlags conversion_flags_ = ConversionFlags::None;
};

}  // namespace cb::dotnet::decompiler::csharp::output_visitor
