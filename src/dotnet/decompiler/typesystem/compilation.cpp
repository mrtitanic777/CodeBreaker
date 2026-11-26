// dotnet/decompiler/typesystem/compilation.cpp -- the compilation, its known-type cache, merged namespaces and the minimal corlib.
#include "dotnet/decompiler/typesystem/compilation.hpp"

#include <algorithm>
#include <unordered_map>

#include "core/error.hpp"
#include "dotnet/decompiler/typesystem/type_system_extensions.hpp"

namespace cb::dotnet::decompiler::typesystem {
namespace {

int identity_hash(const void* p) {
    const auto v = reinterpret_cast<std::uintptr_t>(p);
    return static_cast<int>(static_cast<std::uint32_t>(v ^ (v >> 32)));
}

// KnownTypeCode's names, as .NET's enum ToString writes them.
const char* known_type_code_name(KnownTypeCode code) {
    static constexpr const char* kNames[kKnownTypeCodeCount] = {
        "None", "Object", "DBNull", "Boolean", "Char", "SByte", "Byte", "Int16", "UInt16", "Int32", "UInt32", "Int64", "UInt64", "Single",
        "Double", "Decimal", "DateTime", "17", "String", "Void", "Type", "Array", "Attribute", "ValueType", "Enum", "Delegate",
        "MulticastDelegate", "Exception", "IntPtr", "UIntPtr", "IEnumerable", "IEnumerator", "IEnumerableOfT", "IEnumeratorOfT",
        "ICollection", "ICollectionOfT", "IList", "IListOfT", "IReadOnlyCollectionOfT", "IReadOnlyListOfT", "Task", "TaskOfT", "ValueTask",
        "ValueTaskOfT", "NullableOfT", "IDisposable", "IAsyncDisposable", "INotifyCompletion", "ICriticalNotifyCompletion", "TypedReference",
        "IFormattable", "FormattableString", "DefaultInterpolatedStringHandler", "SpanOfT", "ReadOnlySpanOfT", "MemoryOfT", "Unsafe",
        "IAsyncEnumerableOfT", "IAsyncEnumeratorOfT", "Index", "Range",
    };
    return kNames[static_cast<int>(code)];
}

}  // namespace

// ---- SimpleTypeResolveContext --------------------------------------------------------------------------------------------

SimpleTypeResolveContext::SimpleTypeResolveContext(const IEntity& entity)
    : compilation_(&entity.compilation()), current_module_(entity.parent_module()) {
    const auto* definition = dynamic_cast<const ITypeDefinition*>(&entity);
    current_type_definition_ = definition != nullptr ? definition : entity.declaring_type_definition();
    current_member_ = dynamic_cast<const IMember*>(&entity);
}

SimpleTypeResolveContext SimpleTypeResolveContext::with_current_type_definition(const ITypeDefinition* type_definition) const {
    SimpleTypeResolveContext c = *this;
    c.current_type_definition_ = type_definition;
    return c;
}

SimpleTypeResolveContext SimpleTypeResolveContext::with_current_member(const IMember* member) const {
    SimpleTypeResolveContext c = *this;
    c.current_member_ = member;
    return c;
}

// ---- KnownTypeCache ------------------------------------------------------------------------------------------------------

const IType* KnownTypeCache::find_type(KnownTypeCode type_code) const {
    std::atomic<const IType*>& slot = known_types_[static_cast<std::size_t>(type_code)];
    if (const IType* type = slot.load(std::memory_order_acquire)) return type;
    const IType* found = search_type(type_code);
    const IType* expected = nullptr;
    return slot.compare_exchange_strong(expected, found, std::memory_order_acq_rel) ? found : expected;
}

const IType* KnownTypeCache::search_type(KnownTypeCode type_code) const {
    const KnownTypeReference* type_ref = KnownTypeReference::get(type_code);
    if (type_ref == nullptr) return &SpecialType::unknown_type();
    const TopLevelTypeName type_name(type_ref->namespace_(), type_ref->name(), type_ref->type_parameter_count());
    for (const IModule* asm_ : compilation_.modules())
        if (const ITypeDefinition* def = asm_->get_type_definition(type_name)) return def;
    return make_unknown_type(compilation_.arena(), FullTypeName(type_name));
}

// ---- SimpleCompilation ---------------------------------------------------------------------------------------------------

SimpleCompilation::SimpleCompilation() = default;
SimpleCompilation::~SimpleCompilation() = default;

SimpleCompilation::SimpleCompilation(const IModuleReference& main_assembly, const std::vector<const IModuleReference*>& assembly_references) {
    init(main_assembly, assembly_references);
}

void SimpleCompilation::init(const IModuleReference& main_assembly, const std::vector<const IModuleReference*>& assembly_references) {
    const SimpleTypeResolveContext context(*this);
    main_module_ = main_assembly.resolve(context);
    if (main_module_ == nullptr) throw UsageError("SimpleCompilation: the main module reference does not resolve");
    assemblies_.push_back(main_module_);
    for (const IModuleReference* reference : assembly_references) {
        if (reference == nullptr) throw UsageError("SimpleCompilation: a null assembly reference");
        const IModule* asm_ = reference->resolve(context);
        if (asm_ != nullptr && std::find(assemblies_.begin(), assemblies_.end(), asm_) == assemblies_.end()) assemblies_.push_back(asm_);
        if (asm_ != nullptr && std::find(referenced_assemblies_.begin(), referenced_assemblies_.end(), asm_) == referenced_assemblies_.end())
            referenced_assemblies_.push_back(asm_);
    }
    known_type_cache_ = std::make_unique<KnownTypeCache>(*this);
    initialized_ = true;
}

const IModule& SimpleCompilation::main_module() const {
    if (!initialized_) throw UsageError("Compilation isn't initialized yet");
    return *main_module_;
}

const std::vector<const IModule*>& SimpleCompilation::modules() const {
    if (!initialized_) throw UsageError("Compilation isn't initialized yet");
    return assemblies_;
}

const std::vector<const IModule*>& SimpleCompilation::referenced_modules() const {
    if (!initialized_) throw UsageError("Compilation isn't initialized yet");
    return referenced_assemblies_;
}

const INamespace& SimpleCompilation::root_namespace() const {
    if (!initialized_) throw UsageError("Compilation isn't initialized yet");
    std::call_once(root_once_, [this] { root_namespace_ = create_root_namespace(); });
    return *root_namespace_;
}

const INamespace* SimpleCompilation::create_root_namespace() const {
    std::vector<const INamespace*> namespaces;
    namespaces.reserve(referenced_assemblies_.size() + 1);
    namespaces.push_back(&main_module_->root_namespace());
    for (const IModule* m : referenced_assemblies_) namespaces.push_back(&m->root_namespace());
    return arena_.keep(std::make_unique<MergedNamespace>(*this, std::move(namespaces)));
}

const INamespace* SimpleCompilation::get_namespace_for_extern_alias(std::string_view alias) const {
    if (alias.empty()) return &root_namespace();
    return nullptr;
}

const IType* SimpleCompilation::find_type(KnownTypeCode type_code) const {
    if (!initialized_) throw UsageError("Compilation isn't initialized yet");
    return known_type_cache_->find_type(type_code);
}

std::string SimpleCompilation::to_string() const { return "[SimpleCompilation " + main_module_->assembly_name() + "]"; }

// ---- MergedNamespace -----------------------------------------------------------------------------------------------------

MergedNamespace::MergedNamespace(const ICompilation& compilation, std::vector<const INamespace*> namespaces, std::string extern_alias)
    : extern_alias_(std::move(extern_alias)), compilation_(&compilation), namespaces_(std::move(namespaces)) {
    if (namespaces_.empty()) throw UsageError("MergedNamespace: no namespaces");
}

MergedNamespace::MergedNamespace(const INamespace& parent_namespace, std::vector<const INamespace*> namespaces)
    : extern_alias_(parent_namespace.extern_alias()), compilation_(&parent_namespace.compilation()), parent_namespace_(&parent_namespace),
      namespaces_(std::move(namespaces)) {
    if (namespaces_.empty()) throw UsageError("MergedNamespace: no namespaces");
}

std::vector<const ITypeDefinition*> MergedNamespace::types() const {
    std::vector<const ITypeDefinition*> result;
    for (const INamespace* ns : namespaces_) {
        std::vector<const ITypeDefinition*> t = ns->types();
        result.insert(result.end(), t.begin(), t.end());
    }
    return result;
}

std::vector<const IModule*> MergedNamespace::contributing_modules() const {
    std::vector<const IModule*> result;
    for (const INamespace* ns : namespaces_) {
        std::vector<const IModule*> m = ns->contributing_modules();
        result.insert(result.end(), m.begin(), m.end());
    }
    return result;
}

const MergedNamespace::Children& MergedNamespace::get_child_namespaces() const {
    std::call_once(children_once_, [this] {
        // GroupBy keeps the order in which each name is first seen, and each group's members in order.
        std::vector<std::string> names;
        std::vector<std::vector<const INamespace*>> groups;
        std::unordered_map<std::string, std::size_t> index;
        for (const INamespace* ns : namespaces_) {
            for (const INamespace* child : ns->child_namespaces()) {
                std::string n = child->name();
                const auto [it, added] = index.try_emplace(n, groups.size());
                if (added) {
                    names.push_back(std::move(n));
                    groups.emplace_back();
                }
                groups[it->second].push_back(child);
            }
        }
        for (std::size_t i = 0; i < groups.size(); ++i) {
            children_.names.push_back(names[i]);
            children_.namespaces.push_back(compilation_->arena().keep(std::make_unique<MergedNamespace>(*this, std::move(groups[i]))));
        }
    });
    return children_;
}

std::vector<const INamespace*> MergedNamespace::child_namespaces() const { return get_child_namespaces().namespaces; }

const INamespace* MergedNamespace::get_child_namespace(std::string_view name) const {
    const Children& c = get_child_namespaces();
    for (std::size_t i = 0; i < c.names.size(); ++i)
        if (c.names[i] == name) return c.namespaces[i];
    return nullptr;
}

const ITypeDefinition* MergedNamespace::get_type_definition(std::string_view name, int type_parameter_count) const {
    const ITypeDefinition* any_type_def = nullptr;
    for (const INamespace* ns : namespaces_) {
        if (const ITypeDefinition* type_def = ns->get_type_definition(name, type_parameter_count)) {
            if (type_def->accessibility() == Accessibility::Public) return type_def;   // prefer accessible types
            any_type_def = type_def;
        }
    }
    return any_type_def;
}

std::string MergedNamespace::to_string() const {
    return "[MergedNamespace " + (extern_alias_.empty() ? std::string() : extern_alias_ + "::") + full_name() + " (from " +
           std::to_string(namespaces_.size()) + " assemblies)]";
}

// ---- MinimalCorlib -------------------------------------------------------------------------------------------------------

namespace {

class MinimalCorlib;

class CorlibTypeDefinition final : public ITypeDefinition {
public:
    CorlibTypeDefinition(const MinimalCorlib& corlib, KnownTypeCode type_code);

