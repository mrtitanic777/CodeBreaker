// dotnet/decompiler/srm_extensions.hpp -- ILSpy's helpers over the metadata reader: attribute flags, what kind of type a definition is
// (value type, enum, delegate), declaring types, a type's FullTypeName, whether a reference names a known type, compiler-generated names,
// a method's body and overrides, a field's initial value, a calling convention in IL.
//
// Ported from ILSpy (ICSharpCode.Decompiler: SRMExtensions in SRMExtensions.cs and SRMHacks.cs; Metadata.FullTypeNameSignatureDecoder),
// Copyright AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler). Not yet ported (they need the type system or
// KnownAttribute, and come with them): IsCompilerGenerated, HasKnownAttribute, IsKnownAttribute, GetNullableContext, IsAnonymousType,
// and GetInitialValue's resolution of a TypeRef's size through a compilation (without one, as upstream without one, its size is 0).
#pragma once

#include <string>
#include <vector>

#include "dotnet/decompiler/typesystem/full_type_name.hpp"
#include "dotnet/decompiler/typesystem/known_type_reference.hpp"
#include "dotnet/metadata/pe_reader.hpp"
#include "dotnet/metadata/signature_decoder.hpp"

namespace cb::dotnet::decompiler {

namespace metadata {
class MetadataFile;
}
namespace srm = cb::dotnet::metadata;

// GenericParameterAttributes.AllowByRefLike and MethodImplAttributes.Async, which SRM does not name.
inline constexpr std::uint16_t kAllowByRefLike = 0x0020;
inline constexpr std::uint16_t kMethodImplAsync = 0x2000;

bool is_type_kind(srm::HandleKind kind);
bool is_member_kind(srm::HandleKind kind);
bool is_entity_handle(srm::Handle handle);

bool is_value_type(const srm::TypeDefinition& type, const srm::MetadataReader& reader);
bool is_enum(const srm::TypeDefinition& type, const srm::MetadataReader& reader);
// An enum, and its underlying type (from its instance field's signature).
bool is_enum(const srm::TypeDefinition& type, const srm::MetadataReader& reader, srm::PrimitiveTypeCode& underlying_type);
bool is_delegate(const srm::TypeDefinition& type, const srm::MetadataReader& reader);
// A method that has IL: not abstract or P/Invoke, not internal call, native, unmanaged or runtime, and with an RVA.
bool has_body(const srm::MethodDefinition& method);
int get_code_size(const srm::MethodBodyBlock& body);
srm::MethodDefinitionHandle get_any(const srm::PropertyAccessors& accessors);
srm::MethodDefinitionHandle get_any(const srm::EventAccessors& accessors);
// The generic type of a TypeSpec that is a generic instance; nil otherwise.
srm::EntityHandle get_generic_type(const srm::TypeSpecification& ts, const srm::MetadataReader& metadata);
// The type or member that declares `entity` (std::out_of_range for a kind that has none).
srm::EntityHandle get_declaring_type(srm::EntityHandle entity, const srm::MetadataReader& metadata);
// The TypeRef a nested TypeRef is scoped to; nil for one that is not nested.
srm::TypeReferenceHandle get_declaring_type(const srm::TypeReference& tr);

// A type's name (UsageError for a nil handle, std::out_of_range for a handle that is not a type).
typesystem::FullTypeName get_full_type_name(srm::EntityHandle handle, const srm::MetadataReader& reader);
typesystem::FullTypeName get_full_type_name(srm::TypeDefinitionHandle handle, const srm::MetadataReader& reader);
typesystem::FullTypeName get_full_type_name(srm::TypeReferenceHandle handle, const srm::MetadataReader& reader);
typesystem::FullTypeName get_full_type_name(srm::TypeSpecificationHandle handle, const srm::MetadataReader& reader);
typesystem::FullTypeName get_full_type_name(const srm::TypeDefinition& td, const srm::MetadataReader& reader);
typesystem::FullTypeName get_full_type_name(const srm::ExportedType& type, const srm::MetadataReader& metadata);

// Whether a TypeDef, TypeRef or TypeSpec names the known type (a TypeRef that is nested or exported, or a nested TypeDef, never does).
bool is_known_type(srm::EntityHandle handle, const srm::MetadataReader& reader, typesystem::KnownTypeCode known_type);
bool is_known_type(srm::EntityHandle handle, const srm::MetadataReader& reader, const typesystem::TopLevelTypeName& known_type);

bool is_generated_name(srm::StringHandle handle, const srm::MetadataReader& metadata);
bool is_generated_name(std::string_view name);   // starts with '<' or has a '$'
bool has_generated_name(srm::MethodDefinitionHandle handle, const srm::MetadataReader& metadata);
bool has_generated_name(srm::TypeDefinitionHandle handle, const srm::MetadataReader& metadata);
bool has_generated_name(srm::FieldDefinitionHandle handle, const srm::MetadataReader& metadata);

// The type whose constructor an attribute calls.
srm::EntityHandle get_attribute_type(const srm::CustomAttribute& attribute, const srm::MetadataReader& reader);

// A field's initial data (HasFieldRVA): its bytes, as long as the field's type is (`pointer_size` for native ints and pointers: 0 for the
// size of the file's own (4 for a PE32 image, else 8), as upstream's overload that is given a type system; 8 is its process's IntPtr.Size, for
// the callers that give it none). Empty for a field without data; BadImageFormatError when the data lies
// outside every section or is shorter than the type.
ByteView get_initial_value(const srm::FieldDefinition& field, const metadata::MetadataFile& file, int pointer_size = 0);

srm::EntityHandle get_base_type_or_nil(const srm::TypeDefinition& definition);
// "default", "unmanaged cdecl", "vararg" ...
std::string to_il_syntax(srm::SignatureCallingConvention convention);

// The MethodImpl rows of the declaring type whose body is `handle`.
std::vector<srm::MethodImplementationHandle> get_method_implementations(srm::MethodDefinitionHandle handle, const srm::MetadataReader& reader);

// The full names a signature reduces to (FullTypeNameSignatureDecoder: arrays, pointers and instances are their element or generic type).
class FullTypeNameSignatureDecoder {
public:
    explicit FullTypeNameSignatureDecoder(const srm::MetadataReader& metadata) : metadata_(metadata) {}
    using T = typesystem::FullTypeName;
    T get_array_type(T element, const srm::ArrayShape&) { return element; }
    T get_by_reference_type(T element) { return element; }
    T get_function_pointer_type(const srm::MethodSignature<T>&) { return {}; }
    T get_generic_instantiation(T generic, const std::vector<T>&) { return generic; }
    T get_generic_method_parameter(const int&, int) { return {}; }
    T get_generic_type_parameter(const int&, int) { return {}; }
    T get_modified_type(T, T unmodified, bool) { return unmodified; }
    T get_pinned_type(T element) { return element; }
    T get_pointer_type(T element) { return element; }
    T get_primitive_type(srm::PrimitiveTypeCode code);
    T get_system_type() { return typesystem::TopLevelTypeName("System", "Type"); }
    T get_sz_array_type(T element) { return element; }
    T get_type_from_definition(const srm::MetadataReader& reader, srm::TypeDefinitionHandle handle, std::uint8_t) {
        return get_full_type_name(handle, reader);
    }
    T get_type_from_reference(const srm::MetadataReader& reader, srm::TypeReferenceHandle handle, std::uint8_t) {
        return get_full_type_name(handle, reader);
    }
    T get_type_from_serialized_name(const std::string& name) { return T(name); }
    T get_type_from_specification(const srm::MetadataReader& reader, const int&, srm::TypeSpecificationHandle handle, std::uint8_t);
    srm::PrimitiveTypeCode get_underlying_enum_type(const T&);   // UsageError, as upstream's NotImplementedException
    bool is_system_type(const T& type) { return typesystem::is_known_type(type, typesystem::KnownTypeCode::Type); }

private:
    const srm::MetadataReader& metadata_;
};

}  // namespace cb::dotnet::decompiler
