// dotnet/decompiler/typesystem/type_provider.cpp -- generic contexts, the type provider, pinned types, function pointer signatures and the
// visitor that applies C#'s type attributes.
#include "dotnet/decompiler/typesystem/type_provider.hpp"

#include <algorithm>

#include "dotnet/decompiler/metadata/assembly_references.hpp"
#include "dotnet/decompiler/metadata/metadata_extensions.hpp"
#include "dotnet/decompiler/srm_extensions.hpp"
#include "dotnet/decompiler/typesystem/known_attributes.hpp"
#include "dotnet/decompiler/typesystem/metadata_module.hpp"
#include "dotnet/decompiler/typesystem/reflection_helper.hpp"
#include "dotnet/decompiler/typesystem/type_system_extensions.hpp"
#include "dotnet/metadata/custom_attribute_decoder.hpp"

namespace cb::dotnet::decompiler::typesystem {

// ---- GenericContext ------------------------------------------------------------------------------------------------------

GenericContext GenericContext::of(const ITypeResolveContext& context) {
    GenericContext c;
    if (const ITypeDefinition* td = context.current_type_definition()) c.class_type_parameters = &td->type_parameters();
    if (const auto* m = dynamic_cast<const IMethod*>(context.current_member())) c.method_type_parameters = &m->type_parameters();
    return c;
}

GenericContext GenericContext::of(const IEntity& context) {
    GenericContext c;
    if (const auto* td = dynamic_cast<const ITypeDefinition*>(&context)) {
        c.class_type_parameters = &td->type_parameters();
        return c;
    }
    if (const ITypeDefinition* declaring = context.declaring_type_definition()) c.class_type_parameters = &declaring->type_parameters();
    if (const auto* m = dynamic_cast<const IMethod*>(&context)) c.method_type_parameters = &m->type_parameters();
    return c;
}

const ITypeParameter* GenericContext::get_class_type_parameter(int index) const {
    if (class_type_parameters != nullptr && index >= 0 && index < static_cast<int>(class_type_parameters->size()))
        return (*class_type_parameters)[static_cast<std::size_t>(index)];
    return DummyTypeParameter::get_class_type_parameter(index);
}

const ITypeParameter* GenericContext::get_method_type_parameter(int index) const {
    if (method_type_parameters != nullptr && index >= 0 && index < static_cast<int>(method_type_parameters->size()))
        return (*method_type_parameters)[static_cast<std::size_t>(index)];
    return DummyTypeParameter::get_method_type_parameter(index);
}

TypeParameterSubstitution GenericContext::to_substitution() const {
    const auto as_types = [](const std::vector<const ITypeParameter*>* list) -> std::optional<std::vector<const IType*>> {
        if (list == nullptr || list->empty()) return std::nullopt;   // the type system prefers none over an empty list
        return std::vector<const IType*>(list->begin(), list->end());
    };
    return TypeParameterSubstitution(as_types(class_type_parameters), as_types(method_type_parameters));
}

// ---- PinnedType ----------------------------------------------------------------------------------------------------------

const IType* PinnedType::visit_children(TypeVisitor& visitor) const {
    const IType* new_type = element_type_->accept_visitor(visitor);
    return new_type == element_type_ ? this : make_pinned_type(new_type);
}

const IType* make_pinned_type(const IType* element_type) {
    TypeKey key{32, {element_type}, {}, {}};
    return arena_for({element_type}).intern<IType>(std::move(key), [&]() -> std::unique_ptr<IType> { return std::make_unique<PinnedType>(element_type); });
}

// ---- TypeProvider --------------------------------------------------------------------------------------------------------

namespace {

// MetadataReader.ResolveSignatureTypeKind (without WinRT projections): what a signature's CLASS / VALUETYPE says of a type.
std::optional<bool> is_reference_type(std::uint8_t raw_type_kind) {
    switch (raw_type_kind) {
    case 0x11: return false;   // ValueType
    case 0x12: return true;    // Class
    case 0: return std::nullopt;
    default: throw std::out_of_range("rawTypeKind");
    }
}

// TypeUtils.GetEnumUnderlyingType
const IType* get_enum_underlying_type(const IType* type) {
    type = skip_modifiers(type);
    return type->kind() == TypeKind::Enum ? type->get_definition()->enum_underlying_type() : type;
}

}  // namespace

TypeProvider::TypeProvider(const MetadataModule& module) : module_(&module), compilation_(&module.compilation()) {}

const IType* TypeProvider::get_array_type(const IType* element_type, const srm::ArrayShape& shape) const {
    return make_array_type(*compilation_, element_type, shape.rank);
}

const IType* TypeProvider::get_function_pointer_type(const srm::MethodSignature<const IType*>& signature) const {
    if (signature.header.is_instance()) return compilation_->find_type(KnownTypeCode::IntPtr);   // pointers to member functions: not even in C# 9
    if (module_ == nullptr) throw srm::BadImageFormatError(0, "a function pointer type in an attribute signature");   // (upstream: null module)
    return function_pointer_type_from_signature(signature, *module_);
}

const IType* TypeProvider::get_generic_instantiation(const IType* generic_type, const std::vector<const IType*>& type_arguments) const {
    const int tpc = generic_type->type_parameter_count();
    // a type of another assembly that is not loaded, without the usual `1 suffix
    if (tpc == 0 || tpc != static_cast<int>(type_arguments.size())) return generic_type;
    return make_parameterized_type(generic_type, type_arguments);
}

const IType* TypeProvider::get_primitive_type(srm::PrimitiveTypeCode type_code) const {
    return compilation_->find_type(metadata::to_known_type_code(type_code));
}

const IType* TypeProvider::get_type_from_definition(const srm::MetadataReader& reader, srm::TypeDefinitionHandle handle, std::uint8_t raw_type_kind) const {
    if (module_ != nullptr)
        if (const ITypeDefinition* td = module_->get_definition(handle)) return td;
    return make_unknown_type(compilation_->arena(), get_full_type_name(handle, reader), is_reference_type(raw_type_kind));
}

const IType* TypeProvider::get_type_from_reference(const srm::MetadataReader& reader, srm::TypeReferenceHandle handle, std::uint8_t raw_type_kind) const {
    const IModule* resolved_module = module_ != nullptr ? module_->get_declaring_module(handle) : nullptr;
    const FullTypeName full_type_name = get_full_type_name(handle, reader);
    const IType* type = nullptr;
    if (resolved_module != nullptr) {
        type = typesystem::get_type_definition(*resolved_module, full_type_name);
    } else {
        for (const IModule* asm_ : compilation_->modules())
            if (const ITypeDefinition* td = typesystem::get_type_definition(*asm_, full_type_name)) return td;
    }
    return type != nullptr ? type : make_unknown_type(compilation_->arena(), full_type_name, is_reference_type(raw_type_kind));
}

const IType* TypeProvider::get_type_from_specification(const srm::MetadataReader& reader, const GenericContext& context, srm::TypeSpecificationHandle handle,
                                                       std::uint8_t) const {
    return reader.get_type_specification(handle).decode_signature(*this, context);
}

bool TypeProvider::is_system_type(const IType* type) const { return is_known_type(type, KnownTypeCode::Type); }

const IType* TypeProvider::get_type_from_serialized_name(const std::string& name) const {
    try {
        if (module_ != nullptr) return parse_reflection_name(name, SimpleTypeResolveContext(*module_));
        return parse_reflection_name(name, SimpleTypeResolveContext(*compilation_));
    } catch (const ReflectionNameParseError& ex) {
        throw srm::BadImageFormatError(0, "Invalid type name: \"" + name + "\": " + ex.what());
    }
}

srm::PrimitiveTypeCode TypeProvider::get_underlying_enum_type(const IType* type) const {
    const ITypeDefinition* def = get_enum_underlying_type(type)->get_definition();
    if (def == nullptr) throw metadata::EnumUnderlyingTypeResolveError();
    return metadata::to_primitive_type_code(def->known_type_code());
}

const TypeProvider& minimal_corlib_type_provider() {
    static const SimpleCompilation compilation(minimal_corlib::instance(), {});
    static const TypeProvider provider(compilation);
    return provider;
}

// ---- FunctionPointerType.FromSignature -----------------------------------------------------------------------------------

const FunctionPointerType* function_pointer_type_from_signature(const srm::MethodSignature<const IType*>& signature, const MetadataModule& module) {
    const IType* return_type = signature.return_type;
    bool return_is_ref_read_only = false;
    srm::SignatureCallingConvention calling_convention = signature.header.calling_convention();
    std::vector<const IType*> custom_call_convs;
    while (const auto* mod_return = dynamic_cast<const ModifiedType*>(return_type)) {
        const IType* modifier = mod_return->modifier();
        if (is_known_type(modifier, KnownAttribute::In)) {
            return_type = mod_return->element_type();
            return_is_ref_read_only = true;
        } else if (modifier->name().rfind("CallConv", 0) == 0 && modifier->namespace_() == "System.Runtime.CompilerServices") {
            return_type = mod_return->element_type();
            if (calling_convention == srm::SignatureCallingConvention::Unmanaged) {
                const std::string name = modifier->name();
                if (name == "CallConvCdecl") calling_convention = srm::SignatureCallingConvention::CDecl;
                else if (name == "CallConvFastcall") calling_convention = srm::SignatureCallingConvention::FastCall;
                else if (name == "CallConvStdcall") calling_convention = srm::SignatureCallingConvention::StdCall;
                else if (name == "CallConvThiscall") calling_convention = srm::SignatureCallingConvention::ThisCall;
                else custom_call_convs.push_back(modifier);
            } else {
                custom_call_convs.push_back(modifier);
            }
        } else {
            break;
        }
    }
    std::vector<const IType*> parameter_types;
    std::vector<ReferenceKind> parameter_reference_kinds;
    parameter_types.reserve(signature.parameter_types.size());
    for (const IType* p : signature.parameter_types) {
        const IType* param_type = p;
        ReferenceKind kind = ReferenceKind::None;
        if (const auto* mod = dynamic_cast<const ModifiedType*>(p)) {
            if (is_known_type(mod->modifier(), KnownAttribute::In)) {
                kind = ReferenceKind::In;
                param_type = mod->element_type();
            } else if (is_known_type(mod->modifier(), KnownAttribute::Out)) {
                kind = ReferenceKind::Out;
                param_type = mod->element_type();
            } else if (is_known_type(mod->modifier(), KnownAttribute::RequiresLocation)) {
                kind = ReferenceKind::RefReadOnly;
                param_type = mod->element_type();
            }
        }
        if (param_type->kind() == TypeKind::ByReference) {
            if (kind == ReferenceKind::None) kind = ReferenceKind::Ref;
        } else {
            kind = ReferenceKind::None;
        }
        parameter_types.push_back(param_type);
        parameter_reference_kinds.push_back(kind);
    }
    return make_function_pointer_type(module.compilation(), any(module.type_system_options(), TypeSystemOptions::FunctionPointers), calling_convention,
                                      std::move(custom_call_convs), return_type, return_is_ref_read_only, std::move(parameter_types),
                                      std::move(parameter_reference_kinds));
}

// ---- ApplyAttributeTypeVisitor -------------------------------------------------------------------------------------------

namespace {

class ApplyAttributeTypeVisitor final : public TypeVisitor {
public:
    ApplyAttributeTypeVisitor(const ICompilation& compilation, bool has_dynamic_attribute, std::optional<std::vector<bool>> dynamic_attribute_data,
                              bool has_native_integers_attribute, std::optional<std::vector<bool>> native_integers_attribute_data, TypeSystemOptions options,
                              std::optional<std::vector<std::optional<std::string>>> tuple_element_names, Nullability default_nullability,
                              std::optional<std::vector<Nullability>> nullable_attribute_data)
        : compilation_(compilation), has_dynamic_attribute_(has_dynamic_attribute), dynamic_attribute_data_(std::move(dynamic_attribute_data)),
          has_native_integers_attribute_(has_native_integers_attribute), native_integers_attribute_data_(std::move(native_integers_attribute_data)),
          options_(options), tuple_element_names_(std::move(tuple_element_names)), default_nullability_(default_nullability),
          nullable_attribute_data_(std::move(nullable_attribute_data)) {}

