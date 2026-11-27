#include "dotnet/decompiler/csharp/resolver/member_lookup.hpp"

#include <algorithm>
#include <cassert>
#include <stdexcept>
#include <unordered_map>

#include "dotnet/decompiler/semantics/ambiguous_resolve_result.hpp"
#include "dotnet/decompiler/semantics/member_resolve_result.hpp"
#include "dotnet/decompiler/semantics/this_resolve_result.hpp"
#include "dotnet/decompiler/semantics/type_resolve_result.hpp"
#include "dotnet/decompiler/semantics/unknown_member_resolve_result.hpp"
#include "dotnet/decompiler/typesystem/inheritance_helper.hpp"
#include "dotnet/decompiler/util/insertion_ordered_map.hpp"
#include "dotnet/decompiler/typesystem/type_system_extensions.hpp"
#include "dotnet/decompiler/typesystem/types.hpp"

namespace cb::dotnet::decompiler::csharp::resolver {

using namespace typesystem;
using semantics::AmbiguousMemberResolveResult;
using semantics::AmbiguousTypeResolveResult;
using semantics::MemberResolveResult;
using semantics::ThisResolveResult;
using semantics::TypeResolveResult;
using semantics::UnknownMemberResolveResult;

// ---- static helpers --------------------------------------------------------------------------------------------------------------

bool MemberLookup::is_invocable(const IMember* member) {
    if (member == nullptr) throw std::invalid_argument("member");
    // C# 4.0 spec, 7.4 member lookup
    if (dynamic_cast<const IEvent*>(member) != nullptr || dynamic_cast<const IMethod*>(member) != nullptr) return true;
    const IType* return_type = member->return_type();
    return return_type->kind() == TypeKind::Dynamic || return_type->kind() == TypeKind::Delegate || return_type->kind() == TypeKind::FunctionPointer;
}

bool MemberLookup::BaseTypes::contains(const IType* type) const {
    for (const IType* candidate : types) {
        if (type_equals(candidate, type)) return true;
    }
    return false;
}

// ---- IsAccessible ----------------------------------------------------------------------------------------------------------------

bool MemberLookup::is_protected_access_allowed(ResolveResult* target_resolve_result) const {
    return dynamic_cast<ThisResolveResult*>(target_resolve_result) != nullptr || is_protected_access_allowed(target_resolve_result->type());
}

bool MemberLookup::is_protected_access_allowed(const IType* target_type) const {
    if (target_type->kind() == TypeKind::TypeParameter) target_type = dynamic_cast<const ITypeParameter*>(target_type)->effective_base_class();
    const ITypeDefinition* type_def = target_type->get_definition();
    if (type_def == nullptr) return false;
    for (const ITypeDefinition* c = current_type_definition_; c != nullptr; c = c->declaring_type_definition()) {
        if (is_derived_from(type_def, c)) return true;
    }
    return false;
}

bool MemberLookup::is_accessible(const IEntity* entity, bool allow_protected_access) const {
    if (entity == nullptr) throw std::invalid_argument("entity");
    // C# 4.0 spec, 3.5.2 Accessibility domains
    switch (entity->accessibility()) {
        case Accessibility::None:
            return false;
        case Accessibility::Private:
            // check for members of outer classes (private members of outer classes can be accessed)
            for (const ITypeDefinition* t = current_type_definition_; t != nullptr; t = t->declaring_type_definition()) {
                if (type_equals(t, entity->declaring_type_definition())) return true;
            }
            return false;
        case Accessibility::Public:
            return true;
        case Accessibility::Protected:
            return is_protected_accessible(allow_protected_access, entity);
        case Accessibility::Internal:
            return is_internal_accessible(entity->parent_module());
        case Accessibility::ProtectedOrInternal:
            return is_internal_accessible(entity->parent_module()) || is_protected_accessible(allow_protected_access, entity);
        case Accessibility::ProtectedAndInternal:
            return is_internal_accessible(entity->parent_module()) && is_protected_accessible(allow_protected_access, entity);
        default:
            throw std::invalid_argument("Invalid value for Accessibility");
    }
}

bool MemberLookup::is_internal_accessible(const IModule* module) const {
    return module != nullptr && current_module_ != nullptr && module->internals_visible_to(*current_module_);
}

bool MemberLookup::is_protected_accessible(bool allow_protected_access, const IEntity* entity) const {
    // For static members and type definitions, we do not require the qualifying reference to be derived from the current class
    // (allow_protected_access).
    if (entity->is_static() || entity->symbol_kind() == SymbolKind::TypeDefinition) allow_protected_access = true;

    for (const ITypeDefinition* t = current_type_definition_; t != nullptr; t = t->declaring_type_definition()) {
        if (type_equals(t, entity->declaring_type_definition())) return true;

        // PERF (upstream): this is called several times, once for each member; resolving base types has to be cheap.
        if (allow_protected_access && is_derived_from(t, entity->declaring_type_definition())) return true;
    }
    return false;
}

// ---- GetAccessibleMembers --------------------------------------------------------------------------------------------------------

std::vector<const IEntity*> MemberLookup::get_accessible_members(ResolveResult* target_resolve_result) const {
    if (target_resolve_result == nullptr) throw std::invalid_argument("targetResolveResult");

    std::vector<const IEntity*> result;
    const bool target_is_type_parameter = target_resolve_result->type()->kind() == TypeKind::TypeParameter;
    const bool allow_protected_access = is_protected_access_allowed(target_resolve_result);

    // maps the member name to the list of lookup groups
    util::InsertionOrderedMap<std::string, std::vector<LookupGroup>> lookup_group_dict;

    // This loop will handle base types before derived types. The loop performs three jobs:
    // 1) It marks entries in lookup groups from base classes as removed when those members are hidden by a derived class.
    // 2) It adds a new lookup group with the members from a declaring type.
    // 3) It replaces virtual members with the overridden version, placing the override in the lookup group belonging to the base class.
    for (const IType* type : get_non_interface_base_types(target_resolve_result->type())) {
        std::vector<const IEntity*> entities;
        for (const IMember* member : type->get_members({}, GetMemberOptions::IgnoreInheritedMembers)) entities.push_back(member);
        if (!target_is_type_parameter) {
            const std::vector<const IType*> nested_types =
                type->get_nested_types({}, GetMemberOptions::IgnoreInheritedMembers | GetMemberOptions::ReturnMemberDefinitions);
            // get_definition() might return null if some IType has a strange implementation of get_nested_types.
            for (const IType* nested : nested_types) {
                if (const ITypeDefinition* td = nested->get_definition(); td != nullptr) entities.push_back(td);
            }
        }

        // entities.GroupBy(e => e.Name): groups in the order their names first appear, members in their order
        std::vector<std::pair<std::string, std::vector<const IEntity*>>> entity_groups;
        {
            std::unordered_map<std::string, std::size_t> index;
            for (const IEntity* entity : entities) {
                std::string name = entity->name();
                const auto it = index.find(name);
                if (it == index.end()) {
                    index.emplace(name, entity_groups.size());
                    entity_groups.emplace_back(std::move(name), std::vector<const IEntity*>{entity});
                } else {
                    entity_groups[it->second].second.push_back(entity);
                }
            }
        }

        for (const auto& [group_name, entity_group] : entity_groups) {
            std::vector<LookupGroup>& lookup_groups = lookup_group_dict.get_or_add(group_name);

            std::vector<const IType*> new_nested_types;
            std::vector<const IParameterizedMember*> new_methods;
            const IMember* new_non_method = nullptr;

            BaseTypes type_base_types;

            if (!target_is_type_parameter) {
                std::vector<const IType*> group_types;
                for (const IEntity* entity : entity_group) {
                    if (const IType* t = dynamic_cast<const IType*>(entity); t != nullptr) group_types.push_back(t);
                }
                add_nested_types(type, group_types, 0, lookup_groups, type_base_types, new_nested_types);
            }
            std::vector<const IMember*> group_members;
            for (const IEntity* entity : entity_group) {
                if (const IMember* m = dynamic_cast<const IMember*>(entity); m != nullptr) group_members.push_back(m);
            }
            add_members(type, group_members, allow_protected_access, lookup_groups, false, type_base_types, new_methods, new_non_method);

            if (!new_nested_types.empty() || !new_methods.empty() || new_non_method != nullptr)
                lookup_groups.emplace_back(type, std::move(new_nested_types), std::move(new_methods), new_non_method);
        }
    }

    for (auto& [group_name, lookup_groups] : lookup_group_dict.items()) {
        // Remove interface members hidden by class members.
        if (target_is_type_parameter) {
            // This can happen only with type parameters.
            remove_interface_members_hidden_by_class_members(lookup_groups);
        }

        // Now report the results:
        for (const LookupGroup& lookup_group : lookup_groups) {
            if (!lookup_group.methods_are_hidden) {
                for (const IParameterizedMember* method : lookup_group.methods) result.push_back(dynamic_cast<const IMethod*>(method));
            }
            if (!lookup_group.non_method_is_hidden) result.push_back(lookup_group.non_method);
            for (const IType* type : lookup_group.nested_types) {
                if (const ITypeDefinition* type_def = type->get_definition(); type_def != nullptr) result.push_back(type_def);
            }
        }
    }
    return result;
}

// ---- LookupType ------------------------------------------------------------------------------------------------------------------

ResolveResult* MemberLookup::lookup_type(const IType* declaring_type, const std::string& name, const std::vector<const IType*>& type_arguments,
                                         bool parameterize_result_type) const {
    if (declaring_type == nullptr) throw std::invalid_argument("declaringType");

    const int type_argument_count = static_cast<int>(type_arguments.size());
    const Filter<ITypeDefinition> filter = [&](const ITypeDefinition* d) {
        return inner_type_parameter_count(d) == type_argument_count && d->name() == name && is_accessible(d, true);
    };

    std::vector<LookupGroup> lookup_groups;
    if (declaring_type->kind() != TypeKind::TypeParameter) {
        for (const IType* type : get_non_interface_base_types(declaring_type)) {
            std::vector<const IType*> new_nested_types;
            BaseTypes type_base_types;

            std::vector<const IType*> nested_types;
            if (parameterize_result_type) {
                nested_types = type->get_nested_types(type_arguments, filter, GetMemberOptions::IgnoreInheritedMembers);
            } else {
                nested_types = type->get_nested_types(filter, GetMemberOptions::IgnoreInheritedMembers | GetMemberOptions::ReturnMemberDefinitions);
            }
            add_nested_types(type, nested_types, type_argument_count, lookup_groups, type_base_types, new_nested_types);

            if (!new_nested_types.empty()) lookup_groups.emplace_back(type, std::move(new_nested_types), std::vector<const IParameterizedMember*>{}, nullptr);
        }
    }

    std::erase_if(lookup_groups, [](const LookupGroup& g) { return g.all_hidden(); });
    assert(std::all_of(lookup_groups.begin(), lookup_groups.end(), [](const LookupGroup& g) { return !g.nested_types.empty(); }));

    if (lookup_groups.empty()) return make<UnknownMemberResolveResult>(declaring_type, name, type_arguments);

    const LookupGroup& result_group = lookup_groups.back();

    if (result_group.nested_types.size() > 1 || lookup_groups.size() > 1) return make<AmbiguousTypeResolveResult>(result_group.nested_types[0]);
    return make<TypeResolveResult>(result_group.nested_types[0]);
}

int MemberLookup::inner_type_parameter_count(const IType* type) {
    // inner types contain the type parameters of outer types. therefore this count has to been adjusted.
    return type->type_parameter_count() - (type->declaring_type() != nullptr ? type->declaring_type()->type_parameter_count() : 0);
}

// ---- Lookup ----------------------------------------------------------------------------------------------------------------------

ResolveResult* MemberLookup::lookup(ResolveResult* target_resolve_result, const std::string& name, const std::vector<const IType*>& type_arguments,
                                    bool is_invocation) const {
    if (target_resolve_result == nullptr) throw std::invalid_argument("targetResolveResult");

    const bool target_is_type_parameter = target_resolve_result->type()->kind() == TypeKind::TypeParameter;

    const bool allow_protected_access = is_protected_access_allowed(target_resolve_result);
    const Filter<ITypeDefinition> nested_type_filter = [&](const ITypeDefinition* entity) {
        return entity->name() == name && is_accessible(entity, allow_protected_access);
    };
    const Filter<IMember> member_filter = [&](const IMember* entity) {
        // NOTE (upstream): Atm destructors can be looked up with 'Finalize'
        return entity->symbol_kind() != SymbolKind::Indexer && entity->symbol_kind() != SymbolKind::Operator && entity->name() == name;
    };

    std::vector<LookupGroup> lookup_groups;
    // This loop will handle base types before derived types. The loop performs three jobs:
    // 1) It marks entries in lookup groups from base classes as removed when those members are hidden by a derived class.
    // 2) It adds a new lookup group with the members from a declaring type.
    // 3) It replaces virtual members with the overridden version, placing the override in the lookup group belonging to the base class.
    for (const IType* type : get_non_interface_base_types(target_resolve_result->type())) {
        std::vector<const IType*> new_nested_types;
        std::vector<const IParameterizedMember*> new_methods;
        const IMember* new_non_method = nullptr;

        BaseTypes type_base_types;

        if (!is_invocation && !target_is_type_parameter) {
            // Consider nested types only if it's not an invocation. type->get_nested_types() is checking the type parameter count for an
            // exact match, so we don't need to do that in our filter.
            const std::vector<const IType*> nested_types = type->get_nested_types(type_arguments, nested_type_filter, GetMemberOptions::IgnoreInheritedMembers);
            add_nested_types(type, nested_types, static_cast<int>(type_arguments.size()), lookup_groups, type_base_types, new_nested_types);
        }

        std::vector<const IMember*> members;
        if (type_arguments.empty()) {
            // Note: the IsInvocable check cannot be done as part of the filter, because it must be done after type substitution.
            members = type->get_members(member_filter, GetMemberOptions::IgnoreInheritedMembers);
            if (is_invocation) {
                std::erase_if(members, [](const IMember* m) { return !is_invocable(m); });
            }
        } else {
            // No need to check for is_invocation/is_invocable here: we only fetch methods
            const Filter<IMethod> method_filter = [&](const IMethod* m) { return member_filter(m); };
            for (const IMethod* method : type->get_methods(type_arguments, method_filter, GetMemberOptions::IgnoreInheritedMembers)) members.push_back(method);
        }
        add_members(type, members, allow_protected_access, lookup_groups, false, type_base_types, new_methods, new_non_method);

        if (!new_nested_types.empty() || !new_methods.empty() || new_non_method != nullptr)
            lookup_groups.emplace_back(type, std::move(new_nested_types), std::move(new_methods), new_non_method);
    }

    // Remove interface members hidden by class members.
    if (target_is_type_parameter) {
        // This can happen only with type parameters.
        remove_interface_members_hidden_by_class_members(lookup_groups);
    }

    return create_result(target_resolve_result, lookup_groups, name, type_arguments);
}

// ---- Lookup Indexer --------------------------------------------------------------------------------------------------------------

std::vector<MethodListWithDeclaringType> MemberLookup::lookup_indexers(ResolveResult* target_resolve_result) const {
    if (target_resolve_result == nullptr) throw std::invalid_argument("targetResolveResult");

    const IType* target_type = target_resolve_result->type();
    const bool allow_protected_access = is_protected_access_allowed(target_resolve_result);
    const Filter<IProperty> filter = [](const IProperty* p) { return p->is_indexer() && !p->is_explicit_interface_implementation(); };

    std::vector<LookupGroup> lookup_groups;
    for (const IType* type : get_non_interface_base_types(target_type)) {
        std::vector<const IParameterizedMember*> new_methods;
        const IMember* new_non_method = nullptr;

        BaseTypes type_base_types;

        std::vector<const IMember*> members;
        for (const IProperty* property : type->get_properties(filter, GetMemberOptions::IgnoreInheritedMembers)) members.push_back(property);
        add_members(type, members, allow_protected_access, lookup_groups, true, type_base_types, new_methods, new_non_method);

        if (!new_methods.empty() || new_non_method != nullptr)
            lookup_groups.emplace_back(type, std::vector<const IType*>{}, std::move(new_methods), new_non_method);
    }

    // Remove interface members hidden by class members.
    if (target_type->kind() == TypeKind::TypeParameter) {
        // This can happen only with type parameters.
        remove_interface_members_hidden_by_class_members(lookup_groups);
    }

    // Remove all hidden groups
    std::erase_if(lookup_groups, [](const LookupGroup& g) { return g.methods_are_hidden || g.methods.empty(); });

    std::vector<MethodListWithDeclaringType> method_lists;
    method_lists.reserve(lookup_groups.size());
    for (const LookupGroup& group : lookup_groups) method_lists.emplace_back(group.declaring_type, group.methods);
    return method_lists;
}

// ---- AddNestedTypes --------------------------------------------------------------------------------------------------------------

// Adds the nested types to new_nested_types and removes any hidden members from the existing lookup groups.
void MemberLookup::add_nested_types(const IType* type, const std::vector<const IType*>& nested_types, int type_argument_count,
                                    std::vector<LookupGroup>& lookup_groups, BaseTypes& type_base_types,
                                    std::vector<const IType*>& new_nested_types) const {
    for (const IType* nested_type : nested_types) {
        // Remove all non-types declared in a base type of 'type', and all types with same number of type parameters declared in a base
        // type of 'type'.
        for (LookupGroup& lookup_group : lookup_groups) {
            if (lookup_group.all_hidden()) continue;  // everything is already hidden
            if (!type_base_types.made) {
                type_base_types.types = get_non_interface_base_types(type);
                type_base_types.made = true;
            }

            if (type_base_types.contains(lookup_group.declaring_type)) {
                lookup_group.methods_are_hidden = true;
                lookup_group.non_method_is_hidden = true;
                std::erase_if(lookup_group.nested_types, [&](const IType* t) { return inner_type_parameter_count(t) == type_argument_count; });
            }
        }

        // Add the new nested type.
        new_nested_types.push_back(nested_type);
    }
}

// ---- AddMembers ------------------------------------------------------------------------------------------------------------------

// Adds members to new_methods / new_non_method. Removes any members in the existing lookup groups that were hidden by added members.
// Substitutes 'virtual' members in the existing lookup groups for added 'override' members.
void MemberLookup::add_members(const IType* type, const std::vector<const IMember*>& members, bool allow_protected_access,
                               std::vector<LookupGroup>& lookup_groups, bool treat_all_parameterized_members_as_methods, BaseTypes& type_base_types,
                               std::vector<const IParameterizedMember*>& new_methods, const IMember*& new_non_method) const {
    for (const IMember* member : members) {
        if (!is_accessible(member, allow_protected_access)) continue;

        const IParameterizedMember* method;
        if (treat_all_parameterized_members_as_methods)
            method = dynamic_cast<const IParameterizedMember*>(member);
        else
            method = dynamic_cast<const IMethod*>(member);

        bool replaced_virtual_member_with_override = false;
        if (member->is_override()) {
            // Replacing virtual member with override:

            // Go backwards so that we find the corresponding virtual member in the most-derived type
            for (int i = static_cast<int>(lookup_groups.size()) - 1; i >= 0 && !replaced_virtual_member_with_override; i--) {
                if (!type_base_types.made) {
                    type_base_types.types = get_non_interface_base_types(type);
                    type_base_types.made = true;
                }

                LookupGroup& lookup_group = lookup_groups[static_cast<std::size_t>(i)];
                if (type_base_types.contains(lookup_group.declaring_type)) {
                    if (method != nullptr && !lookup_group.methods_are_hidden) {
                        // Find the matching method, and replace it with the override
                        for (std::size_t j = 0; j < lookup_group.methods.size(); j++) {
                            if (signature_equals(method, lookup_group.methods[j])) {
                                lookup_group.methods[j] = method;
                                replaced_virtual_member_with_override = true;
                                break;
                            }
                        }
                    } else {
                        // If the member type matches, replace it with the override
                        if (lookup_group.non_method != nullptr && lookup_group.non_method->symbol_kind() == member->symbol_kind()) {
                            lookup_group.non_method = member;
                            replaced_virtual_member_with_override = true;
                            break;
                        }
                    }
                }
            }
        }
        // If the member wasn't an override, or if we didn't find any matching virtual method, proceed to add the member.
        if (!replaced_virtual_member_with_override) {
            // Make the member hide other members:
            for (LookupGroup& lookup_group : lookup_groups) {
                if (lookup_group.all_hidden()) continue;  // everything is already hidden
                if (!type_base_types.made) {
                    type_base_types.types = get_non_interface_base_types(type);
                    type_base_types.made = true;
                }

                if (type_base_types.contains(lookup_group.declaring_type)) {
                    // Methods hide all non-methods; Non-methods hide everything
                    lookup_group.nested_types.clear();
                    lookup_group.non_method_is_hidden = true;
                    if (method == nullptr) {  // !(member is IMethod)
                        lookup_group.methods_are_hidden = true;
                    }
                }
            }

            // Add the new member
            if (method != nullptr)
                new_methods.push_back(method);
            else
                new_non_method = member;
        }
    }
}

// ---- RemoveInterfaceMembersHiddenByClassMembers ----------------------------------------------------------------------------------

void MemberLookup::remove_interface_members_hidden_by_class_members(std::vector<LookupGroup>& lookup_groups) {
    for (const LookupGroup& class_lookup_group : lookup_groups) {
        if (is_interface_or_system_object(class_lookup_group.declaring_type)) continue;
        // The current lookup groups contains class members that might hide interface members
        const bool has_nested_types = !class_lookup_group.nested_types.empty();
        if (has_nested_types || !class_lookup_group.non_method_is_hidden) {
            // Hide all members from interface types
            for (LookupGroup& interface_lookup_group : lookup_groups) {
                if (is_interface_or_system_object(interface_lookup_group.declaring_type)) {
                    interface_lookup_group.nested_types.clear();
                    interface_lookup_group.non_method_is_hidden = true;
                    interface_lookup_group.methods_are_hidden = true;
                }
            }
        } else if (!class_lookup_group.methods_are_hidden) {
            for (const IParameterizedMember* class_method : class_lookup_group.methods) {
                // Hide all non-methods from interface types, and all methods with the same signature as a method in this class type.
                for (LookupGroup& interface_lookup_group : lookup_groups) {
                    if (is_interface_or_system_object(interface_lookup_group.declaring_type)) {
                        interface_lookup_group.nested_types.clear();
                        interface_lookup_group.non_method_is_hidden = true;
                        if (!interface_lookup_group.methods.empty() && !interface_lookup_group.methods_are_hidden) {
                            // The mapping of virtual to overridden methods is already done, so we can simply remove the methods from the
                            // collection
                            std::erase_if(interface_lookup_group.methods, [&](const IParameterizedMember* m) { return signature_equals(class_method, m); });
                        }
                    }
                }
            }
        }
    }
}

bool MemberLookup::is_interface_or_system_object(const IType* type) {
    // return true if type is an interface or System.Object
    if (type->kind() == TypeKind::Interface) return true;
    const ITypeDefinition* d = type->get_definition();
    return d != nullptr && d->known_type_code() == KnownTypeCode::Object;
}

// ---- CreateResult ----------------------------------------------------------------------------------------------------------------

ResolveResult* MemberLookup::create_result(ResolveResult* target_resolve_result, std::vector<LookupGroup>& lookup_groups, const std::string& name,
                                           const std::vector<const IType*>& type_arguments) const {
    // Remove all hidden groups
    std::erase_if(lookup_groups, [](const LookupGroup& g) { return g.all_hidden(); });

    if (lookup_groups.empty()) {
        // No members found
        return make<UnknownMemberResolveResult>(target_resolve_result->type(), name, type_arguments);
    }

    if (std::any_of(lookup_groups.begin(), lookup_groups.end(), [](const LookupGroup& g) { return !g.methods_are_hidden && !g.methods.empty(); })) {
        // If there are methods, make a MethodGroupResolveResult. Note that a conflict between a member and a method (possible with
        // multiple interface inheritance) is only a warning, not an error, and the C# compiler will prefer the method group.
        std::vector<MethodListWithDeclaringType> method_lists;
        for (const LookupGroup& lookup_group : lookup_groups) {
            if (!lookup_group.methods_are_hidden && !lookup_group.methods.empty()) {
                MethodListWithDeclaringType method_list_with_decl_type(lookup_group.declaring_type);
                for (const IParameterizedMember* method : lookup_group.methods) method_list_with_decl_type.push_back(method);
                method_lists.push_back(std::move(method_list_with_decl_type));
            }
        }

        return make<MethodGroupResolveResult>(target_resolve_result, name, std::move(method_lists), type_arguments);
    }

    // If there are ambiguities, report the most-derived result (last group)
    const LookupGroup& result_group = lookup_groups.back();
    if (!result_group.nested_types.empty()) {
        if (result_group.nested_types.size() > 1 || !result_group.non_method_is_hidden || lookup_groups.size() > 1)
            return make<AmbiguousTypeResolveResult>(result_group.nested_types[0]);
        return make<TypeResolveResult>(result_group.nested_types[0]);
    }

    if (result_group.non_method->is_static() && dynamic_cast<ThisResolveResult*>(target_resolve_result) != nullptr) {
        target_resolve_result = make<TypeResolveResult>(target_resolve_result->type());
    }

    if (lookup_groups.size() > 1) return make<AmbiguousMemberResolveResult>(target_resolve_result, result_group.non_method);

    if (is_in_enum_member_initializer_) {
        const IField* field = dynamic_cast<const IField*>(result_group.non_method);
        if (field != nullptr && field->declaring_type_definition() != nullptr && field->declaring_type_definition()->kind() == TypeKind::Enum) {
            return make<MemberResolveResult>(target_resolve_result, field, field->declaring_type_definition()->enum_underlying_type(), field->is_const(),
                                             field->get_constant_value());
        }
    }
    return make<MemberResolveResult>(target_resolve_result, result_group.non_method);
}

}  // namespace cb::dotnet::decompiler::csharp::resolver
