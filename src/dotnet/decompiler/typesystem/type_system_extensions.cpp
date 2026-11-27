// dotnet/decompiler/typesystem/type_system_extensions.cpp -- TypeSystemExtensions and BaseTypeCollector.
#include "dotnet/decompiler/typesystem/type_system_extensions.hpp"

#include "dotnet/metadata/type_name.hpp"

#include <algorithm>
#include <stdexcept>
#include <variant>
#include <utility>
#include <cctype>

#include "dotnet/decompiler/metadata/assembly_references.hpp"
#include "dotnet/decompiler/typesystem/attributes.hpp"

namespace cb::dotnet::decompiler::typesystem {
namespace {

// BaseTypeCollector: base types before the types that derive from them, each once, finite despite cyclic inheritance.
class BaseTypeCollector {
public:
    explicit BaseTypeCollector(bool skip_implemented_interfaces) : skip_interfaces_(skip_implemented_interfaces) {}

    void collect(const IType* type) {
        const IType* def = type->get_definition() != nullptr ? static_cast<const IType*>(type->get_definition()) : type;
        // a stack of the definitions being collected, each on it once (class C<X> : C<C<X>> {} would otherwise never end; also type
        // parameters, T : S where S : T)
        for (const IType* a : active_)
            if (a->equals(def)) return;
        active_.push_back(def);
        // each type once (diamond inheritance), after all its base types
        bool contained = false;
        for (const IType* r : result) contained = contained || r->equals(type);
        if (!contained) {
            for (const IType* base : type->direct_base_types()) {
                if (skip_interfaces_ && def != nullptr && def->kind() != TypeKind::Interface && def->kind() != TypeKind::TypeParameter &&
                    base->kind() == TypeKind::Interface)
                    continue;   // skip the interface
                collect(base);
            }
            result.push_back(type);
        }
        active_.pop_back();
    }

    std::vector<const IType*> result;

private:
    bool skip_interfaces_;
    std::vector<const IType*> active_;
};

class TypeClassificationVisitor final : public TypeVisitor {
public:
    bool is_open = false;
    const IEntity* type_parameter_owner = nullptr;