    const IType* visit_mod_opt(const ModifiedType& type) override {
        ++dynamic_type_index_;
        if (any(options_, TypeSystemOptions::KeepModifiers)) return TypeVisitor::visit_mod_opt(type);
        return type.element_type()->accept_visitor(*this);
    }

    const IType* visit_mod_req(const ModifiedType& type) override {
        ++dynamic_type_index_;
        if (any(options_, TypeSystemOptions::KeepModifiers)) return TypeVisitor::visit_mod_req(type);
        return type.element_type()->accept_visitor(*this);
    }

    const IType* visit_pointer_type(const PointerType& type) override {
        ++dynamic_type_index_;
        return TypeVisitor::visit_pointer_type(type);
    }

    const IType* visit_array_type(const ArrayType& type) override {
        const Nullability nullability = get_nullability();
        ++dynamic_type_index_;
        return TypeVisitor::visit_array_type(type)->change_nullability(nullability);
    }

    const IType* visit_by_reference_type(const ByReferenceType& type) override {
        ++dynamic_type_index_;
        return TypeVisitor::visit_by_reference_type(type);
    }

    const IType* visit_parameterized_type(const ParameterizedType& type_in) override {
        const ParameterizedType* type = &type_in;
        const bool use_tuple_types = any(options_, TypeSystemOptions::Tuple);
        int tuple_cardinality = 0;
        if (use_tuple_types && TupleType::is_tuple_compatible(type, tuple_cardinality)) {
            if (tuple_cardinality > 1) {
                const ITypeDefinition* def = type->get_definition();
                const IModule* value_tuple_assembly = def != nullptr ? def->parent_module() : nullptr;
                std::vector<std::optional<std::string>> element_names;
                if (tuple_element_names_ && tuple_type_index_ < static_cast<int>(tuple_element_names_->size())) {
                    element_names.assign(static_cast<std::size_t>(tuple_cardinality), std::nullopt);
                    const int n = std::min(tuple_cardinality, static_cast<int>(tuple_element_names_->size()) - tuple_type_index_);
                    for (int i = 0; i < n; ++i) element_names[static_cast<std::size_t>(i)] = (*tuple_element_names_)[static_cast<std::size_t>(tuple_type_index_ + i)];
                }
                tuple_type_index_ += tuple_cardinality;
                expect_dummy_nullability_for_generic_value_type();
                std::vector<const IType*> element_types;
                element_types.reserve(static_cast<std::size_t>(tuple_cardinality));
                do {
                    const int normal_arg_count = std::min(static_cast<int>(type->type_arguments().size()), TupleType::kRestPosition - 1);
                    for (int i = 0; i < normal_arg_count; ++i) {
                        ++dynamic_type_index_;
                        element_types.push_back(type->type_arguments()[static_cast<std::size_t>(i)]->accept_visitor(*this));
                    }
                    if (static_cast<int>(type->type_arguments().size()) == TupleType::kRestPosition) {
                        type = dynamic_cast<const ParameterizedType*>(type->type_arguments().back());
                        expect_dummy_nullability_for_generic_value_type();
                        ++dynamic_type_index_;
                        int nested_cardinality = 0;
                        if (type != nullptr && TupleType::is_tuple_compatible(type, nested_cardinality)) tuple_type_index_ += nested_cardinality;
                        else type = nullptr;   // (TRest should be another value tuple)
                    } else {
                        type = nullptr;
                    }
                } while (type != nullptr);
                return make_tuple_type(compilation_, std::move(element_types), std::move(element_names), value_tuple_assembly);
            }
            tuple_type_index_ += tuple_cardinality;   // C# has no syntax for tuples of cardinality <= 1
        }
        // the generic type and the type arguments, as the base does, counting dynamic_type_index_
        const IType* generic_type = type->generic_type()->accept_visitor(*this);
        if (generic_type->is_reference_type() != std::optional<bool>(true) && !is_known_type(generic_type, KnownTypeCode::NullableOfT))
            expect_dummy_nullability_for_generic_value_type();
        bool changed = type->generic_type() != generic_type;
        std::vector<const IType*> arguments(type->type_arguments().size());
        for (std::size_t i = 0; i < arguments.size(); ++i) {
            ++dynamic_type_index_;
            arguments[i] = type->type_arguments()[i]->accept_visitor(*this);
            changed = changed || arguments[i] != type->type_arguments()[i];
        }
        if (!changed) return type;
        return make_parameterized_type(generic_type, std::move(arguments));
    }

