// dotnet/decompiler/il/instruction_output_extensions.cpp -- InstructionOutputExtensions (the metadata half).
#include "dotnet/decompiler/il/instruction_output_extensions.hpp"

#include <format>

#include "dotnet/decompiler/metadata/metadata_extensions.hpp"
#include "dotnet/decompiler/srm_extensions.hpp"

namespace cb::dotnet::decompiler::il {
namespace {

using HK = srm::HandleKind;
using disassembler::DisassemblerSignatureTypeProvider;
using disassembler::escape;
using disassembler::TypeWriter;

constexpr std::uint16_t kMemberAccessMask = 0x0007, kPrivateScope = 0x0000;
constexpr std::uint16_t kReferenceTypeConstraint = 0x0004, kNotNullableValueTypeConstraint = 0x0008, kDefaultConstructorConstraint = 0x0010;
constexpr std::uint16_t kCovariant = 0x0001, kContravariant = 0x0002;

void write_type_parameter_list(ITextOutput& output, ILNameSyntax syntax, const std::vector<TypeWriter>& substitution) {
    output.write('<');
    for (std::size_t i = 0; i < substitution.size(); ++i) {
        if (i > 0) output.write(", ");
        substitution[i](syntax);
    }
    output.write('>');
}

void write_parent(ITextOutput& output, const metadata::MetadataFile& file, srm::EntityHandle parent, const metadata::MetadataGenericContext& context,
                  ILNameSyntax syntax) {
    switch (parent.kind()) {
    case HK::MethodDefinition: {
        const srm::MethodDefinition m = file.metadata().get_method_definition(srm::MethodDefinitionHandle(parent));
        write_to(m.get_declaring_type(), file, output, context, syntax);
        break;
    }
    case HK::ModuleReference:
        output.write('[');
        output.write(file.metadata().get_string(file.metadata().get_module_reference(srm::ModuleReferenceHandle(parent)).name()));
        output.write(']');
        break;
    case HK::TypeDefinition:
    case HK::TypeReference:
    case HK::TypeSpecification: write_to(parent, file, output, context, syntax); break;
    default: break;
    }
}

std::string pst_name(std::string_view name, srm::EntityHandle entity) { return std::format("{}$PST{:08X}", name, srm::get_token(entity)); }

void write_method_type_parameters(ITextOutput& output, const metadata::MetadataFile& module, const srm::MetadataReader& metadata, srm::EntityHandle entity,
                                  const std::vector<srm::GenericParameterHandle>& generic_parameters) {
    if (generic_parameters.empty()) return;
    output.write('<');
    for (std::size_t i = 0; i < generic_parameters.size(); ++i) {
        if (i > 0) output.write(", ");
        const srm::GenericParameter gp = metadata.get_generic_parameter(generic_parameters[i]);
        if ((gp.attributes() & kReferenceTypeConstraint) == kReferenceTypeConstraint) output.write("class ");
        else if ((gp.attributes() & kNotNullableValueTypeConstraint) == kNotNullableValueTypeConstraint) output.write("valuetype ");
        if ((gp.attributes() & kDefaultConstructorConstraint) == kDefaultConstructorConstraint) output.write(".ctor ");
        const auto constraints = gp.get_constraints();
        if (!constraints.empty()) {
            output.write('(');
            for (std::size_t j = 0; j < constraints.size(); ++j) {
                if (j > 0) output.write(", ");
                const srm::GenericParameterConstraint c = metadata.get_generic_parameter_constraint(constraints[j]);
                write_to(c.type(), module, output, metadata::MetadataGenericContext(srm::MethodDefinitionHandle(entity), metadata), ILNameSyntax::TypeName);
            }
            output.write(") ");
        }
        if ((gp.attributes() & kContravariant) == kContravariant) output.write('-');
        else if ((gp.attributes() & kCovariant) == kCovariant) output.write('+');
        output.write(escape(metadata.get_string(gp.name())));
    }
    output.write('>');
}

}  // namespace

void write_to(srm::EntityHandle entity, const metadata::MetadataFile& module, ITextOutput& output, const metadata::MetadataGenericContext& generic_context,
              ILNameSyntax syntax) {
    if (entity.is_nil()) {
        output.write("<nil>");
        return;
    }
    const srm::MetadataReader& metadata = module.metadata();
    switch (entity.kind()) {
    case HK::TypeDefinition: {
        const srm::TypeDefinition td = metadata.get_type_definition(srm::TypeDefinitionHandle(entity));
        output.write_reference(module, entity, metadata::to_il_name_string(get_full_type_name(td, metadata)));
        break;
    }
    case HK::TypeReference: {
        const srm::TypeReference tr = metadata.get_type_reference(srm::TypeReferenceHandle(entity));
        srm::EntityHandle scope;
        try {
            scope = tr.resolution_scope();
        } catch (const srm::BadImageFormatError&) {
            scope = {};
        }
        if (!scope.is_nil()) {
            output.write("[");
            srm::TypeReference current = tr;
            while (current.resolution_scope().kind() == HK::TypeReference)
                current = metadata.get_type_reference(srm::TypeReferenceHandle(current.resolution_scope()));
            switch (current.resolution_scope().kind()) {
            case HK::ModuleDefinition: output.write(escape(metadata.get_string(metadata.get_module_definition().name()))); break;
            case HK::ModuleReference: break;
            case HK::AssemblyReference:
                output.write(escape(metadata.get_string(metadata.get_assembly_reference(srm::AssemblyReferenceHandle(current.resolution_scope())).name())));
                break;
            default: break;
            }
            output.write("]");
        }
        output.write_reference(module, entity, metadata::to_il_name_string(get_full_type_name(entity, metadata)));
        break;
    }
    case HK::TypeSpecification: {
        DisassemblerSignatureTypeProvider provider(module, output);
        const TypeWriter signature = metadata.get_type_specification(srm::TypeSpecificationHandle(entity)).decode_signature(provider, generic_context);
        signature(syntax);
        break;
    }
    case HK::FieldDefinition: {
        const srm::FieldDefinition fd = metadata.get_field_definition(srm::FieldDefinitionHandle(entity));
        DisassemblerSignatureTypeProvider provider(module, output);
        const TypeWriter signature = fd.decode_signature(provider, metadata::MetadataGenericContext(fd.get_declaring_type(), metadata));
        signature(ILNameSyntax::SignatureNoNamedTypeParameters);
        output.write(' ');
        write_to(fd.get_declaring_type(), module, output, metadata::MetadataGenericContext(), ILNameSyntax::TypeName);
        output.write("::");
        output.write_reference(module, entity, escape(metadata.get_string(fd.name())));
        break;
    }
    case HK::MethodDefinition: {
        const srm::MethodDefinition md = metadata.get_method_definition(srm::MethodDefinitionHandle(entity));
        DisassemblerSignatureTypeProvider provider(module, output);
        const auto method_signature = md.decode_signature(provider, metadata::MetadataGenericContext(srm::MethodDefinitionHandle(entity), metadata));
        write_to(method_signature.header, output);
        method_signature.return_type(ILNameSyntax::SignatureNoNamedTypeParameters);
        output.write(' ');
        const srm::TypeDefinitionHandle declaring = md.get_declaring_type();
        if (!declaring.is_nil()) {
            write_to(declaring, module, output, generic_context, ILNameSyntax::TypeName);
            output.write("::");
        }
        const bool compiler_controlled = (md.attributes() & kMemberAccessMask) == kPrivateScope;
        if (compiler_controlled) output.write_reference(module, entity, escape(pst_name(metadata.get_string(md.name()), entity)));
        else output.write_reference(module, entity, escape(metadata.get_string(md.name())));
        write_method_type_parameters(output, module, metadata, entity, md.get_generic_parameters());
        write_parameter_list(output, method_signature);
        break;
    }
    case HK::MemberReference: {
        const srm::MemberReference mr = metadata.get_member_reference(srm::MemberReferenceHandle(entity));
        const std::string member_name(metadata.get_string(mr.name()));
        DisassemblerSignatureTypeProvider provider(module, output);
        switch (mr.get_kind()) {
        case srm::MemberReferenceKind::Method: {
            const auto method_signature = mr.decode_method_signature(provider, generic_context);
            write_to(method_signature.header, output);
            method_signature.return_type(ILNameSyntax::SignatureNoNamedTypeParameters);
            output.write(' ');
            write_parent(output, module, mr.parent(), generic_context, syntax);
            output.write("::");
            output.write_reference(module, entity, escape(member_name));
            write_parameter_list(output, method_signature);
            break;
        }
        case srm::MemberReferenceKind::Field: {
            const TypeWriter field_signature = mr.decode_field_signature(provider, generic_context);
            field_signature(ILNameSyntax::SignatureNoNamedTypeParameters);
            output.write(' ');
            write_parent(output, module, mr.parent(), generic_context, syntax);
            output.write("::");
            output.write_reference(module, entity, escape(member_name));
            break;
        }
        }
        break;
    }
    case HK::MethodSpecification: {
        const srm::MethodSpecification ms = metadata.get_method_specification(srm::MethodSpecificationHandle(entity));
        DisassemblerSignatureTypeProvider provider(module, output);
        const std::vector<TypeWriter> substitution = ms.decode_signature(provider, generic_context);
        switch (ms.method().kind()) {
        case HK::MethodDefinition: {
            const srm::MethodDefinition definition = metadata.get_method_definition(srm::MethodDefinitionHandle(ms.method()));
            const std::string method_name(metadata.get_string(definition.name()));
            const auto method_signature = definition.decode_signature(provider, generic_context);
            write_to(method_signature.header, output);
            method_signature.return_type(ILNameSyntax::SignatureNoNamedTypeParameters);
            output.write(' ');
            const srm::TypeDefinitionHandle declaring = definition.get_declaring_type();
            if (!declaring.is_nil()) {
                write_to(declaring, module, output, generic_context, ILNameSyntax::TypeName);
                output.write("::");
            }
            const bool compiler_controlled = (definition.attributes() & kMemberAccessMask) == kPrivateScope;
            output.write(escape(compiler_controlled ? pst_name(method_name, ms.method()) : method_name));
            write_type_parameter_list(output, syntax, substitution);
            write_parameter_list(output, method_signature);
            break;
        }
        case HK::MemberReference: {
            const srm::MemberReference reference = metadata.get_member_reference(srm::MemberReferenceHandle(ms.method()));
            const std::string member_name(metadata.get_string(reference.name()));
            const auto method_signature = reference.decode_method_signature(provider, generic_context);
            write_to(method_signature.header, output);
            method_signature.return_type(ILNameSyntax::SignatureNoNamedTypeParameters);
            output.write(' ');
            write_parent(output, module, reference.parent(), generic_context, syntax);
            output.write("::");
            output.write(escape(member_name));
            write_type_parameter_list(output, syntax, substitution);
            write_parameter_list(output, method_signature);
            break;
        }
        default: break;
        }
        break;
    }
    case HK::StandaloneSignature: {
        const srm::StandaloneSignature sig = metadata.get_standalone_signature(srm::StandaloneSignatureHandle(entity));
        const srm::SignatureHeader header = metadata.get_blob_reader(sig.signature()).read_signature_header();
        if (header.kind() == srm::SignatureKind::Method) {
            DisassemblerSignatureTypeProvider provider(module, output);
            const auto method_signature = sig.decode_method_signature(provider, generic_context);
            write_to(method_signature.header, output);
            method_signature.return_type(ILNameSyntax::SignatureNoNamedTypeParameters);
            write_parameter_list(output, method_signature);
        } else {
            static constexpr const char* kKinds[] = {"Method", "1", "2", "3", "4", "5", "Field", "LocalVariables", "Property", "9", "MethodSpecification"};
            const int k = static_cast<int>(header.kind());
            output.write(std::format("@{:08X} /* signature {} */", srm::get_token(entity), k <= 10 ? kKinds[k] : std::to_string(k).c_str()));
        }
        break;
    }
    default: output.write(std::format("@{:08X}", srm::get_token(entity))); break;
    }
}

void write_parameter_list(ITextOutput& output, const srm::MethodSignature<TypeWriter>& signature) {
    output.write("(");
    for (std::size_t i = 0; i < signature.parameter_types.size(); ++i) {
        if (i > 0) output.write(", ");
        if (static_cast<int>(i) == signature.required_parameter_count) output.write("..., ");
        signature.parameter_types[i](ILNameSyntax::SignatureNoNamedTypeParameters);
    }
    output.write(")");
}

void write_to(const srm::SignatureHeader& header, ITextOutput& output) {
    if (header.has_explicit_this()) output.write("instance explicit ");
    else if (header.is_instance()) output.write("instance ");
    if (header.calling_convention() != srm::SignatureCallingConvention::Default) {
        output.write(to_il_syntax(header.calling_convention()));
        output.write(' ');
    }
}

}  // namespace cb::dotnet::decompiler::il