    // ITypeDefinition
    const std::vector<const ITypeDefinition*>& nested_types() const override {
        static const std::vector<const ITypeDefinition*> none;
        return none;
    }
    const std::vector<const IMember*>& members() const override {
        static const std::vector<const IMember*> none;
        return none;
    }
    std::vector<const IField*> fields() const override { return {}; }
    std::vector<const IMethod*> methods() const override { return {}; }
    std::vector<const IProperty*> properties() const override { return {}; }
    std::vector<const IEvent*> events() const override { return {}; }
    KnownTypeCode known_type_code() const override { return type_code_; }
    const IType* enum_underlying_type() const override { return &SpecialType::unknown_type(); }
    bool is_read_only() const override { return false; }
    std::string metadata_name() const override { return metadata_name_; }
    bool has_extensions() const override { return false; }
    const ExtensionInfo* extension_info() const override { return nullptr; }
    Nullability nullable_context() const override { return Nullability::Oblivious; }
    bool is_record() const override { return false; }
    FullTypeName full_type_name() const override { return FullTypeName(ref().type_name()); }

    // IEntity
    srm::EntityHandle metadata_token() const override { return srm::TypeDefinitionHandle(0); }
    const ITypeDefinition* declaring_type_definition() const override { return nullptr; }
    const IType* declaring_type() const override { return nullptr; }
    const IModule* parent_module() const override;
    std::vector<const IAttribute*> get_attributes() const override { return {}; }
    bool has_attribute(KnownAttribute) const override { return false; }
    const IAttribute* get_attribute(KnownAttribute) const override { return nullptr; }
    Accessibility accessibility() const override { return Accessibility::Public; }
    bool is_static() const override { return false; }
    bool is_abstract() const override { return type_kind_ == TypeKind::Interface; }
    bool is_sealed() const override { return type_kind_ == TypeKind::Struct; }
    SymbolKind symbol_kind() const override { return SymbolKind::TypeDefinition; }
    const ICompilation& compilation() const override;

