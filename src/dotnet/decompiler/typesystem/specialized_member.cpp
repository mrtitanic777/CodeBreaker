// dotnet/decompiler/typesystem/specialized_member.cpp -- the specialized members.
#include "dotnet/decompiler/typesystem/specialized_member.hpp"

#include "dotnet/decompiler/util/char_info.hpp"
#include "dotnet/decompiler/util/number_format.hpp"

namespace cb::dotnet::decompiler::typesystem {
namespace {

enum class Construction : std::uint8_t { Method = 20, Field, Property, Event };

TypeKey member_key(Construction c, const IMember* definition, const TypeParameterSubstitution& s, const IType* declaring_type) {
    TypeKey key{static_cast<std::uint8_t>(c), {definition, declaring_type}, {}, {}};
    key.numbers.push_back(s.class_type_arguments() ? static_cast<std::int64_t>(s.class_type_arguments()->size()) : -1);
    key.numbers.push_back(s.method_type_arguments() ? static_cast<std::int64_t>(s.method_type_arguments()->size()) : -1);
    if (s.class_type_arguments())
        for (const IType* t : *s.class_type_arguments()) key.parts.push_back(t);
    if (s.method_type_arguments())
        for (const IType* t : *s.method_type_arguments()) key.parts.push_back(t);
    return key;
}

// Method type arguments dropped (a member of a generic type, not itself generic, keeps only the class's).
TypeParameterSubstitution class_only(const TypeParameterSubstitution& s) {
    if (s.method_type_arguments() && !s.method_type_arguments()->empty()) return TypeParameterSubstitution(s.class_type_arguments(), std::vector<const IType*>{});
    return s;
}

int combine(std::uint32_t a, int x, std::uint32_t b, int y) {
    return static_cast<int>(a * static_cast<std::uint32_t>(x) + b * static_cast<std::uint32_t>(y));
}

}  // namespace

// ---- SpecializedMember ---------------------------------------------------------------------------------------------------

SpecializedMember::SpecializedMember(const IMember* member_definition, const IType* declaring_type)
    : base_member_(member_definition), declaring_type_(declaring_type) {
    if (member_definition == nullptr) throw UsageError("SpecializedMember: a null member definition");
    if (dynamic_cast<const SpecializedMember*>(member_definition) != nullptr)
        throw UsageError("Member definition cannot be specialized. Please use IMember.Specialize() instead of directly constructing SpecializedMember instances.");
}

void SpecializedMember::add_substitution(const TypeParameterSubstitution& new_substitution) {
    substitution_ = TypeParameterSubstitution::compose(&new_substitution, &substitution_);
}

const IMethod* SpecializedMember::wrap_accessor(std::atomic<const IMethod*>& cache, const IMethod* accessor_definition) const {
    if (accessor_definition == nullptr) return nullptr;
    if (const IMethod* m = cache.load()) return m;
    const IMethod* m = accessor_definition->specialize_method(substitution_);
    cache.store(m);
    return m;
}

const IType* SpecializedMember::declaring_type() const {
    if (const IType* t = declaring_type_.load()) return t;
    const IType* definition_declaring = base_member_->declaring_type();
    const auto* definition_def = dynamic_cast<const ITypeDefinition*>(definition_declaring);
    TypeParameterSubstitution s = substitution_;
    const IType* result = nullptr;
    if (definition_def != nullptr && definition_declaring->type_parameter_count() > 0) {
        const auto& class_args = substitution_.class_type_arguments();
        if (class_args && static_cast<int>(class_args->size()) == definition_declaring->type_parameter_count()) {
            result = make_parameterized_type(definition_def, *class_args);
        } else {
            std::vector<const IType*> own(definition_def->type_parameters().begin(), definition_def->type_parameters().end());
            result = make_parameterized_type(definition_def, std::move(own))->accept_visitor(s);
        }
    } else if (definition_declaring != nullptr) {
        result = definition_declaring->accept_visitor(s);
    }
    declaring_type_.store(result);
    return result;
}

const IType* SpecializedMember::return_type() const {
    if (const IType* t = return_type_.load()) return t;
    TypeParameterSubstitution s = substitution_;
    const IType* result = base_member_->return_type()->accept_visitor(s);
    return_type_.store(result);
    return result;
}

std::vector<const IMember*> SpecializedMember::explicitly_implemented_interface_members() const {
    std::vector<const IMember*> out;
    for (const IMember* m : base_member_->explicitly_implemented_interface_members()) out.push_back(m->specialize(substitution_));
    return out;
}

const IMember* SpecializedMember::specialize(const TypeParameterSubstitution& new_substitution) const {
    return base_member_->specialize(TypeParameterSubstitution::compose(&new_substitution, &substitution_));
}

bool SpecializedMember::equals(const IMember* other, TypeVisitor& type_normalization) const {
    const auto* o = dynamic_cast<const SpecializedMember*>(other);
    return o != nullptr && base_member_->equals(o->base_member_, type_normalization) && substitution_.equals(o->substitution_, type_normalization);
}

bool SpecializedMember::equals(const IMember* other) const {
    const auto* o = dynamic_cast<const SpecializedMember*>(other);
    return o != nullptr && base_member_->equals(o->base_member_) && substitution_.equals(o->substitution_);
}

int SpecializedMember::hash_code() const { return combine(1000000007u, base_member_->hash_code(), 1000000009u, substitution_.hash_code()); }

std::string SpecializedMember::to_string() const {
    return "[" + type_name() + " " + declaring_type()->to_string() + "." + name() + ":" + return_type()->to_string() + "]";
}

const std::vector<const IParameter*>& SpecializedParameterizedMember::parameters() const {
    std::call_once(parameters_once_, [&] {
        TypeParameterSubstitution s = substitution_;
        const auto* definition = dynamic_cast<const IParameterizedMember*>(base_member_);
        for (const IParameter* p : definition->parameters()) {
            owned_parameters_.push_back(std::make_unique<SpecializedParameter>(p, p->type()->accept_visitor(s), this));
            parameters_.push_back(owned_parameters_.back().get());
        }
    });
    return parameters_;
}

std::string SpecializedParameterizedMember::to_string() const {
    std::string b = "[" + type_name() + " " + declaring_type()->reflection_name() + "." + name() + "(";
    for (std::size_t i = 0; i < parameters().size(); ++i) {
        if (i > 0) b += ", ";
        b += parameters()[i]->to_string();
    }
    return b + "):" + return_type()->reflection_name() + "]";
}

// ---- SpecializedMethod ---------------------------------------------------------------------------------------------------

// The type parameters of a specialized generic method: their constraints substituted, the method as their owner.
class SpecializedMethod::SpecializedTypeParameter final : public AbstractTypeParameter {
public:
    SpecializedTypeParameter(const ITypeParameter* base, const IMethod* owner)
        : AbstractTypeParameter(owner, base->index(), base->name(), base->variance()), base_(base) {}
    std::vector<const IAttribute*> get_attributes() const override { return base_->get_attributes(); }
    int hash_code() const override { return base_->hash_code() ^ dynamic_cast<const IMember*>(owner())->hash_code(); }
    bool equals(const IType* other) const override {
        // the owner, not the substitution (which may hold this very type parameter)
        const auto* o = dynamic_cast<const SpecializedTypeParameter*>(other);
        return o != nullptr && base_->equals(o->base_) && dynamic_cast<const IMember*>(owner())->equals(dynamic_cast<const IMember*>(o->owner()));
    }
    bool has_value_type_constraint() const override { return base_->has_value_type_constraint(); }
    bool has_reference_type_constraint() const override { return base_->has_reference_type_constraint(); }
    bool has_default_constructor_constraint() const override { return base_->has_default_constructor_constraint(); }
    bool has_unmanaged_constraint() const override { return base_->has_unmanaged_constraint(); }
    bool allows_ref_like_type() const override { return base_->allows_ref_like_type(); }
    Nullability nullability_constraint() const override { return base_->nullability_constraint(); }
    const std::vector<TypeConstraint>& type_constraints() const override {
        std::call_once(constraints_once_, [&] {
            TypeParameterSubstitution s = substitution;
            for (const TypeConstraint& c : base_->type_constraints()) constraints_.push_back({c.type->accept_visitor(s), c.attributes});
        });
        return constraints_;
    }

