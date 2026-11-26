// dotnet/decompiler/typesystem/known_attributes.hpp -- the attributes the decompiler knows by name (CompilerGenerated, Extension, Nullable,
// ParamArray, StructLayout ...): their type names, and which of them the C# compiler writes as metadata flags rather than attributes.
//
// Ported from ILSpy (ICSharpCode.Decompiler.TypeSystem: KnownAttribute, KnownAttributes), Copyright AlphaSierraPapa for the ILSpy team, MIT
// (see NOTICE in dotnet/decompiler); with SRMExtensions' members on attributes (IsKnownType, HasKnownAttribute, IsKnownAttribute,
// IsCompilerGenerated ...). GetNullableContext is in attributes.hpp (it decodes the attribute).
#pragma once

#include "dotnet/decompiler/typesystem/type_system.hpp"
#include "dotnet/metadata/metadata_reader.hpp"

namespace cb::dotnet::decompiler::typesystem {

enum class KnownAttribute : std::uint8_t {
    None,
    CompilerGenerated,
    CompilerFeatureRequired,
    Extension,
    Dynamic,
    TupleElementNames,
    Nullable,
    NullableContext,
    NullablePublicOnly,
    Conditional,
    Obsolete,
    Embedded,
    IsReadOnly,
    SpecialName,
    DebuggerHidden,
    DebuggerStepThrough,
    DebuggerBrowsable,
    AssemblyVersion,
    InternalsVisibleTo,
    TypeForwardedTo,
    ReferenceAssembly,
    Serializable,
    Flags,
    ComImport,
    CoClass,
    StructLayout,
    DefaultMember,
    IsByRefLike,
    IteratorStateMachine,
    AsyncStateMachine,
    AsyncMethodBuilder,
    AsyncIteratorStateMachine,
    FieldOffset,
    NonSerialized,
    DecimalConstant,
    FixedBuffer,
    DllImport,
    PreserveSig,
    MethodImpl,
    IndexerName,
    ParamArray,
    ParamCollection,
    In,
    Out,
    Optional,
    DefaultParameterValue,
    CallerMemberName,
    CallerFilePath,
    CallerLineNumber,
    ScopedRef,
    RequiresLocation,
    IsUnmanaged,
    MarshalAs,
    PermissionSet,
    NativeInteger,
    PreserveBaseOverrides,
    UnmanagedCallersOnly,
    Required,
    InlineArray,
    ExtensionMarker,
};
inline constexpr int kKnownAttributeCount = static_cast<int>(KnownAttribute::ExtensionMarker) + 1;

const TopLevelTypeName& get_type_name(KnownAttribute attribute);
// The attribute's type in the compilation (an UnknownType when it has none).
const IType* find_type(const ICompilation& compilation, KnownAttribute attribute);
// Which known attribute a type is (None for one that is not an attribute, or not a known one).
KnownAttribute is_known_attribute_type(const ITypeDefinition* attribute_type);
// False for the attributes that are metadata flags (Serializable, DllImport, MarshalAs ...), which are not written as attributes.
bool is_custom_attribute(KnownAttribute attribute);
bool is_known_type(const IType* type, KnownAttribute attribute);
bool is_known_type(const FullTypeName& name, KnownAttribute attribute);
bool is_known_type(const TopLevelTypeName& name, KnownAttribute attribute);

// ---- on metadata (SRMExtensions' attribute members) ----------------------------------------------------------------------

// The handle (a TypeDef or TypeRef) names the attribute's type.
bool is_known_type(srm::EntityHandle handle, const srm::MetadataReader& reader, KnownAttribute attribute);
bool is_known_attribute(const srm::CustomAttribute& attribute, const srm::MetadataReader& metadata, KnownAttribute type);
bool has_known_attribute(const std::vector<srm::CustomAttributeHandle>& custom_attributes, const srm::MetadataReader& metadata, KnownAttribute type);
// Marked [CompilerGenerated].
bool is_compiler_generated(srm::MethodDefinitionHandle handle, const srm::MetadataReader& metadata);
bool is_compiler_generated(srm::FieldDefinitionHandle handle, const srm::MetadataReader& metadata);
bool is_compiler_generated(srm::TypeDefinitionHandle handle, const srm::MetadataReader& metadata);
// The method or type, or the type it is declared in, is [CompilerGenerated].
bool is_compiler_generated_or_is_in_compiler_generated_class(srm::MethodDefinitionHandle handle, const srm::MetadataReader& metadata);
bool is_compiler_generated_or_is_in_compiler_generated_class(srm::TypeDefinitionHandle handle, const srm::MetadataReader& metadata);

}  // namespace cb::dotnet::decompiler::typesystem
