// dotnet/decompiler/documentation/xml_doc_loader.hpp -- finds and loads the .xml documentation of an assembly: next to it (in the user's language
// first), in the .NET Framework's reference assemblies, or for .NET (Core) in the reference pack that goes with the shared runtime the assembly
// lives in.
//
//     std::shared_ptr<XmlDocumentationProvider> docs = xml_doc_loader::load_documentation(*module);      // nullptr when there is none
//     auto text = documentation::get_documentation(*entity);                                              // the entity's own, by its module
//
// Ported from ILSpy (ICSharpCode.Decompiler.Documentation: XmlDocLoader and its AggregatingXmlDocumentationProvider, from XmlDocLoader.cs; and
// NRExtensions.GetDocumentation, which this is the only user of), Copyright AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in
// dotnet/decompiler).
//
// Where upstream looks at the runtime it runs on (typeof(object).Assembly.Location, the redirection target's %CORSYSDIR%), this port uses the newest
// shared Microsoft.NETCore.App of the dotnet found on PATH, as the assembly resolver does (metadata/universal_assembly_resolver.hpp); with none,
// those lookups find nothing. The language of the user is the Windows user interface language (else the LANG variable). The cache of the loaded
// documentation is by module (upstream's ConditionalWeakTable): an entry is dropped when its module's file name differs from the one it was made for,
// which is what a new module at the address of a freed one looks like, but a module must not be freed and a different one with the same name made at
// its address while this cache holds it (call clear_cache()).
#pragma once

#include <filesystem>
#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include "dotnet/decompiler/documentation/xml_documentation_provider.hpp"
#include "dotnet/decompiler/metadata/metadata_file.hpp"

namespace cb::dotnet::decompiler::documentation {

// The documentation of an entity: its provider is that of the entity's module (xml_doc_loader::load_documentation); nullopt when the module has none
// or it has nothing for the entity (NRExtensions.GetDocumentation).
std::optional<std::string> get_documentation(const typesystem::IEntity& entity);

namespace xml_doc_loader {

// The documentation of mscorlib (or, on .NET, of the runtime's corlib); nullptr when there is none.
std::shared_ptr<XmlDocumentationProvider> mscorlib_documentation();

// The documentation of the module: the .xml file next to it, else one found by the framework it targets, else on .NET the reference pack's; nullptr
// for none. Cached by module (a miss too). The provider's constructor errors (IoError, XmlError) are thrown.
std::shared_ptr<XmlDocumentationProvider> load_documentation(const metadata::MetadataFile& module);

// Forgets what load_documentation has cached.
void clear_cache();

// Maps a runtime assembly path like X:\Program Files\dotnet\shared\Microsoft.NETCore.App\10.0.0\System.Private.CoreLib.dll to the matching ref-pack
// target framework directory X:\Program Files\dotnet\packs\Microsoft.NETCore.App.Ref\10.0.0\ref\net10.0. nullopt when the assembly isn't laid out
// under shared/Microsoft.NETCore.App/<version>/ or when the parallel ref pack isn't installed.
std::optional<std::filesystem::path> resolve_modern_ref_pack_tfm_directory(const std::string& assembly_file_name);

// Given the assembly file name, looks up the XML documentation file name (in the user's language first, then without, then English); nullopt if no
// XML documentation file is found.
std::optional<std::string> lookup_localized_xml_doc(const std::string& file_name);

}  // namespace xml_doc_loader

// The directory of the shared runtime this port treats as its own (the newest Microsoft.NETCore.App under the dotnet on PATH, with a trailing
// separator); nullopt when there is none.
std::optional<std::filesystem::path> runtime_directory();

}  // namespace cb::dotnet::decompiler::documentation
