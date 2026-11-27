// dotnet/decompiler/typesystem/metadata_module.cpp -- a module of the type system: definitions, resolution, attributes, namespaces.
#include "dotnet/decompiler/typesystem/metadata_module.hpp"

#include <algorithm>

#include "dotnet/decompiler/metadata/assembly_references.hpp"
#include "dotnet/decompiler/metadata/metadata_extensions.hpp"
#include "dotnet/decompiler/srm_extensions.hpp"
#include "dotnet/decompiler/typesystem/fake_member.hpp"
#include "dotnet/decompiler/typesystem/specialized_member.hpp"
#include "dotnet/decompiler/typesystem/type_system_extensions.hpp"
#include "dotnet/metadata/custom_attribute_decoder.hpp"

namespace cb::dotnet::decompiler::typesystem {
namespace {

bool iequals_ascii(std::string_view a, std::string_view b) {
    if (a.size() != b.size()) return false;
    for (std::size_t i = 0; i < a.size(); ++i) {
        char x = a[i], y = b[i];
        if (x >= 'A' && x <= 'Z') x = static_cast<char>(x - 'A' + 'a');
        if (y >= 'A' && y <= 'Z') y = static_cast<char>(y - 'A' + 'a');
        if (x != y) return false;
    }
    return true;
}

NormalizeTypeVisitor& signature_normalizer() {
    static NormalizeTypeVisitor v = [] {
        NormalizeTypeVisitor n;
        n.replace_class_type_parameters_with_dummy = true;
        n.replace_method_type_parameters_with_dummy = true;
        return n;
    }();
    return v;
}

bool compare_types(const IType* a, const IType* b) {
    return type_equals(a->accept_visitor(signature_normalizer()), b->accept_visitor(signature_normalizer()));
}

bool compare_signatures(const std::vector<const IParameter*>& parameters, const std::vector<const IType*>& parameter_types) {
    if (parameter_types.size() != parameters.size()) return false;
    for (std::size_t i = 0; i < parameter_types.size(); ++i)
        if (!compare_types(parameter_types[i], parameters[i]->type())) return false;
    return true;
}

Accessibility find_minimum_accessibility_for_nrt(const srm::MetadataReader& metadata, const std::vector<srm::CustomAttributeHandle>& custom_attributes) {
    // the least effective accessibility an entity needs for the metadata to keep the nullability of its type
    for (const srm::CustomAttributeHandle handle : custom_attributes) {
        const srm::CustomAttribute custom_attribute = metadata.get_custom_attribute(handle);
        if (!is_known_attribute(custom_attribute, metadata, KnownAttribute::NullablePublicOnly)) continue;
        srm::CustomAttributeValue<const IType*> value;
        try {
            value = custom_attribute.decode_value(minimal_corlib_type_provider());
        } catch (const srm::BadImageFormatError&) {
            continue;
        } catch (const metadata::EnumUnderlyingTypeResolveError&) {
            continue;
        }
        if (value.fixed_arguments.size() == 1)
            if (const auto* includes_internals = std::get_if<bool>(&value.fixed_arguments[0].value))
                return *includes_internals ? Accessibility::ProtectedAndInternal : Accessibility::Protected;
    }
    return Accessibility::None;
}

std::string get_short_name(const std::string& full_assembly_name) {
    const std::size_t pos = full_assembly_name.find(',');
    return pos == std::string::npos ? full_assembly_name : full_assembly_name.substr(0, pos);
}

template <class T>
std::unique_ptr<std::atomic<const T*>[]> make_slots(std::size_t count) {
    return std::make_unique<std::atomic<const T*>[]>(count + 1);
}

[[noreturn]] void handle_out_of_range() { throw srm::BadImageFormatError(0, "Handle with invalid row number."); }

enum class ResolveKind : int { Method = 1, EntityMethodReference, EntityMethodSpecification, FieldReference };

}  // namespace

// ---- construction --------------------------------------------------------------------------------------------------------

MetadataModule::MetadataModule(const ICompilation& compilation, const metadata::MetadataFile& file, TypeSystemOptions options)
    : compilation_(&compilation), file_(&file), metadata_(&file.metadata()), options_(options), type_provider_(*this) {
    const srm::MetadataReader& md = *metadata_;
    if (md.is_assembly()) {
        const srm::AssemblyDefinition asmdef = md.get_assembly_definition();
        try {
            assembly_name_ = std::string(md.get_string(asmdef.name()));
            assembly_version_ = asmdef.version();
            full_assembly_name_ = metadata::get_full_assembly_name(md);
        } catch (const srm::BadImageFormatError&) {
            assembly_name_ = "<ERR: invalid assembly name>";
            full_assembly_name_ = "<ERR: invalid assembly name>";
        }
    } else {
        try {
            assembly_name_ = std::string(md.get_string(md.get_module_definition().name()));
        } catch (const srm::BadImageFormatError&) {
            assembly_name_ = "<ERR: invalid assembly name>";
        }
        full_assembly_name_ = assembly_name_;
    }
    const std::vector<srm::CustomAttributeHandle> custom_attrs = md.get_module_definition().get_custom_attributes();
    nullable_context_ = get_nullable_context(custom_attrs, md).value_or(Nullability::Oblivious);
    min_accessibility_for_nrt_ = find_minimum_accessibility_for_nrt(md, custom_attrs);
    root_namespace_ = std::make_unique<MetadataNamespace>(*this, nullptr, std::string(), md.get_namespace_definition_root_handle());

    // the resolved entities, by row (the Uncached option is read as cached: see the header)
    type_def_count_ = static_cast<std::size_t>(md.get_table_row_count(srm::TableIndex::TypeDef));
    field_def_count_ = static_cast<std::size_t>(md.get_table_row_count(srm::TableIndex::Field));
    method_def_count_ = static_cast<std::size_t>(md.get_table_row_count(srm::TableIndex::MethodDef));
    property_def_count_ = static_cast<std::size_t>(md.get_table_row_count(srm::TableIndex::Property));
    event_def_count_ = static_cast<std::size_t>(md.get_table_row_count(srm::TableIndex::Event));
    assembly_ref_count_ = static_cast<std::size_t>(md.get_table_row_count(srm::TableIndex::AssemblyRef));
    type_defs_ = make_slots<MetadataTypeDefinition>(type_def_count_);
    field_defs_ = make_slots<MetadataField>(field_def_count_);
    method_defs_ = make_slots<MetadataMethod>(method_def_count_);
    property_defs_ = make_slots<MetadataProperty>(property_def_count_);
    event_defs_ = make_slots<MetadataEvent>(event_def_count_);
    referenced_assemblies_ = make_slots<IModule>(assembly_ref_count_);
}

MetadataModule::~MetadataModule() = default;

template <class T>
const T* MetadataModule::keep(std::unique_ptr<T> object) const {
    const T* raw = object.get();
    const std::lock_guard lock(owned_mutex_);
    owned_.push_back(std::shared_ptr<const void>(std::move(object)));
    return raw;
}

MetadataModule::ResolveKey MetadataModule::resolve_key(int kind, std::uint32_t token, const GenericContext& context) {
    auto contents = [](const std::vector<const ITypeParameter*>* list) {
        std::vector<const void*> result;
        if (list != nullptr) result.assign(list->begin(), list->end());
        return result;
    };
    return ResolveKey{kind, token, contents(context.class_type_parameters), contents(context.method_type_parameters)};
}

template <class T, class Make>
const T* MetadataModule::cached_resolve(ResolveKey key, Make&& make) const {
    {
        const std::lock_guard lock(resolve_mutex_);
        if (const auto it = resolved_.find(key); it != resolved_.end()) return static_cast<const T*>(it->second);
    }
    const T* made = make();
    const std::lock_guard lock(resolve_mutex_);
    const auto [it, added] = resolved_.try_emplace(key, made);
    return static_cast<const T*>(it->second);
}

// ---- IModule -------------------------------------------------------------------------------------------------------------

const INamespace& MetadataModule::root_namespace() const { return *root_namespace_; }

std::vector<const ITypeDefinition*> MetadataModule::type_definitions() const {
    std::vector<const ITypeDefinition*> result;
    result.reserve(type_def_count_);
    for (const srm::TypeDefinitionHandle h : metadata_->type_definitions()) result.push_back(get_definition(h));
    return result;
}

std::vector<const ITypeDefinition*> MetadataModule::top_level_type_definitions() const {
    std::vector<const ITypeDefinition*> result;
    for (const ITypeDefinition* td : type_definitions())
        if (td->declaring_type_definition() == nullptr) result.push_back(td);
    return result;
}

const ITypeDefinition* MetadataModule::get_type_definition(const TopLevelTypeName& top_level_type_name) const {
    const srm::TypeDefinitionHandle type_def_handle = file_->get_type_definition(top_level_type_name);
    if (type_def_handle.is_nil()) {
        const srm::ExportedTypeHandle forwarder_handle = file_->get_type_forwarder(FullTypeName(top_level_type_name));
        if (!forwarder_handle.is_nil()) return resolve_forwarded_type(metadata_->get_exported_type(forwarder_handle))->get_definition();
    }
    return get_definition(type_def_handle);
}

const ITypeDefinition* MetadataModule::get_type_definition(std::string_view namespace_name, std::string_view name, int type_parameter_count) const {
    return get_type_definition(TopLevelTypeName(std::string(namespace_name), std::string(name), type_parameter_count));
}

bool MetadataModule::internals_visible_to(const IModule& module) const {
    if (this == &module) return true;
    for (const std::string& short_name : get_internals_visible_to())
        if (iequals_ascii(module.assembly_name(), short_name)) return true;
    return false;
}

const std::vector<std::string>& MetadataModule::get_internals_visible_to() const {
    std::call_once(internals_visible_to_once_, [this] {
        if (!metadata_->is_assembly()) return;
        for (const srm::CustomAttributeHandle attr_handle : metadata_->get_assembly_definition().get_custom_attributes()) {
            const srm::CustomAttribute attr = metadata_->get_custom_attribute(attr_handle);
            if (!is_known_attribute(attr, *metadata_, KnownAttribute::InternalsVisibleTo)) continue;
            const srm::CustomAttributeValue<const IType*> attr_value = attr.decode_value(type_provider_);
            if (attr_value.fixed_arguments.size() == 1)
                if (const auto* s = std::get_if<std::string>(&attr_value.fixed_arguments[0].value)) internals_visible_to_.push_back(get_short_name(*s));
        }
    });
    return internals_visible_to_;
}

// ---- definitions ---------------------------------------------------------------------------------------------------------

namespace {

template <class T, class Handle>
const T* get_or_make(const MetadataModule& module, std::atomic<const T*>* slots, std::size_t count, Handle handle,
                     const std::function<const T*(std::unique_ptr<T>)>& keep) {
    if (handle.is_nil()) return nullptr;
    const std::uint32_t row = handle.row();
    if (row > count) handle_out_of_range();
    if (const T* existing = slots[row].load(std::memory_order_acquire)) return existing;
    auto made = std::make_unique<T>(module, handle);
    const T* expected = nullptr;
    if (slots[row].compare_exchange_strong(expected, made.get(), std::memory_order_acq_rel)) return keep(std::move(made));
    return expected;   // another thread made it first
}

}  // namespace

const ITypeDefinition* MetadataModule::get_definition(srm::TypeDefinitionHandle handle) const {
    return get_or_make<MetadataTypeDefinition>(*this, type_defs_.get(), type_def_count_, handle,
                                               [this](std::unique_ptr<MetadataTypeDefinition> p) { return keep(std::move(p)); });
}

const IField* MetadataModule::get_definition(srm::FieldDefinitionHandle handle) const {
    return get_or_make<MetadataField>(*this, field_defs_.get(), field_def_count_, handle, [this](std::unique_ptr<MetadataField> p) { return keep(std::move(p)); });
}

const IMethod* MetadataModule::get_definition(srm::MethodDefinitionHandle handle) const {
    return get_or_make<MetadataMethod>(*this, method_defs_.get(), method_def_count_, handle, [this](std::unique_ptr<MetadataMethod> p) { return keep(std::move(p)); });
}

const IProperty* MetadataModule::get_definition(srm::PropertyDefinitionHandle handle) const {
    return get_or_make<MetadataProperty>(*this, property_defs_.get(), property_def_count_, handle,
                                         [this](std::unique_ptr<MetadataProperty> p) { return keep(std::move(p)); });
}

const IEvent* MetadataModule::get_definition(srm::EventDefinitionHandle handle) const {
    return get_or_make<MetadataEvent>(*this, event_defs_.get(), event_def_count_, handle, [this](std::unique_ptr<MetadataEvent> p) { return keep(std::move(p)); });
}

// ---- modules -------------------------------------------------------------------------------------------------------------

const IModule* MetadataModule::resolve_module(srm::AssemblyReferenceHandle handle) const {
    if (handle.is_nil()) return nullptr;
    const std::uint32_t row = handle.row();
    if (row > assembly_ref_count_) handle_out_of_range();
    if (const IModule* m = referenced_assemblies_[row].load(std::memory_order_acquire)) return m;
    const IModule* module = find_module_by_reference(*compilation_, metadata::AssemblyReference(*metadata_, handle));
    if (module != nullptr) referenced_assemblies_[row].store(module, std::memory_order_release);   // (an unresolved one is asked again)
    return module;
}

const IModule* MetadataModule::resolve_module(srm::ModuleReferenceHandle handle) const {
    if (handle.is_nil()) return nullptr;
    const std::string name(metadata_->get_string(metadata_->get_module_reference(handle).name()));
    for (const IModule* mod : compilation_->modules())
        if (mod->name() == name) return mod;
    return nullptr;
}

const IModule* MetadataModule::get_declaring_module(srm::TypeReferenceHandle handle) const {
    if (handle.is_nil()) return nullptr;
    const srm::EntityHandle scope = metadata_->get_type_reference(handle).resolution_scope();
    switch (scope.kind()) {
    case srm::HandleKind::TypeReference: return get_declaring_module(srm::TypeReferenceHandle(scope));
    case srm::HandleKind::AssemblyReference: return resolve_module(srm::AssemblyReferenceHandle(scope));
    case srm::HandleKind::ModuleReference: return resolve_module(srm::ModuleReferenceHandle(scope));
    default: return this;
    }
}

// ---- types ---------------------------------------------------------------------------------------------------------------

const IType* MetadataModule::resolve_type(srm::EntityHandle type_ref_def_spec, const GenericContext& context,
                                          const std::vector<srm::CustomAttributeHandle>* type_attributes, Nullability nullable_context) const {
    return resolve_type(type_ref_def_spec, context, options_, type_attributes, nullable_context);
}

const IType* MetadataModule::resolve_type(srm::EntityHandle type_ref_def_spec, const GenericContext& context, TypeSystemOptions custom_options,
                                          const std::vector<srm::CustomAttributeHandle>* type_attributes, Nullability nullable_context) const {
    if (type_ref_def_spec.is_nil()) return &SpecialType::unknown_type();
    const IType* ty = nullptr;
    switch (type_ref_def_spec.kind()) {
    case srm::HandleKind::TypeDefinition: ty = type_provider_.get_type_from_definition(*metadata_, srm::TypeDefinitionHandle(type_ref_def_spec), 0); break;
    case srm::HandleKind::TypeReference: ty = type_provider_.get_type_from_reference(*metadata_, srm::TypeReferenceHandle(type_ref_def_spec), 0); break;
    case srm::HandleKind::TypeSpecification:
        ty = metadata_->get_type_specification(srm::TypeSpecificationHandle(type_ref_def_spec)).decode_signature(type_provider_, context);
        break;
    case srm::HandleKind::ExportedType: return resolve_forwarded_type(metadata_->get_exported_type(srm::ExportedTypeHandle(type_ref_def_spec)));
    default: throw srm::BadImageFormatError(0, "Not a type handle");
    }
    return apply_attributes_to_type(ty, *compilation_, type_attributes, *metadata_, custom_options, nullable_context);
}

const IType* MetadataModule::resolve_declaring_type(srm::EntityHandle declaring_type_reference, const GenericContext& context) const {
    // resolved without dynamic / tuple / nullability / nint, but with tuples in the type arguments
    const TypeSystemOptions removed_options = TypeSystemOptions::Dynamic | TypeSystemOptions::Tuple | TypeSystemOptions::NullabilityAnnotations |
                                              TypeSystemOptions::NativeIntegers | TypeSystemOptions::NativeIntegersWithoutAttribute;
    const IType* ty = resolve_type(declaring_type_reference, context, options_ & ~removed_options);
    return apply_attributes_to_type(ty, *compilation_, nullptr, *metadata_, options_, Nullability::Oblivious, true);
}

const IType* MetadataModule::introduce_tuple_types(const IType* ty) const {
    return apply_attributes_to_type(ty, *compilation_, nullptr, *metadata_, options_, Nullability::Oblivious);
}

// ---- methods -------------------------------------------------------------------------------------------------------------

const IMethod* MetadataModule::resolve_method(srm::EntityHandle method_reference, const GenericContext& context) const {
    if (method_reference.is_nil()) throw UsageError("resolve_method: a nil method reference");
    switch (method_reference.kind()) {
    case srm::HandleKind::MethodDefinition:
    case srm::HandleKind::MemberReference:
    case srm::HandleKind::MethodSpecification: break;
    default: throw srm::BadImageFormatError(0, "Metadata token must be either a methoddef, memberref or methodspec");
    }
    const ResolveKey key = resolve_key(static_cast<int>(ResolveKind::Method), method_reference.token(), context);
    return cached_resolve<IMethod>(key, [&]() -> const IMethod* {
        switch (method_reference.kind()) {
        case srm::HandleKind::MethodDefinition: return resolve_method_definition(srm::MethodDefinitionHandle(method_reference), true);
        case srm::HandleKind::MemberReference: return resolve_method_reference(srm::MemberReferenceHandle(method_reference), context, nullptr, true);
        default: return resolve_method_specification(srm::MethodSpecificationHandle(method_reference), context, true);
        }
    });
}

const IMethod* MetadataModule::resolve_method_definition(srm::MethodDefinitionHandle method_def_handle, bool expand_var_args) const {
    const IMethod* method = get_definition(method_def_handle);
    if (expand_var_args && !method->parameters().empty() && method->parameters().back()->type()->kind() == TypeKind::ArgList)
        method = keep(std::make_unique<VarArgInstanceMethod>(method, std::vector<const IType*>{}));
    return method;
}

const IMethod* MetadataModule::resolve_method_specification(srm::MethodSpecificationHandle method_spec_handle, const GenericContext& context,
                                                            bool expand_var_args) const {
    const srm::MethodSpecification method_spec = metadata_->get_method_specification(method_spec_handle);
    std::vector<const IType*> method_type_args = method_spec.decode_signature(type_provider_, context);
    for (const IType*& t : method_type_args) t = introduce_tuple_types(t);
    if (method_spec.method().kind() == srm::HandleKind::MethodDefinition) {
        // a generic instance of a methoddef (a generic method in a non-generic class of this assembly)
        const IMethod* method = resolve_method_definition(srm::MethodDefinitionHandle(method_spec.method()), expand_var_args);
        return method->specialize_method(TypeParameterSubstitution(std::nullopt, std::move(method_type_args)));
    }
    return resolve_method_reference(srm::MemberReferenceHandle(method_spec.method()), context, &method_type_args, expand_var_args);
}

const IMethod* MetadataModule::resolve_method_reference(srm::MemberReferenceHandle member_ref_handle, const GenericContext& context,
                                                        const std::vector<const IType*>* method_type_arguments, bool expand_var_args) const {
    const srm::MemberReference member_ref = metadata_->get_member_reference(member_ref_handle);
    if (member_ref.get_kind() != srm::MemberReferenceKind::Method) throw srm::BadImageFormatError(0, "Member reference must be method, but was: Field");
    srm::MethodSignature<const IType*> signature;
    std::optional<std::vector<const IType*>> class_type_arguments;
    const IMethod* method = nullptr;
    if (member_ref.parent().kind() == srm::HandleKind::MethodDefinition) {
        method = resolve_method_definition(srm::MethodDefinitionHandle(member_ref.parent()), false);
        signature = member_ref.decode_method_signature(type_provider_, context);
    } else {
        const IType* declaring_type = resolve_declaring_type(member_ref.parent(), context);
        const ITypeDefinition* declaring_type_definition = declaring_type->get_definition();
        if (!declaring_type->type_arguments().empty()) class_type_arguments = declaring_type->type_arguments();
        // the signature is the definition's, though the declaring type may be a generic instance: the member is looked for on the definition
        const std::string name(metadata_->get_string(member_ref.name()));
        signature = member_ref.decode_method_signature(
            type_provider_, GenericContext(declaring_type_definition != nullptr ? &declaring_type_definition->type_parameters() : nullptr));
        if (declaring_type_definition != nullptr) {
            std::vector<const IMethod*> methods;
            if (name == ".ctor") {
                methods = declaring_type_definition->get_constructors();
            } else if (name == ".cctor") {
                for (const IMethod* m : declaring_type_definition->methods())
                    if (m->is_constructor() && m->is_static()) methods.push_back(m);
            } else {
                methods = declaring_type_definition->get_methods([&](const IMethod* m) { return m->name() == name; }, GetMemberOptions::IgnoreInheritedMembers);
                for (const IMethod* m : declaring_type_definition->get_accessors([&](const IMethod* m) { return m->name() == name; }, GetMemberOptions::IgnoreInheritedMembers))
                    methods.push_back(m);
            }
            // the parameters the signature expects
            std::vector<const IType*> parameter_types;
            if (signature.header.calling_convention() == srm::SignatureCallingConvention::VarArgs) {
                parameter_types.assign(signature.parameter_types.begin(), signature.parameter_types.begin() + signature.required_parameter_count);
                parameter_types.push_back(&SpecialType::arg_list());
            } else {
                parameter_types = signature.parameter_types;
            }
            for (const IMethod* m : methods) {
                if (static_cast<int>(m->type_parameters().size()) != signature.generic_parameter_count) continue;
                if (signature.header.is_instance() != !m->is_static()) continue;
                if (compare_signatures(m->parameters(), parameter_types) && compare_types(m->return_type(), signature.return_type)) {
                    method = m;
                    break;
                }
            }
        }
        if (method == nullptr) method = create_fake_method(declaring_type, name, signature);
    }
    if (class_type_arguments || method_type_arguments != nullptr) {
        method = method->specialize_method(TypeParameterSubstitution(
            class_type_arguments, method_type_arguments != nullptr ? std::optional<std::vector<const IType*>>(*method_type_arguments) : std::nullopt));
    }
    if (expand_var_args && signature.header.calling_convention() == srm::SignatureCallingConvention::VarArgs) {
        const std::vector<const IType*> var_args(signature.parameter_types.begin() + signature.required_parameter_count, signature.parameter_types.end());
        method = keep(std::make_unique<VarArgInstanceMethod>(method, var_args));
    }
    return method;
}

const IMethod* MetadataModule::create_fake_method(const IType* declaring_type, const std::string& name, const srm::MethodSignature<const IType*>& signature) const {
    const SymbolKind symbol_kind = name == ".ctor" || name == ".cctor" ? SymbolKind::Constructor : SymbolKind::Method;
    auto m = std::make_unique<FakeMethod>(*compilation_, symbol_kind);
    m->declaring_type_value = declaring_type;
    m->name_value = name;
    m->return_type_value = signature.return_type;
    m->is_static_value = !signature.header.is_instance();
    std::optional<TypeParameterSubstitution> substitution;
    if (signature.generic_parameter_count > 0) {
        std::vector<const IType*> type_parameter_types;
        for (int i = 0; i < signature.generic_parameter_count; ++i) {
            const ITypeParameter* tp = keep(std::make_unique<DefaultTypeParameter>(m.get(), i));
            m->type_parameters_value.push_back(tp);
            type_parameter_types.push_back(tp);
        }
        substitution = TypeParameterSubstitution(declaring_type->type_arguments(), std::move(type_parameter_types));
    } else if (!declaring_type->type_arguments().empty()) {
        substitution = declaring_type->get_substitution();
    }
    std::vector<const IParameter*> parameters;
    for (int i = 0; i < signature.required_parameter_count; ++i) {
        const IType* type = signature.parameter_types[static_cast<std::size_t>(i)];
        if (substitution) type = type->accept_visitor(*substitution);   // the dummy method type parameters become the ones just made
        parameters.push_back(keep(std::make_unique<DefaultParameter>(type, std::string())));
    }
    m->parameters_value = parameters;

    // GuessFakeMethodAccessor: get_/set_/add_/remove_/raise_ methods of a type we do not have are taken for accessors
    if (signature.generic_parameter_count == 0) {
        const bool guessed_getter = name.rfind("get_", 0) == 0;
        const bool guessed_setter = name.rfind("set_", 0) == 0;
        if (guessed_getter || guessed_setter) {
            auto fake_property = std::make_unique<FakeProperty>(*compilation_);
            fake_property->name_value = name.substr(4);
            fake_property->declaring_type_value = declaring_type;
            fake_property->is_static_value = m->is_static_value;
            if (guessed_getter) {
                if (signature.return_type->kind() != TypeKind::Void) {
                    m->accessor_kind_value = MethodSemanticsAttributes::Getter;
                    fake_property->getter_value = m.get();
                    fake_property->return_type_value = signature.return_type;
                    fake_property->is_indexer_value = !parameters.empty();
                    fake_property->parameters_value = parameters;
                    m->accessor_owner_value = keep(std::move(fake_property));
                }
            } else if (!parameters.empty() && signature.return_type->kind() == TypeKind::Void) {
                m->accessor_kind_value = MethodSemanticsAttributes::Setter;
                fake_property->setter_value = m.get();
                fake_property->return_type_value = parameters.back()->type();
                fake_property->is_indexer_value = parameters.size() > 1;
                fake_property->parameters_value.assign(parameters.begin(), parameters.end() - 1);
                m->accessor_owner_value = keep(std::move(fake_property));
            }
        } else {
            static constexpr std::string_view kAdd = "add_", kRemove = "remove_", kRaise = "raise_";
            const bool guessed_add = name.rfind(kAdd, 0) == 0;
            const bool guessed_remove = name.rfind(kRemove, 0) == 0;
            const bool guessed_raise = name.rfind(kRaise, 0) == 0;
            if (guessed_add || guessed_remove || guessed_raise) {
                auto fake_event = std::make_unique<FakeEvent>(*compilation_);
                fake_event->declaring_type_value = declaring_type;
                fake_event->is_static_value = m->is_static_value;
                if (guessed_add) {
                    if (parameters.size() == 1) {
                        m->accessor_kind_value = MethodSemanticsAttributes::Adder;
                        fake_event->name_value = name.substr(kAdd.size());
                        fake_event->add_accessor_value = m.get();
                        fake_event->return_type_value = parameters.front()->type();
                        m->accessor_owner_value = keep(std::move(fake_event));
                    }
                } else if (guessed_remove) {
                    if (parameters.size() == 1) {
                        m->accessor_kind_value = MethodSemanticsAttributes::Remover;
                        fake_event->name_value = name.substr(kRemove.size());
                        fake_event->remove_accessor_value = m.get();
                        fake_event->return_type_value = parameters.front()->type();
                        m->accessor_owner_value = keep(std::move(fake_event));
                    }
                } else {
                    fake_event->name_value = name.substr(kRaise.size());
                    fake_event->invoke_accessor_value = m.get();
                    m->accessor_kind_value = MethodSemanticsAttributes::Raiser;
                    m->accessor_owner_value = keep(std::move(fake_event));
                }
            }
        }
    }
    return keep(std::move(m));
}

// ---- entities ------------------------------------------------------------------------------------------------------------

const IEntity* MetadataModule::resolve_entity(srm::EntityHandle entity_handle, const GenericContext& context) const {
    switch (entity_handle.kind()) {
    case srm::HandleKind::TypeReference:
    case srm::HandleKind::TypeDefinition:
    case srm::HandleKind::TypeSpecification:
    case srm::HandleKind::ExportedType:
        // (ResolveDeclaringType: ResolveType could give nint / nuint, special types without a definition)
        return resolve_declaring_type(entity_handle, context)->get_definition();
    case srm::HandleKind::MemberReference: {
        const srm::MemberReferenceHandle member_reference_handle(entity_handle);
        switch (metadata_->get_member_reference(member_reference_handle).get_kind()) {
        case srm::MemberReferenceKind::Method: {
            // as the MethodDefinition case: varargs are never expanded
            const ResolveKey key = resolve_key(static_cast<int>(ResolveKind::EntityMethodReference), entity_handle.token(), context);
            return cached_resolve<IMethod>(key, [&] { return resolve_method_reference(member_reference_handle, context, nullptr, false); });
        }
        case srm::MemberReferenceKind::Field: {
            const ResolveKey key = resolve_key(static_cast<int>(ResolveKind::FieldReference), entity_handle.token(), context);
            return cached_resolve<IField>(key, [&] { return resolve_field_reference(member_reference_handle, context); });
        }
        }
        throw srm::BadImageFormatError(0, "Unknown MemberReferenceKind");
    }
    case srm::HandleKind::MethodDefinition: return get_definition(srm::MethodDefinitionHandle(entity_handle));
    case srm::HandleKind::MethodSpecification: {
        const ResolveKey key = resolve_key(static_cast<int>(ResolveKind::EntityMethodSpecification), entity_handle.token(), context);
        return cached_resolve<IMethod>(key, [&] { return resolve_method_specification(srm::MethodSpecificationHandle(entity_handle), context, false); });
    }
    case srm::HandleKind::FieldDefinition: return get_definition(srm::FieldDefinitionHandle(entity_handle));
    case srm::HandleKind::EventDefinition: return get_definition(srm::EventDefinitionHandle(entity_handle));
    case srm::HandleKind::PropertyDefinition: return get_definition(srm::PropertyDefinitionHandle(entity_handle));
    default: return nullptr;
    }
}

const IField* MetadataModule::resolve_field_reference(srm::MemberReferenceHandle member_reference_handle, const GenericContext& context) const {
    const srm::MemberReference member_ref = metadata_->get_member_reference(member_reference_handle);
    const IType* declaring_type = resolve_declaring_type(member_ref.parent(), context);
    const ITypeDefinition* declaring_type_definition = declaring_type->get_definition();
    const std::string name(metadata_->get_string(member_ref.name()));
    // the field signature is the definition's, not the generic instance's
    const IType* signature = member_ref.decode_field_signature(
        type_provider_, GenericContext(declaring_type_definition != nullptr ? &declaring_type_definition->type_parameters() : nullptr));
    // ('f' is the definition too, even where declaring_type is a ParameterizedType)
    const std::vector<const IField*> fields = declaring_type->get_fields(
        [&](const IField* f) { return f->name() == name && compare_types(f->return_type(), signature); }, GetMemberOptions::IgnoreInheritedMembers);
    if (!fields.empty()) return fields.front();
    // a field of a generic type: its type arguments are substituted
    if (!declaring_type->type_arguments().empty()) {
        TypeParameterSubstitution substitution = declaring_type->get_substitution();
        signature = signature->accept_visitor(substitution);
    }
    auto field = std::make_unique<FakeField>(*compilation_);
    field->return_type_value = signature;
    field->name_value = name;
    field->declaring_type_value = declaring_type;
    return keep(std::move(field));
}

// ---- standalone signatures -----------------------------------------------------------------------------------------------

std::pair<srm::SignatureHeader, const FunctionPointerType*> MetadataModule::decode_method_signature(srm::StandaloneSignatureHandle handle,
                                                                                                    const GenericContext& generic_context) const {
    const srm::StandaloneSignature standalone_signature = metadata_->get_standalone_signature(handle);
    if (standalone_signature.get_kind() != srm::StandaloneSignatureKind::Method) throw srm::BadImageFormatError(0, "Expected Method signature");
    const srm::MethodSignature<const IType*> sig = standalone_signature.decode_method_signature(type_provider_, generic_context);
    const FunctionPointerType* fpt = function_pointer_type_from_signature(sig, *this);
    return {sig.header, dynamic_cast<const FunctionPointerType*>(introduce_tuple_types(fpt))};
}

std::vector<const IType*> MetadataModule::decode_local_signature(srm::StandaloneSignatureHandle handle, const GenericContext& generic_context) const {
    const srm::StandaloneSignature standalone_signature = metadata_->get_standalone_signature(handle);
    if (standalone_signature.get_kind() != srm::StandaloneSignatureKind::LocalVariables) throw srm::BadImageFormatError(0, "Expected LocalVariables signature");
    std::vector<const IType*> types = standalone_signature.decode_local_signature(type_provider_, generic_context);
    for (const IType*& t : types) t = introduce_tuple_types(t);
    return types;
}

// ---- module and assembly attributes --------------------------------------------------------------------------------------

std::vector<const IAttribute*> MetadataModule::get_assembly_attributes() const {
    std::call_once(assembly_attributes_once_, [this] {
        AttributeListBuilder b(*this);
        if (metadata_->is_assembly()) {
            const srm::AssemblyDefinition assembly = metadata_->get_assembly_definition();
            b.add(assembly.get_custom_attributes(), SymbolKind::Module);
            b.add_security_attributes(assembly.get_declarative_security_attributes());
            b.add(KnownAttribute::AssemblyVersion, KnownTypeCode::String, assembly.version().to_string());
            add_type_forwarder_attributes(b);
        }
        assembly_attributes_ = b.build();
    });
    return assembly_attributes_.attributes;
}

std::vector<const IAttribute*> MetadataModule::get_module_attributes() const {
    std::call_once(module_attributes_once_, [this] {
        AttributeListBuilder b(*this);
        b.add(metadata_->get_module_definition().get_custom_attributes(), SymbolKind::Module);
        if (!metadata_->is_assembly()) add_type_forwarder_attributes(b);
        module_attributes_ = b.build();
    });
    return module_attributes_.attributes;
}

void MetadataModule::add_type_forwarder_attributes(AttributeListBuilder& b) const {
    for (const srm::ExportedTypeHandle t : metadata_->exported_types()) {
        const srm::ExportedType type = metadata_->get_exported_type(t);
        if (type.is_forwarder()) b.add(KnownAttribute::TypeForwardedTo, KnownTypeCode::Type, resolve_forwarded_type(type));
    }
}

const IType* MetadataModule::resolve_forwarded_type(const srm::ExportedType& forwarder) const {
    const IModule* module = resolve_module_of(forwarder);
    const FullTypeName type_name = get_full_type_name(forwarder, *metadata_);
    if (module == nullptr) return make_unknown_type(compilation_->arena(), type_name);
    {
        const BusyLock busy_lock(this);
        if (busy_lock.success())
            if (const ITypeDefinition* td = typesystem::get_type_definition(*module, type_name)) return td;
    }
    return make_unknown_type(compilation_->arena(), type_name);
}

const IModule* MetadataModule::resolve_module_of(const srm::ExportedType& type) const {
    const srm::EntityHandle implementation = type.implementation();
    switch (implementation.kind()) {
    case srm::HandleKind::AssemblyFile: return this;   // (upstream's TODO: resolve the assembly file's module)
    case srm::HandleKind::ExportedType: return resolve_module_of(metadata_->get_exported_type(srm::ExportedTypeHandle(implementation)));
    case srm::HandleKind::AssemblyReference: {
        const srm::AssemblyReference asm_ref = metadata_->get_assembly_reference(srm::AssemblyReferenceHandle(implementation));
        const std::string_view short_name = metadata_->get_string(asm_ref.name());
        for (const IModule* asm_ : compilation_->modules())
            if (iequals_ascii(asm_->assembly_name(), short_name)) return asm_;
        return nullptr;
    }
    default: throw srm::BadImageFormatError(0, "Expected implementation to be either an AssemblyFile, ExportedType or AssemblyReference.");
    }
}

// ---- attribute helpers, visibility, nullability --------------------------------------------------------------------------

const IType* MetadataModule::get_attribute_type(KnownAttribute attr) const {
    std::atomic<const IType*>& slot = known_attribute_types_[static_cast<std::size_t>(attr)];
    if (const IType* ty = slot.load(std::memory_order_acquire)) return ty;
    const IType* ty = find_type(*compilation_, FullTypeName(get_type_name(attr)));
    const IType* expected = nullptr;
    return slot.compare_exchange_strong(expected, ty) ? ty : expected;
}

const IAttribute* MetadataModule::make_attribute(KnownAttribute type) const {
    std::atomic<const IAttribute*>& slot = known_attributes_[static_cast<std::size_t>(type)];
    if (const IAttribute* attr = slot.load(std::memory_order_acquire)) return attr;
    auto made = std::make_unique<DefaultAttribute>(get_attribute_type(type), std::vector<AttributeTypedArgument>{}, std::vector<AttributeNamedArgument>{});
    const IAttribute* expected = nullptr;
    if (slot.compare_exchange_strong(expected, made.get())) return keep(std::move(made));
    return expected;
}

bool MetadataModule::is_visible_field(std::uint16_t field_attributes) const {
    const int att = field_attributes & 0x7;   // FieldAccessMask
    return include_internal_members() || att == 6 || att == 4 || att == 5;   // Public, Family, FamORAssem
}

bool MetadataModule::is_visible_method(std::uint16_t method_attributes) const {
    const int att = method_attributes & 0x7;   // MemberAccessMask
    return include_internal_members() || att == 6 || att == 4 || att == 5;
}

bool MetadataModule::should_decode_nullable_attributes(const IEntity* entity) const {
    if (!any(options_, TypeSystemOptions::NullabilityAnnotations)) return false;
    if (min_accessibility_for_nrt_ == Accessibility::None || entity == nullptr) return true;
    return less_than_or_equal(min_accessibility_for_nrt_, effective_accessibility(*entity));
}

TypeSystemOptions MetadataModule::options_for_entity(const IEntity* entity) const {
    TypeSystemOptions opt = options_;
    if (any(opt, TypeSystemOptions::NullabilityAnnotations) && !should_decode_nullable_attributes(entity)) opt = opt & ~TypeSystemOptions::NullabilityAnnotations;
    return opt;
}

// ---- MetadataNamespace ---------------------------------------------------------------------------------------------------

MetadataNamespace::MetadataNamespace(const MetadataModule& module, const INamespace* parent, std::string full_name, srm::NamespaceDefinitionHandle ns)
    : module_(&module), ns_(ns), parent_(parent), full_name_(std::move(full_name)),
      name_(module.metadata().get_namespace_definition(ns).name()) {}

std::vector<const INamespace*> MetadataNamespace::child_namespaces() const {
    std::call_once(children_once_, [this] {
        const srm::MetadataReader& md = module_->metadata();
        for (const srm::NamespaceDefinitionHandle ns_handle : md.get_namespace_definition(ns_).namespace_definitions()) {
            std::string full_name(md.get_namespace_definition(ns_handle).full_name());
            owned_children_.push_back(std::make_unique<MetadataNamespace>(*module_, this, std::move(full_name), ns_handle));
            children_.push_back(owned_children_.back().get());
        }
    });
    return children_;
}

std::vector<const ITypeDefinition*> MetadataNamespace::types() const {
    std::vector<const ITypeDefinition*> result;
    for (const srm::TypeDefinitionHandle h : module_->metadata().get_namespace_definition(ns_).type_definitions())
        if (const ITypeDefinition* def = module_->get_definition(h)) result.push_back(def);
    return result;
}

const INamespace* MetadataNamespace::get_child_namespace(std::string_view name) const {
    for (const INamespace* ns : child_namespaces())
        if (ns->name() == name) return ns;
    return nullptr;
}

const ITypeDefinition* MetadataNamespace::get_type_definition(std::string_view name, int type_parameter_count) const {
    return module_->get_type_definition(full_name_, name, type_parameter_count);
}

// ---- MetadataFile.WithOptions --------------------------------------------------------------------------------------------

namespace {

class MetadataFileWithOptions final : public IModuleReference {
public:
    MetadataFileWithOptions(const metadata::MetadataFile& file, TypeSystemOptions options) : file_(&file), options_(options) {}
    const IModule* resolve(const ITypeResolveContext& context) const override {
        return context.compilation().arena().keep(std::make_unique<MetadataModule>(context.compilation(), *file_, options_));
    }

private:
    const metadata::MetadataFile* file_;
    TypeSystemOptions options_;
};

}  // namespace

std::unique_ptr<IModuleReference> module_reference_with_options(const metadata::MetadataFile& file, TypeSystemOptions options) {
    return std::make_unique<MetadataFileWithOptions>(file, options);
}

}  // namespace cb::dotnet::decompiler::typesystem