    TypeParameterSubstitution substitution;   // set at the end of the method's constructor

private:
    const ITypeParameter* base_;
    mutable std::once_flag constraints_once_;
    mutable std::vector<TypeConstraint> constraints_;
};

const IMethod* SpecializedMethod::create(const IMethod* method_definition, const TypeParameterSubstitution& substitution) {
    if (TypeParameterSubstitution::identity().equals(substitution)) return method_definition;
    if (dynamic_cast<const ArrayType*>(method_definition->declaring_type()) != nullptr) return make(method_definition, substitution);
    if (method_definition->type_parameters().empty()) {
        if (method_definition->declaring_type()->type_parameter_count() == 0) return method_definition;
        return make(method_definition, class_only(substitution));
    }
    return make(method_definition, substitution);
}

const IMethod* SpecializedMethod::make(const IMethod* method_definition, const TypeParameterSubstitution& substitution, const IType* declaring_type) {
    return method_definition->compilation().arena().intern<SpecializedMethod>(
        member_key(Construction::Method, method_definition, substitution, declaring_type),
        [&] { return std::make_unique<SpecializedMethod>(method_definition, substitution, declaring_type); });
}

SpecializedMethod::SpecializedMethod(const IMethod* method_definition, const TypeParameterSubstitution& substitution, const IType* declaring_type)
    : SpecializedParameterizedMember(method_definition, declaring_type), method_definition_(method_definition),
      is_parameterized_(substitution.method_type_arguments().has_value()) {
    bool have_without = false;
    if (!method_definition->type_parameters().empty()) {
        // a generic method: its type parameters specialized (their constraints, and the right owner)
        for (const ITypeParameter* tp : method_definition->type_parameters()) {
            owned_type_parameters_.push_back(std::make_unique<SpecializedTypeParameter>(tp, this));
            specialized_type_parameters_.push_back(owned_type_parameters_.back().get());
        }
        if (!is_parameterized_) {
            // the base method's type parameters replaced by the specialized ones, unless they were already substituted
            substitution_without_specialized_type_parameters_ = substitution_;
            have_without = true;
            add_substitution(TypeParameterSubstitution(std::nullopt, std::vector<const IType*>(specialized_type_parameters_.begin(), specialized_type_parameters_.end())));
        }
    }
    // the main substitution, after the method type parameters' specialization
    add_substitution(substitution);
    if (have_without)
        substitution_without_specialized_type_parameters_ = TypeParameterSubstitution::compose(&substitution, &substitution_without_specialized_type_parameters_);
    else
        substitution_without_specialized_type_parameters_ = substitution_;   // it has no specialized type parameters in it then
    for (const auto& tp : owned_type_parameters_) tp->substitution = substitution_;
}

const std::vector<const IType*>& SpecializedMethod::type_arguments() const {
    static const std::vector<const IType*> none;
    return substitution_.method_type_arguments() ? *substitution_.method_type_arguments() : none;
}

const std::vector<const ITypeParameter*>& SpecializedMethod::type_parameters() const {
    return owned_type_parameters_.empty() ? method_definition_->type_parameters() : specialized_type_parameters_;
}

const IMember* SpecializedMethod::accessor_owner() const {
    if (const IMember* m = accessor_owner_.load()) return m;
    const IMember* owner = method_definition_->accessor_owner();
    if (owner == nullptr) return nullptr;
    const IMember* result = owner->specialize(substitution_);
    accessor_owner_.store(result);
    return result;
}

bool SpecializedMethod::equals(const IMember* other, TypeVisitor& type_normalization) const {
    const auto* o = dynamic_cast<const SpecializedMethod*>(other);
    return o != nullptr && base_member_->equals(o->base_member_, type_normalization) &&
           substitution_without_specialized_type_parameters_.equals(o->substitution_without_specialized_type_parameters_, type_normalization);
}

bool SpecializedMethod::equals(const IMember* other) const {
    const auto* o = dynamic_cast<const SpecializedMethod*>(other);
    return o != nullptr && base_member_->equals(o->base_member_) &&
           substitution_without_specialized_type_parameters_.equals(o->substitution_without_specialized_type_parameters_);
}

int SpecializedMethod::hash_code() const {
    return combine(1000000013u, base_member_->hash_code(), 1000000009u, substitution_without_specialized_type_parameters_.hash_code());
}

const IMember* SpecializedMethod::specialize(const TypeParameterSubstitution& new_substitution) const {
    return method_definition_->specialize(TypeParameterSubstitution::compose(&new_substitution, &substitution_without_specialized_type_parameters_));
}

const IMethod* SpecializedMethod::specialize_method(const TypeParameterSubstitution& new_substitution) const {
    return method_definition_->specialize_method(TypeParameterSubstitution::compose(&new_substitution, &substitution_without_specialized_type_parameters_));
}

std::string SpecializedMethod::to_string() const {
    std::string b = "[SpecializedMethod " + declaring_type()->reflection_name() + "." + name();
    if (!type_arguments().empty()) {
        b += '[';
        for (std::size_t i = 0; i < type_arguments().size(); ++i) {
            if (i > 0) b += ", ";
            b += type_arguments()[i]->to_string();
        }
        b += ']';
    } else if (!type_parameters().empty()) {
        b += "``" + std::to_string(type_parameters().size());
    }
    b += '(';
    for (std::size_t i = 0; i < parameters().size(); ++i) {
        if (i > 0) b += ", ";
        b += parameters()[i]->to_string();
    }
    return b + "):" + return_type()->to_string() + "]";
}

// ---- fields, properties, events ------------------------------------------------------------------------------------------

const IField* SpecializedField::create(const IField* field_definition, const TypeParameterSubstitution& substitution) {
    if (TypeParameterSubstitution::identity().equals(substitution) || field_definition->declaring_type()->type_parameter_count() == 0) return field_definition;
    return make(field_definition, class_only(substitution));
}

const IField* SpecializedField::make(const IField* field_definition, const TypeParameterSubstitution& substitution, const IType* declaring_type) {
    return field_definition->compilation().arena().intern<SpecializedField>(
        member_key(Construction::Field, field_definition, substitution, declaring_type),
        [&] { return std::make_unique<SpecializedField>(field_definition, substitution, declaring_type); });
}

SpecializedField::SpecializedField(const IField* field_definition, const TypeParameterSubstitution& substitution, const IType* declaring_type)
    : SpecializedMember(field_definition, declaring_type), field_definition_(field_definition) {
    add_substitution(substitution);
}

const IProperty* SpecializedProperty::create(const IProperty* property_definition, const TypeParameterSubstitution& substitution) {
    if (TypeParameterSubstitution::identity().equals(substitution) || property_definition->declaring_type()->type_parameter_count() == 0)
        return property_definition;
    return make(property_definition, class_only(substitution));
}

const IProperty* SpecializedProperty::make(const IProperty* property_definition, const TypeParameterSubstitution& substitution, const IType* declaring_type) {
    return property_definition->compilation().arena().intern<SpecializedProperty>(
        member_key(Construction::Property, property_definition, substitution, declaring_type),
        [&] { return std::make_unique<SpecializedProperty>(property_definition, substitution, declaring_type); });
}

SpecializedProperty::SpecializedProperty(const IProperty* property_definition, const TypeParameterSubstitution& substitution, const IType* declaring_type)
    : SpecializedParameterizedMember(property_definition, declaring_type), property_definition_(property_definition) {
    add_substitution(substitution);
}

const IEvent* SpecializedEvent::create(const IEvent* event_definition, const TypeParameterSubstitution& substitution) {
    if (TypeParameterSubstitution::identity().equals(substitution) || event_definition->declaring_type()->type_parameter_count() == 0) return event_definition;
    return make(event_definition, class_only(substitution));
}

const IEvent* SpecializedEvent::make(const IEvent* event_definition, const TypeParameterSubstitution& substitution, const IType* declaring_type) {
    return event_definition->compilation().arena().intern<SpecializedEvent>(
        member_key(Construction::Event, event_definition, substitution, declaring_type),
        [&] { return std::make_unique<SpecializedEvent>(event_definition, substitution, declaring_type); });
}

SpecializedEvent::SpecializedEvent(const IEvent* event_definition, const TypeParameterSubstitution& substitution, const IType* declaring_type)
    : SpecializedMember(event_definition, declaring_type), event_definition_(event_definition) {
    add_substitution(substitution);
}

// ---- parameters ----------------------------------------------------------------------------------------------------------

std::string SpecializedParameter::to_string() const { return parameter_to_string(*this); }

ConstantValue to_constant_value(const srm::ConstantValue& value) {
    return std::visit([](const auto& v) -> ConstantValue { return v; }, value);
}

std::string constant_to_string(const ConstantValue& value) {
    return std::visit(
        [](const auto& v) -> std::string {
            using V = std::decay_t<decltype(v)>;
            if constexpr (std::is_same_v<V, std::monostate>) return "null";
            else if constexpr (std::is_same_v<V, bool>) return v ? "True" : "False";
            else if constexpr (std::is_same_v<V, char16_t>) return util::utf8(std::u16string(1, v));
            else if constexpr (std::is_same_v<V, float> || std::is_same_v<V, double>) return util::format_r(v);
            else if constexpr (std::is_same_v<V, std::u16string>) return util::utf8(v);
            else if constexpr (std::is_same_v<V, util::Decimal>) return v.to_string();
            else return std::to_string(v);
        },
        value);
}

std::string parameter_to_string(const IParameter& parameter) {
    std::string b;
    switch (parameter.reference_kind()) {
    case ReferenceKind::None: break;
    case ReferenceKind::Ref: b += "ref "; break;
    case ReferenceKind::Out: b += "out "; break;
    case ReferenceKind::In: b += "in "; break;
    case ReferenceKind::RefReadOnly: b += "ref readonly "; break;
    }
    if (parameter.is_params()) b += "params ";
    b += parameter.name() + ":" + parameter.type()->reflection_name();
    if (parameter.is_optional() && parameter.has_constant_value_in_signature()) b += " = " + constant_to_string(parameter.get_constant_value(false));
    return b;
}

}  // namespace cb::dotnet::decompiler::typesystem