    // INamedElement
    std::string name() const override { return ref().name(); }
    std::string full_name() const override { return ref().namespace_() + "." + ref().name(); }
    std::string reflection_name() const override { return FullTypeName(ref().type_name()).reflection_name(); }
    std::string namespace_() const override { return ref().namespace_(); }

    // IType
    TypeKind kind() const override { return type_kind_; }
    std::optional<bool> is_reference_type() const override {
        switch (type_kind_) {
        case TypeKind::Class:
        case TypeKind::Interface: return true;
        case TypeKind::Struct:
        case TypeKind::Enum: return false;
        default: return std::nullopt;
        }
    }
    bool is_by_ref_like() const override { return false; }
    Nullability nullability() const override { return Nullability::Oblivious; }
    const IType* change_nullability(Nullability nullability) const override {
        return nullability == Nullability::Oblivious ? this : make_nullability_annotated_type(this, nullability);
    }
    const ITypeDefinition* get_definition() const override { return this; }
    const ITypeDefinitionOrUnknown* get_definition_or_unknown() const override { return this; }
    int type_parameter_count() const override { return ref().type_parameter_count(); }
    const std::vector<const ITypeParameter*>& type_parameters() const override {
        return DummyTypeParameter::get_class_type_parameter_list(ref().type_parameter_count());
    }
    const std::vector<const IType*>& type_arguments() const override { return type_arguments_; }
    const IType* accept_visitor(TypeVisitor& visitor) const override { return visitor.visit_type_definition(*this); }
    const IType* visit_children(TypeVisitor&) const override { return this; }
    std::vector<const IType*> direct_base_types() const override;
    TypeParameterSubstitution get_substitution() const override { return TypeParameterSubstitution::identity(); }
    std::vector<const IType*> get_nested_types(const Filter<ITypeDefinition>& = {}, GetMemberOptions = GetMemberOptions::None) const override { return {}; }
    std::vector<const IType*> get_nested_types(const std::vector<const IType*>&, const Filter<ITypeDefinition>& = {},
                                               GetMemberOptions = GetMemberOptions::None) const override {
        return {};
    }
    std::vector<const IMethod*> get_constructors(const Filter<IMethod>& = {}, GetMemberOptions = GetMemberOptions::IgnoreInheritedMembers) const override {
        return {};
    }
    std::vector<const IMethod*> get_methods(const Filter<IMethod>& = {}, GetMemberOptions = GetMemberOptions::None) const override { return {}; }
    std::vector<const IMethod*> get_methods(const std::vector<const IType*>&, const Filter<IMethod>& = {}, GetMemberOptions = GetMemberOptions::None) const override {
        return {};
    }
    std::vector<const IProperty*> get_properties(const Filter<IProperty>& = {}, GetMemberOptions = GetMemberOptions::None) const override { return {}; }
    std::vector<const IField*> get_fields(const Filter<IField>& = {}, GetMemberOptions = GetMemberOptions::None) const override { return {}; }
    std::vector<const IEvent*> get_events(const Filter<IEvent>& = {}, GetMemberOptions = GetMemberOptions::None) const override { return {}; }
    std::vector<const IMember*> get_members(const Filter<IMember>& = {}, GetMemberOptions = GetMemberOptions::None) const override { return {}; }
    std::vector<const IMethod*> get_accessors(const Filter<IMethod>& = {}, GetMemberOptions = GetMemberOptions::None) const override { return {}; }
    bool equals(const IType* other) const override { return this == other; }
    int hash_code() const override { return identity_hash(this); }
    std::string to_string() const override { return std::string("[MinimalCorlibType ") + known_type_code_name(type_code_) + "]"; }

private:
    const KnownTypeReference& ref() const { return *KnownTypeReference::get(type_code_); }

