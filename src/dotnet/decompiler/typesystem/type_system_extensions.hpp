// dotnet/decompiler/typesystem/type_system_extensions.hpp -- questions about types the type system answers from its interfaces: every base
// type, derivation, whether a type is open, unbound, unmanaged or a known type, a delegate's Invoke, the element type of an enumerable,
// and finding types and modules of a compilation by name.
//
//     for (const IType* b : get_all_base_types(list_of_int)) ...        // object, the interfaces, List<int> itself last
//     is_derived_from(circle_definition, shape_definition);
//
// Ported from ILSpy (ICSharpCode.Decompiler.TypeSystem: TypeSystemExtensions; TypeSystem.Implementation: BaseTypeCollector), Copyright
// AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler). Not yet ported (they need KnownAttribute, the inheritance
// helper, the resolver or the IL transforms, and come with them): the inline-array questions, the attribute lookups with inheritance,
// IsDefaultValueAssignmentAllowed, GetSymbol, IsBuiltinAttribute, IsDirectImportOf, FindModuleByAssemblyNameInfo, ResolveExtensionInfo.
#pragma once

#include <optional>
#include <string_view>
#include <vector>

#include "dotnet/decompiler/typesystem/types.hpp"

namespace cb::dotnet::decompiler::metadata {
class IAssemblyReference;
}
namespace cb::dotnet::metadata {
class AssemblyNameInfo;
}

namespace cb::dotnet::decompiler::typesystem {

// Every base type of `type` and the type itself, each once, a type after its base types (Object, I1, I2, C for the diamond).
std::vector<const IType*> get_all_base_types(const IType* type);
// The same without implemented interfaces (but an interface's own base interfaces).
std::vector<const IType*> get_non_interface_base_types(const IType* type);
// The interface as the implementing type implements it (with its type arguments); the interface itself when it is not found.
const IType* get_interface_as_implemented_by(const IType* interface_type, const IType* implementing_type);
std::vector<const ITypeDefinition*> get_all_base_type_definitions(const IType* type);
// UsageError for definitions of different compilations; false for a null base.
bool is_derived_from(const ITypeDefinition* type, const ITypeDefinition* base_type);
bool is_derived_from(const ITypeDefinition* type, KnownTypeCode base_type);
// The definition and every type it is nested in, innermost first.
std::vector<const ITypeDefinition*> get_declaring_type_definitions(const ITypeDefinition* definition);

bool is_parameterized_property(const IProperty* property);
bool is_open(const IType* type);   // mentions a type parameter
const IEntity* get_type_parameter_owner(const IType* type);
bool is_unbound(const IType* type);   // a generic definition (or unknown type) without arguments
bool is_unmanaged_type(const IType* type, bool allow_generics);
bool is_array_interface_type(const IType* type);   // IEnumerable<T>, ICollection<T>, IList<T>, IReadOnlyCollection<T>, IReadOnlyList<T>
bool is_known_type(const IType* type, KnownTypeCode known_type);
const IMethod* get_delegate_invoke_method(const IType* type);
const IType* skip_modifiers(const IType* type);
const IType* unwrap_by_ref(const IType* type);
bool has_readonly_modifier(const IMethod* accessor);
bool is_any_pointer(TypeKind kind);

std::vector<const ITypeDefinition*> get_all_type_definitions(const ICompilation& compilation);
std::vector<const ITypeDefinition*> get_top_level_type_definitions(const ICompilation& compilation);
// The type of any module of the compilation; an UnknownType (kept by the compilation) when none has it.
const IType* find_type(const ICompilation& compilation, const FullTypeName& full_type_name);
const ITypeDefinition* get_type_definition(const IModule& module, const FullTypeName& full_type_name);
const ITypeDefinition* get_type_definition(const IModule& module, std::string namespace_name, std::string name, int type_parameter_count = 0);

// The element type of a collection: T of IEnumerable<T> (or IEnumerator<T>), object for the non-generic ones, Unknown otherwise;
// is_generic says which (nullopt for none).
const IType* get_element_type_from_ienumerable(const IType* collection_type, const ICompilation& compilation, bool allow_ienumerator,
                                               std::optional<bool>& is_generic);
bool full_name_is(const IMember* member, std::string_view type, std::string_view name);
const IType* without_nullability(const IType* type);
// The definition with its own type parameters as arguments (the definition itself when it is not generic).
const IType* as_parameterized_type(const ITypeDefinition* definition);
const INamespace* get_namespace_by_full_name(const ICompilation& compilation, std::string_view name);
// The module whose full name (then name) matches the reference, ignoring case.
const IModule* find_module_by_reference(const ICompilation& compilation, const metadata::IAssemblyReference& assembly_name);
// The module of that full assembly name, else of that name (case-insensitively); nullptr for none.
const IModule* find_module_by_assembly_name_info(const ICompilation& compilation, const srm::AssemblyNameInfo& assembly_name);

// An UnknownType kept by `arena` (one object per name).
const IType* make_unknown_type(TypeArena& arena, const FullTypeName& full_type_name, std::optional<bool> is_reference_type = std::nullopt);
const IType* make_unknown_type(TypeArena& arena, std::optional<std::string> namespace_name, std::string name, int type_parameter_count = 0,
                               std::optional<bool> is_reference_type = std::nullopt);

}  // namespace cb::dotnet::decompiler::typesystem
