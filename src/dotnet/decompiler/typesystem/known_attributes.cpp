// dotnet/decompiler/typesystem/known_attributes.cpp -- the known attributes' names (copied from upstream's table, in its order).
#include "dotnet/decompiler/typesystem/known_attributes.hpp"

#include <array>

#include "dotnet/decompiler/srm_extensions.hpp"
#include "dotnet/decompiler/typesystem/type_system_extensions.hpp"

namespace cb::dotnet::decompiler::typesystem {
namespace {

const std::array<TopLevelTypeName, kKnownAttributeCount>& type_names() {
    static const std::array<TopLevelTypeName, kKnownAttributeCount> names = {
        TopLevelTypeName(),   // None
        TopLevelTypeName("System.Runtime.CompilerServices", "CompilerGeneratedAttribute"),   // CompilerGenerated
        TopLevelTypeName("System.Runtime.CompilerServices", "CompilerFeatureRequiredAttribute"),   // CompilerFeatureRequired
        TopLevelTypeName("System.Runtime.CompilerServices", "ExtensionAttribute"),   // Extension
        TopLevelTypeName("System.Runtime.CompilerServices", "DynamicAttribute"),   // Dynamic
        TopLevelTypeName("System.Runtime.CompilerServices", "TupleElementNamesAttribute"),   // TupleElementNames
        TopLevelTypeName("System.Runtime.CompilerServices", "NullableAttribute"),   // Nullable
        TopLevelTypeName("System.Runtime.CompilerServices", "NullableContextAttribute"),   // NullableContext
        TopLevelTypeName("System.Runtime.CompilerServices", "NullablePublicOnlyAttribute"),   // NullablePublicOnly
        TopLevelTypeName("System.Diagnostics", "ConditionalAttribute"),   // Conditional
        TopLevelTypeName("System", "ObsoleteAttribute"),   // Obsolete
        TopLevelTypeName("Microsoft.CodeAnalysis", "EmbeddedAttribute"),   // Embedded
        TopLevelTypeName("System.Runtime.CompilerServices", "IsReadOnlyAttribute"),   // IsReadOnly
        TopLevelTypeName("System.Runtime.CompilerServices", "SpecialNameAttribute"),   // SpecialName
        TopLevelTypeName("System.Diagnostics", "DebuggerHiddenAttribute"),   // DebuggerHidden
        TopLevelTypeName("System.Diagnostics", "DebuggerStepThroughAttribute"),   // DebuggerStepThrough
        TopLevelTypeName("System.Diagnostics", "DebuggerBrowsableAttribute"),   // DebuggerBrowsable
        TopLevelTypeName("System.Reflection", "AssemblyVersionAttribute"),   // AssemblyVersion
        TopLevelTypeName("System.Runtime.CompilerServices", "InternalsVisibleToAttribute"),   // InternalsVisibleTo
        TopLevelTypeName("System.Runtime.CompilerServices", "TypeForwardedToAttribute"),   // TypeForwardedTo
        TopLevelTypeName("System.Runtime.CompilerServices", "ReferenceAssemblyAttribute"),   // ReferenceAssembly
        TopLevelTypeName("System", "SerializableAttribute"),   // Serializable
        TopLevelTypeName("System", "FlagsAttribute"),   // Flags
        TopLevelTypeName("System.Runtime.InteropServices", "ComImportAttribute"),   // ComImport
        TopLevelTypeName("System.Runtime.InteropServices", "CoClassAttribute"),   // CoClass
        TopLevelTypeName("System.Runtime.InteropServices", "StructLayoutAttribute"),   // StructLayout
        TopLevelTypeName("System.Reflection", "DefaultMemberAttribute"),   // DefaultMember
        TopLevelTypeName("System.Runtime.CompilerServices", "IsByRefLikeAttribute"),   // IsByRefLike
        TopLevelTypeName("System.Runtime.CompilerServices", "IteratorStateMachineAttribute"),   // IteratorStateMachine
        TopLevelTypeName("System.Runtime.CompilerServices", "AsyncStateMachineAttribute"),   // AsyncStateMachine
        TopLevelTypeName("System.Runtime.CompilerServices", "AsyncMethodBuilderAttribute"),   // AsyncMethodBuilder
        TopLevelTypeName("System.Runtime.CompilerServices", "AsyncIteratorStateMachineAttribute"),   // AsyncIteratorStateMachine
        TopLevelTypeName("System.Runtime.InteropServices", "FieldOffsetAttribute"),   // FieldOffset
        TopLevelTypeName("System", "NonSerializedAttribute"),   // NonSerialized
        TopLevelTypeName("System.Runtime.CompilerServices", "DecimalConstantAttribute"),   // DecimalConstant
        TopLevelTypeName("System.Runtime.CompilerServices", "FixedBufferAttribute"),   // FixedBuffer
        TopLevelTypeName("System.Runtime.InteropServices", "DllImportAttribute"),   // DllImport
        TopLevelTypeName("System.Runtime.InteropServices", "PreserveSigAttribute"),   // PreserveSig
        TopLevelTypeName("System.Runtime.CompilerServices", "MethodImplAttribute"),   // MethodImpl
        TopLevelTypeName("System.Runtime.CompilerServices", "IndexerNameAttribute"),   // IndexerName
        TopLevelTypeName("System", "ParamArrayAttribute"),   // ParamArray
        TopLevelTypeName("System.Runtime.CompilerServices", "ParamCollectionAttribute"),   // ParamCollection
        TopLevelTypeName("System.Runtime.InteropServices", "InAttribute"),   // In
        TopLevelTypeName("System.Runtime.InteropServices", "OutAttribute"),   // Out
        TopLevelTypeName("System.Runtime.InteropServices", "OptionalAttribute"),   // Optional
        TopLevelTypeName("System.Runtime.InteropServices", "DefaultParameterValueAttribute"),   // DefaultParameterValue
        TopLevelTypeName("System.Runtime.CompilerServices", "CallerMemberNameAttribute"),   // CallerMemberName
        TopLevelTypeName("System.Runtime.CompilerServices", "CallerFilePathAttribute"),   // CallerFilePath
        TopLevelTypeName("System.Runtime.CompilerServices", "CallerLineNumberAttribute"),   // CallerLineNumber
        TopLevelTypeName("System.Runtime.CompilerServices", "ScopedRefAttribute"),   // ScopedRef
        TopLevelTypeName("System.Runtime.CompilerServices", "RequiresLocationAttribute"),   // RequiresLocation
        TopLevelTypeName("System.Runtime.CompilerServices", "IsUnmanagedAttribute"),   // IsUnmanaged
        TopLevelTypeName("System.Runtime.InteropServices", "MarshalAsAttribute"),   // MarshalAs
        TopLevelTypeName("System.Security.Permissions", "PermissionSetAttribute"),   // PermissionSet
        TopLevelTypeName("System.Runtime.CompilerServices", "NativeIntegerAttribute"),   // NativeInteger
        TopLevelTypeName("System.Runtime.CompilerServices", "PreserveBaseOverridesAttribute"),   // PreserveBaseOverrides
        TopLevelTypeName("System.Runtime.InteropServices", "UnmanagedCallersOnlyAttribute"),   // UnmanagedCallersOnly
        TopLevelTypeName("System.Runtime.CompilerServices", "RequiredMemberAttribute"),   // Required
        TopLevelTypeName("System.Runtime.CompilerServices", "InlineArrayAttribute"),   // InlineArray
        TopLevelTypeName("System.Runtime.CompilerServices", "ExtensionMarkerAttribute"),   // ExtensionMarker
    };
    return names;
}

}  // namespace

const TopLevelTypeName& get_type_name(KnownAttribute attribute) { return type_names()[static_cast<std::size_t>(attribute)]; }

const IType* find_type(const ICompilation& compilation, KnownAttribute attribute) { return find_type(compilation, FullTypeName(get_type_name(attribute))); }

KnownAttribute is_known_attribute_type(const ITypeDefinition* attribute_type) {
    bool is_attribute = false;
    for (const IType* t : get_non_interface_base_types(attribute_type)) is_attribute = is_attribute || is_known_type(t, KnownTypeCode::Attribute);
    if (!is_attribute) return KnownAttribute::None;
    const FullTypeName name = attribute_type->full_type_name();
    for (int i = 1; i < kKnownAttributeCount; ++i)
        if (FullTypeName(type_names()[static_cast<std::size_t>(i)]) == name) return static_cast<KnownAttribute>(i);
    return KnownAttribute::None;
}

bool is_custom_attribute(KnownAttribute attribute) {
    switch (attribute) {
    case KnownAttribute::Serializable: case KnownAttribute::ComImport: case KnownAttribute::StructLayout: case KnownAttribute::DllImport:
    case KnownAttribute::PreserveSig: case KnownAttribute::MethodImpl: case KnownAttribute::FieldOffset: case KnownAttribute::NonSerialized:
    case KnownAttribute::MarshalAs: case KnownAttribute::PermissionSet: case KnownAttribute::Optional: case KnownAttribute::DefaultParameterValue:
    case KnownAttribute::In: case KnownAttribute::Out: case KnownAttribute::IndexerName: case KnownAttribute::SpecialName:
        return false;
    default:
        return true;
    }
}

bool is_known_type(const IType* type, KnownAttribute attribute) {
    const ITypeDefinition* def = type->get_definition();
    return def != nullptr && is_known_type(def->full_type_name(), attribute);
}

bool is_known_type(const FullTypeName& name, KnownAttribute attribute) { return name == FullTypeName(get_type_name(attribute)); }
bool is_known_type(const TopLevelTypeName& name, KnownAttribute attribute) { return name == get_type_name(attribute); }

// ---- on metadata ---------------------------------------------------------------------------------------------------------

bool is_known_type(srm::EntityHandle handle, const srm::MetadataReader& reader, KnownAttribute attribute) {
    return decompiler::is_known_type(handle, reader, get_type_name(attribute));
}

bool is_known_attribute(const srm::CustomAttribute& attribute, const srm::MetadataReader& metadata, KnownAttribute type) {
    return is_known_type(get_attribute_type(attribute, metadata), metadata, type);
}

bool has_known_attribute(const std::vector<srm::CustomAttributeHandle>& custom_attributes, const srm::MetadataReader& metadata, KnownAttribute type) {
    for (const srm::CustomAttributeHandle h : custom_attributes)
        if (is_known_attribute(metadata.get_custom_attribute(h), metadata, type)) return true;
    return false;
}

bool is_compiler_generated(srm::MethodDefinitionHandle handle, const srm::MetadataReader& metadata) {
    return has_known_attribute(metadata.get_method_definition(handle).get_custom_attributes(), metadata, KnownAttribute::CompilerGenerated);
}
bool is_compiler_generated(srm::FieldDefinitionHandle handle, const srm::MetadataReader& metadata) {
    return has_known_attribute(metadata.get_field_definition(handle).get_custom_attributes(), metadata, KnownAttribute::CompilerGenerated);
}
bool is_compiler_generated(srm::TypeDefinitionHandle handle, const srm::MetadataReader& metadata) {
    return has_known_attribute(metadata.get_type_definition(handle).get_custom_attributes(), metadata, KnownAttribute::CompilerGenerated);
}

bool is_compiler_generated_or_is_in_compiler_generated_class(srm::MethodDefinitionHandle handle, const srm::MetadataReader& metadata) {
    if (is_compiler_generated(handle, metadata)) return true;
    const srm::TypeDefinitionHandle declaring_type = metadata.get_method_definition(handle).get_declaring_type();
    return !declaring_type.is_nil() && is_compiler_generated(declaring_type, metadata);
}

bool is_compiler_generated_or_is_in_compiler_generated_class(srm::TypeDefinitionHandle handle, const srm::MetadataReader& metadata) {
    if (is_compiler_generated(handle, metadata)) return true;
    const srm::TypeDefinitionHandle declaring_type = metadata.get_type_definition(handle).get_declaring_type();
    return !declaring_type.is_nil() && is_compiler_generated(declaring_type, metadata);
}

}  // namespace cb::dotnet::decompiler::typesystem
