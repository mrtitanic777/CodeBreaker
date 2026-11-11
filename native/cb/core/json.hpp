// cb/core/json.hpp -- reading and writing JSON files and text at the edge.
//
// JSON is an edge format: it is parsed into, and written from, typed structs by `from_json`/`to_json` in the
// owning module's .cpp (nlohmann's NLOHMANN_DEFINE_TYPE_* macros). `Json` values do not travel between modules.
// This header only forward-declares Json (see json_fwd.hpp); include <nlohmann/json.hpp> in the .cpp that needs
// to build or inspect a document.
#pragma once

#include <filesystem>
#include <string>
#include <string_view>

#include "cb/core/json_fwd.hpp"

namespace cb::json {

enum class Style {
    Compact,   // no whitespace: caches, wire messages
    Pretty,    // 2-space indent, UTF-8 as is: files people read or diff (L2, reports, manifests)
    // The layout the remaining Python wrote (", " and ": " separators, non-ASCII escaped as \uXXXX), for files
    // the Python still reads during the transition. Removed with the Python.
    PythonCompatible,
    PythonCompatiblePretty,   // the same with indent=2 (item separator "," at line ends)
};

// Parse text; throws cb::ParseError (file name "<json>", line and column of the fault).
Json parse(std::string_view text);
// Parse a file; throws IoError, or ParseError naming the file.
Json load(const std::filesystem::path& p);
// Parse text without throwing: nullopt when it is not JSON.
bool try_parse(std::string_view text, Json& out);

// Serialise.
std::string dump(const Json& j, Style style = Style::Pretty);
// Write a file (atomically: a reader never sees half a document).
void save(const std::filesystem::path& p, const Json& j, Style style = Style::Pretty);

}  // namespace cb::json
