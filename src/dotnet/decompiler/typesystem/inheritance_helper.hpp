// dotnet/decompiler/typesystem/inheritance_helper.hpp -- which members of base types a member overrides or hides, the member of a derived
// type that overrides a base member, and the comparison of signatures that decides it (same kind, name and parameter types, method type
// parameters compared by position).
//
//     for (const IMember* m : get_base_members(method, false)) ...      // what `method` overrides, nearest base first
//     signature_equals(a, b);                                             // SignatureComparer.Ordinal.Equals
//
// Ported from ILSpy (ICSharpCode.Decompiler.TypeSystem: InheritanceHelper, ParameterListComparer, SignatureComparer), Copyright
// AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler). Only the ordinal name comparer is ported (the only one used).
#pragma once

#include <vector>

#include "dotnet/decompiler/typesystem/type_system.hpp"

namespace cb::dotnet::decompiler::typesystem {

// ParameterListComparer: the same parameter types after normalizing method type parameters, dynamic and tuples (with
// include_modifiers, the same ref kinds and params too).
bool parameter_lists_equal(const std::vector<const IParameter*>& x, const std::vector<const IParameter*>& y, bool include_modifiers = false);
int parameter_list_hash_code(const std::vector<const IParameter*>& list);

// SignatureComparer.Ordinal
bool signature_equals(const IMember* x, const IMember* y);
int signature_hash_code(const IMember* member);

namespace inheritance_helper {
// The base members a member overrides or hides, nearest base type first (with include_implemented_interfaces, the interface members
// too); UsageError for a null member.
std::vector<const IMember*> get_base_members(const IMember* member, bool include_implemented_interfaces);
const IMember* get_base_member(const IMember* member);
// The member of derived_type that overrides (or for a field or event, has the name of) base_member; nullptr for none.
const IMember* get_derived_member(const IMember* base_member, const ITypeDefinition* derived_type);
// The attributes of a type and its base types (most basic first), or of a member and what it overrides.
std::vector<const IAttribute*> get_attributes(const ITypeDefinition* type_def);
const IAttribute* get_attribute(const ITypeDefinition* type_def, KnownAttribute attribute_type);
std::vector<const IAttribute*> get_attributes(const IMember* member);
const IAttribute* get_attribute(const IMember* member, KnownAttribute attribute_type);
}  // namespace inheritance_helper

}  // namespace cb::dotnet::decompiler::typesystem