    const IType* visit_function_pointer_type(const FunctionPointerType& type) override {
        ++dynamic_type_index_;
        if (type.return_is_ref_read_only) ++dynamic_type_index_;
        const IType* return_type = type.return_type->accept_visitor(*this);
        bool changed = type.return_type != return_type;
        std::vector<const IType*> parameters(type.parameter_types.size());
        for (std::size_t i = 0; i < parameters.size(); ++i) {
            switch (type.parameter_reference_kinds[i]) {
            case ReferenceKind::None:
            case ReferenceKind::Ref: dynamic_type_index_ += 1; break;
            case ReferenceKind::Out:
            case ReferenceKind::In:
            case ReferenceKind::RefReadOnly: dynamic_type_index_ += 2; break;   // (counting the modreq / modopt)
            }
            parameters[i] = type.parameter_types[i]->accept_visitor(*this);
            changed = changed || parameters[i] != type.parameter_types[i];
        }
        if (!changed) return &type;
        return type.with_signature(return_type, std::move(parameters));
    }

    const IType* visit_type_definition(const ITypeDefinition& type) override {
        const IType* new_type = &type;
        const KnownTypeCode ktc = type.known_type_code();
        if (ktc == KnownTypeCode::Object && has_dynamic_attribute_) {
            if (!dynamic_attribute_data_ || dynamic_type_index_ >= static_cast<int>(dynamic_attribute_data_->size())) new_type = &SpecialType::dynamic();
            else if ((*dynamic_attribute_data_)[static_cast<std::size_t>(dynamic_type_index_)]) new_type = &SpecialType::dynamic();
        } else if ((ktc == KnownTypeCode::IntPtr || ktc == KnownTypeCode::UIntPtr) && has_native_integers_attribute_) {
            // native integers use the same indexing as dynamic
            const IType* native = ktc == KnownTypeCode::IntPtr ? &SpecialType::nint() : &SpecialType::nuint();
            if (!native_integers_attribute_data_ || native_int_type_index_ >= static_cast<int>(native_integers_attribute_data_->size())) new_type = native;
            else if ((*native_integers_attribute_data_)[static_cast<std::size_t>(native_int_type_index_)]) new_type = native;
            ++native_int_type_index_;
        }
        if (type.is_reference_type() == std::optional<bool>(true)) return new_type->change_nullability(get_nullability());
        return new_type;
    }

