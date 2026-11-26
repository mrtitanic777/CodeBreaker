// dotnet/decompiler/metadata/metadata_token_helpers.hpp -- a token read from IL as a handle, when it can be one.
//
// Ported from ILSpy (ICSharpCode.Decompiler.Metadata: MetadataTokenHelpers), Copyright AlphaSierraPapa for the ILSpy team, MIT (see NOTICE
// in dotnet/decompiler).
#pragma once

#include <optional>

#include "dotnet/metadata/handles.hpp"

namespace cb::dotnet::decompiler::metadata {

// nullopt for a negative token (upstream: SRM would read it as a virtual token) or one of a heap (#US and above).
inline std::optional<dotnet::metadata::EntityHandle> try_as_entity_handle(int metadata_token) {
    if (metadata_token < 0 || static_cast<std::uint32_t>(metadata_token) >= 0x70000000u) return std::nullopt;
    return dotnet::metadata::EntityHandle(static_cast<std::uint32_t>(metadata_token));
}

// The same, nil instead of nullopt.
inline dotnet::metadata::EntityHandle entity_handle_or_nil(int metadata_token) { return try_as_entity_handle(metadata_token).value_or(dotnet::metadata::EntityHandle()); }

}  // namespace cb::dotnet::decompiler::metadata
