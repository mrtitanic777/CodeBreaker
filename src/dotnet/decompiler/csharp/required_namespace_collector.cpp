// dotnet/decompiler/csharp/required_namespace_collector.cpp -- see required_namespace_collector.hpp.
#include "dotnet/decompiler/csharp/required_namespace_collector.hpp"

#include <variant>

#include "dotnet/decompiler/csharp/csharp_decompiler.hpp"
#include "dotnet/decompiler/disassembler/il_parser.hpp"
#include "dotnet/decompiler/metadata/code_mapping_info.hpp"
#include "dotnet/decompiler/metadata/il_opcodes.hpp"
#include "dotnet/decompiler/metadata/metadata_token_helpers.hpp"
#include "dotnet/decompiler/srm_extensions.hpp"
#include "dotnet/decompiler/typesystem/known_type_reference.hpp"
#include "dotnet/decompiler/typesystem/metadata_module.hpp"
#include "dotnet/decompiler/typesystem/tuple_type.hpp"
#include "dotnet/decompiler/typesystem/type_system_extensions.hpp"
#include "dotnet/decompiler/typesystem/types.hpp"

#include "dotnet/decompiler/typesystem/extension_info.hpp"

namespace cb::dotnet::decompiler::csharp {

using metadata::OperandType;
using srm::HandleKind;
using typesystem::GenericContext;
using typesystem::IAttribute;
using typesystem::IEntity;
using typesystem::IEvent;
using typesystem::IField;
using typesystem::IMember;
using typesystem::IMethod;
using typesystem::IProperty;
using typesystem::IType;
using typesystem::ITypeDefinition;
using typesystem::ITypeParameter;
using typesystem::MetadataModule;

namespace {

// Upstream's `static readonly GenericContext genericContext = default`: no type parameters.
const GenericContext generic_context;

}  // namespace

RequiredNamespaceCollector::RequiredNamespaceCollector(NamespaceSet& namespaces) : namespaces_(namespaces) {
    for (int i = 0; i < typesystem::kKnownTypeCodeCount; i++) {
        const auto* ktr = typesystem::KnownTypeReference::get(static_cast<typesystem::KnownTypeCode>(i));
        if (ktr == nullptr) continue;
        namespaces_.insert(ktr->namespace_());
    }
}

void RequiredNamespaceCollector::collect_namespaces(const MetadataModule& module, NamespaceSet& namespaces) {
    RequiredNamespaceCollector collector(namespaces);
    for (const ITypeDefinition* type : module.type_definitions()) collector.collect_namespaces(type, module, MappingInfo());
    collector.handle_attributes(module.get_assembly_attributes());
    collector.handle_attributes(module.get_module_attributes());
}

void RequiredNamespaceCollector::collect_attribute_namespaces(const MetadataModule& module, NamespaceSet& namespaces) {
    RequiredNamespaceCollector collector(namespaces);
    collector.handle_attributes(module.get_assembly_attributes());
    collector.handle_attributes(module.get_module_attributes());
}

void RequiredNamespaceCollector::collect_namespaces(const IEntity* entity, const MetadataModule& module, NamespaceSet& namespaces) {
    RequiredNamespaceCollector collector(namespaces);
    collector.collect_namespaces(entity, module, MappingInfo());
}

void RequiredNamespaceCollector::collect_namespaces(srm::EntityHandle entity, const MetadataModule& module, NamespaceSet& namespaces) {
    if (entity.is_nil()) return;
    collect_namespaces(module.resolve_entity(entity, generic_context), module, namespaces);
}

void RequiredNamespaceCollector::collect_namespaces(const IEntity* entity, const MetadataModule& module, MappingInfo mapping_info) {
    if (entity == nullptr || entity->metadata_token().is_nil() || module.metadata_file() == nullptr) return;
    const metadata::MetadataFile& cor_file = *module.metadata_file();
    if (mapping_info == nullptr)
        mapping_info = CSharpDecompiler::get_code_mapping_info(cor_file, entity->metadata_token());
    if (const auto* td = dynamic_cast<const ITypeDefinition*>(entity)) {
        const typesystem::ExtensionInfo* extension_info = nullptr;
        if (const ITypeDefinition* declaring = td->declaring_type_definition()) {
            extension_info = declaring->extension_info();
            if (extension_info == nullptr && declaring->declaring_type_definition() != nullptr)
                extension_info = declaring->declaring_type_definition()->extension_info();
        }
        if (extension_info != nullptr) {
            const ITypeDefinition* marker_out = nullptr;
            if (extension_info->is_extension_marker_type(td, &marker_out) || extension_info->is_extension_group_type(td))
                td = extension_info->container();
        }

        namespaces_.insert(td->namespace_());
        handle_attributes(td->get_attributes());
        handle_type_parameters(td->type_parameters());

        for (const IType* base_type : td->direct_base_types()) collect_namespaces_for_type_reference(base_type);

        for (const ITypeDefinition* nested_type : td->nested_types()) {
            bool is_group_or_marker = false;
            if (extension_info != nullptr) {
                const ITypeDefinition* marker_out = nullptr;
                is_group_or_marker = extension_info->is_extension_group_type(nested_type) || extension_info->is_extension_marker_type(nested_type, &marker_out);
            }
            if (is_group_or_marker) continue;
            collect_namespaces(nested_type, module, mapping_info);
        }

        for (const IField* field : td->fields()) collect_namespaces(field, module, mapping_info);
        for (const IProperty* property : td->properties()) collect_namespaces(property, module, mapping_info);
        for (const IEvent* ev : td->events()) collect_namespaces(ev, module, mapping_info);
        for (const IMethod* method : td->methods()) collect_namespaces(method, module, mapping_info);
    } else if (const auto* field = dynamic_cast<const IField*>(entity)) {
        handle_attributes(field->get_attributes());
        collect_namespaces_for_type_reference(field->return_type());
    } else if (const auto* method = dynamic_cast<const IMethod*>(entity)) {
        std::vector<srm::MethodDefinitionHandle> parts = mapping_info->get_method_parts(srm::MethodDefinitionHandle(method->metadata_token()));
        for (srm::MethodDefinitionHandle part : parts) {
            const IMethod* part_method = module.resolve_method(part, generic_context);
            handle_attributes(part_method->get_attributes());
            handle_attributes(part_method->get_return_type_attributes());
            collect_namespaces_for_type_reference(part_method->return_type());
            for (const typesystem::IParameter* param : part_method->parameters()) {
                handle_attributes(param->get_attributes());
                collect_namespaces_for_type_reference(param->type());
            }
            handle_type_parameters(part_method->type_parameters());
            handle_overrides(decompiler::get_method_implementations(part, module.metadata()), module);
            srm::MethodDefinition method_def = module.metadata().get_method_definition(part);
            if (method->has_body()) {
                try {
                    srm::MethodBodyBlock body = module.metadata_file()->get_method_body(method_def.relative_virtual_address());
                    collect_namespaces_from_method_body(body, module);
                } catch (const srm::BadImageFormatError&) {
                    continue;
                }
            }
        }
    } else if (const auto* property = dynamic_cast<const IProperty*>(entity)) {
        handle_attributes(property->get_attributes());
        collect_namespaces_for_type_reference(property->return_type());
        collect_namespaces(property->getter(), module, mapping_info);
        collect_namespaces(property->setter(), module, mapping_info);
    } else if (const auto* ev = dynamic_cast<const IEvent*>(entity)) {
        handle_attributes(ev->get_attributes());
        collect_namespaces_for_type_reference(ev->return_type());
        collect_namespaces(ev->add_accessor(), module, mapping_info);
        collect_namespaces(ev->remove_accessor(), module, mapping_info);
    }
}

void RequiredNamespaceCollector::handle_overrides(const std::vector<srm::MethodImplementationHandle>& method_impls, const MetadataModule& module) {
    for (srm::MethodImplementationHandle h : method_impls) {
        srm::MethodImplementation method_impl = module.metadata().get_method_implementation(h);
        collect_namespaces_for_type_reference(module.resolve_type(method_impl.type(), generic_context));
        collect_namespaces_for_member_reference(module.resolve_method(method_impl.method_body(), generic_context));
        collect_namespaces_for_member_reference(module.resolve_method(method_impl.method_declaration(), generic_context));
    }
}

void RequiredNamespaceCollector::collect_namespaces_for_type_reference(const IType* type) {
    if (!visited_types_.insert(type).second) return;
    if (const auto* parameterized_type = dynamic_cast<const typesystem::ParameterizedType*>(type)) {
        namespaces_.insert(parameterized_type->namespace_());
        collect_namespaces_for_type_reference(parameterized_type->generic_type());
        for (const IType* arg : parameterized_type->type_arguments()) collect_namespaces_for_type_reference(arg);
        return;   // no need to collect base types again
    }
    if (const auto* type_with_element_type = dynamic_cast<const typesystem::TypeWithElementType*>(type)) {
        collect_namespaces_for_type_reference(type_with_element_type->element_type());
    } else if (const auto* tuple_type = dynamic_cast<const typesystem::TupleType*>(type)) {
        for (const IType* element_type : tuple_type->element_types()) collect_namespaces_for_type_reference(element_type);
    } else if (const auto* fn_ptr_type = dynamic_cast<const typesystem::FunctionPointerType*>(type)) {
        collect_namespaces_for_type_reference(fn_ptr_type->return_type);
        for (const IType* param_type : fn_ptr_type->parameter_types) collect_namespaces_for_type_reference(param_type);
    } else {
        namespaces_.insert(type->namespace_());
    }
    for (const IType* base_type : typesystem::get_all_base_types(type)) namespaces_.insert(base_type->namespace_());
}

void RequiredNamespaceCollector::handle_attributes(const std::vector<const IAttribute*>& attributes) {
    for (const IAttribute* attr : attributes) {
        namespaces_.insert(attr->attribute_type()->namespace_());
        for (const auto& arg : attr->fixed_arguments()) handle_attribute_value(arg.type, arg.value);
        for (const auto& arg : attr->named_arguments()) handle_attribute_value(arg.type, arg.value);
    }
}

void RequiredNamespaceCollector::handle_attribute_value(const IType* type, const srm::CustomAttributeArgumentValue<const IType*>& value) {
    collect_namespaces_for_type_reference(type);
    if (const auto* typeof_type = std::get_if<const IType*>(&value)) collect_namespaces_for_type_reference(*typeof_type);
    if (const auto* arr = std::get_if<std::vector<srm::CustomAttributeTypedArgument<const IType*>>>(&value)) {
        for (const auto& element : *arr) handle_attribute_value(element.type, element.value);
    }
}

void RequiredNamespaceCollector::handle_type_parameters(const std::vector<const ITypeParameter*>& type_parameters) {
    for (const ITypeParameter* type_param : type_parameters) {
        handle_attributes(type_param->get_attributes());
        for (const IType* constraint : type_param->direct_base_types()) collect_namespaces_for_type_reference(constraint);
    }
}

void RequiredNamespaceCollector::collect_namespaces_from_method_body(const srm::MethodBodyBlock& method, const MetadataModule& module) {
    const srm::MetadataReader& metadata = module.metadata();
    srm::BlobReader instructions = method.il_reader();

    if (!method.local_signature().is_nil()) {
        std::vector<const IType*> local_signature;
        try {
            local_signature = module.decode_local_signature(method.local_signature(), generic_context);
        } catch (const srm::BadImageFormatError&) {
            // Issue #1211: ignore invalid local signatures
            local_signature.clear();
        }
        for (const IType* type : local_signature) collect_namespaces_for_type_reference(type);
    }

    for (const srm::ExceptionRegion& region : method.exception_regions()) {
        if (region.catch_type.is_nil()) continue;
        const IType* ty;
        try {
            ty = module.resolve_type(region.catch_type, generic_context);
        } catch (const srm::BadImageFormatError&) {
            continue;
        }
        collect_namespaces_for_type_reference(ty);
    }

    while (instructions.remaining_bytes() > 0) {
        srm::ILOpCode op_code;
        try {
            op_code = disassembler::decode_op_code(instructions);
        } catch (const srm::BadImageFormatError&) {
            return;
        }
        switch (metadata::get_operand_type(op_code)) {
        case OperandType::Field:
        case OperandType::Method:
        case OperandType::Sig:
        case OperandType::Tok:
        case OperandType::Type: {
            srm::EntityHandle handle;
            try {
                handle = metadata::entity_handle_or_nil(instructions.read_int32());
            } catch (const srm::BadImageFormatError&) {
                return;
            }
            if (handle.is_nil()) break;
            switch (handle.kind()) {
            case HandleKind::TypeDefinition:
            case HandleKind::TypeReference:
            case HandleKind::TypeSpecification: {
                const IType* type;
                try {
                    type = module.resolve_type(handle, generic_context);
                } catch (const srm::BadImageFormatError&) {
                    break;
                }
                collect_namespaces_for_type_reference(type);
                break;
            }
            case HandleKind::FieldDefinition:
            case HandleKind::MethodDefinition:
            case HandleKind::MethodSpecification:
            case HandleKind::MemberReference: {
                const IMember* member;
                try {
                    member = dynamic_cast<const IMember*>(module.resolve_entity(handle, generic_context));
                } catch (const srm::BadImageFormatError&) {
                    break;
                }
                collect_namespaces_for_member_reference(member);
                break;
            }
            case HandleKind::StandaloneSignature: {
                srm::StandaloneSignatureKind kind;
                try {
                    kind = metadata.get_standalone_signature(srm::StandaloneSignatureHandle(handle)).get_kind();
                } catch (const srm::BadImageFormatError&) {
                    break;
                }
                if (kind == srm::StandaloneSignatureKind::Method) {
                    const typesystem::FunctionPointerType* fpt;
                    try {
                        fpt = module.decode_method_signature(srm::StandaloneSignatureHandle(handle), generic_context).second;
                    } catch (const srm::BadImageFormatError&) {
                        break;
                    }
                    collect_namespaces_for_type_reference(fpt);
                }
                break;
            }
            default:
                break;
            }
            break;
        }
        default:
            try {
                disassembler::skip_operand(instructions, op_code);
            } catch (const srm::BadImageFormatError&) {
                return;
            }
            break;
        }
    }
}

void RequiredNamespaceCollector::collect_namespaces_for_member_reference(const IMember* member) {
    if (const auto* field = dynamic_cast<const IField*>(member)) {
        collect_namespaces_for_type_reference(field->declaring_type());
        collect_namespaces_for_type_reference(field->return_type());
    } else if (const auto* method = dynamic_cast<const IMethod*>(member)) {
        collect_namespaces_for_type_reference(method->declaring_type());
        collect_namespaces_for_type_reference(method->return_type());
        for (const typesystem::IParameter* param : method->parameters()) collect_namespaces_for_type_reference(param->type());
        for (const IType* arg : method->type_arguments()) collect_namespaces_for_type_reference(arg);
    }
}

}  // namespace cb::dotnet::decompiler::csharp
