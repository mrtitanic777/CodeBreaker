// source/stub.hpp -- the DOS program in front of a Windows program's new header (its "stub"), as the whole-file writers take it.
//
// A 16-bit (NE), 32-bit (PE) or driver (LE) file starts with a small DOS program, usually the 128 bytes that print "cannot be run in
// DOS mode", sometimes a real one. A writer puts it in the source either as the original's bytes, or -- when it is a real program whose
// own source builds back into those bytes (source/dos_writer) -- as an `incbin` of that source's assembled file. What finds that source is
// handed to the writer (`FindStub`), so the writers do not depend on the DOS writer; source/dos_stub supplies the one that uses it.
#pragma once

#include <cstdint>
#include <filesystem>
#include <functional>
#include <optional>
#include <string_view>
#include <variant>

#include "core/bytes.hpp"

namespace cb::source {

// What the program's DOS stub is written as.
struct SearchStub {};   // from its own source when there is one that builds back (Options::find_stub)
struct KeepStub {};     // as the original's bytes
using Stub = std::variant<SearchStub, KeepStub, std::filesystem::path>;   // or the path of the source's assembled stub

// The assembled stub of a file whose new header is at `new_header`, or nothing when its bytes are to be kept; `say` takes progress lines.
using FindStub = std::function<std::optional<std::filesystem::path>(ByteView file, std::uint64_t new_header, const std::filesystem::path& out,
                                                                     const std::function<void(std::string_view)>& say)>;

}  // namespace cb::source
