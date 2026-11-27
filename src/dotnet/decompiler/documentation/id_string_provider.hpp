// dotnet/decompiler/documentation/id_string_provider.hpp -- the ID strings of C# (C# 4.0 spec, A.3.1) that name a type or member in an XML
// documentation file: "T:System.String", "M:System.String.Contains(System.String)", "P:System.String.Chars(System.Int32)"; made from an
// entity's metadata, and read back to the entity they name.
//
//     id_string_provider::get_id_string(*module, method_handle);                 // "M:Ns.Type.Method(System.Int32)"
//     for (const std::string& id : id_string_provider::get_id_string_candidates(*module, handle)) ...   // the forms C# and C++/CLI write
//     id_string_provider::find_entity("T:System.String", modules);               // the module and handle of the type
//
// Ported from ILSpy (ICSharpCode.Decompiler.Documentation: IdStringProvider, from IdStringProvider.cs), Copyright AlphaSierraPapa for the ILSpy
// team, MIT (see NOTICE in dotnet/decompiler). The extension methods of MetadataFile and IEntity are free functions here. get_id_string of an
// entity that is not backed by metadata throws UsageError (upstream: NotSupportedException); a nil handle too (ArgumentException).
// A text that is not an ID string is a typesystem::ReflectionNameParseError, as upstream's ReflectionNameParseException.
#pragma once

#include <string>
#include <vector>

#include "dotnet/decompiler/metadata/metadata_file.hpp"
#include "dotnet/decompiler/typesystem/type_system.hpp"
#include "dotnet/metadata/handles.hpp"

namespace cb::dotnet::decompiler::documentation::id_string_provider {

// The ID string (C# 4.0 spec, section A.3.1) of the type, field, method, property or event, in the form the C# compiler writes into xml
// documentation files.
std::string get_id_string(const metadata::MetadataFile& module, cb::dotnet::metadata::EntityHandle handle);

// The ID string candidates for the entity, most specific first: the MSVC C++/CLI (ECMA-372-style) form when it differs from the C#/Roslyn form, then
// the C#/Roslyn form. Documentation lookup should try the candidates in order, so that xml doc files written by either compiler can be matched. The
// dialects differ in signatures only: Roslyn ignores custom modifiers and strips the arity marker of instantiated generic types, while MSVC renders
// modifiers ('!' or '|' followed by the modifier type), keeps arity markers, and refers to a default indexed property as 'default'. The C++/CLI
// form comes first because wherever it differs it contains character sequences Roslyn never writes, so it can only match MSVC-generated keys; the
// stripped Roslyn form of one member can collide with the key of a different member in an MSVC-generated file (e.g. overloads differing only in a
// custom modifier). When such a colliding sibling overload exists, the Roslyn form is omitted entirely: assemblies containing such overloads cannot
// come from the C# compiler, so their xml files use the C++/CLI dialect, where that key documents the sibling.
std::vector<std::string> get_id_string_candidates(const metadata::MetadataFile& module, cb::dotnet::metadata::EntityHandle handle);

// The ID string of the entity (computed from its metadata; for a specialized member, the underlying member definition's).
std::string get_id_string(const typesystem::IEntity& entity);

// The name of a type as an ID string writes it ("System.Collections.Generic.List{System.Int32}", "`0", "System.Int32[]").
std::string get_type_name(const typesystem::IType* type);

// Finds the entity in the given type resolve context; nullptr when it is not found.
const typesystem::IEntity* find_entity(const std::string& id_string, const typesystem::ITypeResolveContext& context);

// The module and handle of an entity found by its ID string.
struct FoundEntity {
    const metadata::MetadataFile* module = nullptr;
    cb::dotnet::metadata::EntityHandle handle;
    bool found() const { return module != nullptr; }
};

// Finds the entity with the given ID string ("T:System.String", "M:System.String.Contains(System.String)") in the modules, in priority order (a null
// module is skipped). A type only present as a type forwarder is returned as its ExportedType handle; members of such a type are not followed into the
// target assembly unless that assembly is itself in `modules`. Nothing found: found() is false.
//
// The ID string format cannot represent all names valid in metadata: get_id_string emits raw metadata names, but a name that itself contains ID string
// special characters (e.g. a dot in a type name) is ambiguous when parsed back, because namespace/type-name splits are only tried at dots. Function
// pointer parameter types render as empty (matching Roslyn), so overloads differing only by a function pointer type share an ID and resolve to the
// first candidate.
FoundEntity find_entity(const std::string& id_string, const std::vector<const metadata::MetadataFile*>& modules);

}  // namespace cb::dotnet::decompiler::documentation::id_string_provider
