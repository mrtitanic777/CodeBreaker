// cb/formats/probe.hpp -- what the "is this file one of mine?" probes of the format readers share.
#pragma once

#include <filesystem>
#include <initializer_list>
#include <optional>
#include <string>
#include <string_view>

#include "cb/core/bytes.hpp"

namespace cb::formats {

// The file name's extension, lower-cased, with its dot (".nes"); empty if there is none.
std::string extension_of(const std::filesystem::path& path);

// Does the file name end in one of `extensions` (lower-case, with dots)?
bool has_extension(const std::filesystem::path& path, std::initializer_list<std::string_view> extensions);

// The first `count` bytes of the file starting at `offset` (fewer if the file is shorter); nullopt if it cannot
// be opened. Probes read a few bytes, never the whole file.
std::optional<Bytes> read_prefix(const std::filesystem::path& path, std::uint64_t offset, std::size_t count);

// The size of a regular file; nullopt if there is none.
std::optional<std::uint64_t> size_of(const std::filesystem::path& path);

}  // namespace cb::formats
