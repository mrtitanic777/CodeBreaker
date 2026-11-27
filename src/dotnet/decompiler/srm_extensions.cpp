// dotnet/decompiler/srm_extensions.cpp -- SRMExtensions and FullTypeNameSignatureDecoder.
#include "dotnet/decompiler/srm_extensions.hpp"

#include <format>
#include <stdexcept>

#include "dotnet/decompiler/metadata/metadata_extensions.hpp"
#include "dotnet/decompiler/metadata/metadata_file.hpp"

namespace cb::dotnet::decompiler {
namespace {

using HK = srm::HandleKind;
using typesystem::KnownTypeCode;

bool signature_is_known_type(const srm::MetadataReader& reader, const typesystem::TopLevelTypeName& known, srm::BlobReader& blob) {
    const auto code = blob.try_read_compressed_integer();
    if (!code) return false;
    auto is = [&](KnownTypeCode k) { return typesystem::is_known_type(known, k); };
    switch (*code) {
    case 0x1: return is(KnownTypeCode::Void);
    case 0x2: return is(KnownTypeCode::Boolean);
    case 0x3: return is(KnownTypeCode::Char);
    case 0x4: return is(KnownTypeCode::SByte);
    case 0x5: return is(KnownTypeCode::Byte);
    case 0x6: return is(KnownTypeCode::Int16);
    case 0x7: return is(KnownTypeCode::UInt16);
    case 0x8: return is(KnownTypeCode::Int32);
    case 0x9: return is(KnownTypeCode::UInt32);
    case 0xA: return is(KnownTypeCode::Int64);
    case 0xB: return is(KnownTypeCode::UInt64);
    case 0xC: return is(KnownTypeCode::Single);
    case 0xD: return is(KnownTypeCode::Double);
    case 0xE: return is(KnownTypeCode::String);
    case 0x16: return is(KnownTypeCode::TypedReference);
    case 0x18: return is(KnownTypeCode::IntPtr);
    case 0x19: return is(KnownTypeCode::UIntPtr);
    case 0x1C: return is(KnownTypeCode::Object);
    case 0xF: case 0x10: case 0x45: case 0x1D: case 0x1B: case 0x14: return false;
    case 0x1F: case 0x20:   // a modifier: past it
        blob.read_type_handle();
        return signature_is_known_type(reader, known, blob);
    case 0x15: return signature_is_known_type(reader, known, blob);   // a generic instance: its generic type
    case 0x13: case 0x1E: return false;
    case 0x11: case 0x12: return is_known_type(blob.read_type_handle(), reader, known);
    default: return false;
    }
}

}  // namespace

bool is_type_kind(HK kind) { return kind == HK::TypeDefinition || kind == HK::TypeReference || kind == HK::TypeSpecification; }

bool is_member_kind(HK kind) {
    return kind == HK::MethodDefinition || kind == HK::PropertyDefinition || kind == HK::FieldDefinition || kind == HK::EventDefinition ||
           kind == HK::MemberReference || kind == HK::MethodSpecification;
}

bool is_entity_handle(srm::Handle handle) { return handle.is_nil() || static_cast<std::uint8_t>(handle.kind()) < 112; }

bool is_value_type(const srm::TypeDefinition& type, const srm::MetadataReader& reader) {
    const srm::EntityHandle base = get_base_type_or_nil(type);
    if (base.is_nil()) return false;
    if (is_known_type(base, reader, KnownTypeCode::Enum)) return true;
    if (!is_known_type(base, reader, KnownTypeCode::ValueType)) return false;
    return !typesystem::is_known_type(get_full_type_name(type, reader), KnownTypeCode::Enum);
}

bool is_enum(const srm::TypeDefinition& type, const srm::MetadataReader& reader) {
    const srm::EntityHandle base = get_base_type_or_nil(type);
    return !base.is_nil() && is_known_type(base, reader, KnownTypeCode::Enum);
}

bool is_enum(const srm::TypeDefinition& type, const srm::MetadataReader& reader, srm::PrimitiveTypeCode& underlying_type) {
    underlying_type = static_cast<srm::PrimitiveTypeCode>(0);
    const srm::EntityHandle base = get_base_type_or_nil(type);
    if (base.is_nil() || !is_known_type(base, reader, KnownTypeCode::Enum)) return false;
    for (const srm::FieldDefinitionHandle h : type.get_fields()) {
        const srm::FieldDefinition field = reader.get_field_definition(h);
        if ((field.attributes() & 0x0010) != 0) continue;   // static
        srm::BlobReader blob = reader.get_blob_reader(field.signature());
        if (blob.read_signature_header().kind() != srm::SignatureKind::Field) return false;
        underlying_type = static_cast<srm::PrimitiveTypeCode>(blob.read_byte());
        return true;
    }
    return false;
}

bool is_delegate(const srm::TypeDefinition& type, const srm::MetadataReader& reader) {
    const srm::EntityHandle base = get_base_type_or_nil(type);
    return !base.is_nil() && is_known_type(base, reader, KnownTypeCode::MulticastDelegate);
}

bool has_body(const srm::MethodDefinition& method) {
    constexpr std::uint16_t no_body_attrs = 0x0400 | 0x2000;   // Abstract | PinvokeImpl
    constexpr std::uint16_t no_body_impl = 0x1000 | 0x0001 | 0x0004 | 0x0003;   // InternalCall | Native | Unmanaged | Runtime
    return (method.attributes() & no_body_attrs) == 0 && (method.impl_attributes() & no_body_impl) == 0 && method.relative_virtual_address() > 0;
}

int get_code_size(const srm::MethodBodyBlock& body) { return static_cast<int>(body.il_bytes().size()); }

srm::MethodDefinitionHandle get_any(const srm::PropertyAccessors& a) { return !a.getter.is_nil() ? a.getter : a.setter; }

srm::MethodDefinitionHandle get_any(const srm::EventAccessors& a) {
    if (!a.adder.is_nil()) return a.adder;
    if (!a.remover.is_nil()) return a.remover;
    return a.raiser;
}

srm::EntityHandle get_generic_type(const srm::TypeSpecification& ts, const srm::MetadataReader& metadata) {
    if (ts.signature().is_nil()) return {};
    srm::BlobReader sig = metadata.get_blob_reader(ts.signature());
    if (sig.read_byte() != static_cast<std::uint8_t>(srm::SignatureTypeCode::GenericTypeInstance)) return {};
    const int raw_type_kind = sig.read_compressed_integer();
    if (raw_type_kind < 17 || raw_type_kind > 18) return {};
    return sig.read_type_handle();
}

srm::EntityHandle get_declaring_type(srm::EntityHandle entity, const srm::MetadataReader& metadata) {
    switch (entity.kind()) {
    case HK::TypeDefinition: return metadata.get_type_definition(srm::TypeDefinitionHandle(entity)).get_declaring_type();
    case HK::TypeReference: return get_declaring_type(metadata.get_type_reference(srm::TypeReferenceHandle(entity)));
    case HK::TypeSpecification:
        return get_declaring_type(get_generic_type(metadata.get_type_specification(srm::TypeSpecificationHandle(entity)), metadata), metadata);
    case HK::FieldDefinition: return metadata.get_field_definition(srm::FieldDefinitionHandle(entity)).get_declaring_type();
    case HK::MethodDefinition: return metadata.get_method_definition(srm::MethodDefinitionHandle(entity)).get_declaring_type();
    case HK::MemberReference: return metadata.get_member_reference(srm::MemberReferenceHandle(entity)).parent();
    case HK::EventDefinition:
        return metadata.get_method_definition(get_any(metadata.get_event_definition(srm::EventDefinitionHandle(entity)).get_accessors())).get_declaring_type();
    case HK::PropertyDefinition:
        return metadata.get_method_definition(get_any(metadata.get_property_definition(srm::PropertyDefinitionHandle(entity)).get_accessors()))
            .get_declaring_type();
    case HK::MethodSpecification:
        return get_declaring_type(metadata.get_method_specification(srm::MethodSpecificationHandle(entity)).method(), metadata);
    default: throw std::out_of_range("entity");
    }
}

srm::TypeReferenceHandle get_declaring_type(const srm::TypeReference& tr) {
    const srm::EntityHandle scope = tr.resolution_scope();
    return scope.kind() == HK::TypeReference ? srm::TypeReferenceHandle(scope) : srm::TypeReferenceHandle();
}

typesystem::FullTypeName get_full_type_name(srm::EntityHandle handle, const srm::MetadataReader& reader) {
    if (handle.is_nil()) throw UsageError("get_full_type_name: a nil handle");
    switch (handle.kind()) {
    case HK::TypeDefinition: return get_full_type_name(srm::TypeDefinitionHandle(handle), reader);
    case HK::TypeReference: return get_full_type_name(srm::TypeReferenceHandle(handle), reader);
    case HK::TypeSpecification: return get_full_type_name(srm::TypeSpecificationHandle(handle), reader);
    default: throw std::out_of_range("handle");
    }
}

typesystem::FullTypeName get_full_type_name(srm::TypeSpecificationHandle handle, const srm::MetadataReader& reader) {
    if (handle.is_nil()) throw UsageError("get_full_type_name: a nil handle");
    FullTypeNameSignatureDecoder decoder(reader);
    return reader.get_type_specification(handle).decode_signature(decoder, 0);
}

typesystem::FullTypeName get_full_type_name(srm::TypeReferenceHandle handle, const srm::MetadataReader& reader) {
    if (handle.is_nil()) throw UsageError("get_full_type_name: a nil handle");
    const srm::TypeReference tr = reader.get_type_reference(handle);
    std::string name;
    try {
        name = std::string(reader.get_string(tr.name()));
    } catch (const srm::BadImageFormatError&) {
        name = std::format("TR{:08x}", srm::get_token(handle));
    }
    int tpc = 0;
    name = typesystem::split_type_parameter_count_from_reflection_name(name, tpc);
    srm::TypeReferenceHandle declaring;
    try {
        declaring = get_declaring_type(tr);
    } catch (const srm::BadImageFormatError&) {
        declaring = {};
    }
    if (declaring.is_nil()) {
        std::string ns;
        try {
            ns = tr.namespace_().is_nil() ? "" : std::string(reader.get_string(tr.namespace_()));
        } catch (const srm::BadImageFormatError&) {
            ns.clear();
        }
        return typesystem::TopLevelTypeName(std::move(ns), std::move(name), tpc);
    }
    return get_full_type_name(declaring, reader).nested_type(std::move(name), tpc);
}

typesystem::FullTypeName get_full_type_name(srm::TypeDefinitionHandle handle, const srm::MetadataReader& reader) {
    if (handle.is_nil()) throw UsageError("get_full_type_name: a nil handle");
    return get_full_type_name(reader.get_type_definition(handle), reader);
}

typesystem::FullTypeName get_full_type_name(const srm::TypeDefinition& td, const srm::MetadataReader& reader) {
    int tpc = 0;
    std::string name = typesystem::split_type_parameter_count_from_reflection_name(reader.get_string(td.name()), tpc);
    const srm::TypeDefinitionHandle declaring = td.get_declaring_type();
    if (declaring.is_nil()) {
        std::string ns = td.namespace_().is_nil() ? "" : std::string(reader.get_string(td.namespace_()));
        return typesystem::TopLevelTypeName(std::move(ns), std::move(name), tpc);
    }
    return get_full_type_name(declaring, reader).nested_type(std::move(name), tpc);
}

typesystem::FullTypeName get_full_type_name(const srm::ExportedType& type, const srm::MetadataReader& metadata) {
    int tpc = 0;
    std::string name = typesystem::split_type_parameter_count_from_reflection_name(metadata.get_string(type.name()), tpc);
    if (type.implementation().kind() == HK::ExportedType) {
        const srm::ExportedType outer = metadata.get_exported_type(srm::ExportedTypeHandle(type.implementation()));
        return get_full_type_name(outer, metadata).nested_type(std::move(name), tpc);
    }
    std::string ns = type.namespace_().is_nil() ? "" : std::string(metadata.get_string(type.namespace_()));
    return typesystem::TopLevelTypeName(std::move(ns), std::move(name), tpc);
}

bool is_known_type(srm::EntityHandle handle, const srm::MetadataReader& reader, KnownTypeCode known_type) {
    return is_known_type(handle, reader, typesystem::KnownTypeReference::get(known_type)->type_name());
}

bool is_known_type(srm::EntityHandle handle, const srm::MetadataReader& reader, const typesystem::TopLevelTypeName& known) {
    if (handle.is_nil()) return false;
    srm::StringHandle name_handle, namespace_handle;
    try {
        switch (handle.kind()) {
        case HK::TypeReference: {
            const srm::TypeReference tr = reader.get_type_reference(srm::TypeReferenceHandle(handle));
            // ignore exported and nested types
            if (tr.resolution_scope().is_nil() || tr.resolution_scope().kind() == HK::TypeReference) return false;
            name_handle = tr.name();
            namespace_handle = tr.namespace_();
            break;
        }
        case HK::TypeDefinition: {
            const srm::TypeDefinition td = reader.get_type_definition(srm::TypeDefinitionHandle(handle));
            if (td.is_nested()) return false;
            name_handle = td.name();
            namespace_handle = td.namespace_();
            break;
        }
        case HK::TypeSpecification: {
            const srm::TypeSpecification ts = reader.get_type_specification(srm::TypeSpecificationHandle(handle));
            srm::BlobReader blob = reader.get_blob_reader(ts.signature());
            return signature_is_known_type(reader, known, blob);
        }
        default: return false;
        }
    } catch (const srm::BadImageFormatError&) {
        return false;   // bad metadata while resolving the scope and the rest
    }
    if (known.type_parameter_count() == 0) {
        if (reader.get_string(name_handle) != known.name()) return false;
    } else {
        int tpc = 0;
        const std::string name = typesystem::split_type_parameter_count_from_reflection_name(reader.get_string(name_handle), tpc);
        if (tpc != known.type_parameter_count() || name != known.name()) return false;
    }
    if (namespace_handle.is_nil()) return known.namespace_().empty();
    return reader.get_string(namespace_handle) == known.namespace_();
}

bool is_generated_name(srm::StringHandle handle, const srm::MetadataReader& metadata) {
    return !handle.is_nil() && is_generated_name(metadata.get_string(handle));
}

bool is_generated_name(std::string_view name) { return name.starts_with('<') || name.find('$') != std::string_view::npos; }

bool has_generated_name(srm::MethodDefinitionHandle handle, const srm::MetadataReader& metadata) {
    return is_generated_name(metadata.get_method_definition(handle).name(), metadata);
}
bool has_generated_name(srm::TypeDefinitionHandle handle, const srm::MetadataReader& metadata) {
    return is_generated_name(metadata.get_type_definition(handle).name(), metadata);
}
bool has_generated_name(srm::FieldDefinitionHandle handle, const srm::MetadataReader& metadata) {
    return is_generated_name(metadata.get_field_definition(handle).name(), metadata);
}

srm::EntityHandle get_attribute_type(const srm::CustomAttribute& attribute, const srm::MetadataReader& reader) {
    switch (attribute.constructor().kind()) {
    case HK::MethodDefinition: return reader.get_method_definition(srm::MethodDefinitionHandle(attribute.constructor())).get_declaring_type();
    case HK::MemberReference: return reader.get_member_reference(srm::MemberReferenceHandle(attribute.constructor())).parent();
    default:
        throw srm::BadImageFormatError(0, std::format("Unexpected token kind for attribute constructor: {}", static_cast<int>(attribute.constructor().kind())));
    }
}

namespace {

// How many bytes a field's type holds (FieldValueSizeDecoder, without a compilation).
class FieldValueSizeDecoder {
public:
    explicit FieldValueSizeDecoder(int pointer_size) : pointer_size_(pointer_size) {}
    int get_array_type(int, const srm::ArrayShape&) { return get_primitive_type(srm::PrimitiveTypeCode::Object); }
    int get_sz_array_type(int) { return get_primitive_type(srm::PrimitiveTypeCode::Object); }
    int get_by_reference_type(int) { return pointer_size_; }
    int get_function_pointer_type(const srm::MethodSignature<int>&) { return pointer_size_; }
    int get_generic_instantiation(int generic, const std::vector<int>&) { return generic; }
    int get_generic_method_parameter(const int&, int) { return 0; }
    int get_generic_type_parameter(const int&, int) { return 0; }
    int get_modified_type(int, int unmodified, bool) { return unmodified; }
    int get_pinned_type(int element) { return element; }
    int get_pointer_type(int) { return pointer_size_; }
    int get_primitive_type(srm::PrimitiveTypeCode code) {
        using P = srm::PrimitiveTypeCode;
        switch (code) {
        case P::Boolean: case P::Byte: case P::SByte: return 1;
        case P::Char: case P::Int16: case P::UInt16: return 2;
        case P::Int32: case P::UInt32: case P::Single: return 4;
        case P::Int64: case P::UInt64: case P::Double: return 8;
        case P::IntPtr: case P::UIntPtr: return pointer_size_;
        default: return 0;
        }
    }
    int get_type_from_definition(const srm::MetadataReader& reader, srm::TypeDefinitionHandle handle, std::uint8_t) {
        return reader.get_type_definition(handle).get_layout().size;
    }
    int get_type_from_reference(const srm::MetadataReader&, srm::TypeReferenceHandle, std::uint8_t) { return 0; }   // (no compilation to resolve it in)
    int get_type_from_specification(const srm::MetadataReader& reader, const int& context, srm::TypeSpecificationHandle handle, std::uint8_t) {
        return reader.get_type_specification(handle).decode_signature(*this, context);
    }

private:
    int pointer_size_;
};

}  // namespace

ByteView get_initial_value(const srm::FieldDefinition& field, const metadata::MetadataFile& file, int pointer_size) {
    if ((field.attributes() & 0x0100) == 0) return {};   // HasFieldRVA
    const int rva = field.get_relative_virtual_address();
    if (rva == 0) return {};
    if (pointer_size == 0) {
        const auto* pe_file = dynamic_cast<const metadata::PEFile*>(&file);
        pointer_size = pe_file != nullptr && pe_file->reader().pe_header().magic == 0x10B ? 4 : 8;
    }
    FieldValueSizeDecoder decoder(pointer_size);
    const int size = field.decode_signature(decoder, 0);
    const ByteView section = file.get_section_data(rva);
    if (section.empty() && size != 0) throw srm::BadImageFormatError(static_cast<std::uint64_t>(rva), std::format("Field data (rva=0x{:x}) could not be found in any section!", rva));
    if (size < 0 || static_cast<std::size_t>(size) > section.size()) throw srm::BadImageFormatError(static_cast<std::uint64_t>(rva), std::format("Invalid size {} for field data!", size));
    return section.subspan(0, static_cast<std::size_t>(size));
}

srm::EntityHandle get_base_type_or_nil(const srm::TypeDefinition& definition) {
    try {
        return definition.base_type();
    } catch (const srm::BadImageFormatError&) {
        return {};
    }
}

std::string to_il_syntax(srm::SignatureCallingConvention convention) {
    using C = srm::SignatureCallingConvention;
    switch (convention) {
    case C::Default: return "default";
    case C::CDecl: return "unmanaged cdecl";
    case C::StdCall: return "unmanaged stdcall";
    case C::ThisCall: return "unmanaged thiscall";
    case C::FastCall: return "unmanaged fastcall";
    case C::VarArgs: return "vararg";
    case C::Unmanaged: return "unmanaged";
    default: return std::to_string(static_cast<int>(convention));   // (an undefined value: the enum's ToString is its number)
    }
}

std::vector<srm::MethodImplementationHandle> get_method_implementations(srm::MethodDefinitionHandle handle, const srm::MetadataReader& reader) {
    std::vector<srm::MethodImplementationHandle> out;
    const srm::TypeDefinition type = reader.get_type_definition(reader.get_method_definition(handle).get_declaring_type());
    for (const srm::MethodImplementationHandle h : type.get_method_implementations())
        if (reader.get_method_implementation(h).method_body() == srm::EntityHandle(handle)) out.push_back(h);
    return out;
}

typesystem::FullTypeName FullTypeNameSignatureDecoder::get_primitive_type(srm::PrimitiveTypeCode code) {
    const typesystem::KnownTypeReference* k = typesystem::KnownTypeReference::get(metadata::to_known_type_code(code));
    if (k == nullptr) return {};
    return typesystem::TopLevelTypeName(k->namespace_(), k->name(), k->type_parameter_count());
}

typesystem::FullTypeName FullTypeNameSignatureDecoder::get_type_from_specification(const srm::MetadataReader& reader, const int&,
                                                                                  srm::TypeSpecificationHandle handle, std::uint8_t) {
    FullTypeNameSignatureDecoder inner(metadata_);
    return reader.get_type_specification(handle).decode_signature(inner, 0);
}

srm::PrimitiveTypeCode FullTypeNameSignatureDecoder::get_underlying_enum_type(const T&) {
    throw UsageError("FullTypeNameSignatureDecoder: an enum's underlying type is not known here");
}

}  // namespace cb::dotnet::decompiler
