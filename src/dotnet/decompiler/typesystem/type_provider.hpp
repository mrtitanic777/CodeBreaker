// dotnet/decompiler/typesystem/type_provider.hpp -- how signatures become types: the generic context a signature's !0 and !!0 refer to,
// the provider the metadata decoders call to build each type (dotnet/metadata/signature_decoder.hpp, custom_attribute_decoder.hpp), and
// the pass that applies C#'s attributes on a signature to the type it decoded ([Dynamic] object -> dynamic, ValueTuple -> a tuple,
// [Nullable] -> string?, [NativeInteger] IntPtr -> nint).
//
//     TypeProvider provider(module);
//     const IType* t = md.get_type_specification(h).decode_signature(provider, GenericContext(&type->type_parameters()));
//     t = apply_attributes_to_type(t, compilation, &attributes, md, options, Nullability::Oblivious);
//
// Ported from ILSpy (ICSharpCode.Decompiler.TypeSystem: GenericContext, TypeProvider, ApplyAttributeTypeVisitor; TypeSystem.Implementation:
// PinnedType; Metadata.MetadataExtensions' minimalCorlibTypeProvider; FunctionPointerType.FromSignature), Copyright AlphaSierraPapa for the
// ILSpy team, MIT (see NOTICE in dotnet/decompiler). Not ported: ApplyAttributesToType's PdbExtraTypeInfo overload (comes with the PDB
// reader). SRM's MetadataReader.ResolveSignatureTypeKind is written here (without WinRT projections, which SRM applies only when asked).
#pragma once

#include <optional>
#include <string>
#include <vector>

#include "dotnet/decompiler/typesystem/compilation.hpp"
#include "dotnet/decompiler/typesystem/tuple_type.hpp"
#include "dotnet/decompiler/typesystem/types.hpp"
#include "dotnet/metadata/signature_decoder.hpp"

namespace cb::dotnet::decompiler::typesystem {

class MetadataModule;

// The type parameters a signature's !n (class) and !!n (method) are; a missing list (nullptr) or index gives a dummy parameter.
struct GenericContext {
    const std::vector<const ITypeParameter*>* class_type_parameters = nullptr;
    const std::vector<const ITypeParameter*>* method_type_parameters = nullptr;

    GenericContext() = default;
    explicit GenericContext(const std::vector<const ITypeParameter*>* class_type_parameters_,
                            const std::vector<const ITypeParameter*>* method_type_parameters_ = nullptr)
        : class_type_parameters(class_type_parameters_), method_type_parameters(method_type_parameters_) {}
    // The current type's and member's parameters, or an entity's (a type's own; a member's type's and, for a method, its own).
    static GenericContext of(const ITypeResolveContext& context);
    static GenericContext of(const IEntity& context);

    const ITypeParameter* get_class_type_parameter(int index) const;
    const ITypeParameter* get_method_type_parameter(int index) const;
    // The substitution of the parameters for themselves (none for an empty list, as the type system makes them).
    TypeParameterSubstitution to_substitution() const;
};

// A pinned local's type (`int32& pinned`).
class PinnedType final : public TypeWithElementType {
public:
    explicit PinnedType(const IType* element_type) : TypeWithElementType(element_type) {}
    std::string name_suffix() const override { return " pinned"; }
    std::optional<bool> is_reference_type() const override { return element_type_->is_reference_type(); }
    bool is_by_ref_like() const override { return element_type_->is_by_ref_like(); }
    TypeKind kind() const override { return TypeKind::Other; }
    const IType* visit_children(TypeVisitor& visitor) const override;
};
const IType* make_pinned_type(const IType* element_type);

// The provider of types to the signature and custom attribute decoders, over a module (its definitions, and the modules it references)
// or over a compilation alone (the types of the compilation's modules by name).
class TypeProvider {
public:
    explicit TypeProvider(const MetadataModule& module);
    explicit TypeProvider(const ICompilation& compilation) : compilation_(&compilation) {}
    const ICompilation& compilation() const { return *compilation_; }

    // ISignatureTypeProvider
    const IType* get_array_type(const IType* element_type, const srm::ArrayShape& shape) const;
    const IType* get_by_reference_type(const IType* element_type) const { return make_by_reference_type(element_type); }
    // An instance method pointer is IntPtr (C# has no pointers to member functions).
    const IType* get_function_pointer_type(const srm::MethodSignature<const IType*>& signature) const;
    // The generic type itself where the arguments do not fit it (a type of an assembly that is not loaded, without the `N).
    const IType* get_generic_instantiation(const IType* generic_type, const std::vector<const IType*>& type_arguments) const;
    const IType* get_generic_method_parameter(const GenericContext& context, int index) const { return context.get_method_type_parameter(index); }
    const IType* get_generic_type_parameter(const GenericContext& context, int index) const { return context.get_class_type_parameter(index); }
    const IType* get_modified_type(const IType* modifier, const IType* unmodified_type, bool is_required) const {
        return make_modified_type(modifier, unmodified_type, is_required);
    }
    const IType* get_pinned_type(const IType* element_type) const { return make_pinned_type(element_type); }
    const IType* get_pointer_type(const IType* element_type) const { return make_pointer_type(element_type); }
    const IType* get_primitive_type(srm::PrimitiveTypeCode type_code) const;
    const IType* get_sz_array_type(const IType* element_type) const { return make_array_type(*compilation_, element_type); }
    const IType* get_type_from_definition(const srm::MetadataReader& reader, srm::TypeDefinitionHandle handle, std::uint8_t raw_type_kind) const;
    const IType* get_type_from_reference(const srm::MetadataReader& reader, srm::TypeReferenceHandle handle, std::uint8_t raw_type_kind) const;
    const IType* get_type_from_specification(const srm::MetadataReader& reader, const GenericContext& context, srm::TypeSpecificationHandle handle,
                                             std::uint8_t raw_type_kind) const;

    // ICustomAttributeTypeProvider
    const IType* get_system_type() const { return compilation_->find_type(KnownTypeCode::Type); }
    bool is_system_type(const IType* type) const;
    // BadImageFormatError for a name that is not a type name.
    const IType* get_type_from_serialized_name(const std::string& name) const;
    // EnumUnderlyingTypeResolveError where the enum's definition is not known.
    srm::PrimitiveTypeCode get_underlying_enum_type(const IType* type) const;

private:
    const MetadataModule* module_ = nullptr;
    const ICompilation* compilation_;
};

// The provider over the minimal corlib: for attributes whose signatures mention only built-in types (Nullable, Dynamic,
// TupleElementNames ...), decoded before any compilation exists.
const TypeProvider& minimal_corlib_type_provider();

// FunctionPointerType.FromSignature: in/out/ref readonly from the modifiers, the calling convention from CallConv* modifiers.
const FunctionPointerType* function_pointer_type_from_signature(const srm::MethodSignature<const IType*>& signature, const MetadataModule& module);

// The type with the C# meaning of its attributes applied (dynamic, tuple names, nullability, nint). `attributes`, then
// `additional_attributes` (which win), are the attributes on the signature's owner; nullptr for none. type_children_only applies them
// to the type's parts but not the type itself.
const IType* apply_attributes_to_type(const IType* input_type, const ICompilation& compilation, const std::vector<srm::CustomAttributeHandle>* attributes,
                                      const srm::MetadataReader& metadata, TypeSystemOptions options, Nullability nullable_context,
                                      bool type_children_only = false, const std::vector<srm::CustomAttributeHandle>* additional_attributes = nullptr);

}  // namespace cb::dotnet::decompiler::typesystem
