// dotnet/decompiler/metadata/metadata_extensions.hpp -- helpers over the metadata: an assembly's full name and public key token, the
// top-level types, a type name as IL writes it, the primitive and known type codes of each other, and the raw rows of the tables the
// reader has no view for.
//
// Ported from ILSpy (ICSharpCode.Decompiler.Metadata: MetadataExtensions; Util.Sha1ForNonSecretPurposes is core/crypto's SHA-1), Copyright
// AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler). The raw rows are read through the reader's cells (upstream
// reads the table bytes itself, to the same values). Not yet ported: MinimalAttributeTypeProvider and MinimalSignatureTypeProvider (they
// come with the type system).
#pragma once

#include <string>
#include <tuple>
#include <vector>

#include "dotnet/decompiler/typesystem/full_type_name.hpp"
#include "dotnet/decompiler/typesystem/known_type_reference.hpp"
#include "dotnet/metadata/metadata_reader.hpp"

namespace cb::dotnet::decompiler::metadata {

namespace srm = cb::dotnet::metadata;

// "" when the module is not an assembly; "null" when it has no public key; else the token as 16 hex digits.
std::string get_public_key_token(const srm::MetadataReader& reader);
// "Name, Version=a.b.c.d, Culture=neutral, PublicKeyToken=..." ("" when the module is not an assembly).
std::string get_full_assembly_name(const srm::MetadataReader& reader);
bool try_get_full_assembly_name(const srm::MetadataReader& reader, std::string& assembly_name);
std::string get_full_assembly_name(const srm::AssemblyReference& reference, const srm::MetadataReader& reader);
bool try_get_full_assembly_name(const srm::AssemblyReference& reference, const srm::MetadataReader& reader, std::string& assembly_name);

std::string to_hex_string(ByteView bytes);                 // "0a1b..." (lower case)
std::string to_hex_string_dashed(srm::BlobReader reader);  // "0A-1B-..." (upstream's BlobReader.ToHexString)

std::vector<srm::TypeDefinitionHandle> get_top_level_type_definitions(const srm::MetadataReader& reader);
// A type's name as ILAsm spells it: Namespace.Name`1, nested types after '/', each part escaped where it must be.
std::string to_il_name_string(const typesystem::FullTypeName& type_name, bool omit_generics = false);

srm::PrimitiveTypeCode to_primitive_type_code(typesystem::KnownTypeCode code);   // 0 for a type that is not primitive
typesystem::KnownTypeCode to_known_type_code(srm::PrimitiveTypeCode code);       // None for an invalid code

std::vector<srm::ModuleReferenceHandle> get_module_references(const srm::MetadataReader& reader);
std::vector<srm::TypeSpecificationHandle> get_type_specifications(const srm::MetadataReader& reader);
std::vector<srm::MethodSpecificationHandle> get_method_specifications(const srm::MetadataReader& reader);

// The raw rows of tables without views of their own.
struct MethodSemanticsRow {
    srm::Handle handle;
    std::uint16_t semantics;   // MethodSemanticsAttributes
    srm::MethodDefinitionHandle method;
    srm::EntityHandle association;
};
std::vector<MethodSemanticsRow> get_method_semantics(const srm::MetadataReader& reader);
std::vector<std::tuple<int, std::uint32_t, srm::TypeDefinitionHandle>> get_class_layouts(const srm::MetadataReader& reader);
std::vector<std::pair<int, srm::FieldDefinitionHandle>> get_field_layout_rows(const srm::MetadataReader& reader);
std::vector<std::pair<srm::TypeDefinitionHandle, srm::EventDefinitionHandle>> get_event_maps(const srm::MetadataReader& reader);
std::vector<std::pair<srm::TypeDefinitionHandle, srm::PropertyDefinitionHandle>> get_property_maps(const srm::MetadataReader& reader);
std::vector<std::pair<srm::TypeDefinitionHandle, srm::TypeDefinitionHandle>> get_nested_classes(const srm::MetadataReader& reader);
std::vector<std::pair<int, srm::FieldDefinitionHandle>> get_field_rvas(const srm::MetadataReader& reader);
std::vector<std::pair<srm::EntityHandle, srm::BlobHandle>> get_field_marshals(const srm::MetadataReader& reader);
struct ImplMapRow {
    std::uint16_t mapping_flags;   // MethodImportAttributes
    srm::EntityHandle member_forwarded;
    srm::StringHandle import_name;
    srm::ModuleReferenceHandle import_scope;
};
std::vector<ImplMapRow> get_impl_maps(const srm::MetadataReader& reader);
std::vector<std::pair<srm::TypeDefinitionHandle, srm::EntityHandle>> get_interface_impl_rows(const srm::MetadataReader& reader);
std::vector<srm::EntityHandle> get_ptr_rows(const srm::MetadataReader& reader, srm::TableIndex ptr_table);
std::vector<std::tuple<srm::TypeDefinitionHandle, int, int>> get_type_def_list_columns(const srm::MetadataReader& reader);
std::vector<std::pair<srm::MethodDefinitionHandle, int>> get_method_def_param_lists(const srm::MetadataReader& reader);

}  // namespace cb::dotnet::decompiler::metadata