    const MinimalCorlib* corlib_;
    KnownTypeCode type_code_;
    TypeKind type_kind_;
    std::string metadata_name_;
    std::vector<const IType*> type_arguments_;
};

class CorlibNamespace final : public INamespace {
public:
    CorlibNamespace(const MinimalCorlib& corlib, const INamespace* parent_namespace, std::string full_name, std::string name)
        : corlib_(&corlib), parent_namespace_(parent_namespace), full_name_(std::move(full_name)), name_(std::move(name)) {}

    std::string extern_alias() const override { return {}; }
    std::string full_name() const override { return full_name_; }
    std::string name() const override { return name_; }
    const INamespace* parent_namespace() const override { return parent_namespace_; }
    std::vector<const INamespace*> child_namespaces() const override { return {}; }   // (upstream never fills them)
    std::vector<const ITypeDefinition*> types() const override;
    std::vector<const IModule*> contributing_modules() const override;
    SymbolKind symbol_kind() const override { return SymbolKind::Namespace; }
    const ICompilation& compilation() const override;
    const INamespace* get_child_namespace(std::string_view) const override { return nullptr; }
    const ITypeDefinition* get_type_definition(std::string_view name, int type_parameter_count) const override;

private:
    const MinimalCorlib* corlib_;
    const INamespace* parent_namespace_;
    std::string full_name_, name_;
};

class MinimalCorlib final : public IModule {
public:
    MinimalCorlib(const ICompilation& compilation, const std::vector<const KnownTypeReference*>& types)
        : compilation_(&compilation), root_namespace_(*this, nullptr, {}, {}) {
        type_definitions_.reserve(types.size());
        for (const KnownTypeReference* ktr : types) type_definitions_.push_back(std::make_unique<CorlibTypeDefinition>(*this, ktr->known_type_code()));
    }

