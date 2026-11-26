// dotnet/decompiler/typesystem/get_members_helper.hpp -- the members of a type with those of its base classes: each base type's own
// members, specialized for the type arguments where the base type is a generic instance.
//
// Ported from ILSpy (ICSharpCode.Decompiler.TypeSystem.Implementation: GetMembersHelper), Copyright AlphaSierraPapa for the ILSpy team, MIT
// (see NOTICE in dotnet/decompiler). A null list of type arguments (nullptr) is upstream's null.
#pragma once

#include <vector>

#include "dotnet/decompiler/typesystem/types.hpp"

namespace cb::dotnet::decompiler::typesystem::get_members_helper {

std::vector<const IType*> get_nested_types(const IType* type, const Filter<ITypeDefinition>& filter, GetMemberOptions options);
std::vector<const IType*> get_nested_types(const IType* type, const std::vector<const IType*>* nested_type_arguments, const Filter<ITypeDefinition>& filter,
                                           GetMemberOptions options);
std::vector<const IMethod*> get_methods(const IType* type, const Filter<IMethod>& filter, GetMemberOptions options);
std::vector<const IMethod*> get_methods(const IType* type, const std::vector<const IType*>* type_arguments, const Filter<IMethod>& filter, GetMemberOptions options);
std::vector<const IMethod*> get_accessors(const IType* type, const Filter<IMethod>& filter, GetMemberOptions options);
std::vector<const IMethod*> get_constructors(const IType* type, const Filter<IMethod>& filter, GetMemberOptions options);
std::vector<const IProperty*> get_properties(const IType* type, const Filter<IProperty>& filter, GetMemberOptions options);
std::vector<const IField*> get_fields(const IType* type, const Filter<IField>& filter, GetMemberOptions options);
std::vector<const IEvent*> get_events(const IType* type, const Filter<IEvent>& filter, GetMemberOptions options);
std::vector<const IMember*> get_members(const IType* type, const Filter<IMember>& filter, GetMemberOptions options);

}  // namespace cb::dotnet::decompiler::typesystem::get_members_helper
