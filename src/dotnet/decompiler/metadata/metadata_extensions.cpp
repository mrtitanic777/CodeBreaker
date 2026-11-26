// dotnet/decompiler/metadata/metadata_extensions.cpp -- MetadataExtensions.
#include "dotnet/decompiler/metadata/metadata_extensions.hpp"

#include <format>
#include <stdexcept>

#include "core/crypto.hpp"
#include "dotnet/decompiler/disassembler/disassembler_helpers.hpp"

namespace cb::dotnet::decompiler::metadata {
namespace {

using TI = srm::TableIndex;

// 1. SHA-1 of the public key; 2. its last 8 bytes; 3. reversed (as Cecil does).
std::string calculate_public_key_token(srm::BlobHandle blob, const srm::MetadataReader& reader) {
    const crypto::Sha1Digest hash = crypto::sha1(reader.get_blob_bytes(blob));
    std::string s;
    for (int i = 19; i >= 12; --i) s += std::format("{:02x}", hash[static_cast<std::size_t>(i)]);
    return s;
}

}  // namespace

std::string get_public_key_token(const srm::MetadataReader& reader) {
    if (!reader.is_assembly()) return "";
    const srm::AssemblyDefinition asm_ = reader.get_assembly_definition();
    return asm_.public_key().is_nil() ? "null" : calculate_public_key_token(asm_.public_key(), reader);
}

std::string get_full_assembly_name(const srm::MetadataReader& reader) {
    if (!reader.is_assembly()) return "";
    const srm::AssemblyDefinition asm_ = reader.get_assembly_definition();
    const std::string public_key = get_public_key_token(reader);
    return std::format("{}, Version={}, Culture={}, PublicKeyToken={}", reader.get_string(asm_.name()), asm_.version().to_string(),
                       asm_.culture().is_nil() ? std::string_view("neutral") : reader.get_string(asm_.culture()), public_key);
}

bool try_get_full_assembly_name(const srm::MetadataReader& reader, std::string& assembly_name) {
    try {
        assembly_name = get_full_assembly_name(reader);
        return true;
    } catch (const srm::BadImageFormatError&) {
        assembly_name.clear();
        return false;
    }
}

std::string get_full_assembly_name(const srm::AssemblyReference& reference, const srm::MetadataReader& reader) {
    std::string b(reader.get_string(reference.name()));
    b += ", Version=" + reference.version().to_string();
    b += ", Culture=";
    b += reference.culture().is_nil() ? std::string_view("neutral") : reader.get_string(reference.culture());
    if (reference.public_key_or_token().is_nil()) {
        b += ", PublicKeyToken=null";
    } else if ((reference.flags() & 0x0001) != 0) {   // AssemblyFlags.PublicKey
        b += ", PublicKeyToken=" + calculate_public_key_token(reference.public_key_or_token(), reader);
    } else {
        b += ", PublicKeyToken=" + to_hex_string(reader.get_blob_bytes(reference.public_key_or_token()));
    }
    if ((reference.flags() & 0x0100) != 0) b += ", Retargetable=true";   // AssemblyFlags.Retargetable
    return b;
}

bool try_get_full_assembly_name(const srm::AssemblyReference& reference, const srm::MetadataReader& reader, std::string& assembly_name) {
    try {
        assembly_name = get_full_assembly_name(reference, reader);
        return true;
    } catch (const srm::BadImageFormatError&) {
        assembly_name.clear();
        return false;
    }
}

std::string to_hex_string(ByteView bytes) {
    std::string s;
    s.reserve(bytes.size() * 2);
    for (const std::uint8_t b : bytes) s += std::format("{:02x}", b);
    return s;
}

std::string to_hex_string_dashed(srm::BlobReader reader) {
    std::string s;
    for (int i = 0; i < reader.length(); ++i) {
        if (i > 0) s += '-';
        s += std::format("{:02X}", reader.read_byte());
    }
    return s;
}

std::vector<srm::TypeDefinitionHandle> get_top_level_type_definitions(const srm::MetadataReader& reader) {
    std::vector<srm::TypeDefinitionHandle> out;
    for (const srm::TypeDefinitionHandle h : reader.type_definitions())
        if (reader.get_type_definition(h).get_declaring_type().is_nil()) out.push_back(h);
    return out;
}

std::string to_il_name_string(const typesystem::FullTypeName& type_name, bool omit_generics) {
    std::string name;
    if (type_name.is_nested()) {
        name = type_name.name();
        if (!omit_generics) {
            const int local = type_name.get_nested_type_additional_type_parameter_count(type_name.nesting_level() - 1);
            if (local > 0) name += "`" + std::to_string(local);
        }
        name = disassembler::escape(name);
        return to_il_name_string(type_name.get_declaring_type(), omit_generics) + "/" + name;
    }
    if (!type_name.top_level_type_name().namespace_().empty()) name = type_name.top_level_type_name().namespace_() + "." + type_name.name();
    else name = type_name.name();
    if (!omit_generics && type_name.type_parameter_count() > 0) name += "`" + std::to_string(type_name.type_parameter_count());
    return disassembler::escape(name);
}

srm::PrimitiveTypeCode to_primitive_type_code(typesystem::KnownTypeCode code) {
    using K = typesystem::KnownTypeCode;
    using P = srm::PrimitiveTypeCode;
    switch (code) {
    case K::Object: return P::Object;
    case K::Boolean: return P::Boolean;
    case K::Char: return P::Char;
    case K::SByte: return P::SByte;
    case K::Byte: return P::Byte;
    case K::Int16: return P::Int16;
    case K::UInt16: return P::UInt16;
    case K::Int32: return P::Int32;
    case K::UInt32: return P::UInt32;
    case K::Int64: return P::Int64;
    case K::UInt64: return P::UInt64;
    case K::Single: return P::Single;
    case K::Double: return P::Double;
    case K::String: return P::String;
    case K::Void: return P::Void;
    case K::TypedReference: return P::TypedReference;
    case K::IntPtr: return P::IntPtr;
    case K::UIntPtr: return P::UIntPtr;
    default: return static_cast<P>(0);
    }
}

typesystem::KnownTypeCode to_known_type_code(srm::PrimitiveTypeCode code) {
    using K = typesystem::KnownTypeCode;
    using P = srm::PrimitiveTypeCode;
    switch (code) {
    case P::Boolean: return K::Boolean;
    case P::Byte: return K::Byte;
    case P::SByte: return K::SByte;
    case P::Char: return K::Char;
    case P::Int16: return K::Int16;
    case P::UInt16: return K::UInt16;
    case P::Int32: return K::Int32;
    case P::UInt32: return K::UInt32;
    case P::Int64: return K::Int64;
    case P::UInt64: return K::UInt64;
    case P::Single: return K::Single;
    case P::Double: return K::Double;
    case P::IntPtr: return K::IntPtr;
    case P::UIntPtr: return K::UIntPtr;
    case P::Object: return K::Object;
    case P::String: return K::String;
    case P::TypedReference: return K::TypedReference;
    case P::Void: return K::Void;
    default: return K::None;
    }
}

std::vector<srm::ModuleReferenceHandle> get_module_references(const srm::MetadataReader& reader) {
    std::vector<srm::ModuleReferenceHandle> out;
    for (std::uint32_t r = 1; r <= reader.row_count(TI::ModuleRef); ++r) out.emplace_back(r);
    return out;
}

std::vector<srm::TypeSpecificationHandle> get_type_specifications(const srm::MetadataReader& reader) {
    std::vector<srm::TypeSpecificationHandle> out;
    for (std::uint32_t r = 1; r <= reader.row_count(TI::TypeSpec); ++r) out.emplace_back(r);
    return out;
}

std::vector<srm::MethodSpecificationHandle> get_method_specifications(const srm::MetadataReader& reader) {
    std::vector<srm::MethodSpecificationHandle> out;
    for (std::uint32_t r = 1; r <= reader.row_count(TI::MethodSpec); ++r) out.emplace_back(r);
    return out;
}

std::vector<MethodSemanticsRow> get_method_semantics(const srm::MetadataReader& reader) {
    std::vector<MethodSemanticsRow> out;
    for (std::uint32_t r = 1; r <= reader.row_count(TI::MethodSemantics); ++r) {
        const std::uint32_t assoc = reader.cell(TI::MethodSemantics, r, 2);
        const srm::EntityHandle e = (assoc & 1) ? srm::EntityHandle(srm::HandleKind::PropertyDefinition, assoc >> 1)
                                                : srm::EntityHandle(srm::HandleKind::EventDefinition, assoc >> 1);
        out.push_back({srm::Handle(srm::EntityHandle(static_cast<std::uint32_t>(TI::MethodSemantics) << 24 | r)),
                       static_cast<std::uint16_t>(reader.cell(TI::MethodSemantics, r, 0)), srm::MethodDefinitionHandle(reader.cell(TI::MethodSemantics, r, 1)), e});
    }
    return out;
}

std::vector<std::tuple<int, std::uint32_t, srm::TypeDefinitionHandle>> get_class_layouts(const srm::MetadataReader& reader) {
    std::vector<std::tuple<int, std::uint32_t, srm::TypeDefinitionHandle>> out;
    for (std::uint32_t r = 1; r <= reader.row_count(TI::ClassLayout); ++r)
        out.emplace_back(static_cast<int>(reader.cell(TI::ClassLayout, r, 0)), reader.cell(TI::ClassLayout, r, 1),
                         srm::TypeDefinitionHandle(reader.cell(TI::ClassLayout, r, 2)));
    return out;
}

std::vector<std::pair<int, srm::FieldDefinitionHandle>> get_field_layout_rows(const srm::MetadataReader& reader) {
    std::vector<std::pair<int, srm::FieldDefinitionHandle>> out;
    for (std::uint32_t r = 1; r <= reader.row_count(TI::FieldLayout); ++r)
        out.emplace_back(static_cast<int>(reader.cell(TI::FieldLayout, r, 0)), srm::FieldDefinitionHandle(reader.cell(TI::FieldLayout, r, 1)));
    return out;
}

std::vector<std::pair<srm::TypeDefinitionHandle, srm::EventDefinitionHandle>> get_event_maps(const srm::MetadataReader& reader) {
    std::vector<std::pair<srm::TypeDefinitionHandle, srm::EventDefinitionHandle>> out;
    for (std::uint32_t r = 1; r <= reader.row_count(TI::EventMap); ++r)
        out.emplace_back(srm::TypeDefinitionHandle(reader.cell(TI::EventMap, r, 0)), srm::EventDefinitionHandle(reader.cell(TI::EventMap, r, 1)));
    return out;
}

std::vector<std::pair<srm::TypeDefinitionHandle, srm::PropertyDefinitionHandle>> get_property_maps(const srm::MetadataReader& reader) {
    std::vector<std::pair<srm::TypeDefinitionHandle, srm::PropertyDefinitionHandle>> out;
    for (std::uint32_t r = 1; r <= reader.row_count(TI::PropertyMap); ++r)
        out.emplace_back(srm::TypeDefinitionHandle(reader.cell(TI::PropertyMap, r, 0)), srm::PropertyDefinitionHandle(reader.cell(TI::PropertyMap, r, 1)));
    return out;
}

std::vector<std::pair<srm::TypeDefinitionHandle, srm::TypeDefinitionHandle>> get_nested_classes(const srm::MetadataReader& reader) {
    std::vector<std::pair<srm::TypeDefinitionHandle, srm::TypeDefinitionHandle>> out;
    for (std::uint32_t r = 1; r <= reader.row_count(TI::NestedClass); ++r)
        out.emplace_back(srm::TypeDefinitionHandle(reader.cell(TI::NestedClass, r, 0)), srm::TypeDefinitionHandle(reader.cell(TI::NestedClass, r, 1)));
    return out;
}

std::vector<std::pair<int, srm::FieldDefinitionHandle>> get_field_rvas(const srm::MetadataReader& reader) {
    std::vector<std::pair<int, srm::FieldDefinitionHandle>> out;
    for (std::uint32_t r = 1; r <= reader.row_count(TI::FieldRva); ++r)
        out.emplace_back(static_cast<int>(reader.cell(TI::FieldRva, r, 0)), srm::FieldDefinitionHandle(reader.cell(TI::FieldRva, r, 1)));
    return out;
}

std::vector<std::pair<srm::EntityHandle, srm::BlobHandle>> get_field_marshals(const srm::MetadataReader& reader) {
    std::vector<std::pair<srm::EntityHandle, srm::BlobHandle>> out;
    for (std::uint32_t r = 1; r <= reader.row_count(TI::FieldMarshal); ++r) {
        const std::uint32_t tag = reader.cell(TI::FieldMarshal, r, 0);
        const srm::EntityHandle parent = (tag & 1) ? srm::EntityHandle(srm::HandleKind::Parameter, tag >> 1)
                                                   : srm::EntityHandle(srm::HandleKind::FieldDefinition, tag >> 1);
        out.emplace_back(parent, srm::BlobHandle(reader.cell(TI::FieldMarshal, r, 1)));
    }
    return out;
}

std::vector<ImplMapRow> get_impl_maps(const srm::MetadataReader& reader) {
    std::vector<ImplMapRow> out;
    for (std::uint32_t r = 1; r <= reader.row_count(TI::ImplMap); ++r) {
        const std::uint32_t tag = reader.cell(TI::ImplMap, r, 1);
        const srm::EntityHandle member = (tag & 1) ? srm::EntityHandle(srm::HandleKind::MethodDefinition, tag >> 1)
                                                   : srm::EntityHandle(srm::HandleKind::FieldDefinition, tag >> 1);
        out.push_back({static_cast<std::uint16_t>(reader.cell(TI::ImplMap, r, 0)), member, srm::StringHandle(reader.cell(TI::ImplMap, r, 2)),
                       srm::ModuleReferenceHandle(reader.cell(TI::ImplMap, r, 3))});
    }
    return out;
}

std::vector<std::pair<srm::TypeDefinitionHandle, srm::EntityHandle>> get_interface_impl_rows(const srm::MetadataReader& reader) {
    std::vector<std::pair<srm::TypeDefinitionHandle, srm::EntityHandle>> out;
    for (std::uint32_t r = 1; r <= reader.row_count(TI::InterfaceImpl); ++r) {
        const std::uint32_t tag = reader.cell(TI::InterfaceImpl, r, 1);
        const srm::HandleKind kind = (tag & 3) == 0 ? srm::HandleKind::TypeDefinition
                                     : (tag & 3) == 1 ? srm::HandleKind::TypeReference
                                                      : srm::HandleKind::TypeSpecification;
        out.emplace_back(srm::TypeDefinitionHandle(reader.cell(TI::InterfaceImpl, r, 0)), srm::EntityHandle(kind, tag >> 2));
    }
    return out;
}

std::vector<srm::EntityHandle> get_ptr_rows(const srm::MetadataReader& reader, srm::TableIndex ptr_table) {
    srm::HandleKind referenced;
    switch (ptr_table) {
    case TI::EventPtr: referenced = srm::HandleKind::EventDefinition; break;
    case TI::FieldPtr: referenced = srm::HandleKind::FieldDefinition; break;
    case TI::MethodPtr: referenced = srm::HandleKind::MethodDefinition; break;
    case TI::ParamPtr: referenced = srm::HandleKind::Parameter; break;
    case TI::PropertyPtr: referenced = srm::HandleKind::PropertyDefinition; break;
    default: throw std::out_of_range("ptrTable");
    }
    std::vector<srm::EntityHandle> out;
    for (std::uint32_t r = 1; r <= reader.row_count(ptr_table); ++r) out.emplace_back(referenced, reader.cell(ptr_table, r, 0));
    return out;
}

std::vector<std::tuple<srm::TypeDefinitionHandle, int, int>> get_type_def_list_columns(const srm::MetadataReader& reader) {
    std::vector<std::tuple<srm::TypeDefinitionHandle, int, int>> out;
    for (const srm::TypeDefinitionHandle h : reader.type_definitions())
        out.emplace_back(h, static_cast<int>(reader.cell(TI::TypeDef, h.row(), 4)), static_cast<int>(reader.cell(TI::TypeDef, h.row(), 5)));
    return out;
}

std::vector<std::pair<srm::MethodDefinitionHandle, int>> get_method_def_param_lists(const srm::MetadataReader& reader) {
    std::vector<std::pair<srm::MethodDefinitionHandle, int>> out;
    for (const srm::MethodDefinitionHandle h : reader.method_definitions())
        out.emplace_back(h, static_cast<int>(reader.cell(TI::MethodDef, h.row(), 5)));
    return out;
}

}  // namespace cb::dotnet::decompiler::metadata