    const ICompilation& compilation() const override { return *compilation_; }
    bool is_main_module() const override { return &compilation_->main_module() == this; }
    std::string assembly_name() const override { return "corlib"; }
    srm::Version assembly_version() const override { return srm::Version{0, 0, 0, 0}; }
    std::string full_assembly_name() const override { return "corlib"; }
    std::string name() const override { return "corlib"; }
    SymbolKind symbol_kind() const override { return SymbolKind::Module; }
    const metadata::MetadataFile* metadata_file() const override { return nullptr; }
    const INamespace& root_namespace() const override { return root_namespace_; }
    std::vector<const ITypeDefinition*> top_level_type_definitions() const override {
        std::vector<const ITypeDefinition*> result;
        result.reserve(type_definitions_.size());
        for (const auto& t : type_definitions_) result.push_back(t.get());
        return result;
    }
    std::vector<const ITypeDefinition*> type_definitions() const override { return top_level_type_definitions(); }
    const ITypeDefinition* get_type_definition(const TopLevelTypeName& top_level_type_name) const override {
        for (const auto& t : type_definitions_)
            if (t->full_type_name() == FullTypeName(top_level_type_name)) return t.get();
        return nullptr;
    }
    std::vector<const IAttribute*> get_assembly_attributes() const override { return {}; }
    std::vector<const IAttribute*> get_module_attributes() const override { return {}; }
    bool internals_visible_to(const IModule& module) const override { return &module == this; }

private:
    const ICompilation* compilation_;
    std::vector<std::unique_ptr<CorlibTypeDefinition>> type_definitions_;
    CorlibNamespace root_namespace_;
};

CorlibTypeDefinition::CorlibTypeDefinition(const MinimalCorlib& corlib, KnownTypeCode type_code)
    : corlib_(&corlib), type_code_(type_code), type_kind_(KnownTypeReference::get(type_code)->type_kind()) {
    const KnownTypeReference& ktr = ref();
    metadata_name_ = ktr.name() + (ktr.type_parameter_count() > 0 ? "`" + std::to_string(ktr.type_parameter_count()) : std::string());
    for (const ITypeParameter* p : DummyTypeParameter::get_class_type_parameter_list(ktr.type_parameter_count())) type_arguments_.push_back(p);
}

const IModule* CorlibTypeDefinition::parent_module() const { return corlib_; }
const ICompilation& CorlibTypeDefinition::compilation() const { return corlib_->compilation(); }

std::vector<const IType*> CorlibTypeDefinition::direct_base_types() const {
    const KnownTypeCode base_type = ref().base_type();
    if (base_type != KnownTypeCode::None) return {corlib_->compilation().find_type(base_type)};
    return {};
}

std::vector<const ITypeDefinition*> CorlibNamespace::types() const {
    std::vector<const ITypeDefinition*> result;
    for (const ITypeDefinition* td : corlib_->top_level_type_definitions())
        if (td->namespace_() == full_name_) result.push_back(td);
    return result;
}

std::vector<const IModule*> CorlibNamespace::contributing_modules() const { return {corlib_}; }
const ICompilation& CorlibNamespace::compilation() const { return corlib_->compilation(); }

const ITypeDefinition* CorlibNamespace::get_type_definition(std::string_view name, int type_parameter_count) const {
    return typesystem::get_type_definition(*corlib_, full_name_, std::string(name), type_parameter_count);
}

class CorlibModuleReference final : public IModuleReference {
public:
    explicit CorlibModuleReference(std::vector<const KnownTypeReference*> types) : types_(std::move(types)) {}
    const IModule* resolve(const ITypeResolveContext& context) const override {
        return context.compilation().arena().keep(std::make_unique<MinimalCorlib>(context.compilation(), types_));
    }

private:
    std::vector<const KnownTypeReference*> types_;
};

std::vector<const KnownTypeReference*> all_known_types() {
    std::vector<const KnownTypeReference*> result;
    for (const KnownTypeReference* k : KnownTypeReference::all())
        if (k != nullptr) result.push_back(k);
    return result;
}

}  // namespace

namespace minimal_corlib {

const IModuleReference& instance() {
    static const CorlibModuleReference reference(all_known_types());
    return reference;
}

std::unique_ptr<IModuleReference> create_with_types(std::vector<const KnownTypeReference*> types) {
    return std::make_unique<CorlibModuleReference>(std::move(types));
}

}  // namespace minimal_corlib

}  // namespace cb::dotnet::decompiler::typesystem
