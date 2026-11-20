// formats/xbox/chd.hpp -- CHD (MAME's Compressed Hunks of Data), the compressed disc image some Xbox discs are kept in.
//
//     chd::Header h = chd::read_header_file(path);          version, sizes, the codecs; no tool needed
//     chd::extract(chdman, "game.chd", "game.iso");         chd -> ISO, through MAME's chdman
//     chd::build(chdman, "game.iso", "game.chd");           ISO -> DVD CHD
//     chd::verify(chdman, "game.chd");                      chdman's integrity check
//
// The CHD v5 header is read here: big-endian, "MComprHD", header length, version (5), four compressor tags (a tag is four characters, zero for
// none), logical size, map offset, metadata offset, hunk size, unit size. A CHD's multi-codec hunk format (zlib, lzma, flac ...) is maintained
// upstream and is not reimplemented: conversions are driven through chdman, which the caller names (settings: chdman, else on PATH). An older
// version is UnsupportedError (`chdman copy` upgrades it to v5).
//
// Errors: FormatError (TruncatedError) for bytes that are not a CHD; ToolError for a missing chdman, a failed conversion, or one that wrote nothing.
#pragma once

#include <cstdint>
#include <filesystem>
#include <optional>
#include <string>
#include <vector>

#include "core/bytes.hpp"
#include "core/cli.hpp"

namespace cb::formats::xbox::chd {

inline constexpr std::string_view kMagic = "MComprHD";
inline constexpr std::size_t kHeaderBytes = 124;   // the v5 header

struct Header {
    std::uint32_t version = 0, header_len = 0;
    std::uint64_t logical_bytes = 0;
    std::uint32_t hunk_bytes = 0, unit_bytes = 0;
    std::vector<std::string> compressors;   // the codec tags in use, in order ("zlib", "lzma", ...); none for an uncompressed one
    std::uint64_t meta_offset = 0, map_offset = 0;
};

// `head` is the start of the file (the 124 bytes of a v5 header).
Header read_header(ByteView head);
Header read_header_file(const std::filesystem::path& file);

// MAME's chdman, as the caller found it.
using Tool = std::optional<std::filesystem::path>;

// chd -> ISO (chdman extractdvd). -> the bytes written.
std::uint64_t extract(const Tool& chdman, const std::filesystem::path& chd, const std::filesystem::path& iso);
// ISO -> chd (chdman createdvd; an existing output is replaced). -> the bytes written.
std::uint64_t build(const Tool& chdman, const std::filesystem::path& iso, const std::filesystem::path& chd);
// chdman's integrity report.
std::string verify(const Tool& chdman, const std::filesystem::path& chd);

struct Environment {
    Tool chdman;
};

// `chd info|toiso|tochd|verify ...`: the module's command line.
cli::Command command(const Environment& environment);

}  // namespace cb::formats::xbox::chd
