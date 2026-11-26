// dotnet/decompiler/typesystem/types.cpp -- the types of types.hpp, the arena and the factories.
#include "dotnet/decompiler/typesystem/types.hpp"

#include <deque>
#include <format>
#include <stdexcept>

#include "dotnet/decompiler/typesystem/fake_member.hpp"
#include "dotnet/decompiler/typesystem/get_members_helper.hpp"
#include "dotnet/decompiler/typesystem/type_system_extensions.hpp"

namespace cb::dotnet::decompiler::typesystem {
namespace {

enum class Construction : std::uint8_t { Array = 1, Pointer, ByReference, Modified, Parameterized, NullabilityAnnotated };

const std::vector<const ITypeParameter*>& no_type_parameters() {
    static const std::vector<const ITypeParameter*> none;
    return none;
}
const std::vector<const IType*>& no_types() {
    static const std::vector<const IType*> none;
    return none;
}

int identity_hash(const void* p) {
    const auto v = reinterpret_cast<std::uintptr_t>(p);
    return static_cast<int>(static_cast<std::uint32_t>(v ^ (v >> 32)));
}

int mul_add(int hash, int factor, int add) { return static_cast<int>(static_cast<std::uint32_t>(hash) * static_cast<std::uint32_t>(factor) + static_cast<std::uint32_t>(add)); }

template <class T>
Filter<T> and_filter(const Filter<IMember>& filter) {
    if (!filter) return {};
    return [filter](const T* m) { return filter(m); };
}

template <class T>
Filter<T> filter_non_static_or_virtual(const Filter<T>& filter) {
    return [filter](const T* m) {
        return (!m->is_static() || m->symbol_kind() == SymbolKind::Operator || m->is_virtual()) && (!filter || filter(m));
    };
}

template <class T>
Filter<T> filter_non_static(const Filter<T>& filter) {
    return [filter](const T* m) { return !m->is_static() && (!filter || filter(m)); };
}

thread_local std::vector<const void*> busy_objects;

}  // namespace

// ---- the arena ----------------------------------------------------------------------------------------------------------

std::size_t TypeKeyHash::operator()(const TypeKey& k) const noexcept {
    std::size_t h = k.construction;
    for (const void* p : k.parts) h = h * 1000003 ^ std::hash<const void*>()(p);
    for (const std::int64_t n : k.numbers) h = h * 1000003 ^ std::hash<std::int64_t>()(n);
    for (const std::string& s : k.names) h = h * 1000003 ^ std::hash<std::string>()(s);
    return h;
}

TypeArena::~TypeArena() = default;   // (the objects are deleted through their own types: TypeArena::intern and keep)

TypeArena& TypeArena::global() {
    static TypeArena arena;
    return arena;
}

const ICompilation* compilation_of(const IType* type) {
    if (type == nullptr) return nullptr;
    if (const auto* p = dynamic_cast<const ICompilationProvider*>(type)) return &p->compilation();
    if (const auto* m = dynamic_cast<const ModifiedType*>(type)) {
        if (const ICompilation* c = compilation_of(m->element_type())) return c;
        return compilation_of(m->modifier());
    }
    if (const auto* e = dynamic_cast<const TypeWithElementType*>(type)) return compilation_of(e->element_type());
    if (const auto* p = dynamic_cast<const ParameterizedType*>(type)) {
        if (const ICompilation* c = compilation_of(p->generic_type())) return c;
        for (const IType* a : p->type_arguments())
            if (const ICompilation* c = compilation_of(a)) return c;
        return nullptr;
    }
    if (const auto* n = dynamic_cast<const NullabilityAnnotatedType*>(type)) return compilation_of(n->type_without_annotation());
    if (const auto* i = dynamic_cast<const IntersectionType*>(type)) {
        for (const IType* t : i->types())
            if (const ICompilation* c = compilation_of(t)) return c;
    }
    return nullptr;
}

TypeArena& arena_for(std::initializer_list<const IType*> parts) {
    for (const IType* p : parts)
        if (const ICompilation* c = compilation_of(p)) return c->arena();
    return TypeArena::global();
}

const IType* make_array_type(const ICompilation& compilation, const IType* element_type, int dimensions, Nullability nullability) {
    TypeKey key{static_cast<std::uint8_t>(Construction::Array), {&compilation, element_type}, {dimensions, static_cast<int>(nullability)}, {}};
    return compilation.arena().intern<IType>(std::move(key), [&]() -> std::unique_ptr<IType> { return std::make_unique<ArrayType>(compilation, element_type, dimensions, nullability); });
}

const IType* make_pointer_type(const IType* element_type) {
    TypeKey key{static_cast<std::uint8_t>(Construction::Pointer), {element_type}, {}, {}};
    return arena_for({element_type}).intern<IType>(std::move(key), [&]() -> std::unique_ptr<IType> { return std::make_unique<PointerType>(element_type); });
}

const IType* make_by_reference_type(const IType* element_type) {
    TypeKey key{static_cast<std::uint8_t>(Construction::ByReference), {element_type}, {}, {}};
    return arena_for({element_type}).intern<IType>(std::move(key), [&]() -> std::unique_ptr<IType> { return std::make_unique<ByReferenceType>(element_type); });
}

const IType* make_modified_type(const IType* modifier, const IType* unmodified, bool is_required) {
    TypeKey key{static_cast<std::uint8_t>(Construction::Modified), {modifier, unmodified}, {is_required ? 1 : 0}, {}};
    return arena_for({unmodified, modifier}).intern<IType>(std::move(key), [&]() -> std::unique_ptr<IType> { return std::make_unique<ModifiedType>(modifier, unmodified, is_required); });
}

const IType* make_parameterized_type(const IType* generic_type, std::vector<const IType*> type_arguments) {
    TypeKey key{static_cast<std::uint8_t>(Construction::Parameterized), {generic_type}, {}, {}};
    for (const IType* a : type_arguments) key.parts.push_back(a);
    TypeArena* arena = &arena_for({generic_type});
    if (arena == &TypeArena::global())
        for (const IType* a : type_arguments)
            if (const ICompilation* c = compilation_of(a)) {
                arena = &c->arena();
                break;
            }
    return arena->intern<IType>(std::move(key), [&]() -> std::unique_ptr<IType> { return std::make_unique<ParameterizedType>(generic_type, type_arguments); });
}

const IType* make_nullability_annotated_type(const IType* type, Nullability nullability) {
    TypeKey key{static_cast<std::uint8_t>(Construction::NullabilityAnnotated), {type}, {static_cast<int>(nullability)}, {}};
    return arena_for({type}).intern<IType>(std::move(key), [&]() -> std::unique_ptr<IType> {
        if (const auto* tp = dynamic_cast<const ITypeParameter*>(type)) return std::make_unique<NullabilityAnnotatedTypeParameter>(tp, nullability);
        return std::make_unique<NullabilityAnnotatedType>(type, nullability);
    });
}

// ---- the visitor ---------------------------------------------------------------------------------------------------------

const IType* TypeVisitor::visit_type_definition(const ITypeDefinition& type) { return type.visit_children(*this); }
const IType* TypeVisitor::visit_type_parameter(const ITypeParameter& type) { return type.visit_children(*this); }
const IType* TypeVisitor::visit_parameterized_type(const ParameterizedType& type) { return type.visit_children(*this); }
const IType* TypeVisitor::visit_array_type(const ArrayType& type) { return type.visit_children(*this); }
const IType* TypeVisitor::visit_pointer_type(const PointerType& type) { return type.visit_children(*this); }
const IType* TypeVisitor::visit_by_reference_type(const ByReferenceType& type) { return type.visit_children(*this); }
const IType* TypeVisitor::visit_other_type(const IType& type) { return type.visit_children(*this); }
const IType* TypeVisitor::visit_mod_req(const ModifiedType& type) { return type.visit_children(*this); }
const IType* TypeVisitor::visit_mod_opt(const ModifiedType& type) { return type.visit_children(*this); }
const IType* TypeVisitor::visit_nullability_annotated_type(const NullabilityAnnotatedType& type) { return type.visit_children(*this); }
// (visit_tuple_type and visit_function_pointer_type are in tuple_type.cpp and function_pointer_type.cpp, with their types)

// ---- the substitution ----------------------------------------------------------------------------------------------------

const TypeParameterSubstitution& TypeParameterSubstitution::identity() {
    static const TypeParameterSubstitution id;
    return id;
}

TypeParameterSubstitution TypeParameterSubstitution::compose(const TypeParameterSubstitution* g, const TypeParameterSubstitution* f) {
    if (g == nullptr) return f != nullptr ? *f : TypeParameterSubstitution();
    if (f == nullptr || f->is_identity()) return *g;
    // a copy of f with g applied to its elements; a list f does not have keeps g's
    TypeParameterSubstitution gv = *g;
    auto composed = [&](const std::vector<const IType*>& input) {
        std::vector<const IType*> result;
        result.reserve(input.size());
        for (const IType* t : input) result.push_back(t->accept_visitor(gv));
        return result;
    };
    std::optional<std::vector<const IType*>> c = f->class_type_arguments_ ? std::optional(composed(*f->class_type_arguments_)) : g->class_type_arguments_;
    std::optional<std::vector<const IType*>> m = f->method_type_arguments_ ? std::optional(composed(*f->method_type_arguments_)) : g->method_type_arguments_;
    return TypeParameterSubstitution(std::move(c), std::move(m));
}

namespace {

bool type_list_equals(const std::optional<std::vector<const IType*>>& a, const std::optional<std::vector<const IType*>>& b, TypeVisitor* normalization) {
    if (!a && !b) return true;
    if (!a || !b || a->size() != b->size()) return false;
    for (std::size_t i = 0; i < a->size(); ++i) {
        const IType* x = (*a)[i];
        const IType* y = (*b)[i];
        if (normalization != nullptr) {
            x = x->accept_visitor(*normalization);
            y = y->accept_visitor(*normalization);
        }
        if (!x->equals(y)) return false;
    }
    return true;
}

int type_list_hash(const std::optional<std::vector<const IType*>>& list) {
    if (!list) return 0;
    int h = 1;
    for (const IType* t : *list) h = mul_add(h, 27, t->hash_code());
    return h;
}

}  // namespace

bool TypeParameterSubstitution::equals(const TypeParameterSubstitution& other, TypeVisitor& normalization) const {
    return type_list_equals(class_type_arguments_, other.class_type_arguments_, &normalization) &&
           type_list_equals(method_type_arguments_, other.method_type_arguments_, &normalization);
}

bool TypeParameterSubstitution::equals(const TypeParameterSubstitution& other) const {
    return type_list_equals(class_type_arguments_, other.class_type_arguments_, nullptr) &&
           type_list_equals(method_type_arguments_, other.method_type_arguments_, nullptr);
}

int TypeParameterSubstitution::hash_code() const {
    return static_cast<int>(1124131u * static_cast<std::uint32_t>(type_list_hash(class_type_arguments_)) +
                            1821779u * static_cast<std::uint32_t>(type_list_hash(method_type_arguments_)));
}

const IType* TypeParameterSubstitution::visit_type_parameter(const ITypeParameter& type) {
    const int index = type.index();
    if (class_type_arguments_ && type.owner_type() == SymbolKind::TypeDefinition) {
        if (index >= 0 && index < static_cast<int>(class_type_arguments_->size())) return (*class_type_arguments_)[static_cast<std::size_t>(index)];
        return &SpecialType::unknown_type();
    }
    if (method_type_arguments_ && type.owner_type() == SymbolKind::Method) {
        if (index >= 0 && index < static_cast<int>(method_type_arguments_->size())) return (*method_type_arguments_)[static_cast<std::size_t>(index)];
        return &SpecialType::unknown_type();
    }
    return TypeVisitor::visit_type_parameter(type);
}

const IType* TypeParameterSubstitution::visit_nullability_annotated_type(const NullabilityAnnotatedType& type) {
    if (const auto* tp = dynamic_cast<const NullabilityAnnotatedTypeParameter*>(&type)) {
        if (tp->nullability() == Nullability::Nullable) return visit_type_parameter(*tp)->change_nullability(Nullability::Nullable);
        return visit_type_parameter(*tp);   // T! with T = an oblivious string is an oblivious string
    }
    return TypeVisitor::visit_nullability_annotated_type(type);
}

std::string TypeParameterSubstitution::to_string() const {
    std::string b = "[";
    bool first = true;
    auto sep = [&] {
        if (first) first = false;
        else b += ", ";
    };
    if (class_type_arguments_) {
        for (std::size_t i = 0; i < class_type_arguments_->size(); ++i) {
            sep();
            b += std::format("`{} -> {}", i, (*class_type_arguments_)[i]->reflection_name());
        }
        if (class_type_arguments_->empty()) {
            sep();
            b += "[]";
        }
    }
    if (method_type_arguments_) {
        for (std::size_t i = 0; i < method_type_arguments_->size(); ++i) {
            sep();
            b += std::format("``{} -> {}", i, (*method_type_arguments_)[i]->reflection_name());
        }
        if (method_type_arguments_->empty()) {
            sep();
            b += "[]";
        }
    }
    return b + "]";
}

// ---- AbstractType --------------------------------------------------------------------------------------------------------

std::string AbstractType::full_name() const {
    const std::string ns = namespace_();
    return ns.empty() ? name() : ns + "." + name();
}

const std::vector<const ITypeParameter*>& AbstractType::type_parameters() const { return no_type_parameters(); }
const std::vector<const IType*>& AbstractType::type_arguments() const { return no_types(); }

std::vector<const IMember*> AbstractType::get_members(const Filter<IMember>& filter, GetMemberOptions options) const {
    std::vector<const IMember*> out;
    for (const IMethod* m : get_methods(and_filter<IMethod>(filter), options)) out.push_back(m);
    for (const IProperty* m : get_properties(and_filter<IProperty>(filter), options)) out.push_back(m);
    for (const IField* m : get_fields(and_filter<IField>(filter), options)) out.push_back(m);
    for (const IEvent* m : get_events(and_filter<IEvent>(filter), options)) out.push_back(m);
    return out;
}

TypeParameterSubstitution AbstractType::get_substitution() const { return TypeParameterSubstitution::identity(); }
int AbstractType::hash_code() const { return identity_hash(this); }

// ---- SpecialType and UnknownType -----------------------------------------------------------------------------------------

const SpecialType& SpecialType::unknown_type() {
    static const SpecialType t(TypeKind::Unknown, "?", std::nullopt);
    return t;
}
const SpecialType& SpecialType::null_type() {
    static const SpecialType t(TypeKind::Null, "null", true);
    return t;
}
const SpecialType& SpecialType::no_type() {
    static const SpecialType t(TypeKind::None, "?", std::nullopt);
    return t;
}
const SpecialType& SpecialType::dynamic() {
    static const SpecialType t(TypeKind::Dynamic, "dynamic", true);
    return t;
}
const SpecialType& SpecialType::nint() {
    static const SpecialType t(TypeKind::NInt, "nint", false);
    return t;
}
const SpecialType& SpecialType::nuint() {
    static const SpecialType t(TypeKind::NUInt, "nuint", false);
    return t;
}
const SpecialType& SpecialType::arg_list() {
    static const SpecialType t(TypeKind::ArgList, "__arglist", std::nullopt);
    return t;
}
const SpecialType& SpecialType::unbound_type_argument() {
    static const SpecialType t(TypeKind::UnboundTypeArgument, "", std::nullopt);
    return t;
}

bool SpecialType::equals(const IType* other) const {
    const auto* s = dynamic_cast<const SpecialType*>(other);
    return s != nullptr && s->kind() == kind_;
}

const IType* SpecialType::change_nullability(Nullability nullability) const {
    if (nullability == Nullability::Oblivious || kind_ != TypeKind::Dynamic) return this;
    return make_nullability_annotated_type(this, nullability);
}

UnknownType::UnknownType(std::optional<std::string> namespace_name, std::string name, int type_parameter_count, std::optional<bool> is_reference_type)
    : namespace_known_(namespace_name.has_value()),
      full_type_name_(TopLevelTypeName(namespace_name.value_or(""), std::move(name), type_parameter_count)),
      is_reference_type_(is_reference_type) {}

UnknownType::UnknownType(const FullTypeName& full_type_name, std::optional<bool> is_reference_type) : is_reference_type_(is_reference_type) {
    const bool is_default = !full_type_name.is_nested() && full_type_name.name().empty() && full_type_name.top_level_type_name().namespace_().empty() &&
                            full_type_name.type_parameter_count() == 0;
    if (is_default) {   // upstream: a default FullTypeName (a null name)
        namespace_known_ = false;
        full_type_name_ = TopLevelTypeName("", "?", 0);
    } else {
        namespace_known_ = true;
        full_type_name_ = full_type_name;
    }
}

const std::vector<const ITypeParameter*>& UnknownType::type_parameters() const {
    return DummyTypeParameter::get_class_type_parameter_list(type_parameter_count());
}

const std::vector<const IType*>& UnknownType::type_arguments() const {
    // (upstream returns its type parameters as the arguments: the same objects, seen as types)
    static std::mutex m;
    static std::deque<std::vector<const IType*>> lists;
    const int n = type_parameter_count();
    const std::lock_guard lock(m);
    while (static_cast<int>(lists.size()) <= n) {
        std::vector<const IType*> l;
        for (const ITypeParameter* p : DummyTypeParameter::get_class_type_parameter_list(static_cast<int>(lists.size()))) l.push_back(p);
        lists.push_back(std::move(l));
    }
    return lists[static_cast<std::size_t>(n)];
}

const IType* UnknownType::change_nullability(Nullability nullability) const {
    if (nullability == Nullability::Oblivious || is_reference_type_ == false) return this;
    return make_nullability_annotated_type(this, nullability);
}

int UnknownType::hash_code() const {
    return (namespace_known_ ? 812571 : 12651) ^ static_cast<int>(std::hash<FullTypeName>()(full_type_name_));
}

bool UnknownType::equals(const IType* other) const {
    const auto* o = dynamic_cast<const UnknownType*>(other);
    return o != nullptr && namespace_known_ == o->namespace_known_ && full_type_name_ == o->full_type_name_ && is_reference_type_ == o->is_reference_type_;
}

// ---- types with an element type ------------------------------------------------------------------------------------------

TypeWithElementType::TypeWithElementType(const IType* element_type) : element_type_(element_type) {
    if (element_type == nullptr) throw UsageError("TypeWithElementType: a null element type");
}

ArrayType::ArrayType(const ICompilation& compilation, const IType* element_type, int dimensions, Nullability nullability)
    : TypeWithElementType(element_type), compilation_(&compilation), dimensions_(dimensions), nullability_(nullability) {
    if (dimensions <= 0) throw std::out_of_range("dimensions must be positive");
    if (const auto* p = dynamic_cast<const ICompilationProvider*>(element_type); p != nullptr && &p->compilation() != &compilation)
        throw UsageError("Cannot create an array type using a different compilation from the element type.");
}

const IType* ArrayType::change_nullability(Nullability nullability) const {
    if (nullability == nullability_) return this;
    return make_array_type(*compilation_, element_type_, dimensions_, nullability);
}

int ArrayType::hash_code() const { return mul_add(element_type_->hash_code(), 71681, dimensions_); }

bool ArrayType::equals(const IType* other) const {
    const auto* a = dynamic_cast<const ArrayType*>(other);
    return a != nullptr && element_type_->equals(a->element_type_) && a->dimensions_ == dimensions_ && a->nullability_ == nullability_;
}

std::string ArrayType::to_string() const {
    switch (nullability_) {
    case Nullability::Nullable: return element_type_->to_string() + name_suffix() + "?";
    case Nullability::NotNullable: return element_type_->to_string() + name_suffix() + "!";
    default: return element_type_->to_string() + name_suffix();
    }
}

std::vector<const IType*> ArrayType::direct_base_types() const {
    std::vector<const IType*> base;
    const IType* t = compilation_->find_type(KnownTypeCode::Array);
    if (t->kind() != TypeKind::Unknown) base.push_back(t);
    if (dimensions_ == 1 && element_type_->kind() != TypeKind::Pointer) {
        // one-dimensional arrays implement IList<T>, and since .NET 4.5 IReadOnlyList<T>
        if (const auto* def = dynamic_cast<const ITypeDefinition*>(compilation_->find_type(KnownTypeCode::IListOfT)))
            base.push_back(make_parameterized_type(def, {element_type_}));
        if (const auto* def = dynamic_cast<const ITypeDefinition*>(compilation_->find_type(KnownTypeCode::IReadOnlyListOfT)))
            base.push_back(make_parameterized_type(def, {element_type_}));
    }
    return base;
}

std::vector<const IMethod*> ArrayType::get_methods(const Filter<IMethod>& filter, GetMemberOptions options) const {
    if (has(options, GetMemberOptions::IgnoreInheritedMembers)) return {};
    return compilation_->find_type(KnownTypeCode::Array)->get_methods(filter, options);
}

std::vector<const IMethod*> ArrayType::get_methods(const std::vector<const IType*>& type_arguments, const Filter<IMethod>& filter, GetMemberOptions options) const {
    if (has(options, GetMemberOptions::IgnoreInheritedMembers)) return {};
    return compilation_->find_type(KnownTypeCode::Array)->get_methods(type_arguments, filter, options);
}

std::vector<const IMethod*> ArrayType::get_accessors(const Filter<IMethod>& filter, GetMemberOptions options) const {
    if (has(options, GetMemberOptions::IgnoreInheritedMembers)) return {};
    return compilation_->find_type(KnownTypeCode::Array)->get_accessors(filter, options);
}

std::vector<const IProperty*> ArrayType::get_properties(const Filter<IProperty>& filter, GetMemberOptions options) const {
    if (has(options, GetMemberOptions::IgnoreInheritedMembers)) return {};
    return compilation_->find_type(KnownTypeCode::Array)->get_properties(filter, options);
}

const IType* ArrayType::visit_children(TypeVisitor& visitor) const {
    const IType* e = element_type_->accept_visitor(visitor);
    return e == element_type_ ? this : make_array_type(*compilation_, e, dimensions_, nullability_);
}

bool PointerType::equals(const IType* other) const {
    const auto* p = dynamic_cast<const PointerType*>(other);
    return p != nullptr && element_type_->equals(p->element_type_);
}

const IType* PointerType::visit_children(TypeVisitor& visitor) const {
    const IType* e = element_type_->accept_visitor(visitor);
    return e == element_type_ ? this : make_pointer_type(e);
}

bool ByReferenceType::equals(const IType* other) const {
    const auto* p = dynamic_cast<const ByReferenceType*>(other);
    return p != nullptr && element_type_->equals(p->element_type_);
}

const IType* ByReferenceType::visit_children(TypeVisitor& visitor) const {
    const IType* e = element_type_->accept_visitor(visitor);
    return e == element_type_ ? this : make_by_reference_type(e);
}

ModifiedType::ModifiedType(const IType* modifier, const IType* unmodified, bool is_required)
    : TypeWithElementType(unmodified), kind_(is_required ? TypeKind::ModReq : TypeKind::ModOpt), modifier_(modifier) {
    if (modifier == nullptr) throw UsageError("ModifiedType: a null modifier");
}

const IType* ModifiedType::change_nullability(Nullability nullability) const {
    const IType* e = element_type_->change_nullability(nullability);
    return e == element_type_ ? this : make_modified_type(modifier_, e, kind_ == TypeKind::ModReq);
}

const IType* ModifiedType::visit_children(TypeVisitor& visitor) const {
    const IType* e = element_type_->accept_visitor(visitor);
    const IType* m = modifier_->accept_visitor(visitor);
    if (m != modifier_ || e != element_type_) return make_modified_type(m, e, kind_ == TypeKind::ModReq);
    return this;
}

bool ModifiedType::equals(const IType* other) const {
    const auto* o = dynamic_cast<const ModifiedType*>(other);
    return o != nullptr && kind_ == o->kind_ && modifier_->equals(o->modifier_) && element_type_->equals(o->element_type_);
}

int ModifiedType::hash_code() const {
    return static_cast<int>(static_cast<std::uint32_t>(kind_) ^ (static_cast<std::uint32_t>(element_type_->hash_code()) * 1344795899u) ^
                            (static_cast<std::uint32_t>(modifier_->hash_code()) * 901375117u));
}

// ---- ParameterizedType ---------------------------------------------------------------------------------------------------

ParameterizedType::ParameterizedType(const IType* generic_type, std::vector<const IType*> type_arguments)
    : generic_type_(generic_type), type_arguments_(std::move(type_arguments)) {
    if (generic_type == nullptr) throw UsageError("ParameterizedType: a null generic type");
    if (type_arguments_.empty()) throw UsageError("Cannot use ParameterizedType with 0 type arguments.");
    if (generic_type->type_parameter_count() != static_cast<int>(type_arguments_.size()))
        throw UsageError("Number of type arguments must match the type definition's number of type parameters");
    const auto* gp = dynamic_cast<const ICompilationProvider*>(generic_type);
    for (std::size_t i = 0; i < type_arguments_.size(); ++i) {
        if (type_arguments_[i] == nullptr) throw UsageError(std::format("ParameterizedType: typeArguments[{}] is null", i));
        const auto* p = dynamic_cast<const ICompilationProvider*>(type_arguments_[i]);
        if (p != nullptr && gp != nullptr && &p->compilation() != &gp->compilation())
            throw UsageError("Cannot parameterize a type with type arguments from a different compilation.");
    }
}

const IType* ParameterizedType::change_nullability(Nullability nullability) const {
    const IType* g = generic_type_->change_nullability(nullability);
    return g == generic_type_ ? this : make_parameterized_type(g, type_arguments_);
}

const IType* ParameterizedType::declaring_type() const {
    const IType* declaring = generic_type_->declaring_type();
    if (declaring != nullptr && declaring->type_parameter_count() > 0 && declaring->type_parameter_count() <= generic_type_->type_parameter_count())
        return make_parameterized_type(declaring, std::vector<const IType*>(type_arguments_.begin(), type_arguments_.begin() + declaring->type_parameter_count()));
    return declaring;
}

std::string ParameterizedType::reflection_name() const {
    std::string b = generic_type_->reflection_name() + "[";
    for (std::size_t i = 0; i < type_arguments_.size(); ++i) {
        if (i > 0) b += ',';
        b += "[" + type_arguments_[i]->reflection_name() + "]";
    }
    return b + "]";
}

std::string ParameterizedType::to_string() const {
    std::string b = generic_type_->to_string() + "[";
    for (std::size_t i = 0; i < type_arguments_.size(); ++i) {
        if (i > 0) b += ',';
        b += "[" + type_arguments_[i]->to_string() + "]";
    }
    return b + "]";
}

TypeParameterSubstitution ParameterizedType::get_substitution() const { return TypeParameterSubstitution(type_arguments_, std::nullopt); }

TypeParameterSubstitution ParameterizedType::get_substitution(const std::optional<std::vector<const IType*>>& method_type_arguments) const {
    return TypeParameterSubstitution(type_arguments_, method_type_arguments);
}

std::vector<const IType*> ParameterizedType::direct_base_types() const {
    TypeParameterSubstitution substitution = get_substitution();
    std::vector<const IType*> out;
    for (const IType* t : generic_type_->direct_base_types()) out.push_back(t->accept_visitor(substitution));
    return out;
}

std::vector<const IType*> ParameterizedType::get_nested_types(const Filter<ITypeDefinition>& filter, GetMemberOptions options) const {
    if (has(options, GetMemberOptions::ReturnMemberDefinitions)) return generic_type_->get_nested_types(filter, options);
    return get_members_helper::get_nested_types(this, filter, options);
}

std::vector<const IType*> ParameterizedType::get_nested_types(const std::vector<const IType*>& type_arguments, const Filter<ITypeDefinition>& filter,
                                                              GetMemberOptions options) const {
    if (has(options, GetMemberOptions::ReturnMemberDefinitions)) return generic_type_->get_nested_types(type_arguments, filter, options);
    return get_members_helper::get_nested_types(this, &type_arguments, filter, options);
}

std::vector<const IMethod*> ParameterizedType::get_constructors(const Filter<IMethod>& filter, GetMemberOptions options) const {
    if (has(options, GetMemberOptions::ReturnMemberDefinitions)) return generic_type_->get_constructors(filter, options);
    return get_members_helper::get_constructors(this, filter, options);
}

std::vector<const IMethod*> ParameterizedType::get_methods(const Filter<IMethod>& filter, GetMemberOptions options) const {
    if (has(options, GetMemberOptions::ReturnMemberDefinitions)) return generic_type_->get_methods(filter, options);
    return get_members_helper::get_methods(this, filter, options);
}

std::vector<const IMethod*> ParameterizedType::get_methods(const std::vector<const IType*>& type_arguments, const Filter<IMethod>& filter,
                                                          GetMemberOptions options) const {
    if (has(options, GetMemberOptions::ReturnMemberDefinitions)) return generic_type_->get_methods(type_arguments, filter, options);
    return get_members_helper::get_methods(this, &type_arguments, filter, options);
}

std::vector<const IProperty*> ParameterizedType::get_properties(const Filter<IProperty>& filter, GetMemberOptions options) const {
    if (has(options, GetMemberOptions::ReturnMemberDefinitions)) return generic_type_->get_properties(filter, options);
    return get_members_helper::get_properties(this, filter, options);
}

std::vector<const IField*> ParameterizedType::get_fields(const Filter<IField>& filter, GetMemberOptions options) const {
    if (has(options, GetMemberOptions::ReturnMemberDefinitions)) return generic_type_->get_fields(filter, options);
    return get_members_helper::get_fields(this, filter, options);
}

std::vector<const IEvent*> ParameterizedType::get_events(const Filter<IEvent>& filter, GetMemberOptions options) const {
    if (has(options, GetMemberOptions::ReturnMemberDefinitions)) return generic_type_->get_events(filter, options);
    return get_members_helper::get_events(this, filter, options);
}

std::vector<const IMember*> ParameterizedType::get_members(const Filter<IMember>& filter, GetMemberOptions options) const {
    if (has(options, GetMemberOptions::ReturnMemberDefinitions)) return generic_type_->get_members(filter, options);
    return get_members_helper::get_members(this, filter, options);
}

std::vector<const IMethod*> ParameterizedType::get_accessors(const Filter<IMethod>& filter, GetMemberOptions options) const {
    if (has(options, GetMemberOptions::ReturnMemberDefinitions)) return generic_type_->get_accessors(filter, options);
    return get_members_helper::get_accessors(this, filter, options);
}

bool ParameterizedType::equals(const IType* other) const {
    if (this == other) return true;
    const auto* c = dynamic_cast<const ParameterizedType*>(other);
    if (c == nullptr || !generic_type_->equals(c->generic_type_) || type_arguments_.size() != c->type_arguments_.size()) return false;
    for (std::size_t i = 0; i < type_arguments_.size(); ++i)
        if (!type_arguments_[i]->equals(c->type_arguments_[i])) return false;
    return true;
}

int ParameterizedType::hash_code() const {
    int h = generic_type_->hash_code();
    for (const IType* t : type_arguments_) h = mul_add(h, 1000000007, static_cast<int>(1000000009u * static_cast<std::uint32_t>(t->hash_code())));
    return h;
}

const IType* ParameterizedType::visit_children(TypeVisitor& visitor) const {
    const IType* g = generic_type_->accept_visitor(visitor);
    std::optional<std::vector<const IType*>> ta;   // made only once something changed
    if (g != generic_type_) ta.emplace(type_arguments_.size());
    for (std::size_t i = 0; i < type_arguments_.size(); ++i) {
        const IType* r = type_arguments_[i]->accept_visitor(visitor);
        if (r == nullptr) throw InternalError("TypeVisitor.Visit-method returned null");
        if (!ta && r != type_arguments_[i]) {
            ta.emplace(type_arguments_.size());
            for (std::size_t j = 0; j < i; ++j) (*ta)[j] = type_arguments_[j];
        }
        if (ta) (*ta)[i] = r;
    }
    if (!ta) return this;
    return make_parameterized_type(g, std::move(*ta));
}

// ---- decorated types -----------------------------------------------------------------------------------------------------

TypeParameterSubstitution DecoratedType::get_substitution() const { return base_type_->get_substitution(); }
int DecoratedType::hash_code() const { return identity_hash(this); }

NullabilityAnnotatedType::NullabilityAnnotatedType(const IType* type, Nullability nullability) : DecoratedType(type), nullability_(nullability) {}

bool NullabilityAnnotatedType::equals(const IType* other) const {
    const auto* nat = dynamic_cast<const NullabilityAnnotatedType*>(other);
    return nat != nullptr && nat->nullability_ == nullability_ && nat->base_type_->equals(base_type_);
}

const IType* NullabilityAnnotatedType::change_nullability(Nullability nullability) const {
    if (nullability == nullability_) return this;
    return base_type_->change_nullability(nullability);
}

const IType* NullabilityAnnotatedType::visit_children(TypeVisitor& visitor) const {
    const IType* new_base = base_type_->accept_visitor(visitor);
    if (new_base == base_type_) return this;
    if (new_base->nullability() == Nullability::Nullable) return new_base;   // T! with T = U? is U? (generic method substitution)
    if (new_base->kind() == TypeKind::TypeParameter || new_base->is_reference_type() == true) return new_base->change_nullability(nullability_);
    return new_base;   // T! with T = int is int, not int!
}

std::string NullabilityAnnotatedType::to_string() const {
    switch (nullability_) {
    case Nullability::Nullable: return base_type_->to_string() + "?";
    case Nullability::NotNullable: return base_type_->to_string() + "!";
    default: return base_type_->to_string() + "~";
    }
}

NullabilityAnnotatedTypeParameter::NullabilityAnnotatedTypeParameter(const ITypeParameter* type, Nullability nullability)
    : NullabilityAnnotatedType(type, nullability), type_parameter_(type) {}

// ---- type parameters -----------------------------------------------------------------------------------------------------

AbstractTypeParameter::AbstractTypeParameter(const IEntity* owner, int index, std::optional<std::string> name, VarianceModifier variance)
    : compilation_(owner != nullptr ? &owner->compilation() : nullptr), owner_type_(owner != nullptr ? owner->symbol_kind() : SymbolKind::None), owner_(owner),
      index_(index), variance_(variance) {
    if (owner == nullptr) throw UsageError("AbstractTypeParameter: a null owner");
    name_ = name.value_or((owner_type_ == SymbolKind::Method ? "!!" : "!") + std::to_string(index));
}

AbstractTypeParameter::AbstractTypeParameter(const ICompilation& compilation, SymbolKind owner_type, int index, std::optional<std::string> name,
                                             VarianceModifier variance)
    : compilation_(&compilation), owner_type_(owner_type), index_(index), variance_(variance) {
    name_ = name.value_or((owner_type_ == SymbolKind::Method ? "!!" : "!") + std::to_string(index));
}

const IType* AbstractTypeParameter::effective_base_class() const {
    if (const IType* cached = effective_base_class_.load()) return cached;
    const BusyLock lock(this);   // against cyclic type parameters
    if (!lock.success()) return &SpecialType::unknown_type();   // (not cached)
    const IType* result = calculate_effective_base_class();
    effective_base_class_.store(result);
    return result;
}

const IType* AbstractTypeParameter::calculate_effective_base_class() const {
    if (has_value_type_constraint()) return compilation_->find_type(KnownTypeCode::ValueType);
    std::vector<const IType*> class_constraints;
    for (const IType* c : direct_base_types()) {
        if (c->kind() == TypeKind::Class) {
            class_constraints.push_back(c);
        } else if (c->kind() == TypeKind::TypeParameter) {
            const IType* base = dynamic_cast<const ITypeParameter*>(c)->effective_base_class();
            if (base->kind() == TypeKind::Class) class_constraints.push_back(base);
        }
    }
    if (class_constraints.empty()) return compilation_->find_type(KnownTypeCode::Object);
    // the most derived of them
    const IType* result = class_constraints[0];
    for (std::size_t i = 1; i < class_constraints.size(); ++i)
        if (is_derived_from(class_constraints[i]->get_definition(), result->get_definition())) result = class_constraints[i];
    return result;
}

const std::vector<const IType*>& AbstractTypeParameter::effective_interface_set() const {
    if (interfaces_ready_.load()) return effective_interface_set_;
    static const std::vector<const IType*> none;
    const BusyLock lock(this);
    if (!lock.success()) return none;   // (not cached)
    std::call_once(interfaces_once_, [&] {
        effective_interface_set_ = calculate_effective_interface_set();
        interfaces_ready_.store(true);
    });
    return effective_interface_set_;
}

std::vector<const IType*> AbstractTypeParameter::calculate_effective_interface_set() const {
    // a HashSet in upstream: unique by equality, in the order first added
    std::vector<const IType*> result;
    auto add = [&](const IType* t) {
        for (const IType* r : result)
            if (r->equals(t)) return;
        result.push_back(t);
    };
    for (const IType* c : direct_base_types()) {
        if (c->kind() == TypeKind::Interface) add(c);
        else if (c->kind() == TypeKind::TypeParameter)
            for (const IType* i : dynamic_cast<const ITypeParameter*>(c)->effective_interface_set()) add(i);
    }
    return result;
}

std::optional<bool> AbstractTypeParameter::is_reference_type() const {
    if (has_value_type_constraint()) return false;
    if (has_reference_type_constraint()) return true;
    // a reference type when the effective base class is a class other than object, ValueType or Enum
    const IType* base = effective_base_class();
    if (base->kind() == TypeKind::Class || base->kind() == TypeKind::Delegate) {
        if (const ITypeDefinition* def = base->get_definition()) {
            switch (def->known_type_code()) {
            case KnownTypeCode::Object: case KnownTypeCode::ValueType: case KnownTypeCode::Enum: return std::nullopt;
            default: break;
            }
        }
        return true;
    }
    if (base->kind() == TypeKind::Struct || base->kind() == TypeKind::Enum) return false;
    return std::nullopt;
}

const IType* AbstractTypeParameter::change_nullability(Nullability nullability) const {
    if (nullability == Nullability::Oblivious) return this;
    return make_nullability_annotated_type(this, nullability);
}

const std::vector<const ITypeParameter*>& AbstractTypeParameter::type_parameters() const { return no_type_parameters(); }
const std::vector<const IType*>& AbstractTypeParameter::type_arguments() const { return no_types(); }

std::vector<const IType*> AbstractTypeParameter::direct_base_types() const {
    std::vector<const IType*> out;
    for (const TypeConstraint& c : type_constraints()) out.push_back(c.type);
    return out;
}

std::string AbstractTypeParameter::reflection_name() const { return (owner_type_ == SymbolKind::Method ? "``" : "`") + std::to_string(index_); }

std::vector<const IMethod*> AbstractTypeParameter::get_constructors(const Filter<IMethod>& filter, GetMemberOptions options) const {
    if (has(options, GetMemberOptions::IgnoreInheritedMembers)) {
        if (has_default_constructor_constraint() || has_value_type_constraint()) {
            const IMethod* ctor = create_dummy_constructor(*compilation_, this);
            if (!filter || filter(ctor)) return {ctor};
        }
        return {};
    }
    return get_members_helper::get_constructors(this, filter, options);
}

std::vector<const IMethod*> AbstractTypeParameter::get_methods(const Filter<IMethod>& filter, GetMemberOptions options) const {
    if (has(options, GetMemberOptions::IgnoreInheritedMembers)) return {};
    return get_members_helper::get_methods(this, filter_non_static_or_virtual(filter), options);
}

std::vector<const IMethod*> AbstractTypeParameter::get_methods(const std::vector<const IType*>& type_arguments, const Filter<IMethod>& filter,
                                                              GetMemberOptions options) const {
    if (has(options, GetMemberOptions::IgnoreInheritedMembers)) return {};
    return get_members_helper::get_methods(this, &type_arguments, filter_non_static_or_virtual(filter), options);
}

std::vector<const IProperty*> AbstractTypeParameter::get_properties(const Filter<IProperty>& filter, GetMemberOptions options) const {
    if (has(options, GetMemberOptions::IgnoreInheritedMembers)) return {};
    return get_members_helper::get_properties(this, filter_non_static_or_virtual(filter), options);
}

std::vector<const IField*> AbstractTypeParameter::get_fields(const Filter<IField>& filter, GetMemberOptions options) const {
    if (has(options, GetMemberOptions::IgnoreInheritedMembers)) return {};
    return get_members_helper::get_fields(this, filter_non_static_or_virtual(filter), options);
}

std::vector<const IEvent*> AbstractTypeParameter::get_events(const Filter<IEvent>& filter, GetMemberOptions options) const {
    if (has(options, GetMemberOptions::IgnoreInheritedMembers)) return {};
    return get_members_helper::get_events(this, filter_non_static_or_virtual(filter), options);
}

std::vector<const IMember*> AbstractTypeParameter::get_members(const Filter<IMember>& filter, GetMemberOptions options) const {
    if (has(options, GetMemberOptions::IgnoreInheritedMembers)) return {};
    return get_members_helper::get_members(this, filter_non_static_or_virtual(filter), options);
}

std::vector<const IMethod*> AbstractTypeParameter::get_accessors(const Filter<IMethod>& filter, GetMemberOptions options) const {
    if (has(options, GetMemberOptions::IgnoreInheritedMembers)) return {};
    return get_members_helper::get_accessors(this, filter_non_static_or_virtual(filter), options);
}

TypeParameterSubstitution AbstractTypeParameter::get_substitution() const { return TypeParameterSubstitution::identity(); }
int AbstractTypeParameter::hash_code() const { return identity_hash(this); }

DefaultTypeParameter::DefaultTypeParameter(const IEntity* owner, int index, std::optional<std::string> name, VarianceModifier variance,
                                           std::vector<const IAttribute*> attributes, bool has_value_type_constraint, bool has_reference_type_constraint,
                                           bool has_default_constructor_constraint, const std::vector<const IType*>& constraints, Nullability nullability_constraint)
    : AbstractTypeParameter(owner, index, std::move(name), variance), has_value_type_constraint_(has_value_type_constraint),
      has_reference_type_constraint_(has_reference_type_constraint), has_default_constructor_constraint_(has_default_constructor_constraint),
      nullability_constraint_(nullability_constraint), attributes_(std::move(attributes)) {
    make_constraints(constraints);
}

DefaultTypeParameter::DefaultTypeParameter(const ICompilation& compilation, SymbolKind owner_type, int index, std::optional<std::string> name,
                                           VarianceModifier variance, std::vector<const IAttribute*> attributes, bool has_value_type_constraint,
                                           bool has_reference_type_constraint, bool has_default_constructor_constraint, const std::vector<const IType*>& constraints,
                                           Nullability nullability_constraint)
    : AbstractTypeParameter(compilation, owner_type, index, std::move(name), variance), has_value_type_constraint_(has_value_type_constraint),
      has_reference_type_constraint_(has_reference_type_constraint), has_default_constructor_constraint_(has_default_constructor_constraint),
      nullability_constraint_(nullability_constraint), attributes_(std::move(attributes)) {
    make_constraints(constraints);
}

void DefaultTypeParameter::make_constraints(const std::vector<const IType*>& constraints) {
    bool has_non_interface = false;
    for (const IType* c : constraints) {
        type_constraints_.push_back({c, {}});
        if (c->kind() != TypeKind::Interface) has_non_interface = true;
    }
    // no System.Object constraint when another constraint is a base class
    if (has_value_type_constraint_ || !has_non_interface)
        type_constraints_.push_back({compilation().find_type(has_value_type_constraint_ ? KnownTypeCode::ValueType : KnownTypeCode::Object), {}});
}

namespace {

struct DummyStore {
    std::mutex mutex;
    std::deque<std::unique_ptr<DummyTypeParameter>> method, klass;
    std::deque<std::vector<const ITypeParameter*>> class_lists;
};

DummyStore& dummies() {
    static DummyStore store;
    return store;
}

const ITypeParameter* dummy(std::deque<std::unique_ptr<DummyTypeParameter>>& list, SymbolKind kind, int index) {
    while (static_cast<int>(list.size()) <= index) list.push_back(std::make_unique<DummyTypeParameter>(kind, static_cast<int>(list.size())));
    return list[static_cast<std::size_t>(index)].get();
}

}  // namespace

const ITypeParameter* DummyTypeParameter::get_method_type_parameter(int index) {
    const std::lock_guard lock(dummies().mutex);
    return dummy(dummies().method, SymbolKind::Method, index);
}

const ITypeParameter* DummyTypeParameter::get_class_type_parameter(int index) {
    const std::lock_guard lock(dummies().mutex);
    return dummy(dummies().klass, SymbolKind::TypeDefinition, index);
}

const std::vector<const ITypeParameter*>& DummyTypeParameter::get_class_type_parameter_list(int length) {
    DummyStore& s = dummies();
    const std::lock_guard lock(s.mutex);
    while (static_cast<int>(s.class_lists.size()) <= length) {
        std::vector<const ITypeParameter*> l;
        for (int j = 0; j < static_cast<int>(s.class_lists.size()); ++j) l.push_back(dummy(s.klass, SymbolKind::TypeDefinition, j));
        s.class_lists.push_back(std::move(l));
    }
    return s.class_lists[static_cast<std::size_t>(length)];
}

std::string DummyTypeParameter::name() const { return (owner_type_ == SymbolKind::Method ? "!!" : "!") + std::to_string(index_); }
std::string DummyTypeParameter::reflection_name() const { return (owner_type_ == SymbolKind::Method ? "``" : "`") + std::to_string(index_); }
const IType* DummyTypeParameter::effective_base_class() const { return &SpecialType::unknown_type(); }
const std::vector<const IType*>& DummyTypeParameter::effective_interface_set() const { return no_types(); }
const std::vector<TypeConstraint>& DummyTypeParameter::type_constraints() const {
    static const std::vector<TypeConstraint> none;
    return none;
}
const IType* DummyTypeParameter::change_nullability(Nullability nullability) const {
    if (nullability == Nullability::Oblivious) return this;
    return make_nullability_annotated_type(this, nullability);
}

// ---- IntersectionType ----------------------------------------------------------------------------------------------------

const IType* IntersectionType::create(const std::vector<const IType*>& types) {
    std::vector<const IType*> distinct;
    for (const IType* t : types) {
        if (t == nullptr) throw UsageError("IntersectionType: a null type");
        bool seen = false;
        for (const IType* d : distinct) seen = seen || d->equals(t);
        if (!seen) distinct.push_back(t);
    }
    if (distinct.empty()) return &SpecialType::unknown_type();
    if (distinct.size() == 1) return distinct[0];
    TypeArena& arena = arena_for({distinct[0], distinct[1]});
    return arena.keep(std::make_unique<IntersectionType>(std::move(distinct)));
}

std::string IntersectionType::name() const {
    std::string b;
    for (const IType* t : types_) {
        if (!b.empty()) b += " & ";
        b += t->name();
    }
    return b;
}

std::string IntersectionType::reflection_name() const {
    std::string b;
    for (const IType* t : types_) {
        if (!b.empty()) b += " & ";
        b += t->reflection_name();
    }
    return b;
}

std::optional<bool> IntersectionType::is_reference_type() const {
    for (const IType* t : types_)
        if (const auto r = t->is_reference_type()) return *r;
    return std::nullopt;
}

int IntersectionType::hash_code() const {
    int h = 0;
    for (const IType* t : types_) h = mul_add(h, 7137517, t->hash_code());
    return h;
}

bool IntersectionType::equals(const IType* other) const {
    const auto* o = dynamic_cast<const IntersectionType*>(other);
    if (o == nullptr || types_.size() != o->types_.size()) return false;
    for (std::size_t i = 0; i < types_.size(); ++i)
        if (!types_[i]->equals(o->types_[i])) return false;
    return true;
}

std::vector<const IMethod*> IntersectionType::get_methods(const Filter<IMethod>& filter, GetMemberOptions options) const {
    return get_members_helper::get_methods(this, filter_non_static(filter), options);
}
std::vector<const IMethod*> IntersectionType::get_methods(const std::vector<const IType*>& type_arguments, const Filter<IMethod>& filter,
                                                         GetMemberOptions options) const {
    return get_members_helper::get_methods(this, &type_arguments, filter, options);   // (as upstream: without the non-static filter)
}
std::vector<const IProperty*> IntersectionType::get_properties(const Filter<IProperty>& filter, GetMemberOptions options) const {
    return get_members_helper::get_properties(this, filter_non_static(filter), options);
}
std::vector<const IField*> IntersectionType::get_fields(const Filter<IField>& filter, GetMemberOptions options) const {
    return get_members_helper::get_fields(this, filter_non_static(filter), options);
}
std::vector<const IEvent*> IntersectionType::get_events(const Filter<IEvent>& filter, GetMemberOptions options) const {
    return get_members_helper::get_events(this, filter_non_static(filter), options);
}
std::vector<const IMember*> IntersectionType::get_members(const Filter<IMember>& filter, GetMemberOptions options) const {
    return get_members_helper::get_members(this, filter_non_static(filter), options);
}
std::vector<const IMethod*> IntersectionType::get_accessors(const Filter<IMethod>& filter, GetMemberOptions options) const {
    return get_members_helper::get_accessors(this, filter_non_static(filter), options);
}

// ---- NullableType and NormalizeTypeVisitor -------------------------------------------------------------------------------

namespace nullable_type {

bool is_nullable(const IType* type) {
    if (type == nullptr) throw UsageError("is_nullable: a null type");
    const auto* pt = dynamic_cast<const ParameterizedType*>(skip_modifiers(type));
    return pt != nullptr && pt->type_parameter_count() == 1 && is_known_type(pt->generic_type(), KnownTypeCode::NullableOfT);
}

bool is_non_nullable_value_type(const IType* type) { return type->is_reference_type() == false && !is_nullable(type); }

const IType* get_underlying_type(const IType* type) {
    if (type == nullptr) throw UsageError("get_underlying_type: a null type");
    const auto* pt = dynamic_cast<const ParameterizedType*>(skip_modifiers(type));
    if (pt != nullptr && pt->type_parameter_count() == 1 && is_known_type(pt->generic_type(), KnownTypeCode::NullableOfT)) return pt->get_type_argument(0);
    return type;
}

const IType* create(const ICompilation& compilation, const IType* element_type) {
    if (element_type == nullptr) throw UsageError("nullable_type::create: a null element type");
    const IType* nullable = compilation.find_type(KnownTypeCode::NullableOfT);
    if (const ITypeDefinition* def = nullable->get_definition()) return make_parameterized_type(def, {element_type});
    return nullable;
}

}  // namespace nullable_type

NormalizeTypeVisitor& NormalizeTypeVisitor::type_erasure() {
    static NormalizeTypeVisitor v = [] {
        NormalizeTypeVisitor n;
        n.replace_class_type_parameters_with_dummy = false;
        n.replace_method_type_parameters_with_dummy = false;
        n.dynamic_and_object = true;
        n.int_ptr_to_nint = true;
        n.tuple_to_underlying_type = true;
        n.remove_mod_opt = true;
        n.remove_mod_req = true;
        n.remove_nullability = true;
        return n;
    }();
    return v;
}

NormalizeTypeVisitor& NormalizeTypeVisitor::ignore_nullability_and_tuples() {
    static NormalizeTypeVisitor v = [] {
        NormalizeTypeVisitor n;
        n.replace_class_type_parameters_with_dummy = false;
        n.replace_method_type_parameters_with_dummy = false;
        n.dynamic_and_object = false;
        n.int_ptr_to_nint = false;
        n.tuple_to_underlying_type = true;
        n.remove_mod_opt = true;
        n.remove_mod_req = true;
        n.remove_nullability = true;
        return n;
    }();
    return v;
}

NormalizeTypeVisitor& NormalizeTypeVisitor::ignore_nullability() {
    static NormalizeTypeVisitor v = [] {
        NormalizeTypeVisitor n;
        n.replace_class_type_parameters_with_dummy = false;
        n.replace_method_type_parameters_with_dummy = false;
        n.dynamic_and_object = false;
        n.int_ptr_to_nint = false;
        n.tuple_to_underlying_type = false;
        n.remove_mod_opt = true;
        n.remove_mod_req = true;
        n.remove_nullability = true;
        return n;
    }();
    return v;
}

bool NormalizeTypeVisitor::equivalent_types(const IType* a, const IType* b) { return a->accept_visitor(*this)->equals(b->accept_visitor(*this)); }

const IType* NormalizeTypeVisitor::visit_type_parameter(const ITypeParameter& type) {
    if (type.owner_type() == SymbolKind::Method && replace_method_type_parameters_with_dummy) return DummyTypeParameter::get_method_type_parameter(type.index());
    if (type.owner_type() == SymbolKind::TypeDefinition && replace_class_type_parameters_with_dummy) return DummyTypeParameter::get_class_type_parameter(type.index());
    if (remove_nullability)
        if (const auto* natp = dynamic_cast<const NullabilityAnnotatedTypeParameter*>(&type)) return natp->type_without_annotation()->accept_visitor(*this);
    return TypeVisitor::visit_type_parameter(type);
}

const IType* NormalizeTypeVisitor::visit_type_definition(const ITypeDefinition& type) {
    switch (type.known_type_code()) {
    case KnownTypeCode::Object:
        // dynamic -> object the other way round, so that no compilation is needed to find object
        if (dynamic_and_object) return remove_nullability ? &SpecialType::dynamic() : SpecialType::dynamic().change_nullability(type.nullability());
        break;
    case KnownTypeCode::IntPtr:
        if (int_ptr_to_nint) return &SpecialType::nint();
        break;
    case KnownTypeCode::UIntPtr:
        if (int_ptr_to_nint) return &SpecialType::nuint();
        break;
    default: break;
    }
    return TypeVisitor::visit_type_definition(type);
}

const IType* NormalizeTypeVisitor::visit_nullability_annotated_type(const NullabilityAnnotatedType& type) {
    if (remove_nullability) return type.type_without_annotation()->accept_visitor(*this);
    return TypeVisitor::visit_nullability_annotated_type(type);
}

const IType* NormalizeTypeVisitor::visit_array_type(const ArrayType& type) {
    if (remove_nullability) return TypeVisitor::visit_array_type(type)->change_nullability(Nullability::Oblivious);
    return TypeVisitor::visit_array_type(type);
}

const IType* NormalizeTypeVisitor::visit_mod_opt(const ModifiedType& type) {
    if (remove_mod_opt) return type.element_type()->accept_visitor(*this);
    return TypeVisitor::visit_mod_opt(type);
}

const IType* NormalizeTypeVisitor::visit_mod_req(const ModifiedType& type) {
    if (remove_mod_req) return type.element_type()->accept_visitor(*this);
    return TypeVisitor::visit_mod_req(type);
}

// ---- BusyManager ---------------------------------------------------------------------------------------------------------

BusyLock::BusyLock(const void* object) {
    for (const void* o : busy_objects)
        if (o == object) {
            success_ = false;
            return;
        }
    busy_objects.push_back(object);
    success_ = true;
}

BusyLock::~BusyLock() {
    if (success_) busy_objects.pop_back();
}

}  // namespace cb::dotnet::decompiler::typesystem