    const IType* visit_type_parameter(const ITypeParameter& type) override {
        is_open = true;
        // with classes and methods, or nested classes, the most specific owner
        const int level = nesting_level(type.owner());
        if (level > owner_level_) {
            type_parameter_owner = type.owner();
            owner_level_ = level;
        }
        return TypeVisitor::visit_type_parameter(type);
    }

private:
    static int nesting_level(const IEntity* entity) {
        int level = 0;
        while (entity != nullptr) {
            ++level;
            entity = entity->declaring_type_definition();
        }
        return level;
    }
    int owner_level_ = 0;
};

bool iequals(std::string_view a, std::string_view b) {
    return a.size() == b.size() &&
           std::equal(a.begin(), a.end(), b.begin(), [](char x, char y) { return std::tolower(static_cast<unsigned char>(x)) == std::tolower(static_cast<unsigned char>(y)); });
}

}  // namespace

std::vector<const IType*> get_all_base_types(const IType* type) {
    if (type == nullptr) throw UsageError("get_all_base_types: a null type");
    BaseTypeCollector c(false);
    c.collect(type);
    return std::move(c.result);
}

std::vector<const IType*> get_non_interface_base_types(const IType* type) {
    if (type == nullptr) throw UsageError("get_non_interface_base_types: a null type");
    BaseTypeCollector c(true);
    c.collect(type);
    return std::move(c.result);
}

const IType* get_interface_as_implemented_by(const IType* interface_type, const IType* implementing_type) {
    for (const IType* base : get_all_base_types(implementing_type))
        if (base->kind() == TypeKind::Interface && NormalizeTypeVisitor::ignore_nullability_and_tuples().equivalent_types(base, interface_type)) return base;
    return interface_type;
}

std::vector<const ITypeDefinition*> get_all_base_type_definitions(const IType* type) {
    std::vector<const ITypeDefinition*> out;
    for (const IType* t : get_all_base_types(type)) {
        const ITypeDefinition* d = t->get_definition();
        if (d != nullptr && std::find(out.begin(), out.end(), d) == out.end()) out.push_back(d);   // Distinct: definitions compare by reference
    }
    return out;
}

bool is_derived_from(const ITypeDefinition* type, const ITypeDefinition* base_type) {
    if (type == nullptr) throw UsageError("is_derived_from: a null type");
    if (base_type == nullptr) return false;
    if (&type->compilation() != &base_type->compilation()) throw UsageError("Both arguments to IsDerivedFrom() must be from the same compilation.");
    const auto defs = get_all_base_type_definitions(type);
    return std::find(defs.begin(), defs.end(), base_type) != defs.end();
}

bool is_derived_from(const ITypeDefinition* type, KnownTypeCode base_type) {
    if (type == nullptr) throw UsageError("is_derived_from: a null type");
    if (base_type == KnownTypeCode::None) return false;
    return is_derived_from(type, type->compilation().find_type(base_type)->get_definition());
}

std::vector<const ITypeDefinition*> get_declaring_type_definitions(const ITypeDefinition* definition) {
    if (definition == nullptr) throw UsageError("get_declaring_type_definitions: a null definition");
    std::vector<const ITypeDefinition*> out;
    for (; definition != nullptr; definition = definition->declaring_type_definition()) out.push_back(definition);
    return out;
}

bool is_parameterized_property(const IProperty* property) { return property->symbol_kind() == SymbolKind::Property && !property->parameters().empty(); }

bool is_open(const IType* type) {
    if (type == nullptr) throw UsageError("is_open: a null type");
    TypeClassificationVisitor v;
    type->accept_visitor(v);
    return v.is_open;
}

const IEntity* get_type_parameter_owner(const IType* type) {
    if (type == nullptr) throw UsageError("get_type_parameter_owner: a null type");
    TypeClassificationVisitor v;
    type->accept_visitor(v);
    return v.type_parameter_owner;
}

bool is_unbound(const IType* type) {
    if (type == nullptr) throw UsageError("is_unbound: a null type");
    return (dynamic_cast<const ITypeDefinition*>(type) != nullptr || dynamic_cast<const UnknownType*>(type) != nullptr) && type->type_parameter_count() > 0;
}

bool is_unmanaged_type(const IType* type, bool allow_generics) {
    std::vector<const IType*> types;   // the structs being looked into (a HashSet upstream)
    std::function<bool(const IType*)> internal = [&](const IType* t) -> bool {
        switch (t->kind()) {
        case TypeKind::Enum: case TypeKind::Pointer: case TypeKind::FunctionPointer: case TypeKind::NInt: case TypeKind::NUInt: return true;
        default: break;
        }
        if (const auto* tp = dynamic_cast<const ITypeParameter*>(t)) return tp->has_unmanaged_constraint();
        const ITypeDefinition* def = t->get_definition();
        if (def == nullptr) return false;
        switch (def->known_type_code()) {
        case KnownTypeCode::Void: case KnownTypeCode::Boolean: case KnownTypeCode::Char: case KnownTypeCode::SByte: case KnownTypeCode::Byte:
        case KnownTypeCode::Int16: case KnownTypeCode::UInt16: case KnownTypeCode::Int32: case KnownTypeCode::UInt32: case KnownTypeCode::Int64:
        case KnownTypeCode::UInt64: case KnownTypeCode::Decimal: case KnownTypeCode::Single: case KnownTypeCode::Double: case KnownTypeCode::IntPtr:
        case KnownTypeCode::UIntPtr: case KnownTypeCode::TypedReference:
            return true;
        default: break;
        }
        if (t->kind() != TypeKind::Struct) return false;
        if (!allow_generics && def->type_parameter_count() > 0) return false;
        types.push_back(t);
        auto remove = [&] { types.erase(std::remove_if(types.begin(), types.end(), [&](const IType* x) { return x->equals(t); }), types.end()); };
        for (const IField* f : t->get_fields([](const IField* f) { return !f->is_static(); })) {
            const bool seen = std::any_of(types.begin(), types.end(), [&](const IType* x) { return x->equals(f->type()); });
            if (seen || !internal(f->type())) {
                remove();
                return false;
            }
        }
        remove();
        return true;
    };
    return internal(type);
}

bool is_array_interface_type(const IType* type) {
    if (type == nullptr || type->type_parameter_count() != 1) return false;
    const ITypeDefinition* def = type->get_definition();
    if (def == nullptr) return false;
    switch (def->known_type_code()) {
    case KnownTypeCode::IEnumerableOfT: case KnownTypeCode::ICollectionOfT: case KnownTypeCode::IListOfT: case KnownTypeCode::IReadOnlyCollectionOfT:
    case KnownTypeCode::IReadOnlyListOfT:
        return true;
    default: return false;
    }
}

bool is_inline_array_type(const IType* type) {
    if (type->kind() != TypeKind::Struct) return false;
    const ITypeDefinition* td = type->get_definition();
    if (td == nullptr) return false;
    return td->has_attribute(KnownAttribute::InlineArray);
}

std::optional<int> get_inline_array_length(const IType* type) {
    if (type->kind() != TypeKind::Struct) return std::nullopt;
    const ITypeDefinition* td = type->get_definition();
    if (td == nullptr) return std::nullopt;
    const IAttribute* attr = td->get_attribute(KnownAttribute::InlineArray);
    if (attr == nullptr || attr->fixed_arguments().empty()) return std::nullopt;
    if (const auto* length = std::get_if<std::int32_t>(&attr->fixed_arguments()[0].value)) return *length;
    return std::nullopt;
}

const IType* get_inline_array_element_type(const IType* array_type) {
    if (array_type != nullptr) {
        auto fields = array_type->get_fields([](const IField* f) { return !f->is_static(); });
        if (fields.size() > 1) throw std::logic_error("Sequence contains more than one element");
        if (fields.size() == 1) return fields[0]->type();
    }
    return &SpecialType::unknown_type();
}

bool is_known_type(const IType* type, KnownTypeCode known_type) {
    const ITypeDefinition* def = type->get_definition();
    return def != nullptr && def->known_type_code() == known_type;
}

const IMethod* get_delegate_invoke_method(const IType* type) {
    if (type == nullptr) throw UsageError("get_delegate_invoke_method: a null type");
    if (type->kind() != TypeKind::Delegate) return nullptr;
    const auto methods = type->get_methods([](const IMethod* m) { return m->name() == "Invoke"; }, GetMemberOptions::IgnoreInheritedMembers);
    return methods.empty() ? nullptr : methods.front();
}

const IType* skip_modifiers(const IType* type) {
    while (const auto* mt = dynamic_cast<const ModifiedType*>(type)) type = mt->element_type();
    return type;
}

const IType* unwrap_by_ref(const IType* type) {
    if (const auto* byref = dynamic_cast<const ByReferenceType*>(type)) return byref->element_type();
    return type;
}

bool has_readonly_modifier(const IMethod* accessor) {
    return accessor->this_is_ref_read_only() && accessor->declaring_type_definition() != nullptr && !accessor->declaring_type_definition()->is_read_only();
}

bool is_any_pointer(TypeKind kind) { return kind == TypeKind::Pointer || kind == TypeKind::FunctionPointer; }

std::vector<const ITypeDefinition*> get_all_type_definitions(const ICompilation& compilation) {
    std::vector<const ITypeDefinition*> out;
    for (const IModule* m : compilation.modules())
        for (const ITypeDefinition* t : m->type_definitions()) out.push_back(t);
    return out;
}

std::vector<const ITypeDefinition*> get_top_level_type_definitions(const ICompilation& compilation) {
    std::vector<const ITypeDefinition*> out;
    for (const IModule* m : compilation.modules())
        for (const ITypeDefinition* t : m->top_level_type_definitions()) out.push_back(t);
    return out;
}

const IType* find_type(const ICompilation& compilation, const FullTypeName& full_type_name) {
    for (const IModule* m : compilation.modules())
        if (const ITypeDefinition* def = get_type_definition(*m, full_type_name)) return def;
    return make_unknown_type(compilation.arena(), full_type_name);
}

const ITypeDefinition* get_type_definition(const IModule& module, const FullTypeName& full_type_name) {
    const TopLevelTypeName& top = full_type_name.top_level_type_name();
    const ITypeDefinition* def = module.get_type_definition(top);
    if (def == nullptr) return nullptr;
    int tpc = top.type_parameter_count();
    for (int i = 0; i < full_type_name.nesting_level(); ++i) {
        const std::string& name = full_type_name.get_nested_type_name(i);
        tpc += full_type_name.get_nested_type_additional_type_parameter_count(i);
        const ITypeDefinition* found = nullptr;
        for (const ITypeDefinition* nested : def->nested_types())
            if (nested->name() == name && nested->type_parameter_count() == tpc) {
                found = nested;
                break;
            }
        def = found;
        if (def == nullptr) break;
    }
    return def;
}

const ITypeDefinition* get_type_definition(const IModule& module, std::string namespace_name, std::string name, int type_parameter_count) {
    return module.get_type_definition(TopLevelTypeName(std::move(namespace_name), std::move(name), type_parameter_count));
}

const IType* get_element_type_from_ienumerable(const IType* collection_type, const ICompilation& compilation, bool allow_ienumerator,
                                               std::optional<bool>& is_generic) {
    bool found_non_generic = false;
    for (const IType* base : get_all_base_types(collection_type)) {
        const ITypeDefinition* def = base->get_definition();
        if (def == nullptr) continue;
        const KnownTypeCode code = def->known_type_code();
        if (code == KnownTypeCode::IEnumerableOfT || (allow_ienumerator && code == KnownTypeCode::IEnumeratorOfT)) {
            if (const auto* pt = dynamic_cast<const ParameterizedType*>(base)) {
                is_generic = true;
                return pt->get_type_argument(0);
            }
        }
        if (code == KnownTypeCode::IEnumerable || (allow_ienumerator && code == KnownTypeCode::IEnumerator)) found_non_generic = true;
    }
    if (found_non_generic) {   // System.Collections.IEnumerable in the hierarchy: the element type is object
        is_generic = false;
        return compilation.find_type(KnownTypeCode::Object);
    }
    is_generic.reset();
    return &SpecialType::unknown_type();
}

bool full_name_is(const IMember* member, std::string_view type, std::string_view name) {
    return member->name() == name && member->declaring_type() != nullptr && member->declaring_type()->full_name() == type;
}

const IType* without_nullability(const IType* type) { return type->change_nullability(Nullability::Oblivious); }

const IType* as_parameterized_type(const ITypeDefinition* definition) {
    if (definition->type_parameter_count() == 0) return definition;
    return make_parameterized_type(definition, definition->type_arguments());
}

const INamespace* get_namespace_by_full_name(const ICompilation& compilation, std::string_view name) {
    const INamespace* ns = &compilation.root_namespace();
    if (name.empty()) return ns;
    std::size_t start = 0;
    while (true) {
        const std::size_t dot = name.find('.', start);
        const INamespace* child = ns->get_child_namespace(name.substr(start, dot == std::string_view::npos ? std::string_view::npos : dot - start));
        if (child == nullptr) return nullptr;
        ns = child;
        if (dot == std::string_view::npos) return ns;
        start = dot + 1;
    }
}

const IModule* find_module_by_reference(const ICompilation& compilation, const metadata::IAssemblyReference& assembly_name) {
    for (const IModule* m : compilation.modules())
        if (iequals(m->full_assembly_name(), assembly_name.full_name())) return m;
    for (const IModule* m : compilation.modules())
        if (iequals(m->name(), assembly_name.name())) return m;
    return nullptr;
}

const IModule* find_module_by_assembly_name_info(const ICompilation& compilation, const srm::AssemblyNameInfo& assembly_name) {
    for (const IModule* m : compilation.modules())
        if (iequals(m->full_assembly_name(), assembly_name.full_name())) return m;
    for (const IModule* m : compilation.modules())
        if (iequals(m->name(), assembly_name.name())) return m;
    return nullptr;
}

const IType* make_unknown_type(TypeArena& arena, const FullTypeName& full_type_name, std::optional<bool> is_reference_type) {
    TypeKey key{40, {}, {is_reference_type ? (*is_reference_type ? 1 : 0) : -1}, {full_type_name.reflection_name()}};
    return arena.intern<IType>(std::move(key), [&]() -> std::unique_ptr<IType> { return std::make_unique<UnknownType>(full_type_name, is_reference_type); });
}

const IType* make_unknown_type(TypeArena& arena, std::optional<std::string> namespace_name, std::string name, int type_parameter_count,
                               std::optional<bool> is_reference_type) {
    TypeKey key{41, {}, {is_reference_type ? (*is_reference_type ? 1 : 0) : -1, type_parameter_count, namespace_name ? 1 : 0},
                {namespace_name.value_or(""), name}};
    return arena.intern<IType>(std::move(key), [&]() -> std::unique_ptr<IType> {
        return std::make_unique<UnknownType>(namespace_name, name, type_parameter_count, is_reference_type);
    });
}

// ---- AccessibilityExtensions (declared in type_system.hpp) ----------------------------------------------------------------
// The enum's order is the partial order "visible to a subset of", except that protected and internal are not comparable.

bool less_than_or_equal(Accessibility a, Accessibility b) { return a <= b && !(a == Accessibility::Protected && b == Accessibility::Internal); }

Accessibility intersect(Accessibility a, Accessibility b) {
    if (a > b) std::swap(a, b);
    if (a == Accessibility::Protected && b == Accessibility::Internal) return Accessibility::ProtectedAndInternal;
    return a;
}

Accessibility union_of(Accessibility a, Accessibility b) {
    if (a > b) std::swap(a, b);
    if (a == Accessibility::Protected && b == Accessibility::Internal) return Accessibility::ProtectedOrInternal;
    return b;
}

Accessibility effective_accessibility(const IEntity& entity) {
    Accessibility accessibility = entity.accessibility();
    for (const ITypeDefinition* type_def = entity.declaring_type_definition(); type_def != nullptr; type_def = type_def->declaring_type_definition())
        accessibility = intersect(accessibility, type_def->accessibility());
    return accessibility;
}

}  // namespace cb::dotnet::decompiler::typesystem
