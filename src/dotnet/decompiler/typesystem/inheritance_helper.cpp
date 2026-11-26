// dotnet/decompiler/typesystem/inheritance_helper.cpp -- base members, derived members, signature comparison.
#include "dotnet/decompiler/typesystem/inheritance_helper.hpp"

#include <algorithm>
#include <unordered_set>

#include "core/error.hpp"
#include "dotnet/decompiler/typesystem/type_system_extensions.hpp"
#include "dotnet/decompiler/typesystem/types.hpp"

namespace cb::dotnet::decompiler::typesystem {
namespace {

NormalizeTypeVisitor& normalization_visitor() {
    static NormalizeTypeVisitor v = [] {
        NormalizeTypeVisitor n;
        n.replace_class_type_parameters_with_dummy = false;
        n.replace_method_type_parameters_with_dummy = true;
        n.dynamic_and_object = true;
        n.tuple_to_underlying_type = true;
        return n;
    }();
    return v;
}

}  // namespace

bool parameter_lists_equal(const std::vector<const IParameter*>& x, const std::vector<const IParameter*>& y, bool include_modifiers) {
    if (&x == &y) return true;
    if (x.size() != y.size()) return false;
    for (std::size_t i = 0; i < x.size(); ++i) {
        const IParameter* a = x[i];
        const IParameter* b = y[i];
        if (a == nullptr && b == nullptr) continue;
        if (a == nullptr || b == nullptr) return false;
        if (include_modifiers && (a->reference_kind() != b->reference_kind() || a->is_params() != b->is_params())) return false;
        // "Method<T>(T a)" and "Method<S>(S b)" are the same list: method type parameters are normalized
        const IType* a_type = a->type()->accept_visitor(normalization_visitor());
        const IType* b_type = b->type()->accept_visitor(normalization_visitor());
        if (!type_equals(a_type, b_type)) return false;
    }
    return true;
}

int parameter_list_hash_code(const std::vector<const IParameter*>& list) {
    auto hash = static_cast<std::uint32_t>(list.size());
    for (const IParameter* p : list) {
        hash *= 27;
        hash += static_cast<std::uint32_t>(p->type()->accept_visitor(normalization_visitor())->hash_code());
    }
    return static_cast<int>(hash);
}

bool signature_equals(const IMember* x, const IMember* y) {
    if (x == y) return true;
    if (x == nullptr || y == nullptr || x->symbol_kind() != y->symbol_kind() || x->name() != y->name()) return false;
    const auto* px = dynamic_cast<const IParameterizedMember*>(x);
    const auto* py = dynamic_cast<const IParameterizedMember*>(y);
    if (px != nullptr && py != nullptr) {
        const auto* mx = dynamic_cast<const IMethod*>(x);
        const auto* my = dynamic_cast<const IMethod*>(y);
        if (mx != nullptr && my != nullptr && mx->type_parameters().size() != my->type_parameters().size()) return false;
        return parameter_lists_equal(px->parameters(), py->parameters());
    }
    return true;
}

int signature_hash_code(const IMember* member) {
    auto hash = static_cast<std::uint32_t>(static_cast<int>(member->symbol_kind()) * 33 + static_cast<int>(std::hash<std::string>()(member->name())));
    if (const auto* pm = dynamic_cast<const IParameterizedMember*>(member)) {
        hash *= 27;
        hash += static_cast<std::uint32_t>(parameter_list_hash_code(pm->parameters()));
        if (const auto* m = dynamic_cast<const IMethod*>(member)) hash += static_cast<std::uint32_t>(m->type_parameters().size());
    }
    return static_cast<int>(hash);
}

namespace inheritance_helper {

std::vector<const IMember*> get_base_members(const IMember* member, bool include_implemented_interfaces) {
    if (member == nullptr) throw UsageError("get_base_members: a null member");
    std::vector<const IMember*> result;
    if (include_implemented_interfaces) {
        const std::vector<const IMember*> implemented = member->explicitly_implemented_interface_members();
        if (member->is_explicit_interface_implementation() && implemented.size() == 1) {
            // a C#-style explicit interface implementation
            member = implemented.front();
            result.push_back(member);
        }
    }
    // remove the generic specialization
    const TypeParameterSubstitution substitution = member->substitution();
    member = member->member_definition();
    const ITypeDefinition* declaring = member->declaring_type_definition();
    if (declaring == nullptr) return result;   // a global method
    const std::vector<const IType*> all_base_types = include_implemented_interfaces ? get_all_base_types(declaring) : get_non_interface_base_types(declaring);
    const std::string name = member->name();
    for (auto it = all_base_types.rbegin(); it != all_base_types.rend(); ++it) {
        const IType* base_type = *it;
        if (base_type == static_cast<const IType*>(declaring)) continue;
        std::vector<const IMember*> base_members;
        if (member->symbol_kind() == SymbolKind::Accessor) {
            for (const IMethod* m : base_type->get_accessors(
                     [&](const IMethod* m) { return m->name() == name && m->accessibility() > Accessibility::Private; }, GetMemberOptions::IgnoreInheritedMembers))
                base_members.push_back(m);
        } else {
            base_members = base_type->get_members([&](const IMember* m) { return m->name() == name && m->accessibility() > Accessibility::Private; },
                                                  GetMemberOptions::IgnoreInheritedMembers);
        }
        for (const IMember* base_member : base_members)
            if (signature_equals(member, base_member)) result.push_back(base_member->specialize(substitution));
    }
    return result;
}

const IMember* get_base_member(const IMember* member) {
    const std::vector<const IMember*> base_members = get_base_members(member, false);
    return base_members.empty() ? nullptr : base_members.front();
}

const IMember* get_derived_member(const IMember* base_member, const ITypeDefinition* derived_type) {
    if (base_member == nullptr || derived_type == nullptr) throw UsageError("get_derived_member: a null argument");
    if (&base_member->compilation() != &derived_type->compilation()) throw UsageError("baseMember and derivedType must be from the same compilation");
    base_member = base_member->member_definition();
    const bool include_interfaces = base_member->declaring_type_definition() != nullptr && base_member->declaring_type_definition()->kind() == TypeKind::Interface;
    const auto overrides = [&](const IMember* derived) {
        for (const IMember* m : get_base_members(derived, include_interfaces))
            if (m->member_definition() == base_member) return true;
        return false;
    };
    if (const auto* method = dynamic_cast<const IMethod*>(base_member)) {
        for (const IMethod* derived_method : derived_type->methods())
            if (derived_method->name() == method->name() && derived_method->parameters().size() == method->parameters().size() &&
                derived_method->type_parameters().size() == method->type_parameters().size() && overrides(derived_method))
                return derived_method;
    }
    if (const auto* property = dynamic_cast<const IProperty*>(base_member)) {
        for (const IProperty* derived_property : derived_type->properties())
            if (derived_property->name() == property->name() && derived_property->parameters().size() == property->parameters().size() && overrides(derived_property))
                return derived_property;
    }
    if (dynamic_cast<const IEvent*>(base_member) != nullptr) {
        for (const IEvent* derived_event : derived_type->events())
            if (derived_event->name() == base_member->name()) return derived_event;
    }
    if (dynamic_cast<const IField*>(base_member) != nullptr) {
        for (const IField* derived_field : derived_type->fields())
            if (derived_field->name() == base_member->name()) return derived_field;
    }
    return nullptr;
}

std::vector<const IAttribute*> get_attributes(const ITypeDefinition* type_def) {
    std::vector<const IAttribute*> result;
    const std::vector<const IType*> base_types = get_non_interface_base_types(type_def);
    for (auto it = base_types.rbegin(); it != base_types.rend(); ++it) {
        const ITypeDefinition* base_type_def = (*it)->get_definition();
        if (base_type_def == nullptr) continue;
        for (const IAttribute* attr : base_type_def->get_attributes()) result.push_back(attr);
    }
    return result;
}

const IAttribute* get_attribute(const ITypeDefinition* type_def, KnownAttribute attribute_type) {
    const std::vector<const IType*> base_types = get_non_interface_base_types(type_def);
    for (auto it = base_types.rbegin(); it != base_types.rend(); ++it) {
        const ITypeDefinition* base_type_def = (*it)->get_definition();
        if (base_type_def == nullptr) continue;
        if (const IAttribute* attr = base_type_def->get_attribute(attribute_type)) return attr;
    }
    return nullptr;
}

std::vector<const IAttribute*> get_attributes(const IMember* member) {
    std::vector<const IAttribute*> result;
    std::unordered_set<const IMember*> visited_members;
    for (;;) {
        member = member->member_definition();   // the definitions suffice
        if (!visited_members.insert(member).second) break;   // cyclic inheritance
        for (const IAttribute* attr : member->get_attributes()) result.push_back(attr);
        if (!member->is_override()) break;
        const IMember* base_member = get_base_member(member);
        if (base_member == nullptr) break;
        member = base_member;
    }
    return result;
}

const IAttribute* get_attribute(const IMember* member, KnownAttribute attribute_type) {
    std::unordered_set<const IMember*> visited_members;
    for (;;) {
        member = member->member_definition();
        if (!visited_members.insert(member).second) break;
        if (const IAttribute* attr = member->get_attribute(attribute_type)) return attr;
        if (!member->is_override()) break;
        const IMember* base_member = get_base_member(member);
        if (base_member == nullptr) break;
        member = base_member;
    }
    return nullptr;
}

}  // namespace inheritance_helper

}  // namespace cb::dotnet::decompiler::typesystem
