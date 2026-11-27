// dotnet/decompiler/ilspycmd/resource_extensions.hpp -- the embedded resources of a module as ilspycmd names them: "<resource>", and for
// a .resources container "<container>/<entry>" for each entry; and the bytes or value behind such a name.
//
//     for (const std::string& path : ResourceExtensions::enumerate_resource_paths(*module)) ...      // --list-resources
//     util::ResourceValue value;
//     if (ResourceExtensions::try_get_resource(*module, "Sample.g.resources/a.txt", &value)) ...     // --resource
//
// Ported from ILSpy (ICSharpCode.ILSpyCmd: ResourceExtensions, from ResourceExtensions.cs), Copyright (c) 2026 AlphaSierraPapa for the
// SharpDevelop Team, MIT (see NOTICE in dotnet/decompiler).
//
// Not ported: DecompileBaml (BAML to XAML: the BAML decompiler, ICSharpCode.BamlDecompiler, is not part of this port).
#pragma once

#include <string>
#include <vector>

#include "dotnet/decompiler/metadata/metadata_file.hpp"
#include "dotnet/decompiler/util/resources_file.hpp"

namespace cb::dotnet::decompiler::ilspycmd {

class ResourceExtensions {
public:
    // The embedded resources, a .resources file by its entries ("<container>/<entry>"; a container that cannot be read by its own name).
    static std::vector<std::string> enumerate_resource_paths(const metadata::MetadataFile& module);

    // The resource or entry of that name (any case). A resource is its bytes (a Bytes), an entry of a .resources file its value
    // (a stream or a serialized object as bytes). false when there is none, or the resource's data cannot be read.
    static bool try_get_resource(const metadata::MetadataFile& module, const std::string& resource_path, util::ResourceValue* value);
};

}  // namespace cb::dotnet::decompiler::ilspycmd
