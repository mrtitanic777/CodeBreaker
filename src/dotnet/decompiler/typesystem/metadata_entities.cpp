// dotnet/decompiler/typesystem/metadata_entities.cpp -- the Metadata* entities, DefaultParameter and VarArgInstanceMethod.
#include "dotnet/decompiler/typesystem/metadata_entities.hpp"

#include <format>
#include <unordered_set>

#include "dotnet/decompiler/metadata/metadata_extensions.hpp"
#include "dotnet/decompiler/metadata/method_semantics_lookup.hpp"
#include "dotnet/decompiler/srm_extensions.hpp"
#include "dotnet/decompiler/typesystem/fake_member.hpp"
#include "dotnet/decompiler/typesystem/get_members_helper.hpp"
#include "dotnet/decompiler/typesystem/inheritance_helper.hpp"
#include "dotnet/decompiler/typesystem/metadata_module.hpp"
#include "dotnet/decompiler/typesystem/specialized_member.hpp"
#include "dotnet/decompiler/typesystem/type_system_extensions.hpp"
#include "dotnet/metadata/custom_attribute_decoder.hpp"

namespace cb::dotnet::decompiler::typesystem {
namespace {

// TypeAttributes, MethodAttributes, FieldAttributes ... (System.Reflection), as far as they are read here
namespace type_attr {
constexpr std::uint32_t kVisibilityMask = 0x7, kInterface = 0x20, kLayoutMask = 0x18, kSequentialLayout = 0x8, kExplicitLayout = 0x10,
                        kSerializable = 0x2000, kImport = 0x1000, kSpecialName = 0x400, kRTSpecialName = 0x800, kStringFormatMask = 0x30000,
                        kAnsiClass = 0, kUnicodeClass = 0x10000, kAutoClass = 0x20000;
}
namespace method_attr {
constexpr std::uint16_t kStatic = 0x10, kFinal = 0x20, kVirtual = 0x40, kHideBySig = 0x80, kNewSlot = 0x100, kAbstract = 0x400, kSpecialName = 0x800,
                        kRTSpecialName = 0x1000, kPinvokeImpl = 0x2000, kFamily = 0x4;
}

std::string token_text(srm::EntityHandle handle) { return std::format("{:08X}", handle.token()); }

std::string symbol_kind_name(SymbolKind kind) {
    static constexpr const char* kNames[] = {"None", "Module", "TypeDefinition", "Field", "Property", "Indexer", "Event", "Method", "Operator", "Constructor",
                                             "Destructor", "Accessor", "Namespace", "Variable", "Parameter", "TypeParameter", "Constraint", "ReturnType"};
    return kNames[static_cast<int>(kind)];
}

template <class T>
std::vector<const T*> filtered(const std::vector<const T*>& input, const Filter<T>& filter) {
    if (!filter) return input;
    std::vector<const T*> result;
    for (const T* m : input)
        if (filter(m)) result.push_back(m);
    return result;
}

int file_hash(const MetadataModule& module) {
    const auto v = reinterpret_cast<std::uintptr_t>(module.metadata_file());
    return static_cast<int>(static_cast<std::uint32_t>(v ^ (v >> 32)));
}

// IEntity.IsCompilerGenerated / IsCompilerGeneratedOrIsInCompilerGeneratedClass (NRExtensions)
bool is_compiler_generated_or_in_compiler_generated_class(const IEntity* entity) {
    for (; entity != nullptr; entity = entity->declaring_type_definition())
        if (entity->has_attribute(KnownAttribute::CompilerGenerated)) return true;
    return false;
}

// TransformDisplayClassUsage.IsPotentialClosure: a compiler-generated class or struct of the same type tree.
bool is_potential_closure(const ITypeDefinition* decompiled_type_definition, const ITypeDefinition* potential_display_class) {
    if (potential_display_class == nullptr || !is_compiler_generated_or_in_compiler_generated_class(potential_display_class)) return false;
    switch (potential_display_class->kind()) {
    case TypeKind::Struct: break;
    case TypeKind::Class:
        for (const IType* t : potential_display_class->direct_base_types())
            if (!is_known_type(t, KnownTypeCode::Object)) return false;
        break;
    default: return false;
    }
    std::unordered_set<const ITypeDefinition*> ancestors;
    for (const ITypeDefinition* p = potential_display_class->declaring_type_definition(); p != nullptr; p = p->declaring_type_definition()) ancestors.insert(p);
    for (const ITypeDefinition* d = decompiled_type_definition; d != nullptr; d = d->declaring_type_definition())
        if (ancestors.contains(d)) return true;
    return false;
}

// LocalFunctionDecompiler.IsClosureParameter: a ref to a display struct.
bool is_closure_parameter(const IParameter* parameter, const ITypeDefinition* current_type_definition) {
    const auto* brt = dynamic_cast<const ByReferenceType*>(parameter->type());
    if (brt == nullptr) return false;
    const ITypeDefinition* type = brt->element_type()->get_definition();
    return type != nullptr && type->kind() == TypeKind::Struct && is_potential_closure(current_type_definition, type);
}

// IParameter.IsDefaultValueAssignmentAllowed (TypeSystemExtensions): this and every later parameter may have a default value.
bool default_value_assignment_allowed_individual(const IParameter* parameter) {
    const ReferenceKind k = parameter->reference_kind();
    return parameter->is_optional() && parameter->has_constant_value_in_signature() &&
           (k == ReferenceKind::None || k == ReferenceKind::In || k == ReferenceKind::RefReadOnly);
}

bool is_default_value_assignment_allowed(const IParameter* parameter) {
    if (!default_value_assignment_allowed_individual(parameter)) return false;
    if (parameter->owner() == nullptr) return true;
    const std::vector<const IParameter*>& parameters = parameter->owner()->parameters();
    for (auto it = parameters.rbegin(); it != parameters.rend(); ++it) {
        const IParameter* other = *it;
        if (other == parameter) break;
        if (is_closure_parameter(other, other->owner()->declaring_type_definition())) continue;
        if (default_value_assignment_allowed_individual(other) || other->is_params()) continue;
        return false;
    }
    return true;
}

// The attributes of an entity: has_attribute and get_attribute as upstream's, over the kept list.
template <class Entity>
bool entity_has_attribute(const Entity& entity, const MetadataModule& module, const std::vector<srm::CustomAttributeHandle>& handles, KnownAttribute attribute,
                          SymbolKind target) {
    if (!is_custom_attribute(attribute)) return find_attribute(entity.get_attributes(), attribute) != nullptr;
    return AttributeListBuilder(module).has_attribute(module.metadata(), handles, attribute, target);
}

template <class Entity>
const IAttribute* entity_get_attribute(const Entity& entity, const MetadataModule& module, const std::vector<srm::CustomAttributeHandle>& handles,
                                       KnownAttribute attribute, SymbolKind target) {
    if (!is_custom_attribute(attribute)) return find_attribute(entity.get_attributes(), attribute);
    return find_custom_attribute(module, handles, attribute, target, entity.get_attributes());
}

}  // namespace

bool is_operator_method_name(std::string_view name) {
    static constexpr std::string_view kNames[] = {
        "op_LogicalNot", "op_OnesComplement", "op_Increment", "op_CheckedIncrement", "op_Decrement", "op_CheckedDecrement", "op_True", "op_False",
        "op_UnaryPlus", "op_UnaryNegation", "op_CheckedUnaryNegation", "op_Addition", "op_CheckedAddition", "op_Subtraction", "op_CheckedSubtraction",
        "op_Multiply", "op_CheckedMultiply", "op_Division", "op_CheckedDivision", "op_Modulus", "op_BitwiseAnd", "op_BitwiseOr", "op_ExclusiveOr",
        "op_LeftShift", "op_RightShift", "op_UnsignedRightShift", "op_Equality", "op_Inequality", "op_GreaterThan", "op_LessThan",
        "op_GreaterThanOrEqual", "op_LessThanOrEqual", "op_Implicit", "op_Explicit", "op_CheckedExplicit",
    };
    for (const std::string_view n : kNames)
        if (n == name) return true;
    return false;
}

// ---- DefaultParameter ----------------------------------------------------------------------------------------------------

DefaultParameter::DefaultParameter(const IType* type, std::string name, const IParameterizedMember* owner, std::vector<const IAttribute*> attributes,
                                   ReferenceKind reference_kind, bool is_params, bool is_optional, ConstantValue default_value)
    : type_(type), name_(std::move(name)), attributes_(std::move(attributes)), reference_kind_(reference_kind), is_params_(is_params), is_optional_(is_optional),
      default_value_(std::move(default_value)), owner_(owner) {
    if (type == nullptr) throw UsageError("DefaultParameter: a null type");
}

std::string DefaultParameter::to_string() const { return parameter_to_string(*this); }

// ---- MetadataTypeDefinition ----------------------------------------------------------------------------------------------

MetadataTypeDefinition::MetadataTypeDefinition(const MetadataModule& module, srm::TypeDefinitionHandle handle) : module_(&module), handle_(handle) {
    const srm::MetadataReader& md = module.metadata();
    const srm::TypeDefinition td = md.get_type_definition(handle);
    attributes_ = td.attributes();
    full_type_name_ = get_full_type_name(td, md);
    metadata_name_ = std::string(md.get_string(td.name()));
    const std::vector<srm::GenericParameterHandle> generic_parameters = td.get_generic_parameters();
    const std::vector<srm::CustomAttributeHandle> custom_attributes = td.get_custom_attributes();
    if (full_type_name_.is_nested()) {
        declaring_type_definition_ = module.get_definition(td.get_declaring_type());
        // a nested type's first type parameters are its outer type's
        std::size_t outer = 0;
        if (declaring_type_definition_ != nullptr) {
            const auto& outer_tps = declaring_type_definition_->type_parameters();
            outer = std::min(outer_tps.size(), generic_parameters.size());
            type_parameters_.assign(outer_tps.begin(), outer_tps.begin() + static_cast<std::ptrdiff_t>(outer));
        }
        owned_type_parameters_ = MetadataTypeParameter::create(module, this, generic_parameters, outer);
        nullable_context_ = get_nullable_context(custom_attributes, md)
                                .value_or(declaring_type_definition_ != nullptr ? declaring_type_definition_->nullable_context() : module.nullable_context());
    } else {
        owned_type_parameters_ = MetadataTypeParameter::create(module, this, generic_parameters, 0);
        nullable_context_ = get_nullable_context(custom_attributes, md).value_or(module.nullable_context());
        const TopLevelTypeName& top_level_type_name = full_type_name_.top_level_type_name();
        for (int i = 0; i < kKnownTypeCodeCount; ++i) {
            const KnownTypeReference* ktr = KnownTypeReference::get(static_cast<KnownTypeCode>(i));
            if (ktr != nullptr && ktr->type_name() == top_level_type_name) {
                known_type_code_ = static_cast<KnownTypeCode>(i);
                break;
            }
        }
    }
    for (const auto& tp : owned_type_parameters_) type_parameters_.push_back(tp.get());
    type_arguments_.assign(type_parameters_.begin(), type_parameters_.end());

    // the kind
    const TypeSystemOptions options = module.type_system_options();
    srm::PrimitiveTypeCode underlying_type{};
    if ((attributes_ & type_attr::kInterface) == type_attr::kInterface) {
        kind_ = TypeKind::Interface;
    } else if (is_enum(td, md, underlying_type)) {
        kind_ = TypeKind::Enum;
        enum_underlying_type_ = module.compilation().find_type(metadata::to_known_type_code(underlying_type));
    } else if (is_value_type(td, md)) {
        if (known_type_code_ == KnownTypeCode::Void) {
            kind_ = TypeKind::Void;
        } else {
            kind_ = TypeKind::Struct;
            is_by_ref_like_ = any(options, TypeSystemOptions::RefStructs) && has_known_attribute(custom_attributes, md, KnownAttribute::IsByRefLike);
            is_read_only_ = any(options, TypeSystemOptions::ReadOnlyStructsAndParameters) && has_known_attribute(custom_attributes, md, KnownAttribute::IsReadOnly);
        }
    } else if (is_delegate(td, md)) {
        kind_ = TypeKind::Delegate;
    } else {
        kind_ = TypeKind::Class;
        has_extensions_ = is_static() && any(options, TypeSystemOptions::ExtensionMethods) && has_known_attribute(custom_attributes, md, KnownAttribute::Extension);
    }
}

MetadataTypeDefinition::~MetadataTypeDefinition() = default;

namespace {
ExtensionInfoMaker& extension_info_maker() {
    static ExtensionInfoMaker maker = nullptr;
    return maker;
}
}  // namespace

void set_extension_info_maker(ExtensionInfoMaker maker) { extension_info_maker() = maker; }

// MetadataTypeDefinition.ExtensionInfo: null for a class without [Extension], for a module without the ExtensionMembers option, and for
// a static class whose extension methods all use the classic "this" parameter (it carries [Extension] but has no extension blocks).
const ExtensionInfo* MetadataTypeDefinition::extension_info() const {
    if (!has_extensions_) return nullptr;
    if (!any(module_->type_system_options(), TypeSystemOptions::ExtensionMembers)) return nullptr;
    std::call_once(extension_info_once_, [this] {
        if (extension_info_maker() != nullptr) extension_info_ = extension_info_maker()(*module_, this);
    });
    return extension_info_.get();
}

const IModule* MetadataTypeDefinition::parent_module() const { return module_; }
const ICompilation& MetadataTypeDefinition::compilation() const { return module_->compilation(); }

std::string MetadataTypeDefinition::to_string() const { return token_text(handle_) + " " + full_type_name_.reflection_name(); }

const std::vector<const ITypeDefinition*>& MetadataTypeDefinition::nested_types() const {
    std::call_once(nested_once_, [this] {
        for (const srm::TypeDefinitionHandle h : module_->metadata().get_type_definition(handle_).get_nested_types())
            nested_types_.push_back(module_->get_definition(h));
    });
    return nested_types_;
}

const std::vector<const IMember*>& MetadataTypeDefinition::members() const {
    std::call_once(members_once_, [this] {
        for (const IField* f : fields()) members_.push_back(f);
        for (const IMethod* m : methods()) members_.push_back(m);
        for (const IProperty* p : properties()) members_.push_back(p);
        for (const IEvent* e : events()) members_.push_back(e);
    });
    return members_;
}

std::vector<const IField*> MetadataTypeDefinition::fields() const {
    std::call_once(fields_once_, [this] {
        const srm::MetadataReader& md = module_->metadata();
        for (const srm::FieldDefinitionHandle h : md.get_type_definition(handle_).get_fields())
            if (module_->is_visible_field(md.get_field_definition(h).attributes())) fields_.push_back(module_->get_definition(h));
    });
    return fields_;
}

std::vector<const IProperty*> MetadataTypeDefinition::properties() const {
    std::call_once(properties_once_, [this] {
        const srm::MetadataReader& md = module_->metadata();
        for (const srm::PropertyDefinitionHandle h : md.get_type_definition(handle_).get_properties()) {
            const srm::PropertyAccessors accessors = md.get_property_definition(h).get_accessors();
            const bool getter_visible = !accessors.getter.is_nil() && module_->is_visible_method(md.get_method_definition(accessors.getter).attributes());
            const bool setter_visible = !accessors.setter.is_nil() && module_->is_visible_method(md.get_method_definition(accessors.setter).attributes());
            if (getter_visible || setter_visible) properties_.push_back(module_->get_definition(h));
        }
    });
    return properties_;
}

std::vector<const IEvent*> MetadataTypeDefinition::events() const {
    std::call_once(events_once_, [this] {
        const srm::MetadataReader& md = module_->metadata();
        for (const srm::EventDefinitionHandle h : md.get_type_definition(handle_).get_events()) {
            const srm::EventAccessors accessors = md.get_event_definition(h).get_accessors();
            if (accessors.adder.is_nil()) continue;
            if (module_->is_visible_method(md.get_method_definition(accessors.adder).attributes())) events_.push_back(module_->get_definition(h));
        }
    });
    return events_;
}

std::vector<const IMethod*> MetadataTypeDefinition::methods() const {
    std::call_once(methods_once_, [this] {
        const srm::MetadataReader& md = module_->metadata();
        const metadata::MethodSemanticsLookup& method_semantics = module_->metadata_file()->method_semantics_lookup();
        bool has_default_ctor = false;
        for (const srm::MethodDefinitionHandle h : md.get_type_definition(handle_).get_methods()) {
            if (method_semantics.get_semantics(h).second != MethodSemanticsAttributes::None) continue;   // accessors are their owner's
            if (!module_->is_visible_method(md.get_method_definition(h).attributes())) continue;
            const IMethod* method = module_->get_definition(h);
            if (method->symbol_kind() == SymbolKind::Constructor && !method->is_static() && method->parameters().empty()) has_default_ctor = true;
            methods_.push_back(method);
        }
        if (!has_default_ctor && (kind_ == TypeKind::Struct || kind_ == TypeKind::Enum))
            methods_.push_back(create_dummy_constructor(module_->compilation(), this, Accessibility::Public));
    });
    return methods_;
}

std::optional<bool> MetadataTypeDefinition::is_reference_type() const {
    switch (kind_) {
    case TypeKind::Struct:
    case TypeKind::Enum:
    case TypeKind::Void: return false;
    default: return true;
    }
}

const IType* MetadataTypeDefinition::change_nullability(Nullability nullability) const {
    if (nullability == Nullability::Oblivious || is_reference_type() == std::optional<bool>(false)) return this;
    return make_nullability_annotated_type(this, nullability);
}

std::vector<const IType*> MetadataTypeDefinition::direct_base_types() const {
    std::call_once(bases_once_, [this] {
        const srm::MetadataReader& md = module_->metadata();
        const srm::TypeDefinition td = md.get_type_definition(handle_);
        const GenericContext context(&type_parameters_);
        const IType* base_type = nullptr;
        try {
            const srm::EntityHandle base_type_handle = td.base_type();
            if (!base_type_handle.is_nil()) {
                const std::vector<srm::CustomAttributeHandle> attributes = md.get_custom_attributes(handle_);
                base_type = module_->resolve_type(base_type_handle, context, &attributes, Nullability::Oblivious);
            }
        } catch (const srm::BadImageFormatError&) {
            base_type = &SpecialType::unknown_type();
        }
        if (base_type != nullptr) direct_base_types_.push_back(base_type);
        else if (kind_ == TypeKind::Interface) direct_base_types_.push_back(module_->compilation().find_type(KnownTypeCode::Object));   // (as the type system wants)
        for (const srm::InterfaceImplementationHandle h : td.get_interface_implementations()) {
            const srm::InterfaceImplementation iface = md.get_interface_implementation(h);
            const std::vector<srm::CustomAttributeHandle> attributes = iface.get_custom_attributes();
            direct_base_types_.push_back(module_->resolve_type(iface.interface_(), context, &attributes, Nullability::Oblivious));
        }
    });
    return direct_base_types_;
}

std::vector<const IAttribute*> MetadataTypeDefinition::get_attributes() const {
    return attributes_list_.get([this] {
        AttributeListBuilder b(*module_);
        const srm::MetadataReader& md = module_->metadata();
        const srm::TypeDefinition type_definition = md.get_type_definition(handle_);
        const std::uint32_t attributes = type_definition.attributes();
        if ((attributes & type_attr::kSerializable) != 0) b.add(KnownAttribute::Serializable);
        if ((attributes & type_attr::kImport) != 0) b.add(KnownAttribute::ComImport);
        if ((attributes & (type_attr::kSpecialName | type_attr::kRTSpecialName)) == type_attr::kSpecialName) b.add(KnownAttribute::SpecialName);

        // [StructLayout]: LayoutKind Sequential 0, Explicit 2, Auto 3; CharSet None 1, Ansi 2, Unicode 3, Auto 4
        int layout_kind = 3;
        switch (attributes & type_attr::kLayoutMask) {
        case type_attr::kSequentialLayout: layout_kind = 0; break;
        case type_attr::kExplicitLayout: layout_kind = 2; break;
        default: break;
        }
        int char_set = 1;
        switch (attributes & type_attr::kStringFormatMask) {
        case type_attr::kAnsiClass: char_set = 2; break;
        case type_attr::kAutoClass: char_set = 4; break;
        case type_attr::kUnicodeClass: char_set = 3; break;
        default: break;
        }
        const srm::TypeLayout layout = type_definition.get_layout();
        const int default_layout_kind = kind_ == TypeKind::Struct ? 0 : 3;
        if (layout_kind != default_layout_kind || char_set != 2 || layout.packing_size > 0 || layout.size > 0) {
            AttributeBuilder struct_layout(*module_, KnownAttribute::StructLayout);
            struct_layout.add_fixed_arg(TopLevelTypeName("System.Runtime.InteropServices", "LayoutKind"), std::int32_t{layout_kind});
            if (char_set != 2)
                struct_layout.add_named_arg("CharSet", find_type(module_->compilation(), FullTypeName(TopLevelTypeName("System.Runtime.InteropServices", "CharSet"))),
                                            std::int32_t{char_set});
            if (layout.packing_size > 0) struct_layout.add_named_arg("Pack", KnownTypeCode::Int32, std::int32_t{layout.packing_size});
            if (layout.size > 0) struct_layout.add_named_arg("Size", KnownTypeCode::Int32, std::int32_t{layout.size});
            b.add(struct_layout.build());
        }
        b.add(type_definition.get_custom_attributes(), SymbolKind::TypeDefinition);
        b.add_security_attributes(type_definition.get_declarative_security_attributes());
        return b.build();
    });
}

bool MetadataTypeDefinition::has_attribute(KnownAttribute attribute) const {
    return entity_has_attribute(*this, *module_, module_->metadata().get_type_definition(handle_).get_custom_attributes(), attribute, SymbolKind::TypeDefinition);
}

const IAttribute* MetadataTypeDefinition::get_attribute(KnownAttribute attribute) const {
    return entity_get_attribute(*this, *module_, module_->metadata().get_type_definition(handle_).get_custom_attributes(), attribute, SymbolKind::TypeDefinition);
}

const std::optional<std::string>& MetadataTypeDefinition::default_member_name() const {
    std::call_once(default_member_once_, [this] {
        const srm::MetadataReader& md = module_->metadata();
        for (const srm::CustomAttributeHandle h : md.get_type_definition(handle_).get_custom_attributes()) {
            const srm::CustomAttribute a = md.get_custom_attribute(h);
            if (!is_known_attribute(a, md, KnownAttribute::DefaultMember)) continue;
            const srm::CustomAttributeValue<const IType*> value = a.decode_value(module_->type_provider());
            if (value.fixed_arguments.size() == 1)
                if (const auto* name = std::get_if<std::string>(&value.fixed_arguments[0].value)) {
                    default_member_name_ = *name;
                    break;
                }
        }
    });
    return default_member_name_;
}

Accessibility MetadataTypeDefinition::accessibility() const {
    switch (attributes_ & type_attr::kVisibilityMask) {
    case 0:   // NotPublic
    case 5: return Accessibility::Internal;   // NestedAssembly
    case 1:   // Public
    case 2: return Accessibility::Public;   // NestedPublic
    case 3: return Accessibility::Private;
    case 4: return Accessibility::Protected;
    case 6: return Accessibility::ProtectedAndInternal;
    case 7: return Accessibility::ProtectedOrInternal;
    default: return Accessibility::None;
    }
}

std::string MetadataTypeDefinition::full_name() const {
    if (declaring_type_definition_ != nullptr) return declaring_type_definition_->full_name() + "." + name();
    const std::string ns = namespace_();
    if (!ns.empty()) return ns + "." + name();
    return name();
}

bool MetadataTypeDefinition::equals(const IType* other) const {
    const auto* td = dynamic_cast<const MetadataTypeDefinition*>(other);
    return td != nullptr && handle_ == td->handle_ && module_->metadata_file() == td->module_->metadata_file();
}

int MetadataTypeDefinition::hash_code() const { return 0x2e0520f2 ^ file_hash(*module_) ^ static_cast<int>(handle_.row()); }

std::vector<const IType*> MetadataTypeDefinition::get_nested_types(const Filter<ITypeDefinition>& filter, GetMemberOptions options) const {
    if (has(options, GetMemberOptions::IgnoreInheritedMembers | GetMemberOptions::ReturnMemberDefinitions)) {
        std::vector<const IType*> result;
        for (const ITypeDefinition* t : nested_types())
            if (!filter || filter(t)) result.push_back(t);
        return result;
    }
    return get_members_helper::get_nested_types(this, filter, options);
}

std::vector<const IType*> MetadataTypeDefinition::get_nested_types(const std::vector<const IType*>& type_arguments, const Filter<ITypeDefinition>& filter,
                                                                   GetMemberOptions options) const {
    return get_members_helper::get_nested_types(this, &type_arguments, filter, options);
}

std::vector<const IMethod*> MetadataTypeDefinition::get_methods(const Filter<IMethod>& filter, GetMemberOptions options) const {
    if (kind_ == TypeKind::Void) return {};
    if (has(options, GetMemberOptions::IgnoreInheritedMembers)) {
        std::vector<const IMethod*> result;
        for (const IMethod* m : methods())
            if (!m->is_constructor() && (!filter || filter(m))) result.push_back(m);
        return result;
    }
    return get_members_helper::get_methods(this, filter, options);
}

std::vector<const IMethod*> MetadataTypeDefinition::get_methods(const std::vector<const IType*>& type_arguments, const Filter<IMethod>& filter,
                                                                GetMemberOptions options) const {
    if (kind_ == TypeKind::Void) return {};
    return get_members_helper::get_methods(this, &type_arguments, filter, options);
}

std::vector<const IMethod*> MetadataTypeDefinition::get_constructors(const Filter<IMethod>& filter, GetMemberOptions options) const {
    if (kind_ == TypeKind::Void) return {};
    if (com_helper::is_com_import(this)) {
        // a COM interface is constructed through its CoClass
        const IType* co_class = com_helper::get_co_class(this);
        const BusyLock busy_lock(this);
        if (!busy_lock.success()) return {};
        std::vector<const IMethod*> result;
        for (const IMethod* m : co_class->get_constructors(filter, options)) result.push_back(SpecializedMethod::make(m, m->substitution(), this));
        return result;
    }
    if (has(options, GetMemberOptions::IgnoreInheritedMembers)) {
        std::vector<const IMethod*> result;
        for (const IMethod* m : methods())
            if (m->is_constructor() && !m->is_static() && (!filter || filter(m))) result.push_back(m);
        return result;
    }
    return get_members_helper::get_constructors(this, filter, options);
}

std::vector<const IProperty*> MetadataTypeDefinition::get_properties(const Filter<IProperty>& filter, GetMemberOptions options) const {
    if (kind_ == TypeKind::Void) return {};
    if (has(options, GetMemberOptions::IgnoreInheritedMembers)) return filtered(properties(), filter);
    return get_members_helper::get_properties(this, filter, options);
}

std::vector<const IField*> MetadataTypeDefinition::get_fields(const Filter<IField>& filter, GetMemberOptions options) const {
    if (kind_ == TypeKind::Void) return {};
    if (has(options, GetMemberOptions::IgnoreInheritedMembers)) return filtered(fields(), filter);
    return get_members_helper::get_fields(this, filter, options);
}

std::vector<const IEvent*> MetadataTypeDefinition::get_events(const Filter<IEvent>& filter, GetMemberOptions options) const {
    if (kind_ == TypeKind::Void) return {};
    if (has(options, GetMemberOptions::IgnoreInheritedMembers)) return filtered(events(), filter);
    return get_members_helper::get_events(this, filter, options);
}

std::vector<const IMember*> MetadataTypeDefinition::get_members(const Filter<IMember>& filter, GetMemberOptions options) const {
    if (kind_ == TypeKind::Void) return {};
    if (has(options, GetMemberOptions::IgnoreInheritedMembers)) return filtered(members(), filter);
    return get_members_helper::get_members(this, filter, options);
}

std::vector<const IMethod*> MetadataTypeDefinition::get_accessors(const Filter<IMethod>& filter, GetMemberOptions options) const {
    if (kind_ == TypeKind::Void) return {};
    if (!has(options, GetMemberOptions::IgnoreInheritedMembers)) return get_members_helper::get_accessors(this, filter, options);
    std::vector<const IMethod*> result;
    const auto add = [&](const IMethod* m) {
        if (m != nullptr && (!filter || filter(m))) result.push_back(m);
    };
    for (const IProperty* prop : properties()) {
        add(prop->getter());
        add(prop->setter());
    }
    for (const IEvent* ev : events()) {
        add(ev->add_accessor());
        add(ev->remove_accessor());
        // (upstream yields the remover where the invoker matches the filter; kept, except where the event has no remover)
        const IMethod* invoker = ev->invoke_accessor();
        if (invoker != nullptr && (!filter || filter(invoker)) && ev->remove_accessor() != nullptr) result.push_back(ev->remove_accessor());
    }
    return result;
}

std::vector<const IMethod*> MetadataTypeDefinition::get_overrides(srm::MethodDefinitionHandle method) const {
    const srm::MetadataReader& md = module_->metadata();
    std::vector<const IMethod*> result;
    for (const srm::MethodImplementationHandle impl_handle : md.get_type_definition(handle_).get_method_implementations()) {
        const srm::MethodImplementation impl = md.get_method_implementation(impl_handle);
        if (impl.method_body() == srm::EntityHandle(method)) result.push_back(module_->resolve_method(impl.method_declaration(), GenericContext(&type_parameters_)));
    }
    return result;
}

bool MetadataTypeDefinition::has_overrides(srm::MethodDefinitionHandle method) const {
    const srm::MetadataReader& md = module_->metadata();
    for (const srm::MethodImplementationHandle impl_handle : md.get_type_definition(handle_).get_method_implementations())
        if (md.get_method_implementation(impl_handle).method_body() == srm::EntityHandle(method)) return true;
    return false;
}

bool MetadataTypeDefinition::is_record() const {
    std::call_once(record_once_, [this] {
        if (kind_ != TypeKind::Class && kind_ != TypeKind::Struct) return;
        const bool is_struct = kind_ == TypeKind::Struct;
        const srm::MetadataReader& md = module_->metadata();
        bool get_equality_contract = is_struct, to_string = false, print_members = false, get_hash_code = false, equals = false, op_equality = false,
             op_inequality = false, clone = is_struct;
        for (const srm::MethodDefinitionHandle method_handle : md.get_type_definition(handle_).get_methods()) {
            const std::string_view name = md.get_string(md.get_method_definition(method_handle).name());
            if (name == "Clone") return;   // error CS8859: members named 'Clone' are disallowed in records
            get_equality_contract |= name == "get_EqualityContract";
            to_string |= name == "ToString";
            print_members |= name == "PrintMembers";
            get_hash_code |= name == "GetHashCode";
            equals |= name == "Equals";
            op_equality |= name == "op_Equality";
            op_inequality |= name == "op_Inequality";
            clone |= name == "<Clone>$";
        }
        // a record class may have its ToString in a base class only; a record struct must have it (PrintMembers is called from it)
        if (is_struct && !to_string) return;
        is_record_ = get_equality_contract && print_members && get_hash_code && equals && op_equality && op_inequality && clone;
    });
    return is_record_;
}

// ---- MetadataTypeParameter -----------------------------------------------------------------------------------------------

namespace {

VarianceModifier get_variance(std::uint16_t attr) {
    switch (attr & 0x3) {
    case 2: return VarianceModifier::Contravariant;
    case 1: return VarianceModifier::Covariant;
    default: return VarianceModifier::Invariant;
    }
}

}  // namespace

std::vector<std::unique_ptr<MetadataTypeParameter>> MetadataTypeParameter::create(const MetadataModule& module, const IEntity* owner,
                                                                                  const std::vector<srm::GenericParameterHandle>& handles, std::size_t first_own) {
    std::vector<std::unique_ptr<MetadataTypeParameter>> tps;
    const srm::MetadataReader& md = module.metadata();
    for (std::size_t i = first_own; i < handles.size(); ++i) {
        const srm::GenericParameter gp = md.get_generic_parameter(handles[i]);
        tps.push_back(std::make_unique<MetadataTypeParameter>(module, owner, static_cast<int>(i), module.get_string(gp.name()), handles[i], gp.attributes()));
    }
    return tps;
}

MetadataTypeParameter::MetadataTypeParameter(const MetadataModule& module, const IEntity* owner, int index, std::string name, srm::GenericParameterHandle handle,
                                             std::uint16_t attributes)
    : AbstractTypeParameter(owner, index, std::move(name), get_variance(attributes)), module_(&module), handle_(handle), attr_(attributes) {}

std::vector<const IAttribute*> MetadataTypeParameter::get_attributes() const {
    return attributes_list_.get([this] {
        AttributeListBuilder b(*module_);
        b.add(module_->metadata().get_generic_parameter(handle_).get_custom_attributes(), SymbolKind::TypeParameter);
        return b.build();
    });
}

bool MetadataTypeParameter::has_unmanaged_constraint() const {
    std::call_once(unmanaged_once_, [this] {
        if (!any(module_->type_system_options(), TypeSystemOptions::UnmanagedConstraints)) return;
        const srm::MetadataReader& md = module_->metadata();
        unmanaged_constraint_ = has_known_attribute(md.get_generic_parameter(handle_).get_custom_attributes(), md, KnownAttribute::IsUnmanaged);
    });
    return unmanaged_constraint_;
}

Nullability MetadataTypeParameter::nullability_constraint() const {
    std::call_once(nullability_once_, [this] { nullability_constraint_ = load_nullability_constraint(); });
    return nullability_constraint_;
}

Nullability MetadataTypeParameter::load_nullability_constraint() const {
    if (!module_->should_decode_nullable_attributes(owner())) return Nullability::Oblivious;
    const srm::MetadataReader& md = module_->metadata();
    for (const srm::CustomAttributeHandle h : md.get_generic_parameter(handle_).get_custom_attributes()) {
        const srm::CustomAttribute custom_attribute = md.get_custom_attribute(h);
        if (!is_known_attribute(custom_attribute, md, KnownAttribute::Nullable)) continue;
        const srm::CustomAttributeValue<const IType*> attr_val = custom_attribute.decode_value(module_->type_provider());
        if (attr_val.fixed_arguments.size() == 1)
            if (const auto* b = std::get_if<std::uint8_t>(&attr_val.fixed_arguments[0].value); b != nullptr && *b <= 2) return static_cast<Nullability>(*b);
    }
    if (const auto* method = dynamic_cast<const MetadataMethod*>(owner())) return method->nullable_context();
    if (const auto* td = dynamic_cast<const ITypeDefinition*>(owner())) return td->nullable_context();
    return Nullability::Oblivious;
}

const std::vector<TypeConstraint>& MetadataTypeParameter::type_constraints() const {
    std::call_once(constraints_once_, [this] {
        const srm::MetadataReader& md = module_->metadata();
        const srm::GenericParameter gp = md.get_generic_parameter(handle_);
        Nullability nullable_context = Nullability::Oblivious;
        if (const auto* type_def = dynamic_cast<const ITypeDefinition*>(owner())) nullable_context = type_def->nullable_context();
        else if (const auto* method = dynamic_cast<const MetadataMethod*>(owner())) nullable_context = method->nullable_context();
        const std::vector<srm::GenericParameterConstraintHandle> constraint_handles = gp.get_constraints();
        constraint_attributes_.reserve(constraint_handles.size());   // (the lists the constraints point into stay where they are)
        bool has_non_interface_constraint = false;
        for (const srm::GenericParameterConstraintHandle constraint_handle : constraint_handles) {
            const srm::GenericParameterConstraint constraint = md.get_generic_parameter_constraint(constraint_handle);
            const std::vector<srm::CustomAttributeHandle> attrs = constraint.get_custom_attributes();
            const IType* ty = module_->resolve_type(constraint.type(), GenericContext::of(*owner()), &attrs, nullable_context);
            if (attrs.empty()) {
                constraints_.push_back({ty, {}});
            } else {
                AttributeListBuilder b(*module_);
                b.add(attrs, SymbolKind::Constraint);
                constraint_attributes_.push_back(b.build());
                constraints_.push_back({ty, constraint_attributes_.back().attributes});
            }
            has_non_interface_constraint |= ty->kind() != TypeKind::Interface;
        }
        if (has_value_type_constraint()) constraints_.push_back({compilation().find_type(KnownTypeCode::ValueType), {}});
        else if (!has_non_interface_constraint) constraints_.push_back({compilation().find_type(KnownTypeCode::Object), {}});
    });
    return constraints_;
}

bool MetadataTypeParameter::equals(const IType* other) const {
    const auto* tp = dynamic_cast<const MetadataTypeParameter*>(other);
    return tp != nullptr && handle_ == tp->handle_ && module_->metadata_file() == tp->module_->metadata_file();
}

int MetadataTypeParameter::hash_code() const { return 0x51fc5b83 ^ file_hash(*module_) ^ static_cast<int>(handle_.row()); }

std::string MetadataTypeParameter::to_string() const { return token_text(handle_) + " " + reflection_name(); }

// ---- the members' common parts -------------------------------------------------------------------------------------------

const IModule* MetadataMemberBase::parent_module() const { return module_; }
const ICompilation& MetadataMemberBase::compilation() const { return module_->compilation(); }

std::string MetadataMemberBase::full_name() const {
    const IType* d = declaring_type();
    return (d != nullptr ? d->full_name() : std::string()) + "." + name();
}

std::string MetadataMemberBase::reflection_name() const {
    const IType* d = declaring_type();
    return (d != nullptr ? d->reflection_name() : std::string()) + "." + name();
}

std::string MetadataMemberBase::namespace_() const {
    const IType* d = declaring_type();
    return d != nullptr ? d->namespace_() : std::string();
}

std::string MetadataMemberBase::to_string() const {
    const IType* d = declaring_type();
    return token_text(metadata_token()) + " " + (d != nullptr ? d->reflection_name() : std::string()) + "." + name();
}

// ---- MetadataMethod ------------------------------------------------------------------------------------------------------

MetadataMethod::MetadataMethod(const MetadataModule& module, srm::MethodDefinitionHandle handle) : MetadataMemberBase(module), handle_(handle) {
    const srm::MetadataReader& md = module.metadata();
    const srm::MethodDefinition def = md.get_method_definition(handle);
    attributes_ = def.attributes();
    const auto [accessor_owner, semantics_attribute] = module.metadata_file()->method_semantics_lookup().get_semantics(handle);
    constexpr std::uint16_t finalizer_attributes = method_attr::kVirtual | method_attr::kFamily | method_attr::kHideBySig;
    owned_type_parameters_ = MetadataTypeParameter::create(module, this, def.get_generic_parameters(), 0);
    for (const auto& tp : owned_type_parameters_) type_parameters_.push_back(tp.get());
    type_arguments_.assign(type_parameters_.begin(), type_parameters_.end());
    if (semantics_attribute != MethodSemanticsAttributes::None && !accessor_owner.is_nil() &&
        (accessor_owner.kind() == srm::HandleKind::PropertyDefinition || accessor_owner.kind() == srm::HandleKind::EventDefinition)) {
        symbol_kind_ = SymbolKind::Accessor;
        accessor_owner_ = accessor_owner;
        accessor_kind_ = semantics_attribute;
    } else if ((attributes_ & (method_attr::kSpecialName | method_attr::kRTSpecialName)) != 0 && type_parameters_.empty()) {
        const std::string n = name();
        if (n == ".cctor" || n == ".ctor") symbol_kind_ = SymbolKind::Constructor;
        else if (n.rfind("op_", 0) == 0 && is_operator_method_name(n)) symbol_kind_ = SymbolKind::Operator;
    } else if ((attributes_ & finalizer_attributes) == finalizer_attributes && type_parameters_.empty()) {
        const auto* declaring = dynamic_cast<const MetadataTypeDefinition*>(declaring_type_definition());
        if (name() == "Finalize" && parameters().empty() && is_known_type(return_type(), KnownTypeCode::Void) && declaring != nullptr &&
            declaring->kind() == TypeKind::Class)
            symbol_kind_ = SymbolKind::Destructor;
    } else if ((attributes_ & method_attr::kStatic) != 0 && type_parameters_.empty()) {
        // operators that are explicit interface implementations are not SpecialName
        const std::string n = name();
        const std::size_t index = n.rfind('.');
        if (index != std::string::npos && index > 0) {
            const std::string short_name = n.substr(index + 1);
            if (short_name.rfind("op_", 0) == 0 && is_operator_method_name(short_name)) symbol_kind_ = SymbolKind::Operator;
        }
    }
    is_extension_method_ = (attributes_ & method_attr::kStatic) == method_attr::kStatic && any(module.type_system_options(), TypeSystemOptions::ExtensionMethods) &&
                           has_known_attribute(def.get_custom_attributes(), md, KnownAttribute::Extension);
}

MetadataMethod::~MetadataMethod() = default;

std::string MetadataMethod::name() const {
    std::call_once(name_once_, [this] { name_ = module_->get_string(module_->metadata().get_method_definition(handle_).name()); });
    return name_;
}

bool MetadataMethod::has_body() const { return decompiler::has_body(module_->metadata().get_method_definition(handle_)); }

const IMember* MetadataMethod::accessor_owner() const {
    if (accessor_owner_.is_nil()) return nullptr;
    if (accessor_owner_.kind() == srm::HandleKind::PropertyDefinition) return module_->get_definition(srm::PropertyDefinitionHandle(accessor_owner_));
    if (accessor_owner_.kind() == srm::HandleKind::EventDefinition) return module_->get_definition(srm::EventDefinitionHandle(accessor_owner_));
    return nullptr;
}

const std::vector<const IParameter*>& MetadataMethod::parameters() const {
    decode_signature();
    return parameters_;
}

const IType* MetadataMethod::return_type() const {
    decode_signature();
    return return_type_;
}

bool MetadataMethod::is_init_only() const {
    decode_signature();
    return is_init_only_;
}

Nullability MetadataMethod::nullable_context() const {
    const srm::MetadataReader& md = module_->metadata();
    const ITypeDefinition* declaring = declaring_type_definition();
    return get_nullable_context(md.get_method_definition(handle_).get_custom_attributes(), md)
        .value_or(declaring != nullptr ? declaring->nullable_context() : Nullability::Oblivious);
}

void MetadataMethod::decode_signature() const {
    std::call_once(signature_once_, [this] {
        const srm::MetadataReader& md = module_->metadata();
        const srm::MethodDefinition method_def = md.get_method_definition(handle_);
        const IType* declaring = declaring_type();
        const GenericContext generic_context(declaring != nullptr ? &declaring->type_parameters() : nullptr, &type_parameters_);
        const ModifiedType* mod = nullptr;
        try {
            const Nullability nullable_ctx = nullable_context();
            const srm::MethodSignature<const IType*> signature = method_def.decode_signature(module_->type_provider(), generic_context);
            const srm::HandleList<srm::ParameterHandle> parameter_handles = method_def.get_parameters();
            DecodedSignature decoded = decode_signature(*module_, this, signature, &parameter_handles, nullable_ctx, module_->options_for_entity(this));
            return_type_ = decoded.return_type;
            owned_parameters_ = std::move(decoded.parameters);
            mod = decoded.return_type_modifier;
        } catch (const srm::BadImageFormatError&) {
            return_type_ = &SpecialType::unknown_type();
            owned_parameters_.clear();
            mod = nullptr;
        }
        parameters_.clear();
        for (const auto& p : owned_parameters_) parameters_.push_back(p.get());
        is_init_only_ = mod != nullptr && mod->modifier()->name() == "IsExternalInit" && mod->modifier()->namespace_() == "System.Runtime.CompilerServices";
    });
}

MetadataMethod::DecodedSignature MetadataMethod::decode_signature(const MetadataModule& module, const IParameterizedMember* owner,
                                                                  const srm::MethodSignature<const IType*>& signature,
                                                                  const srm::HandleList<srm::ParameterHandle>* parameter_handles, Nullability nullable_context,
                                                                  TypeSystemOptions type_system_options,
                                                                  const std::vector<srm::CustomAttributeHandle>* additional_return_type_attributes) {
    const srm::MetadataReader& md = module.metadata();
    const ICompilation& compilation = module.compilation();
    const bool var_args = signature.header.calling_convention() == srm::SignatureCallingConvention::VarArgs;
    DecodedSignature result;
    result.parameters.resize(static_cast<std::size_t>(signature.required_parameter_count + (var_args ? 1 : 0)));
    int i = 0;
    std::optional<std::vector<srm::CustomAttributeHandle>> return_type_attributes;
    const auto default_parameter = [&](int index) {
        const IType* parameter_type = apply_attributes_to_type(signature.parameter_types[static_cast<std::size_t>(index)], compilation, nullptr, md,
                                                               type_system_options, nullable_context);
        return std::make_unique<DefaultParameter>(parameter_type, std::string(), owner, std::vector<const IAttribute*>{},
                                                  parameter_type->kind() == TypeKind::ByReference ? ReferenceKind::Ref : ReferenceKind::None);
    };
    if (parameter_handles != nullptr) {
        for (const srm::ParameterHandle parameter_handle : *parameter_handles) {
            const srm::Parameter par = md.get_parameter(parameter_handle);
            const int sequence_number = par.sequence_number();
            if (sequence_number == 0) {
                // the "parameter" of the return type's attributes (for a property, its own attributes are preferred: additional_return_type_attributes)
                return_type_attributes = par.get_custom_attributes();
            } else if (i < sequence_number && sequence_number <= signature.required_parameter_count) {
                // "Successive rows of the Param table that are owned by the same method shall be ordered by increasing Sequence value -
                // although gaps in the sequence are allowed": the gaps get parameters without metadata
                while (i < sequence_number - 1) {
                    result.parameters[static_cast<std::size_t>(i)] = default_parameter(i);
                    ++i;
                }
                const std::vector<srm::CustomAttributeHandle> attributes = par.get_custom_attributes();
                const IType* parameter_type = apply_attributes_to_type(signature.parameter_types[static_cast<std::size_t>(i)], compilation, &attributes, md,
                                                                       type_system_options, nullable_context);
                result.parameters[static_cast<std::size_t>(i)] = std::make_unique<MetadataParameter>(module, owner, parameter_type, parameter_handle);
                ++i;
            }
        }
    }
    while (i < signature.required_parameter_count) {
        result.parameters[static_cast<std::size_t>(i)] = default_parameter(i);
        ++i;
    }
    if (var_args) {
        result.parameters[static_cast<std::size_t>(i)] = std::make_unique<DefaultParameter>(&SpecialType::arg_list(), std::string(), owner);
        ++i;
    }
    result.return_type = apply_attributes_to_type(signature.return_type, compilation, return_type_attributes ? &*return_type_attributes : nullptr, md,
                                                  type_system_options, nullable_context, false, additional_return_type_attributes);
    result.return_type_modifier = dynamic_cast<const ModifiedType*>(signature.return_type);
    return result;
}

bool MetadataMethod::is_explicit_interface_implementation() const {
    if (name().find('.') == std::string::npos) return false;
    const auto* type_def = dynamic_cast<const MetadataTypeDefinition*>(declaring_type_definition());
    return type_def != nullptr && type_def->has_overrides(handle_);
}

std::vector<const IMember*> MetadataMethod::explicitly_implemented_interface_members() const {
    const auto* type_def = dynamic_cast<const MetadataTypeDefinition*>(declaring_type_definition());
    if (type_def == nullptr) return {};
    std::vector<const IMember*> result;
    for (const IMethod* m : type_def->get_overrides(handle_)) result.push_back(m);
    return result;
}

const ITypeDefinition* MetadataMethod::declaring_type_definition() const {
    std::call_once(declaring_once_, [this] { declaring_type_ = module_->get_definition(module_->metadata().get_method_definition(handle_).get_declaring_type()); });
    return declaring_type_;
}

std::vector<const IAttribute*> MetadataMethod::get_attributes() const {
    return attributes_list_.get([this] {
        AttributeListBuilder b(*module_);
        const srm::MetadataReader& md = module_->metadata();
        const srm::MethodDefinition def = md.get_method_definition(handle_);
        const ICompilation& compilation = module_->compilation();
        const auto find_interop_type = [&](const char* name) {
            return find_type(compilation, FullTypeName(TopLevelTypeName("System.Runtime.InteropServices", name, 0)));
        };
        std::uint16_t impl_attributes = def.impl_attributes() & static_cast<std::uint16_t>(~0x3);   // without the CodeTypeMask
        if (any(module_->type_system_options(), TypeSystemOptions::RuntimeAsync)) impl_attributes &= static_cast<std::uint16_t>(~kMethodImplAsync);
        const int method_code_type = def.impl_attributes() & 0x3;

        // [DllImport]
        const srm::MethodImport info = def.get_import();
        if ((attributes_ & method_attr::kPinvokeImpl) == method_attr::kPinvokeImpl && !info.module.is_nil()) {
            AttributeBuilder dll_import(*module_, KnownAttribute::DllImport);
            dll_import.add_fixed_arg(KnownTypeCode::String, std::string(md.get_string(md.get_module_reference(info.module).name())));
            const std::uint16_t import_attrs = info.attributes;
            if ((import_attrs & 0x20) == 0x20) dll_import.add_named_arg("BestFitMapping", KnownTypeCode::Boolean, false);   // BestFitMappingDisable
            if ((import_attrs & 0x10) == 0x10) dll_import.add_named_arg("BestFitMapping", KnownTypeCode::Boolean, true);    // BestFitMappingEnable
            int calling_convention = 0;   // CallingConvention: Winapi 1, Cdecl 2, StdCall 3, ThisCall 4, FastCall 5
            switch (import_attrs & 0x700) {
            case 0: calling_convention = 0; break;   // (not set)
            case 0x200: calling_convention = 2; break;
            case 0x500: calling_convention = 5; break;
            case 0x300: calling_convention = 3; break;
            case 0x400: calling_convention = 4; break;
            case 0x100: calling_convention = 1; break;
            default: throw UsageError("unknown calling convention");
            }
            if (calling_convention != 1) dll_import.add_named_arg("CallingConvention", find_interop_type("CallingConvention"), std::int32_t{calling_convention});
            int char_set = 1;   // CharSet: None 1, Ansi 2, Unicode 3, Auto 4
            switch (import_attrs & 0x6) {
            case 0x2: char_set = 2; break;
            case 0x6: char_set = 4; break;
            case 0x4: char_set = 3; break;
            default: break;
            }
            if (char_set != 1) dll_import.add_named_arg("CharSet", find_interop_type("CharSet"), std::int32_t{char_set});
            if (!info.name.is_nil() && info.name != def.name()) dll_import.add_named_arg("EntryPoint", KnownTypeCode::String, std::string(md.get_string(info.name)));
            if ((import_attrs & 0x1) == 0x1) dll_import.add_named_arg("ExactSpelling", KnownTypeCode::Boolean, true);
            if ((impl_attributes & 0x80) == 0x80) impl_attributes &= static_cast<std::uint16_t>(~0x80);   // PreserveSig
            else dll_import.add_named_arg("PreserveSig", KnownTypeCode::Boolean, false);
            if ((import_attrs & 0x40) == 0x40) dll_import.add_named_arg("SetLastError", KnownTypeCode::Boolean, true);
            if ((import_attrs & 0x2000) == 0x2000) dll_import.add_named_arg("ThrowOnUnmappableChar", KnownTypeCode::Boolean, false);
            if ((import_attrs & 0x1000) == 0x1000) dll_import.add_named_arg("ThrowOnUnmappableChar", KnownTypeCode::Boolean, true);
            b.add(dll_import.build());
        }
        // [PreserveSig]
        if (impl_attributes == 0x80 && method_code_type == 0) {
            b.add(KnownAttribute::PreserveSig);
            impl_attributes = 0;
        }
        // [MethodImpl]
        if (impl_attributes != 0) {
            AttributeBuilder method_impl(*module_, KnownAttribute::MethodImpl);
            method_impl.add_fixed_arg(TopLevelTypeName("System.Runtime.CompilerServices", "MethodImplOptions"), std::int32_t{impl_attributes});
            if (method_code_type != 0)
                method_impl.add_named_arg("MethodCodeType", TopLevelTypeName("System.Runtime.CompilerServices", "MethodCodeType"), std::int32_t{method_code_type});
            b.add(method_impl.build());
        }
        if ((def.attributes() & (method_attr::kSpecialName | method_attr::kRTSpecialName)) == method_attr::kSpecialName && symbol_kind_ == SymbolKind::Method)
            b.add(KnownAttribute::SpecialName);
        b.add(def.get_custom_attributes(), symbol_kind_);
        b.add_security_attributes(def.get_declarative_security_attributes());
        return b.build();
    });
}

bool MetadataMethod::has_attribute(KnownAttribute attribute) const {
    return entity_has_attribute(*this, *module_, module_->metadata().get_method_definition(handle_).get_custom_attributes(), attribute, symbol_kind_);
}

const IAttribute* MetadataMethod::get_attribute(KnownAttribute attribute) const {
    return entity_get_attribute(*this, *module_, module_->metadata().get_method_definition(handle_).get_custom_attributes(), attribute, symbol_kind_);
}

std::vector<const IAttribute*> MetadataMethod::get_return_type_attributes() const {
    return return_type_attributes_list_.get([this] {
        AttributeListBuilder b(*module_);
        const srm::MetadataReader& md = module_->metadata();
        const srm::HandleList<srm::ParameterHandle> parameters = md.get_method_definition(handle_).get_parameters();
        if (!parameters.empty()) {
            const srm::Parameter ret_param = md.get_parameter(parameters[0]);
            if (ret_param.sequence_number() == 0) {
                b.add_marshal_info(ret_param.get_marshalling_descriptor());
                b.add(ret_param.get_custom_attributes(), SymbolKind::ReturnType);
            }
        }
        return b.build();
    });
}

bool MetadataMethod::return_type_is_ref_read_only() const {
    std::call_once(return_readonly_once_, [this] {
        const srm::MetadataReader& md = module_->metadata();
        const srm::HandleList<srm::ParameterHandle> parameters = md.get_method_definition(handle_).get_parameters();
        if (!parameters.empty()) {
            const srm::Parameter ret_param = md.get_parameter(parameters[0]);
            if (ret_param.sequence_number() == 0)
                return_type_is_ref_readonly_ = has_known_attribute(ret_param.get_custom_attributes(), md, KnownAttribute::IsReadOnly);
        }
    });
    return return_type_is_ref_readonly_;
}

bool MetadataMethod::this_is_ref_read_only() const {
    std::call_once(this_readonly_once_, [this] {
        const srm::MetadataReader& md = module_->metadata();
        const ITypeDefinition* declaring = declaring_type_definition();
        bool has_read_only_attr = declaring != nullptr && declaring->is_read_only();
        if (any(module_->type_system_options(), TypeSystemOptions::ReadOnlyMethods))
            has_read_only_attr |= has_known_attribute(md.get_method_definition(handle_).get_custom_attributes(), md, KnownAttribute::IsReadOnly);
        this_is_ref_readonly_ = has_read_only_attr;
    });
    return this_is_ref_readonly_;
}

Accessibility MetadataMethod::accessibility() const { return get_accessibility(attributes_); }

Accessibility MetadataMethod::get_accessibility(std::uint16_t method_attributes) {
    switch (method_attributes & 0x7) {   // MemberAccessMask
    case 6: return Accessibility::Public;
    case 3: return Accessibility::Internal;   // Assembly
    case 1: return Accessibility::Private;
    case 4: return Accessibility::Protected;   // Family
    case 2: return Accessibility::ProtectedAndInternal;
    case 5: return Accessibility::ProtectedOrInternal;
    default: return Accessibility::None;
    }
}

bool MetadataMethod::is_virtual() const {
    if (is_static()) return (attributes_ & (method_attr::kAbstract | method_attr::kVirtual)) == method_attr::kVirtual;
    constexpr std::uint16_t mask = method_attr::kAbstract | method_attr::kVirtual | method_attr::kNewSlot | method_attr::kFinal;
    return (attributes_ & mask) == (method_attr::kVirtual | method_attr::kNewSlot);
}

bool MetadataMethod::equals(const IMember* other) const {
    const auto* m = dynamic_cast<const MetadataMethod*>(other);
    return m != nullptr && handle_ == m->handle_ && module_->metadata_file() == m->module_->metadata_file();
}

int MetadataMethod::hash_code() const { return 0x5a00d671 ^ file_hash(*module_) ^ static_cast<int>(handle_.row()); }

const IMember* MetadataMethod::specialize(const TypeParameterSubstitution& substitution) const { return SpecializedMethod::create(this, substitution); }
const IMethod* MetadataMethod::specialize_method(const TypeParameterSubstitution& substitution) const { return SpecializedMethod::create(this, substitution); }

// ---- MetadataParameter ---------------------------------------------------------------------------------------------------

MetadataParameter::MetadataParameter(const MetadataModule& module, const IParameterizedMember* owner, const IType* type, srm::ParameterHandle handle)
    : module_(&module), handle_(handle), attributes_(module.metadata().get_parameter(handle).attributes()), type_(type), owner_(owner) {}

std::vector<const IAttribute*> MetadataParameter::get_attributes() const {
    return attributes_list_.get([this] {
        AttributeListBuilder b(*module_);
        const srm::Parameter parameter = module_->metadata().get_parameter(handle_);
        const bool default_value_assignment_allowed = is_default_value_assignment_allowed(this);
        if (is_optional() && !default_value_assignment_allowed) b.add(KnownAttribute::Optional);
        if (!is_decimal_constant() && has_constant_value_in_signature() && !default_value_assignment_allowed)
            b.add(KnownAttribute::DefaultParameterValue, KnownTypeCode::Object, to_attribute_value(get_constant_value(false)));
        const ReferenceKind rk = reference_kind();
        if ((attributes_ & 0x1) == 0x1 && rk != ReferenceKind::In && rk != ReferenceKind::RefReadOnly) b.add(KnownAttribute::In);
        if ((attributes_ & 0x2) == 0x2 && rk != ReferenceKind::Out) b.add(KnownAttribute::Out);
        b.add(parameter.get_custom_attributes(), SymbolKind::Parameter);
        b.add_marshal_info(parameter.get_marshalling_descriptor());
        return b.build();
    });
}

ReferenceKind MetadataParameter::reference_kind() const {
    if (type_->kind() != TypeKind::ByReference) return ReferenceKind::None;
    if ((attributes_ & 0x3) == 0x2) return ReferenceKind::Out;   // Out without In
    const srm::MetadataReader& md = module_->metadata();
    const TypeSystemOptions options = module_->type_system_options();
    if (any(options, TypeSystemOptions::ReadOnlyStructsAndParameters) &&
        has_known_attribute(md.get_parameter(handle_).get_custom_attributes(), md, KnownAttribute::IsReadOnly))
        return ReferenceKind::In;
    if (any(options, TypeSystemOptions::RefReadOnlyParameters) && (attributes_ & 0x3) == 0x1 &&
        has_known_attribute(md.get_parameter(handle_).get_custom_attributes(), md, KnownAttribute::RequiresLocation))
        return ReferenceKind::RefReadOnly;
    return ReferenceKind::Ref;
}

LifetimeAnnotation MetadataParameter::lifetime() const {
    const TypeSystemOptions options = module_->type_system_options();
    if (!any(options, TypeSystemOptions::ScopedRef)) return {};
    const srm::MetadataReader& md = module_->metadata();
    const std::vector<srm::CustomAttributeHandle> attributes = md.get_parameter(handle_).get_custom_attributes();
    if (any(options, TypeSystemOptions::ParamsCollections) && has_known_attribute(attributes, md, KnownAttribute::ParamCollection)) return {};   // implicitly scoped
    if (has_known_attribute(attributes, md, KnownAttribute::ScopedRef)) return LifetimeAnnotation{true};
    return {};
}

bool MetadataParameter::is_params() const {
    const srm::MetadataReader& md = module_->metadata();
    const std::vector<srm::CustomAttributeHandle> attributes = md.get_parameter(handle_).get_custom_attributes();
    if (type_->kind() == TypeKind::Array) return has_known_attribute(attributes, md, KnownAttribute::ParamArray);
    if (any(module_->type_system_options(), TypeSystemOptions::ParamsCollections)) return has_known_attribute(attributes, md, KnownAttribute::ParamCollection);
    return false;
}

std::string MetadataParameter::name() const {
    std::call_once(name_once_, [this] { name_ = module_->get_string(module_->metadata().get_parameter(handle_).name()); });
    return name_;
}

ConstantValue MetadataParameter::get_constant_value(bool throw_on_invalid_metadata) const {
    try {
        const srm::MetadataReader& md = module_->metadata();
        const srm::Parameter parameter_def = md.get_parameter(handle_);
        if (is_decimal_constant()) return decimal_constant_helper::get_decimal_constant_value(*module_, parameter_def.get_custom_attributes());
        const srm::ConstantHandle constant_handle = parameter_def.get_default_value();
        if (constant_handle.is_nil()) return std::monostate();
        const srm::Constant constant = md.get_constant(constant_handle);
        srm::BlobReader blob_reader = md.get_blob_reader(constant.value());
        try {
            return to_constant_value(blob_reader.read_constant(constant.type_code()));
        } catch (const std::out_of_range&) {
            throw srm::BadImageFormatError(0, "Constant with invalid typecode: " + std::to_string(static_cast<int>(constant.type_code())));
        }
    } catch (const srm::BadImageFormatError&) {
        if (throw_on_invalid_metadata) throw;
        return std::monostate();
    }
}

bool MetadataParameter::has_constant_value_in_signature() const {
    std::call_once(constant_in_signature_once_, [this] {
        if (is_decimal_constant()) constant_value_in_signature_ = decimal_constant_helper::allows_decimal_constants(*module_);
        else constant_value_in_signature_ = !module_->metadata().get_parameter(handle_).get_default_value().is_nil();
    });
    return constant_value_in_signature_;
}

bool MetadataParameter::is_decimal_constant() const {
    if (!is_optional()) return false;   // only optional parameters can be constants
    std::call_once(decimal_once_, [this] {
        decimal_constant_ = decimal_constant_helper::is_decimal_constant(*module_, module_->metadata().get_parameter(handle_).get_custom_attributes());
    });
    return decimal_constant_;
}

std::string MetadataParameter::to_string() const { return token_text(handle_) + " " + parameter_to_string(*this); }

// ---- MetadataField -------------------------------------------------------------------------------------------------------

MetadataField::MetadataField(const MetadataModule& module, srm::FieldDefinitionHandle handle)
    : MetadataMemberBase(module), handle_(handle), attributes_(module.metadata().get_field_definition(handle).attributes()) {}

std::string MetadataField::name() const {
    std::call_once(name_once_, [this] { name_ = module_->get_string(module_->metadata().get_field_definition(handle_).name()); });
    return name_;
}

Accessibility MetadataField::accessibility() const {
    switch (attributes_ & 0x7) {   // FieldAccessMask
    case 6: return Accessibility::Public;
    case 2: return Accessibility::ProtectedAndInternal;   // FamANDAssem
    case 3: return Accessibility::Internal;   // Assembly
    case 4: return Accessibility::Protected;   // Family
    case 5: return Accessibility::ProtectedOrInternal;   // FamORAssem
    default: return Accessibility::Private;
    }
}

const ITypeDefinition* MetadataField::declaring_type_definition() const {
    std::call_once(declaring_once_, [this] { declaring_type_ = module_->get_definition(module_->metadata().get_field_definition(handle_).get_declaring_type()); });
    return declaring_type_;
}

std::vector<const IAttribute*> MetadataField::get_attributes() const {
    return attributes_list_.get([this] {
        AttributeListBuilder b(*module_);
        const srm::FieldDefinition field_def = module_->metadata().get_field_definition(handle_);
        const int offset = field_def.get_offset();
        if (offset != -1) b.add(KnownAttribute::FieldOffset, KnownTypeCode::Int32, std::int32_t{offset});
        if ((field_def.attributes() & 0x80) != 0) b.add(KnownAttribute::NonSerialized);   // NotSerialized
        if ((field_def.attributes() & (0x200 | 0x400)) == 0x200) b.add(KnownAttribute::SpecialName);
        b.add_marshal_info(field_def.get_marshalling_descriptor());
        b.add(field_def.get_custom_attributes(), SymbolKind::Field);
        return b.build();
    });
}

bool MetadataField::has_attribute(KnownAttribute attribute) const {
    return entity_has_attribute(*this, *module_, module_->metadata().get_field_definition(handle_).get_custom_attributes(), attribute, SymbolKind::Field);
}

const IAttribute* MetadataField::get_attribute(KnownAttribute attribute) const {
    return entity_get_attribute(*this, *module_, module_->metadata().get_field_definition(handle_).get_custom_attributes(), attribute, SymbolKind::Field);
}

bool MetadataField::return_type_is_ref_read_only() const {
    const srm::MetadataReader& md = module_->metadata();
    return has_known_attribute(md.get_field_definition(handle_).get_custom_attributes(), md, KnownAttribute::IsReadOnly);
}

bool MetadataField::is_volatile() const {
    decode_type_and_volatile_flag();
    return is_volatile_;
}

const IType* MetadataField::type() const {
    decode_type_and_volatile_flag();
    return type_;
}

void MetadataField::decode_type_and_volatile_flag() const {
    std::call_once(type_once_, [this] {
        const srm::MetadataReader& md = module_->metadata();
        const srm::FieldDefinition field_def = md.get_field_definition(handle_);
        try {
            const IType* declaring = declaring_type();
            const IType* ty = field_def.decode_signature(module_->type_provider(), GenericContext(declaring != nullptr ? &declaring->type_parameters() : nullptr));
            if (const auto* mod = dynamic_cast<const ModifiedType*>(ty);
                mod != nullptr && mod->modifier()->name() == "IsVolatile" && mod->modifier()->namespace_() == "System.Runtime.CompilerServices")
                is_volatile_ = true;
            const std::vector<srm::CustomAttributeHandle> attributes = field_def.get_custom_attributes();
            const ITypeDefinition* declaring_def = declaring_type_definition();
            type_ = apply_attributes_to_type(ty, module_->compilation(), &attributes, md, module_->options_for_entity(this),
                                             declaring_def != nullptr ? declaring_def->nullable_context() : Nullability::Oblivious);
        } catch (const srm::BadImageFormatError&) {
            type_ = &SpecialType::unknown_type();
        }
    });
}

bool MetadataField::is_const() const {
    return (attributes_ & 0x40) != 0 || (is_decimal_constant() && decimal_constant_helper::allows_decimal_constants(*module_));   // Literal
}

bool MetadataField::is_decimal_constant() const {
    if ((attributes_ & (0x10 | 0x20)) != (0x10 | 0x20)) return false;   // only static readonly fields
    std::call_once(decimal_once_, [this] {
        decimal_constant_ = decimal_constant_helper::is_decimal_constant(*module_, module_->metadata().get_field_definition(handle_).get_custom_attributes());
    });
    return decimal_constant_;
}

ConstantValue MetadataField::get_constant_value(bool throw_on_invalid_metadata) const {
    try {
        std::call_once(constant_once_, [this] {
            const srm::MetadataReader& md = module_->metadata();
            const srm::FieldDefinition field_def = md.get_field_definition(handle_);
            if (is_decimal_constant() && decimal_constant_helper::allows_decimal_constants(*module_)) {
                constant_value_ = decimal_constant_helper::get_decimal_constant_value(*module_, field_def.get_custom_attributes());
                return;
            }
            const srm::ConstantHandle constant_handle = field_def.get_default_value();
            if (constant_handle.is_nil()) return;
            const srm::Constant constant = md.get_constant(constant_handle);
            srm::BlobReader blob_reader = md.get_blob_reader(constant.value());
            try {
                constant_value_ = to_constant_value(blob_reader.read_constant(constant.type_code()));
            } catch (const std::out_of_range&) {
                throw srm::BadImageFormatError(0, "Constant with invalid typecode: " + std::to_string(static_cast<int>(constant.type_code())));
            }
        });
        return constant_value_;
    } catch (const srm::BadImageFormatError&) {
        if (throw_on_invalid_metadata) throw;
        return std::monostate();
    }
}

bool MetadataField::equals(const IMember* other) const {
    const auto* f = dynamic_cast<const MetadataField*>(other);
    return f != nullptr && handle_ == f->handle_ && module_->metadata_file() == f->module_->metadata_file();
}

int MetadataField::hash_code() const { return 0x11dda32b ^ file_hash(*module_) ^ static_cast<int>(handle_.row()); }

const IMember* MetadataField::specialize(const TypeParameterSubstitution& substitution) const { return SpecializedField::create(this, substitution); }

// ---- MetadataProperty ----------------------------------------------------------------------------------------------------

namespace {
// (a cast of another member, out of the constructor, where MSVC warns about casts of a member under construction)
const IProperty* as_property(const IMember* member) { return dynamic_cast<const IProperty*>(member); }
}  // namespace

MetadataProperty::MetadataProperty(const MetadataModule& module, srm::PropertyDefinitionHandle handle) : MetadataMemberBase(module), handle_(handle) {
    const srm::MetadataReader& md = module.metadata();
    const srm::PropertyDefinition prop = md.get_property_definition(handle);
    const srm::PropertyAccessors accessors = prop.get_accessors();
    getter_ = module.get_definition(accessors.getter);
    setter_ = module.get_definition(accessors.setter);
    name_ = std::string(md.get_string(prop.name()));
    if (determine_is_indexer(name_)) {
        symbol_kind_ = SymbolKind::Indexer;
    } else if (name_.find('.') != std::string::npos) {
        // an explicit interface implementation: the kind of the interface's property
        const std::vector<const IMember*> implemented = explicitly_implemented_interface_members();
        const IProperty* interface_prop = implemented.empty() ? nullptr : as_property(implemented.front());
        symbol_kind_ = interface_prop != nullptr ? interface_prop->symbol_kind() : SymbolKind::Property;
    }
}

MetadataProperty::~MetadataProperty() = default;

bool MetadataProperty::determine_is_indexer(const std::string& name) const {
    const auto* declaring = dynamic_cast<const MetadataTypeDefinition*>(declaring_type_definition());
    if (declaring == nullptr) return false;
    const std::optional<std::string>& default_member_name = declaring->default_member_name();
    if (!default_member_name || *default_member_name != name) return false;
    return !parameters().empty();
}

const std::vector<const IParameter*>& MetadataProperty::parameters() const {
    decode_signature();
    return parameters_;
}

const IType* MetadataProperty::return_type() const {
    decode_signature();
    return return_type_;
}

bool MetadataProperty::return_type_is_ref_read_only() const {
    const srm::MetadataReader& md = module_->metadata();
    return has_known_attribute(md.get_property_definition(handle_).get_custom_attributes(), md, KnownAttribute::IsReadOnly);
}

void MetadataProperty::decode_signature() const {
    std::call_once(signature_once_, [this] {
        const srm::MetadataReader& md = module_->metadata();
        const srm::PropertyDefinition property_def = md.get_property_definition(handle_);
        try {
            const IType* declaring = declaring_type();
            const GenericContext generic_context(declaring != nullptr ? &declaring->type_parameters() : nullptr);
            const srm::MethodSignature<const IType*> signature = property_def.decode_signature(module_->type_provider(), generic_context);
            const srm::PropertyAccessors accessors = property_def.get_accessors();
            const ITypeDefinition* decl_type_def = declaring_type_definition();
            const Nullability declared_context = decl_type_def != nullptr ? decl_type_def->nullable_context() : Nullability::Oblivious;
            std::optional<srm::HandleList<srm::ParameterHandle>> parameter_handles;
            Nullability nullable_context = declared_context;
            if (!accessors.getter.is_nil()) {
                const srm::MethodDefinition getter = md.get_method_definition(accessors.getter);
                parameter_handles = getter.get_parameters();
                nullable_context = get_nullable_context(getter.get_custom_attributes(), md).value_or(declared_context);
            } else if (!accessors.setter.is_nil()) {
                const srm::MethodDefinition setter = md.get_method_definition(accessors.setter);
                parameter_handles = setter.get_parameters();
                nullable_context = get_nullable_context(setter.get_custom_attributes(), md).value_or(declared_context);
            }
            // OptionsForEntity of the declaring type, not of the property: the property's accessibility is computed, not stored, and
            // computing it decodes this signature (Roslyn does the same in PEPropertySymbol)
            const TypeSystemOptions type_options = module_->options_for_entity(decl_type_def);
            const std::vector<srm::CustomAttributeHandle> property_attributes = property_def.get_custom_attributes();
            MetadataMethod::DecodedSignature decoded = MetadataMethod::decode_signature(*module_, this, signature, parameter_handles ? &*parameter_handles : nullptr,
                                                                                        nullable_context, type_options, &property_attributes);
            return_type_ = decoded.return_type;
            owned_parameters_ = std::move(decoded.parameters);
        } catch (const srm::BadImageFormatError&) {
            return_type_ = &SpecialType::unknown_type();
            owned_parameters_.clear();
        }
        parameters_.clear();
        for (const auto& p : owned_parameters_) parameters_.push_back(p.get());
    });
}

bool MetadataProperty::is_explicit_interface_implementation() const {
    const IMethod* any = any_accessor();
    return any != nullptr && any->is_explicit_interface_implementation();
}

std::vector<const IMember*> MetadataProperty::explicitly_implemented_interface_members() const { return get_interface_members_from_accessor(any_accessor()); }

std::vector<const IMember*> MetadataProperty::get_interface_members_from_accessor(const IMethod* method) {
    if (method == nullptr) return {};
    std::vector<const IMember*> result;
    for (const IMember* m : method->explicitly_implemented_interface_members())
        if (const auto* accessor = dynamic_cast<const IMethod*>(m))
            if (const IMember* owner = accessor->accessor_owner()) result.push_back(owner);
    return result;
}

const ITypeDefinition* MetadataProperty::declaring_type_definition() const {
    const IMethod* any = any_accessor();
    return any != nullptr ? any->declaring_type_definition() : nullptr;
}

const IType* MetadataProperty::declaring_type() const {
    const IMethod* any = any_accessor();
    return any != nullptr ? any->declaring_type() : nullptr;
}

std::vector<const IAttribute*> MetadataProperty::get_attributes() const {
    return attributes_list_.get([this] {
        AttributeListBuilder b(*module_);
        const srm::PropertyDefinition property_def = module_->metadata().get_property_definition(handle_);
        if (is_indexer() && name_ != "Item" && !is_explicit_interface_implementation()) b.add(KnownAttribute::IndexerName, KnownTypeCode::String, name_);
        if ((property_def.attributes() & (0x200 | 0x400)) == 0x200) b.add(KnownAttribute::SpecialName);
        b.add(property_def.get_custom_attributes(), symbol_kind_);
        return b.build();
    });
}

bool MetadataProperty::has_attribute(KnownAttribute attribute) const {
    return entity_has_attribute(*this, *module_, module_->metadata().get_property_definition(handle_).get_custom_attributes(), attribute, symbol_kind_);
}

const IAttribute* MetadataProperty::get_attribute(KnownAttribute attribute) const {
    return entity_get_attribute(*this, *module_, module_->metadata().get_property_definition(handle_).get_custom_attributes(), attribute, symbol_kind_);
}

Accessibility MetadataProperty::accessibility() const {
    std::call_once(accessibility_once_, [this] { cached_accessibility_ = compute_accessibility(); });
    return cached_accessibility_;
}

Accessibility MetadataProperty::compute_accessibility() const {
    if (is_override() && (getter_ == nullptr || setter_ == nullptr)) {
        // an override may override one accessor only: the accessibility comes from the base member
        for (const IMember* base_member : inheritance_helper::get_base_members(this, false)) {
            if (base_member->is_override()) continue;
            // "protected internal" is "protected" across assembly boundaries (ILSpy issue 2653)
            const IModule* base_module = base_member->parent_module();
            const metadata::MetadataFile* base_file = base_module != nullptr ? base_module->metadata_file() : nullptr;
            if (base_member->accessibility() == Accessibility::ProtectedOrInternal && module_->metadata_file() != base_file) return Accessibility::Protected;
            return base_member->accessibility();
        }
    }
    return union_of(getter_ != nullptr ? getter_->accessibility() : Accessibility::None, setter_ != nullptr ? setter_->accessibility() : Accessibility::None);
}

bool MetadataProperty::is_static() const { return any_accessor() != nullptr && any_accessor()->is_static(); }
bool MetadataProperty::is_abstract() const { return any_accessor() != nullptr && any_accessor()->is_abstract(); }
bool MetadataProperty::is_sealed() const { return any_accessor() != nullptr && any_accessor()->is_sealed(); }
bool MetadataProperty::is_virtual() const { return any_accessor() != nullptr && any_accessor()->is_virtual(); }
bool MetadataProperty::is_override() const { return any_accessor() != nullptr && any_accessor()->is_override(); }
bool MetadataProperty::is_overridable() const { return any_accessor() != nullptr && any_accessor()->is_overridable(); }

bool MetadataProperty::equals(const IMember* other) const {
    const auto* p = dynamic_cast<const MetadataProperty*>(other);
    return p != nullptr && handle_ == p->handle_ && module_->metadata_file() == p->module_->metadata_file();
}

int MetadataProperty::hash_code() const { return 0x32b6a76c ^ file_hash(*module_) ^ static_cast<int>(handle_.row()); }

const IMember* MetadataProperty::specialize(const TypeParameterSubstitution& substitution) const { return SpecializedProperty::create(this, substitution); }

// ---- MetadataEvent -------------------------------------------------------------------------------------------------------

MetadataEvent::MetadataEvent(const MetadataModule& module, srm::EventDefinitionHandle handle) : MetadataMemberBase(module), handle_(handle) {
    const srm::MetadataReader& md = module.metadata();
    const srm::EventDefinition ev = md.get_event_definition(handle);
    accessors_ = ev.get_accessors();
    name_ = std::string(md.get_string(ev.name()));
}

const IMethod* MetadataEvent::add_accessor() const { return module_->get_definition(accessors_.adder); }
const IMethod* MetadataEvent::remove_accessor() const { return module_->get_definition(accessors_.remover); }
const IMethod* MetadataEvent::invoke_accessor() const { return module_->get_definition(accessors_.raiser); }
const IMethod* MetadataEvent::any_accessor() const { return module_->get_definition(get_any(accessors_)); }

const IType* MetadataEvent::return_type() const {
    std::call_once(return_type_once_, [this] {
        const srm::MetadataReader& md = module_->metadata();
        const srm::EventDefinition ev = md.get_event_definition(handle_);
        const ITypeDefinition* declaring_type_def = declaring_type_definition();
        const GenericContext context(declaring_type_def != nullptr ? &declaring_type_def->type_parameters() : nullptr);
        const Nullability nullable_context = declaring_type_def != nullptr ? declaring_type_def->nullable_context() : Nullability::Oblivious;
        // the event has no accessibility of its own in the metadata: its type's decides whether nullability applies
        const TypeSystemOptions type_options = module_->options_for_entity(declaring_type_def);
        const std::vector<srm::CustomAttributeHandle> attributes = ev.get_custom_attributes();
        return_type_ = module_->resolve_type(ev.type(), context, type_options, &attributes, nullable_context);
    });
    return return_type_;
}

bool MetadataEvent::is_explicit_interface_implementation() const {
    const IMethod* any = any_accessor();
    return any != nullptr && any->is_explicit_interface_implementation();
}

std::vector<const IMember*> MetadataEvent::explicitly_implemented_interface_members() const {
    return MetadataProperty::get_interface_members_from_accessor(any_accessor());
}

const ITypeDefinition* MetadataEvent::declaring_type_definition() const {
    const IMethod* any = any_accessor();
    return any != nullptr ? any->declaring_type_definition() : nullptr;
}

const IType* MetadataEvent::declaring_type() const {
    const IMethod* any = any_accessor();
    return any != nullptr ? any->declaring_type() : nullptr;
}

std::vector<const IAttribute*> MetadataEvent::get_attributes() const {
    return attributes_list_.get([this] {
        AttributeListBuilder b(*module_);
        const srm::EventDefinition event_def = module_->metadata().get_event_definition(handle_);
        if ((event_def.attributes() & (0x200 | 0x400)) == 0x200) b.add(KnownAttribute::SpecialName);
        b.add(event_def.get_custom_attributes(), SymbolKind::Event);
        return b.build();
    });
}

bool MetadataEvent::has_attribute(KnownAttribute attribute) const {
    return entity_has_attribute(*this, *module_, module_->metadata().get_event_definition(handle_).get_custom_attributes(), attribute, SymbolKind::Event);
}

const IAttribute* MetadataEvent::get_attribute(KnownAttribute attribute) const {
    return entity_get_attribute(*this, *module_, module_->metadata().get_event_definition(handle_).get_custom_attributes(), attribute, SymbolKind::Event);
}

Accessibility MetadataEvent::accessibility() const {
    const IMethod* any = any_accessor();
    return any != nullptr ? any->accessibility() : Accessibility::None;
}

bool MetadataEvent::is_static() const { return any_accessor() != nullptr && any_accessor()->is_static(); }
bool MetadataEvent::is_abstract() const { return any_accessor() != nullptr && any_accessor()->is_abstract(); }
bool MetadataEvent::is_sealed() const { return any_accessor() != nullptr && any_accessor()->is_sealed(); }
bool MetadataEvent::is_virtual() const { return any_accessor() != nullptr && any_accessor()->is_virtual(); }
bool MetadataEvent::is_override() const { return any_accessor() != nullptr && any_accessor()->is_override(); }
bool MetadataEvent::is_overridable() const { return any_accessor() != nullptr && any_accessor()->is_overridable(); }

bool MetadataEvent::equals(const IMember* other) const {
    const auto* e = dynamic_cast<const MetadataEvent*>(other);
    return e != nullptr && handle_ == e->handle_ && module_->metadata_file() == e->module_->metadata_file();
}

int MetadataEvent::hash_code() const { return 0x7937039a ^ file_hash(*module_) ^ static_cast<int>(handle_.row()); }

const IMember* MetadataEvent::specialize(const TypeParameterSubstitution& substitution) const { return SpecializedEvent::create(this, substitution); }

// ---- VarArgInstanceMethod ------------------------------------------------------------------------------------------------

VarArgInstanceMethod::VarArgInstanceMethod(const IMethod* base_method, const std::vector<const IType*>& var_arg_types) : base_method_(base_method) {
    parameters_ = base_method->parameters();
    if (parameters_.empty() || parameters_.back()->type()->kind() != TypeKind::ArgList) throw UsageError("VarArgInstanceMethod: the method is not vararg");
    parameters_.pop_back();
    for (const IType* var_arg : var_arg_types) {
        owned_parameters_.push_back(std::make_unique<DefaultParameter>(var_arg, std::string(), this));
        parameters_.push_back(owned_parameters_.back().get());
    }
}

bool VarArgInstanceMethod::equals(const IMember* other) const {
    const auto* o = dynamic_cast<const VarArgInstanceMethod*>(other);
    return o != nullptr && member_equals(base_method_, o->base_method_);
}

bool VarArgInstanceMethod::equals(const IMember* other, TypeVisitor& type_normalization) const {
    const auto* o = dynamic_cast<const VarArgInstanceMethod*>(other);
    return o != nullptr && base_method_->equals(o->base_method_, type_normalization);
}

std::string VarArgInstanceMethod::to_string() const {
    std::string b = "[" + symbol_kind_name(symbol_kind());
    if (const IType* declaring = declaring_type()) b += declaring->reflection_name() + ".";
    b += name();
    if (!type_parameters().empty()) b += "``" + std::to_string(type_parameters().size());
    b += "(";
    for (std::size_t i = 0; i < parameters_.size(); ++i) {
        if (i > 0) b += ", ";
        if (static_cast<int>(i) == regular_parameter_count()) b += "..., ";
        b += parameters_[i]->type()->reflection_name();
    }
    if (static_cast<int>(parameters_.size()) == regular_parameter_count()) b += ", ...";
    b += "):" + return_type()->reflection_name() + "]";
    return b;
}

const IMethod* VarArgInstanceMethod::specialize_method(const TypeParameterSubstitution& substitution) const {
    TypeParameterSubstitution s = substitution;
    std::vector<const IType*> var_arg_types;
    for (std::size_t i = base_method_->parameters().size() - 1; i < parameters_.size(); ++i) var_arg_types.push_back(parameters_[i]->type()->accept_visitor(s));
    return compilation().arena().keep(std::make_unique<VarArgInstanceMethod>(base_method_->specialize_method(substitution), var_arg_types));
}

}  // namespace cb::dotnet::decompiler::typesystem