    const IType* visit_other_type(const IType& type_in) override {
        const IType* type = TypeVisitor::visit_other_type(type_in);
        if (type->kind() == TypeKind::Unknown && type->is_reference_type() == std::optional<bool>(true)) type = type->change_nullability(get_nullability());
        return type;
    }

    const IType* visit_type_parameter(const ITypeParameter& type) override { return type.change_nullability(get_nullability()); }

private:
    Nullability get_nullability() {
        if (nullable_attribute_data_ && nullability_type_index_ < static_cast<int>(nullable_attribute_data_->size()))
            return (*nullable_attribute_data_)[static_cast<std::size_t>(nullability_type_index_++)];
        return default_nullability_;
    }
    void expect_dummy_nullability_for_generic_value_type() { get_nullability(); }

    const ICompilation& compilation_;
    bool has_dynamic_attribute_;
    std::optional<std::vector<bool>> dynamic_attribute_data_;
    bool has_native_integers_attribute_;
    std::optional<std::vector<bool>> native_integers_attribute_data_;
    TypeSystemOptions options_;
    std::optional<std::vector<std::optional<std::string>>> tuple_element_names_;
    Nullability default_nullability_;
    std::optional<std::vector<Nullability>> nullable_attribute_data_;
    int dynamic_type_index_ = 0, tuple_type_index_ = 0, nullability_type_index_ = 0, native_int_type_index_ = 0;
};

using Arguments = std::vector<srm::CustomAttributeTypedArgument<const IType*>>;

// The single fixed argument's array, when every element holds a T.
template <class Check>
const Arguments* single_array_argument(const srm::CustomAttributeValue<const IType*>& value, Check&& all_ok) {
    if (value.fixed_arguments.size() != 1) return nullptr;
    const auto* values = std::get_if<Arguments>(&value.fixed_arguments[0].value);
    if (values == nullptr) return nullptr;
    for (const auto& v : *values)
        if (!all_ok(v.value)) return nullptr;
    return values;
}

}  // namespace

const IType* apply_attributes_to_type(const IType* input_type, const ICompilation& compilation, const std::vector<srm::CustomAttributeHandle>* attributes,
                                      const srm::MetadataReader& metadata, TypeSystemOptions options, Nullability nullable_context, bool type_children_only,
                                      const std::vector<srm::CustomAttributeHandle>* additional_attributes) {
    bool has_dynamic_attribute = false;
    std::optional<std::vector<bool>> dynamic_attribute_data;
    bool has_native_integers_attribute = any(options, TypeSystemOptions::NativeIntegersWithoutAttribute);
    std::optional<std::vector<bool>> native_integers_attribute_data;
    std::optional<std::vector<std::optional<std::string>>> tuple_element_names;
    Nullability nullability = any(options, TypeSystemOptions::NullabilityAnnotations) ? nullable_context : Nullability::Oblivious;
    std::optional<std::vector<Nullability>> nullable_attribute_data;

    const auto process_attribute = [&](srm::CustomAttributeHandle attr_handle) {
        const srm::CustomAttribute attr = metadata.get_custom_attribute(attr_handle);
        const srm::EntityHandle attr_type = get_attribute_type(attr, metadata);
        const auto is_bool = [](const auto& v) { return std::holds_alternative<bool>(v); };
        const auto to_bools = [](const Arguments& values) {
            std::vector<bool> r;
            for (const auto& v : values) r.push_back(std::get<bool>(v.value));
            return r;
        };
        if (any(options, TypeSystemOptions::Dynamic) && is_known_type(attr_type, metadata, KnownAttribute::Dynamic)) {
            has_dynamic_attribute = true;
            const auto ctor = attr.decode_value(minimal_corlib_type_provider());
            if (const Arguments* values = single_array_argument(ctor, is_bool)) dynamic_attribute_data = to_bools(*values);
        } else if (any(options, TypeSystemOptions::NativeIntegers) && is_known_type(attr_type, metadata, KnownAttribute::NativeInteger)) {
            has_native_integers_attribute = true;
            const auto ctor = attr.decode_value(minimal_corlib_type_provider());
            if (const Arguments* values = single_array_argument(ctor, is_bool)) native_integers_attribute_data = to_bools(*values);
        } else if (any(options, TypeSystemOptions::Tuple) && is_known_type(attr_type, metadata, KnownAttribute::TupleElementNames)) {
            const auto ctor = attr.decode_value(minimal_corlib_type_provider());
            const auto is_string_or_null = [](const auto& v) { return std::holds_alternative<std::string>(v) || std::holds_alternative<std::monostate>(v); };
            if (const Arguments* values = single_array_argument(ctor, is_string_or_null)) {
                std::vector<std::optional<std::string>> names;
                for (const auto& v : *values) {
                    if (const auto* s = std::get_if<std::string>(&v.value)) names.emplace_back(*s);
                    else names.emplace_back(std::nullopt);
                }
                tuple_element_names = std::move(names);
            }
        } else if (any(options, TypeSystemOptions::NullabilityAnnotations) && is_known_type(attr_type, metadata, KnownAttribute::Nullable)) {
            const auto ctor = attr.decode_value(minimal_corlib_type_provider());
            if (ctor.fixed_arguments.size() == 1) {
                const auto& arg = ctor.fixed_arguments[0];
                const auto is_small_byte = [](const auto& v) {
                    const auto* b = std::get_if<std::uint8_t>(&v);
                    return b != nullptr && *b <= 2;
                };
                if (const Arguments* values = single_array_argument(ctor, is_small_byte)) {
                    std::vector<Nullability> data;
                    for (const auto& v : *values) data.push_back(static_cast<Nullability>(std::get<std::uint8_t>(v.value)));
                    nullable_attribute_data = std::move(data);
                } else if (is_small_byte(arg.value)) {
                    nullability = static_cast<Nullability>(std::get<std::uint8_t>(arg.value));
                }
            }
        }
    };

    const TypeSystemOptions relevant_options = TypeSystemOptions::Dynamic | TypeSystemOptions::Tuple | TypeSystemOptions::NullabilityAnnotations |
                                               TypeSystemOptions::NativeIntegers;
    if (attributes != nullptr && any(options, relevant_options))
        for (const srm::CustomAttributeHandle h : *attributes) process_attribute(h);
    if (additional_attributes != nullptr && any(options, relevant_options))   // (these override the values from the normal attributes)
        for (const srm::CustomAttributeHandle h : *additional_attributes) process_attribute(h);

    if (has_dynamic_attribute || has_native_integers_attribute || nullability != Nullability::Oblivious || nullable_attribute_data ||
        (options & (TypeSystemOptions::Tuple | TypeSystemOptions::KeepModifiers)) != TypeSystemOptions::KeepModifiers) {
        ApplyAttributeTypeVisitor visitor(compilation, has_dynamic_attribute, std::move(dynamic_attribute_data), has_native_integers_attribute,
                                          std::move(native_integers_attribute_data), options, std::move(tuple_element_names), nullability,
                                          std::move(nullable_attribute_data));
        return type_children_only ? input_type->visit_children(visitor) : input_type->accept_visitor(visitor);
    }
    return input_type;
}

}  // namespace cb::dotnet::decompiler::typesystem
