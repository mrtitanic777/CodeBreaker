// dotnet/decompiler/csharp/resolver/member_lookup.hpp -- member lookup (C# 4.0 spec, 7.4): which members and nested types a name means on
// a target, from where the code is (accessibility, hiding by derived classes, overrides replacing virtual members), and which indexers
// a target has.
//
//     MemberLookup lookup(current_type_definition, compilation.main_module(), false);
//     ResolveResult* rr = lookup.lookup(target, "Add", {}, /*is_invocation*/ true);    // a MethodGroupResolveResult
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.Resolver: MemberLookup, from MemberLookup.cs), Copyright AlphaSierraPapa for the ILSpy
// team, MIT (see NOTICE in dotnet/decompiler). Upstream's lazily evaluated lists (GetAccessibleMembers' yield return, the base types of a
// type) are made when asked, in the same order.
#pragma once

#include <string>
#include <vector>

#include "dotnet/decompiler/csharp/resolver/method_group_resolve_result.hpp"
#include "dotnet/decompiler/semantics/resolve_result.hpp"

namespace cb::dotnet::decompiler::csharp::resolver {

class MemberLookup {
public:
    // Whether the member is considered to be invocable (an event, a method, or a member of dynamic, delegate or function pointer type).
    static bool is_invocable(const typesystem::IMember* member);

    // The definition and module the code is in (either may be null); in an enum member initializer a member of the enum is a constant of
    // the enum's underlying type.
    MemberLookup(const typesystem::ITypeDefinition* current_type_definition, const typesystem::IModule* current_module,
                 bool is_in_enum_member_initializer = false)
        : current_type_definition_(current_type_definition),
          current_module_(current_module),
          is_in_enum_member_initializer_(is_in_enum_member_initializer) {}

    // Whether access to protected instance members of the target expression is possible.
    bool is_protected_access_allowed(ResolveResult* target_resolve_result) const;
    // Whether access to protected instance members of the target type is possible. This does not consider the special case of the 'base'
    // reference: use the overload taking the ResolveResult when possible.
    bool is_protected_access_allowed(const typesystem::IType* target_type) const;
    // Whether the entity is accessible in the current class. allow_protected_access: whether protected access to instance members is
    // allowed (true if the type of the reference is derived from the current class; protected static members may be accessible even if false).
    bool is_accessible(const typesystem::IEntity* entity, bool allow_protected_access) const;

    // All members that are accessible and not hidden (by being overridden or shadowed): members and nested type definitions, no
    // extension methods.
    std::vector<const typesystem::IEntity*> get_accessible_members(ResolveResult* target_resolve_result) const;

    // Looks up a nested type by name.
    ResolveResult* lookup_type(const typesystem::IType* declaring_type, const std::string& name,
                               const std::vector<const typesystem::IType*>& type_arguments, bool parameterize_result_type = true) const;
    // Performs a member lookup.
    ResolveResult* lookup(ResolveResult* target_resolve_result, const std::string& name, const std::vector<const typesystem::IType*>& type_arguments,
                          bool is_invocation) const;
    // Looks up the indexers on the target type, grouped by declaring type.
    std::vector<MethodListWithDeclaringType> lookup_indexers(ResolveResult* target_resolve_result) const;

private:
    // What one type contributes to a lookup: its nested types, its methods, its other member, and which of them a derived type hides.
    struct LookupGroup {
        const typesystem::IType* declaring_type = nullptr;
        // When a nested type is hidden, it is simply removed from the list.
        std::vector<const typesystem::IType*> nested_types;
        // When members are hidden they are only marked as hidden: the 'override' processing still needs them to find the virtual member
        // an override replaces.
        std::vector<const typesystem::IParameterizedMember*> methods;
        bool methods_are_hidden = false;
        const typesystem::IMember* non_method = nullptr;
        bool non_method_is_hidden = false;

        LookupGroup(const typesystem::IType* declaring_type, std::vector<const typesystem::IType*> nested_types,
                    std::vector<const typesystem::IParameterizedMember*> methods, const typesystem::IMember* non_method)
            : declaring_type(declaring_type),
              nested_types(std::move(nested_types)),
              methods(std::move(methods)),
              methods_are_hidden(this->methods.empty()),
              non_method(non_method),
              non_method_is_hidden(non_method == nullptr) {}

        bool all_hidden() const { return nested_types.empty() && non_method_is_hidden && methods_are_hidden; }
    };

    // The base types of a type that are not interfaces, made on the first need (upstream's lazy `typeBaseTypes`).
    struct BaseTypes {
        bool made = false;
        std::vector<const typesystem::IType*> types;
        bool contains(const typesystem::IType* type) const;
    };

    bool is_internal_accessible(const typesystem::IModule* module) const;
    bool is_protected_accessible(bool allow_protected_access, const typesystem::IEntity* entity) const;

    void add_nested_types(const typesystem::IType* type, const std::vector<const typesystem::IType*>& nested_types, int type_argument_count,
                          std::vector<LookupGroup>& lookup_groups, BaseTypes& type_base_types,
                          std::vector<const typesystem::IType*>& new_nested_types) const;
    void add_members(const typesystem::IType* type, const std::vector<const typesystem::IMember*>& members, bool allow_protected_access,
                     std::vector<LookupGroup>& lookup_groups, bool treat_all_parameterized_members_as_methods, BaseTypes& type_base_types,
                     std::vector<const typesystem::IParameterizedMember*>& new_methods, const typesystem::IMember*& new_non_method) const;
    static void remove_interface_members_hidden_by_class_members(std::vector<LookupGroup>& lookup_groups);
    static bool is_interface_or_system_object(const typesystem::IType* type);
    ResolveResult* create_result(ResolveResult* target_resolve_result, std::vector<LookupGroup>& lookup_groups, const std::string& name,
                                 const std::vector<const typesystem::IType*>& type_arguments) const;
    // The number of type parameters a type declares itself (inner types carry the type parameters of outer types).
    static int inner_type_parameter_count(const typesystem::IType* type);

    const typesystem::ITypeDefinition* current_type_definition_;
    const typesystem::IModule* current_module_;
    bool is_in_enum_member_initializer_;
};

}  // namespace cb::dotnet::decompiler::csharp::resolver
