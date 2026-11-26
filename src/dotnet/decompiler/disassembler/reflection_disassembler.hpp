// dotnet/decompiler/disassembler/reflection_disassembler.hpp -- a module as IL text, as ILSpy's IL view and `ilspycmd -il` write it: types
// with their attributes, layout, interfaces, nested types, fields (with .data), methods (with their bodies), events and properties; the
// assembly and module headers and the assembly references.
//
//     auto module = metadata::PEFile::open("Sample.dll");
//     PlainTextOutput out;
//     ReflectionDisassembler d(out, util::CancellationToken());
//     d.write_module_contents(*module);
//     out.text();
//
// Ported from ILSpy (ICSharpCode.Decompiler.Disassembler: ReflectionDisassembler), Copyright AlphaSierraPapa for the ILSpy team, MIT (see
// NOTICE in dotnet/decompiler). Debug information (sequence points, local names from a PDB) comes with DebugInfo.
#pragma once

#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "dotnet/decompiler/disassembler/entity_processor.hpp"
#include "dotnet/decompiler/disassembler/method_body_disassembler.hpp"
#include "dotnet/decompiler/metadata/assembly_references.hpp"

namespace cb::dotnet::decompiler::disassembler {

// A metadata token as a comment ("/* 06000001 */"): always when the handle could not be read (nullopt), else when tokens are shown.
void write_metadata_token(ITextOutput& output, const metadata::MetadataFile& module, std::optional<srm::Handle> handle, int metadata_token,
                          bool space_after, bool space_before, bool show_metadata_tokens, bool base10);

// What `ilspycmd -il` writes for a module (ILSpyCmd's ShowIL): "// IL code: <name>", then the module's contents.
std::string show_il(const metadata::MetadataFile& module);

class ReflectionDisassembler {
public:
    ReflectionDisassembler(ITextOutput& output, util::CancellationToken cancellation_token);
    ReflectionDisassembler(ITextOutput& output, std::unique_ptr<MethodBodyDisassembler> method_body_disassembler, util::CancellationToken cancellation_token);

    MethodBodyDisassembler& method_body_disassembler() { return *method_body_disassembler_; }
    bool expand_member_definitions = false;
    bool decode_custom_attribute_blobs = false;
    std::shared_ptr<metadata::IAssemblyResolver> assembly_resolver;   // nullptr: security declarations and attribute values stay blobs (as `ilspycmd -il`)
    IEntityProcessor* entity_processor = nullptr;

    void disassemble_method(const metadata::MetadataFile& module, srm::MethodDefinitionHandle handle);
    void disassemble_method_header(const metadata::MetadataFile& module, srm::MethodDefinitionHandle handle);
    void disassemble_field(const metadata::MetadataFile& module, srm::FieldDefinitionHandle handle);
    void disassemble_field_header(const metadata::MetadataFile& module, srm::FieldDefinitionHandle handle);
    void disassemble_property(const metadata::MetadataFile& module, srm::PropertyDefinitionHandle property);
    void disassemble_property_header(const metadata::MetadataFile& module, srm::PropertyDefinitionHandle property);
    void disassemble_event(const metadata::MetadataFile& module, srm::EventDefinitionHandle handle);
    void disassemble_event_header(const metadata::MetadataFile& module, srm::EventDefinitionHandle handle);
    void disassemble_type(const metadata::MetadataFile& module, srm::TypeDefinitionHandle type);
    void disassemble_type_header(const metadata::MetadataFile& module, srm::TypeDefinitionHandle type);
    void disassemble_namespace(std::string_view name_space, const metadata::MetadataFile& module, const std::vector<srm::TypeDefinitionHandle>& types);
    void write_assembly_header(const metadata::MetadataFile& module);
    void write_assembly_references(const srm::MetadataReader& metadata);
    void write_module_header(const metadata::MetadataFile& module, bool skip_mvid = false);
    void write_module_contents(const metadata::MetadataFile& module);

private:
    struct SecurityType;   // a security or attribute value's type: (PrimitiveTypeCode Code, string Name)
    class SecurityDeclarationDecoder;

    void disassemble_method_header_internal(const metadata::MetadataFile& module, srm::MethodDefinitionHandle handle, const metadata::MetadataGenericContext& context);
    void disassemble_method_block(const metadata::MetadataFile& module, srm::MethodDefinitionHandle handle, const metadata::MetadataGenericContext& context);
    void write_security_declarations(const metadata::MetadataFile& module, const std::vector<srm::DeclarativeSecurityAttributeHandle>& declarations);
    void try_decode_security_declaration(TextOutputWithRollback& output, srm::BlobReader blob, const metadata::MetadataFile& module);
    template <class Value>
    void write_value(ITextOutput& output, const SecurityType& type, const Value& value);
    void write_marshal_info(srm::BlobReader marshal_info);
    void write_native_type(srm::BlobReader& blob);
    template <class Params>
    void write_parameters(const srm::MetadataReader& metadata, const Params& parameters, const srm::MethodSignature<TypeWriter>& signature);
    void write_generic_parameters_and_attributes(const metadata::MetadataFile& module, const metadata::MetadataGenericContext& context, srm::GenericParameterHandle handle);
    void write_parameter_attributes(const metadata::MetadataFile& module, srm::ParameterHandle handle);
    void write_constant(const srm::MetadataReader& metadata, const srm::Constant& constant);
    char disassemble_field_header_internal(const metadata::MetadataFile& module, srm::FieldDefinitionHandle handle, const srm::MetadataReader& metadata,
                                           const srm::FieldDefinition& field);
    char get_rva_section_prefix(const metadata::MetadataFile& module, int rva);
    srm::PropertyAccessors disassemble_property_header_internal(const metadata::MetadataFile& module, srm::PropertyDefinitionHandle handle,
                                                                const srm::MetadataReader& metadata, const srm::PropertyDefinition& property);
    void write_nested_method(std::string_view keyword, const metadata::MetadataFile& module, srm::MethodDefinitionHandle method);
    void disassemble_event_header_internal(const metadata::MetadataFile& module, srm::EventDefinitionHandle handle, const srm::EventDefinition& event,
                                           const srm::EventAccessors& accessors);
    void disassemble_type_header_internal(const metadata::MetadataFile& module, srm::TypeDefinitionHandle handle, const srm::TypeDefinition& type,
                                          const metadata::MetadataGenericContext& context);
    void write_type_parameters(ITextOutput& output, const metadata::MetadataFile& module, const metadata::MetadataGenericContext& context,
                               const std::vector<srm::GenericParameterHandle>& parameters);
    template <class H>
    std::vector<H> process(const metadata::MetadataFile& module, std::vector<H> items);
    void write_attributes(const metadata::MetadataFile& module, const std::vector<srm::CustomAttributeHandle>& attributes);
    void write_decoded_custom_attribute_blob(const srm::CustomAttribute& attribute, const metadata::MetadataFile& module);
    void write_blob(srm::BlobHandle blob, const srm::MetadataReader& metadata);
    void write_blob(srm::BlobReader reader);
    void open_block(bool default_collapsed);
    void close_block(std::optional<std::string_view> comment = std::nullopt);

    ITextOutput& output_;
    util::CancellationToken cancellation_token_;
    bool is_in_type_ = false;   // inside a whole type (folds collapse by default)
    std::unique_ptr<MethodBodyDisassembler> method_body_disassembler_;
};

}  // namespace cb::dotnet::decompiler::disassembler
