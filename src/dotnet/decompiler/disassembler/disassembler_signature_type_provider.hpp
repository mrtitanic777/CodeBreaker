// dotnet/decompiler/disassembler/disassembler_signature_type_provider.hpp -- a signature decoded into IL text: each type becomes a
// function that writes it to the output in a given ILNameSyntax ("class [System.Runtime]System.Type", "!!0", "int32[0...]").
//
// Ported from ILSpy (ICSharpCode.Decompiler.Disassembler: DisassemblerSignatureTypeProvider), Copyright AlphaSierraPapa for the ILSpy
// team, MIT (see NOTICE in dotnet/decompiler). Upstream's Action<ILNameSyntax> is a std::function; each one holds a copy of the provider
// (its module and output), so it may be called after the provider that made it is gone.
#pragma once

#include <functional>
#include <vector>

#include "dotnet/decompiler/disassembler/disassembler_helpers.hpp"
#include "dotnet/metadata/signature_decoder.hpp"

namespace cb::dotnet::decompiler::disassembler {

using TypeWriter = std::function<void(ILNameSyntax)>;

class DisassemblerSignatureTypeProvider {
public:
    DisassemblerSignatureTypeProvider(const metadata::MetadataFile& module, ITextOutput& output)
        : module_(module), metadata_(module.metadata()), output_(output) {}

    TypeWriter get_array_type(TypeWriter element, const srm::ArrayShape& shape);
    TypeWriter get_by_reference_type(TypeWriter element);
    TypeWriter get_function_pointer_type(const srm::MethodSignature<TypeWriter>& signature);
    TypeWriter get_generic_instantiation(TypeWriter generic, const std::vector<TypeWriter>& arguments);
    TypeWriter get_generic_method_parameter(const metadata::MetadataGenericContext& context, int index);
    TypeWriter get_generic_type_parameter(const metadata::MetadataGenericContext& context, int index);
    TypeWriter get_modified_type(TypeWriter modifier, TypeWriter unmodified, bool is_required);
    TypeWriter get_pinned_type(TypeWriter element);
    TypeWriter get_pointer_type(TypeWriter element);
    TypeWriter get_primitive_type(srm::PrimitiveTypeCode code);
    TypeWriter get_sz_array_type(TypeWriter element);
    TypeWriter get_type_from_definition(const srm::MetadataReader& reader, srm::TypeDefinitionHandle handle, std::uint8_t raw_type_kind);
    TypeWriter get_type_from_reference(const srm::MetadataReader& reader, srm::TypeReferenceHandle handle, std::uint8_t raw_type_kind);
    TypeWriter get_type_from_specification(const srm::MetadataReader& reader, const metadata::MetadataGenericContext& context,
                                           srm::TypeSpecificationHandle handle, std::uint8_t raw_type_kind);

private:
    void write_type_parameter(srm::GenericParameterHandle parameter, int index, ILNameSyntax syntax) const;
    TypeWriter write_type_handle(srm::EntityHandle handle, std::uint8_t raw_type_kind);

    const metadata::MetadataFile& module_;
    const srm::MetadataReader& metadata_;
    ITextOutput& output_;
};

}  // namespace cb::dotnet::decompiler::disassembler
